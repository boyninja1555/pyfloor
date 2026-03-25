#!/bin/bash
set -e
scripts/check-venv.sh
source .venv/bin/activate
python -m build
twine upload dist/*