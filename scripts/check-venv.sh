#!/bin/bash
set -e
if [ ! -d ".venv" ]; then
    echo Please use init.sh before attempting a run. It initializes a Python Virtual Environment and installs required dependencies.
    exit 1
fi