#!/bin/bash
set -e
scripts/check-venv.sh
source .venv/bin/activate
python3 -m pyfloor.cli $*