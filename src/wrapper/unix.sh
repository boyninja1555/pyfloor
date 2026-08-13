#!/usr/bin/env bash
set -e

if ! command -v python3 &> /dev/null; then
    echo "Python is required to manage this project."
    exit 1
fi

usage() {
    echo "Usage: $0 <action> [args...]"
    echo "Actions:"
    echo -e "\tinit [-p|--pro]         Finishes project initialization (only run if new project!)"
    echo -e "\tsetup                   Sets up venv and dependencies (auto-run after init)"
    echo -e "\treset                   Resets project to fresh state"
    echo -e "\tinstall [pip-package]   Installs dependencies or a specific PIP package"
    echo -e "\tsync                    Syncronizes dependencies to requirements.txt (auto-run)"
    echo -e "\trun                     Runs program"
    echo -e "\tbuild                   Builds program"
    echo -e "\tclean                   Removes build files"
}

init() {
    echo "Finishing PyFloor project initialization..."

    cat << "EOF" > .gitignore
# Build files
build/
dist/
*.spec

# Python
.venv/
__pycache__/

# Environment
.vscode/
.idea/
*.iml
.DS_Store
EOF

    cat << "EOF" > pyfloor.config
name=Example
description=
authors=
entry=example.main
EOF

    mkdir src
    if [[ $# -eq 0 || ( "$1" != "--pro" && "$1" != "-p" ) ]]; then
        mkdir src/example
        cat << "EOF" > src/example/main.py
print("Hello, world!")
EOF
    fi

    setup
}

setup() {
    echo "Setting up existing PyFloor project..."
    python3 -m venv .venv
    source .venv/bin/activate
    pip install PyInstaller
    install
}

reset() {
    echo "Resetting PyFloor project to fresh state..."
    rm -rf .venv src
    rm -f pyfloor.config .gitignore
    clean
}

install() {
    if [[ ! -d ".venv" ]]; then
        echo "Please run setup (or init) first!"
        exit 1
    fi

    source .venv/bin/activate

    if [[ ! -f "requirements.txt" ]]; then
        sync
    else
        if [[ $# -eq 0 ]]; then
            echo "Installing dependencies..."
            pip install PyInstaller
            pip install -r requirements.txt
            sync
        else
            echo "Installing $1..."
            pip install "$1"
            sync
        fi
    fi
}

sync() {
    if [[ ! -d ".venv" ]]; then
        echo "Please run setup (or init) first!"
        exit 1
    fi

    source .venv/bin/activate
    echo "Syncing dependencies file..."
    pip freeze > requirements.txt
}

run() {
    if [[ ! -d ".venv" ]]; then
        echo "Please run setup (or init) first!"
        exit 1
    fi

    source .venv/bin/activate
    entry=$(sed -n 's/^entry=//p' pyfloor.config | head -n 1)
    if [[ -z "$entry" ]]; then
        echo "No entry point configured in pyfloor.config!"
        exit 1
    fi

    entry_file="src/${entry//./\/}.py"
    if [[ ! -f "$entry_file" ]]; then
        echo "Entry point does not exist! $entry_file"
        exit 1
    fi

    python3 "$entry_file"
}

build() {
    if [[ ! -d ".venv" ]]; then
        echo "Please run setup (or init) first!"
        exit 1
    fi

    source .venv/bin/activate
    entry=$(sed -n 's/^entry=//p' pyfloor.config | head -n 1)
    if [[ -z "$entry" ]]; then
        echo "No entry point configured in pyfloor.config!"
        exit 1
    fi

    name=$(sed -n 's/^name=//p' pyfloor.config | head -n 1)
    if [[ -z "$name" ]]; then
        echo "No name configured in pyfloor.config!"
        exit 1
    fi

    entry_file="src/${entry//./\/}.py"
    echo "Building PyFloor program..."
    python -m PyInstaller --onefile --name "$name" "$entry_file"
}

clean() {
    echo "Removing build files..."
    rm -rf build/ dist/
    rm -f *.spec
}

case "$1" in
    init)
        init "${@:2}"
        ;;

    setup)
        setup
        ;;

    reset)
        reset
        ;;

    install)
        install "${@:2}"
        ;;

    sync)
        sync
        ;;
    
    run)
        run
        ;;

    build)
        build
        ;;

    clean)
        clean
        ;;

    *)
        usage
        exit 2
        ;;
esac