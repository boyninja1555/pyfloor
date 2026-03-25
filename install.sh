#!/bin/bash
set -e
scripts/check-venv.sh
source .venv/bin/activate
pip install -r requirements.txt
echo Dependencies installed from requirements.txt, dump new dependencies with dumpdepends.sh