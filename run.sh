#!/bin/bash
set -e
scripts/check-venv.sh
source .venv/bin/activate
python3 pyfloor/cli.py $*