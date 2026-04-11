#!/usr/bin/env python3
"""
Traaitt Checkpoint Generator
Generates checkpoint data from a running XTEnetwork daemon

Usage:
    python3 update-checkpoints.py --host 127.0.0.1 --port 14485 --interval 5000 --output checkpoints.csv

Then use the generated checkpoints.csv with:
    cd scripts/internal-checkpoints-tool
    node checkpoints.js > new_checkpoints.h
"""

import argparse
import json
import requests
import sys
from typing import List, Tuple


def rpc_call(host: str, port: int, method: str, params: dict = None) -> dict:
    """Make JSON-RPC call to daemon"""
    url = f"http://{host}:{port}/json_rpc"

    payload = {
        "jsonrpc": "2.0",
        "id": "checkpoint_gen",
        "method": method,
        "params": params or {}
    }

    try:
        response = requests.post(url, json=payload, timeout=30)
        response.raise_for_status()
        data = response.json()

        if "error" in data:
            raise Exception(f"RPC Error: {data['error']}")

        return data.get("result", {})
    except requests.exceptions.RequestException as e:
        raise Exception(f"Failed to connect to daemon: {e}")


def get_block_count(host: str, port: int) -> int:
    """Get current blockchain height"""
    result = rpc_call(host, port, "getblockcount")
    return result.get("count", 0)


def get_block_hash(host: str, port: int, height: int) -> str:
    """Get block hash at specific height"""
    # Try f_block_json first (block explorer API)
    try:
        result = rpc_call(host, port, "f_block_json", {"hash": f"{height}"})
        if "block" in result:
            return result["block"]["hash"]
    except:
        pass

    # Fallback to getblockhash
    try:
        result = rpc_call(host, port, "getblockhash", {"height": height})
        return result
    except:
        pass

    raise Exception(f"Could not get block hash for height {height}")


def generate_checkpoints(
    host: str,
    port: int,
    interval: int = 5000,
    start_height: int = 0,
    end_height: int = None
) -> List[Tuple[int, str]]:
    """Generate checkpoint list"""

    print(f"Connecting to daemon at {host}:{port}...", file=sys.stderr)

    # Get current height
    current_height = get_block_count(host, port)
    print(f"Current blockchain height: {current_height}", file=sys.stderr)

    if end_height is None:
        end_height = current_height - 10  # Leave buffer for reorgs

    checkpoints = []

    # Always include genesis block
    if start_height == 0:
        print(f"Fetching genesis block (0)...", file=sys.stderr)
        try:
            genesis_hash = get_block_hash(host, port, 0)
            checkpoints.append((0, genesis_hash))
        except Exception as e:
            print(f"Warning: Could not fetch genesis block: {e}", file=sys.stderr)

    # Generate checkpoints at intervals
    height = interval
    while height <= end_height:
        if height >= start_height:
            print(f"Fetching checkpoint at height {height}...", file=sys.stderr)
            try:
                block_hash = get_block_hash(host, port, height)
                checkpoints.append((height, block_hash))
            except Exception as e:
                print(f"Error fetching block {height}: {e}", file=sys.stderr)
                continue

        height += interval

    print(f"\nGenerated {len(checkpoints)} checkpoints", file=sys.stderr)
    return checkpoints


def write_csv(checkpoints: List[Tuple[int, str]], output_file: str):
    """Write checkpoints to CSV file"""
    with open(output_file, 'w') as f:
        for height, block_hash in checkpoints:
            f.write(f"{height},{block_hash}\n")
    print(f"Checkpoints written to {output_file}", file=sys.stderr)


def format_for_header(checkpoints: List[Tuple[int, str]]) -> str:
    """Format checkpoints for C++ header file"""
    lines = []
    lines.append("const std::initializer_list<CheckpointData> CHECKPOINTS = {")

    for height, block_hash in checkpoints:
        lines.append(f'    {{{height}, "{block_hash}"}},')

    # Remove trailing comma from last entry
    if lines:
        lines[-1] = lines[-1].rstrip(',')

    lines.append("};")
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(
        description="Generate Traaitt checkpoints from daemon"
    )
    parser.add_argument(
        "--host",
        default="127.0.0.1",
        help="Daemon RPC host (default: 127.0.0.1)"
    )
    parser.add_argument(
        "--port",
        type=int,
        default=14485,
        help="Daemon RPC port (default: 14485)"
    )
    parser.add_argument(
        "--interval",
        type=int,
        default=5000,
        help="Checkpoint interval in blocks (default: 5000)"
    )
    parser.add_argument(
        "--start",
        type=int,
        default=0,
        help="Start height (default: 0)"
    )
    parser.add_argument(
        "--end",
        type=int,
        default=None,
        help="End height (default: current - 10)"
    )
    parser.add_argument(
        "--output",
        default="checkpoints.csv",
        help="Output CSV file (default: checkpoints.csv)"
    )
    parser.add_argument(
        "--format",
        choices=["csv", "cpp"],
        default="csv",
        help="Output format (default: csv)"
    )

    args = parser.parse_args()

    try:
        # Generate checkpoints
        checkpoints = generate_checkpoints(
            args.host,
            args.port,
            args.interval,
            args.start,
            args.end
        )

        if not checkpoints:
            print("No checkpoints generated!", file=sys.stderr)
            sys.exit(1)

        # Output based on format
        if args.format == "csv":
            write_csv(checkpoints, args.output)
        else:  # cpp
            print(format_for_header(checkpoints))

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
