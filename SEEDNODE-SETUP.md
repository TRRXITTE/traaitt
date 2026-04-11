# Traaitt [XTE] Public Seed Node Configuration
## TRRXITTE Team - Production Deployment

**Target Setup**: 8x Public Seed Nodes
**Specs per node**: 2 vCPU, 4GB RAM, SSD + External Swap Drive
**Purpose**: High-availability seed nodes for network bootstrap

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│  8x Seed Nodes (Geographically Distributed)                │
├─────────────────────────────────────────────────────────────┤
│  Node 1: Region US-East      (IP: TBD)                      │
│  Node 2: Region US-West      (IP: TBD)                      │
│  Node 3: Region EU-Central   (IP: TBD)                      │
│  Node 4: Region EU-West      (IP: TBD)                      │
│  Node 5: Region APAC-East    (IP: TBD)                      │
│  Node 6: Region APAC-South   (IP: TBD)                      │
│  Node 7: Region SA-East      (IP: TBD)                      │
│  Node 8: Region AU-East      (IP: TBD)                      │
└─────────────────────────────────────────────────────────────┘
```

---

## Per-Node Hardware Configuration

### VPS Specifications
- **CPU**: 2 vCPU (x86_64)
- **RAM**: 4GB DDR4
- **Primary Storage**: 100GB SSD (blockchain data)
- **Swap Volume**: 20GB External Block Storage (swap only)
- **Bandwidth**: 5TB/month minimum
- **Network**: 1Gbps port

### Recommended VPS Providers
| Provider | Region | Plan | Monthly Cost |
|----------|--------|------|--------------|
| Hetzner | EU | CPX21 | €8.90 |
| DigitalOcean | Global | 2vCPU/4GB | $24 |
| Vultr | Global | High Frequency | $18 |
| Linode | Global | 4GB | $24 |

**Total monthly cost**: ~$150-200 for 8 nodes

---

## External Swap Drive Setup

### Step 1: Attach External Volume

```bash
# List available block devices
lsblk

# Expected output:
# NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
# sda      8:0    0  100G  0 disk           <- Main SSD
# └─sda1   8:1    0  100G  0 part /
# sdb      8:16   0   20G  0 disk           <- External swap volume
```

### Step 2: Format and Configure External Swap

```bash
#!/bin/bash
# setup-external-swap.sh

# Identify external volume (change sdb if different)
SWAP_DEVICE="/dev/sdb"

# WARNING: This will erase all data on the device!
echo "Setting up swap on ${SWAP_DEVICE}..."

# Create swap partition
sudo parted ${SWAP_DEVICE} --script mklabel gpt
sudo parted ${SWAP_DEVICE} --script mkpart primary linux-swap 0% 100%

# Format as swap
sudo mkswap ${SWAP_DEVICE}1

# Get UUID for fstab
SWAP_UUID=$(sudo blkid -s UUID -o value ${SWAP_DEVICE}1)

# Enable swap
sudo swapon ${SWAP_DEVICE}1

# Make permanent (using UUID for reliability)
echo "UUID=${SWAP_UUID} none swap sw 0 0" | sudo tee -a /etc/fstab

# Optimize swap settings for SSD
sudo sysctl vm.swappiness=5
sudo sysctl vm.vfs_cache_pressure=50
echo 'vm.swappiness=5' | sudo tee -a /etc/sysctl.conf
echo 'vm.vfs_cache_pressure=50' | sudo tee -a /etc/sysctl.conf

echo "Swap configuration complete!"
free -h
swapon --show
```

**Note**: Using external volume isolates swap I/O from blockchain database I/O, improving performance.

---

## Daemon Configuration (Per Node)

### /home/traaitt/traaitt-seednode.conf

```json
{
  "data-dir": "/home/traaitt/.traaitt",
  "log-file": "/var/log/traaitt/XTEnetwork.log",
  "log-level": 2,
  "load-checkpoints": "default",
  "db-max-open-files": 150,
  "db-read-buffer-size": 128,
  "db-threads": 2,
  "db-write-buffer-size": 256,
  "p2p-bind-ip": "0.0.0.0",
  "p2p-bind-port": 14451,
  "p2p-external-port": 14451,
  "rpc-bind-ip": "127.0.0.1",
  "rpc-bind-port": 14485,
  "enable-blockexplorer": false,
  "enable-blockexplorer-detailed": false,
  "no-console": true,
  "add-priority-node": [
    "SEED_NODE_2_IP:14451",
    "SEED_NODE_3_IP:14451"
  ]
}
```

**Important**:
- Replace `SEED_NODE_X_IP` with actual IPs of other seed nodes
- Each node should connect to 2-3 other seed nodes (mesh topology)
- Use `p2p-bind-ip: 0.0.0.0` for public access

---

## Systemd Service Configuration

### /etc/systemd/system/traaitt-seednode.service

```ini
[Unit]
Description=Traaitt XTE Public Seed Node
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=traaitt
Group=traaitt
WorkingDirectory=/home/traaitt
ExecStart=/home/traaitt/traaitt/build/src/XTEnetwork --config-file /home/traaitt/traaitt-seednode.conf
Restart=always
RestartSec=10
StartLimitInterval=300
StartLimitBurst=5

# Resource limits for 4GB RAM
MemoryMax=3.5G
MemoryHigh=3G
TasksMax=4096

# Logging
StandardOutput=journal
StandardError=journal
SyslogIdentifier=traaitt-seednode

# Security hardening
NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=full
ProtectHome=read-only
ReadWritePaths=/home/traaitt/.traaitt /var/log/traaitt
ProtectKernelTunables=true
ProtectControlGroups=true
RestrictRealtime=true
RestrictNamespaces=true

# File descriptor limits
LimitNOFILE=65536

[Install]
WantedBy=multi-user.target
```

---

## Firewall Configuration

```bash
#!/bin/bash
# setup-firewall.sh

# UFW Firewall rules for public seed node
sudo ufw --force reset
sudo ufw default deny incoming
sudo ufw default allow outgoing

# SSH (change port if using non-standard)
sudo ufw allow 22/tcp comment 'SSH'

# Traaitt P2P (REQUIRED for seed node)
sudo ufw allow 14451/tcp comment 'Traaitt P2P'

# Optional: RPC (ONLY if providing public RPC - NOT RECOMMENDED)
# sudo ufw allow from TRUSTED_IP to any port 14485 proto tcp

# Rate limiting on SSH
sudo ufw limit 22/tcp

# Enable firewall
sudo ufw --force enable
sudo ufw status verbose
```

---

## Complete Automated Setup Script

### deploy-seednode.sh

```bash
#!/bin/bash
set -e

# Traaitt Seed Node Deployment Script
# For: 2 vCPU / 4GB RAM + External Swap Drive

echo "================================================"
echo "Traaitt [XTE] Public Seed Node Setup"
echo "TRRXITTE Team - Production Configuration"
echo "================================================"
echo ""

# Configuration variables
TRAAITT_USER="traaitt"
TRAAITT_HOME="/home/${TRAAITT_USER}"
SWAP_DEVICE="/dev/sdb"  # Change if different
NODE_IP=$(curl -s ifconfig.me)

# Update system
echo "[1/10] Updating system packages..."
sudo apt update && sudo apt upgrade -y

# Install dependencies
echo "[2/10] Installing build dependencies..."
sudo apt install -y \
  build-essential cmake git wget curl \
  libboost-all-dev libssl-dev pkg-config \
  htop iotop vnstat ufw fail2ban \
  logrotate

# Create traaitt user
echo "[3/10] Creating traaitt user..."
if ! id "${TRAAITT_USER}" &>/dev/null; then
  sudo useradd -m -s /bin/bash ${TRAAITT_USER}
fi

# Setup external swap
echo "[4/10] Configuring external swap volume..."
if [ -b "${SWAP_DEVICE}" ]; then
  sudo parted ${SWAP_DEVICE} --script mklabel gpt
  sudo parted ${SWAP_DEVICE} --script mkpart primary linux-swap 0% 100%
  sudo mkswap ${SWAP_DEVICE}1
  SWAP_UUID=$(sudo blkid -s UUID -o value ${SWAP_DEVICE}1)
  sudo swapon ${SWAP_DEVICE}1

  # Add to fstab if not exists
  if ! grep -q "${SWAP_UUID}" /etc/fstab; then
    echo "UUID=${SWAP_UUID} none swap sw 0 0" | sudo tee -a /etc/fstab
  fi
else
  echo "Warning: Swap device ${SWAP_DEVICE} not found, skipping..."
fi

# Optimize kernel parameters
echo "[5/10] Optimizing kernel parameters..."
sudo tee /etc/sysctl.d/99-traaitt.conf > /dev/null << 'EOF'
# Swap optimization (external drive)
vm.swappiness=5
vm.vfs_cache_pressure=50

# Network optimization
net.core.rmem_max=134217728
net.core.wmem_max=134217728
net.ipv4.tcp_rmem=4096 87380 67108864
net.ipv4.tcp_wmem=4096 65536 67108864
net.core.somaxconn=1024
net.ipv4.tcp_max_syn_backlog=2048

# File descriptors
fs.file-max=100000

# SSD optimization
vm.dirty_ratio=10
vm.dirty_background_ratio=5
EOF
sudo sysctl -p /etc/sysctl.d/99-traaitt.conf

# Clone and build Traaitt
echo "[6/10] Cloning Traaitt repository..."
sudo -u ${TRAAITT_USER} bash << 'EOFUSER'
cd ~
if [ ! -d "traaitt" ]; then
  git clone https://github.com/TRRXITTE/traaitt.git
fi
cd traaitt
mkdir -p build
cd build
cmake ..
make -j$(nproc)
EOFUSER

# Create config file
echo "[7/10] Creating daemon configuration..."
sudo -u ${TRAAITT_USER} tee ${TRAAITT_HOME}/traaitt-seednode.conf > /dev/null << 'EOF'
{
  "data-dir": "/home/traaitt/.traaitt",
  "log-file": "/var/log/traaitt/XTEnetwork.log",
  "log-level": 2,
  "load-checkpoints": "default",
  "db-max-open-files": 150,
  "db-read-buffer-size": 128,
  "db-threads": 2,
  "db-write-buffer-size": 256,
  "p2p-bind-ip": "0.0.0.0",
  "p2p-bind-port": 14451,
  "p2p-external-port": 14451,
  "rpc-bind-ip": "127.0.0.1",
  "rpc-bind-port": 14485,
  "enable-blockexplorer": false,
  "no-console": true
}
EOF

# Create log directory
sudo mkdir -p /var/log/traaitt
sudo chown ${TRAAITT_USER}:${TRAAITT_USER} /var/log/traaitt

# Setup logrotate
echo "[8/10] Configuring log rotation..."
sudo tee /etc/logrotate.d/traaitt > /dev/null << 'EOF'
/var/log/traaitt/*.log {
    daily
    missingok
    rotate 7
    compress
    delaycompress
    notifempty
    create 0640 traaitt traaitt
    sharedscripts
    postrotate
        systemctl reload traaitt-seednode > /dev/null 2>&1 || true
    endscript
}
EOF

# Create systemd service
echo "[9/10] Installing systemd service..."
sudo tee /etc/systemd/system/traaitt-seednode.service > /dev/null << 'EOF'
[Unit]
Description=Traaitt XTE Public Seed Node
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=traaitt
Group=traaitt
WorkingDirectory=/home/traaitt
ExecStart=/home/traaitt/traaitt/build/src/XTEnetwork --config-file /home/traaitt/traaitt-seednode.conf
Restart=always
RestartSec=10
StartLimitInterval=300
StartLimitBurst=5

MemoryMax=3.5G
MemoryHigh=3G
TasksMax=4096

StandardOutput=journal
StandardError=journal
SyslogIdentifier=traaitt-seednode

NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=full
ProtectHome=read-only
ReadWritePaths=/home/traaitt/.traaitt /var/log/traaitt
LimitNOFILE=65536

[Install]
WantedBy=multi-user.target
EOF

sudo systemctl daemon-reload
sudo systemctl enable traaitt-seednode

# Configure firewall
echo "[10/10] Configuring firewall..."
sudo ufw --force reset
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow 22/tcp comment 'SSH'
sudo ufw allow 14451/tcp comment 'Traaitt P2P'
sudo ufw limit 22/tcp
sudo ufw --force enable

echo ""
echo "================================================"
echo "Setup Complete!"
echo "================================================"
echo ""
echo "Node IP: ${NODE_IP}"
echo "P2P Port: 14451"
echo ""
echo "Next steps:"
echo "1. Start daemon: sudo systemctl start traaitt-seednode"
echo "2. Check status: sudo systemctl status traaitt-seednode"
echo "3. View logs: sudo journalctl -u traaitt-seednode -f"
echo "4. Add this node to CryptoNoteConfig.h:"
echo "   \"${NODE_IP}:14451\","
echo ""
echo "Swap status:"
free -h
echo ""
echo "System resources:"
df -h /
echo ""
```

---

## Post-Deployment Configuration

### 1. Start All Nodes

```bash
# On each of the 8 servers
sudo systemctl start traaitt-seednode
sudo systemctl status traaitt-seednode
```

### 2. Update CryptoNoteConfig.h with New Seed Nodes

**Edit**: `src/config/CryptoNoteConfig.h` (lines 356-365)

```cpp
const char *const SEED_NODES[] = {
  "NODE_1_IP:14451",  // US-East
  "NODE_2_IP:14451",  // US-West
  "NODE_3_IP:14451",  // EU-Central
  "NODE_4_IP:14451",  // EU-West
  "NODE_5_IP:14451",  // APAC-East
  "NODE_6_IP:14451",  // APAC-South
  "NODE_7_IP:14451",  // SA-East
  "NODE_8_IP:14451",  // AU-East
};
```

### 3. Rebuild and Release New Binaries

```bash
# Update version in CMakeLists.txt if needed
cd ~/traaitt/build
cmake ..
make -j$(nproc)

# Create release
cd src
tar -czf XTE-v0.10.46-linux-x64.tar.gz XTEnetwork traaittwallet++
```

---

## Monitoring Dashboard

### monitoring.sh - Real-time Overview

```bash
#!/bin/bash
# Deploy on management machine to monitor all 8 nodes

NODES=(
  "NODE_1_IP:14451"
  "NODE_2_IP:14451"
  "NODE_3_IP:14451"
  "NODE_4_IP:14451"
  "NODE_5_IP:14451"
  "NODE_6_IP:14451"
  "NODE_7_IP:14451"
  "NODE_8_IP:14451"
)

echo "========================================="
echo "Traaitt Seed Node Network Status"
echo "========================================="
echo ""

for NODE in "${NODES[@]}"; do
  IP="${NODE%:*}"
  PORT="${NODE#*:}"

  echo -n "Node ${IP}: "

  # Check if P2P port is open
  if timeout 3 bash -c "echo > /dev/tcp/${IP}/${PORT}" 2>/dev/null; then
    # Get block height via RPC (if accessible)
    HEIGHT=$(ssh traaitt@${IP} "curl -s -X POST http://127.0.0.1:14485/json_rpc -H 'Content-Type: application/json' -d '{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"getblockcount\"}' | jq -r .result.count" 2>/dev/null)

    if [ -n "$HEIGHT" ]; then
      echo "✓ ONLINE (Height: ${HEIGHT})"
    else
      echo "✓ ONLINE (Height: Unknown)"
    fi
  else
    echo "✗ OFFLINE"
  fi
done

echo ""
```

---

## Maintenance Scripts

### update-all-nodes.sh

```bash
#!/bin/bash
# Update daemon on all nodes

NODES=(
  "traaitt@NODE_1_IP"
  "traaitt@NODE_2_IP"
  "traaitt@NODE_3_IP"
  "traaitt@NODE_4_IP"
  "traaitt@NODE_5_IP"
  "traaitt@NODE_6_IP"
  "traaitt@NODE_7_IP"
  "traaitt@NODE_8_IP"
)

for NODE in "${NODES[@]}"; do
  echo "Updating ${NODE}..."
  ssh ${NODE} << 'EOFUPDATE'
    cd ~/traaitt
    git pull
    cd build
    make -j$(nproc)
    sudo systemctl restart traaitt-seednode
    echo "Update complete!"
EOFUPDATE
done
```

---

## Resource Monitoring

### Per-Node Metrics to Track

```bash
# Check daemon memory usage
ps aux | grep XTEnetwork

# Check swap usage
free -h
swapon --show

# Check blockchain size
du -sh /home/traaitt/.traaitt

# Check network bandwidth
vnstat -d

# Check SSD health
sudo smartctl -a /dev/sda | grep -E 'Power_On_Hours|Percentage_Used'

# Check external swap drive health
sudo smartctl -a /dev/sdb
```

---

## Expected Performance (Per Node)

| Metric | Value |
|--------|-------|
| RAM Usage | 2.5-3.5GB |
| Swap Usage | 0.5-1GB (external drive) |
| CPU Usage (idle) | 5-15% |
| CPU Usage (sync) | 80-100% |
| Disk I/O (steady) | 10-50 MB/s |
| Network (outbound) | 2-5 TB/month |
| Peer Connections | 8-12 concurrent |
| Block validation | ~1-2 seconds |
| Initial sync time | 6-10 hours |

---

## Cost Breakdown (8 Nodes)

| Provider | Monthly Cost | Annual Cost |
|----------|--------------|-------------|
| Hetzner (8x CPX21) | €71.20 (~$78) | $936 |
| DigitalOcean (8x 4GB) | $192 | $2,304 |
| Vultr (8x HF) | $144 | $1,728 |

**Recommended**: Hetzner for best value (~$80/month for 8 nodes)

---

## Security Hardening

### 1. SSH Key-Only Authentication

```bash
# Disable password authentication
sudo sed -i 's/#PasswordAuthentication yes/PasswordAuthentication no/' /etc/ssh/sshd_config
sudo systemctl restart sshd
```

### 2. Fail2ban for Brute Force Protection

```bash
sudo apt install fail2ban
sudo systemctl enable fail2ban
sudo systemctl start fail2ban
```

### 3. Automatic Security Updates

```bash
sudo apt install unattended-upgrades
sudo dpkg-reconfigure --priority=low unattended-upgrades
```

---

## Troubleshooting

### Daemon won't start
```bash
# Check logs
sudo journalctl -u traaitt-seednode -n 100 --no-pager

# Check permissions
sudo chown -R traaitt:traaitt /home/traaitt/.traaitt

# Check disk space
df -h
```

### High swap usage
```bash
# Check what's using swap
sudo grep VmSwap /proc/*/status | sort -k2 -n -r | head

# Reduce daemon buffers if needed
db-write-buffer-size: 128
db-read-buffer-size: 64
```

### Peers not connecting
```bash
# Check firewall
sudo ufw status

# Check port is listening
sudo netstat -tlnp | grep 14451

# Test externally
telnet NODE_IP 14451
```

---

## Contact & Support

**TRRXITTE Team**
- Website: https://traaitt.com
- GitHub: https://github.com/TRRXITTE/traaitt
- Submit node details for DNS: https://traaitt.com/contact

---

## Deployment Checklist

- [ ] 8x VPS instances provisioned (2 vCPU, 4GB RAM)
- [ ] External 20GB swap volumes attached to each
- [ ] `deploy-seednode.sh` executed on all nodes
- [ ] Firewall configured (port 14451 open)
- [ ] Daemons started and syncing
- [ ] Monitoring script deployed
- [ ] Node IPs added to `CryptoNoteConfig.h`
- [ ] New binaries built and released
- [ ] DNS records configured (optional)
- [ ] Documentation updated with new seed nodes

**Deployment Date**: _________________
**Deployed By**: _________________
**Version**: v0.10.45.00
