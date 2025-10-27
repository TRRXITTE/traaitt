# traaitt [XTE] Full Node Start Parameters

This document outlines the command-line options for running a traaitt [XTE] full node using the `XTEnetwork` daemon (v0.10.45.00). traaitt [XTE] is a privacy-focused cryptocurrency network designed for secure, global transactions. Running a full node supports the network's decentralization by validating and relaying transactions while maintaining a complete copy of the blockchain. The parameters below include optimized settings for faster sync, efficient database usage, and reliable network connectivity.

**Official Resources**:
- Website: [https://traaitt.com](https://traaitt.com)
- License: [https://traaitt.com/LICENSE](https://traaitt.com/LICENSE)
- Source Code: [https://github.com/TRRXITTE/traaitt](https://github.com/TRRXITTE/traaitt)
- Community/Support: Check [https://traaitt.com](https://traaitt.com) for forums or Discord links

**Hardware Recommendations**:
- **Minimum**: 4GB RAM, 4-core CPU, 50GB SSD
- **Recommended**: 8GB RAM, 8-core CPU, 100GB+ SSD, unmetered broadband
- **Note**: Use an SSD for faster blockchain sync (HDDs are slower).

**Usage**: Run the daemon with `./XTEnetwork [options]` (e.g., `./XTEnetwork --data-dir /path/to/data --db-threads 8`).

## Optimized Start Command
For an optimized Traaitt [XTE] full node, use this command (adjust paths and IPs as needed):
```
./XTEnetwork \
  -c traaitt.conf \
  --data-dir /home/traaitt/.traaitt \
  --log-file XTEnetwork.log \
  --log-level 2 \
  --load-checkpoints default \
  --db-enable-compression \
  --db-max-open-files 256 \
  --db-read-buffer-size 256 \
  --db-threads 8 \
  --db-write-buffer-size 512 \
  --p2p-bind-ip 0.0.0.0 \
  --p2p-bind-port 14451 \
  --p2p-external-port 14451 \
  --rpc-bind-ip 127.0.0.1 \
  --rpc-bind-port 14485 \
  ```



Wallet RPC applications should use the IP 0.0.0.0 instead 127.0.0.1, If run on linux 24/7 server message us for custom domain DNS for easy wallet connection and explorer network node