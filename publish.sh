#!/bin/bash
set -e
scripts/check-venv.sh
source .venv/bin/activate
twine upload dist/*