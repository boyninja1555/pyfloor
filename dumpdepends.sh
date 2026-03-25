#!/bin/bash
set -e
scripts/check-venv.sh
source .venv/bin/activate
pip freeze > requirements.txt
echo Dependencies dumped to requirements.txt, install with install.sh