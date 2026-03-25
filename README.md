<div align="center">
    <h1>PyFloor</h1>
</div>

## Overview

**PyFloor** is a Python project generator & devlauncher that can *probably* generate projects in 0.8 seconds. Initial setup does not give you any options because most Python devs don't configure anything until later. This command below generates a project in your current directory and installs a VENV + default dependencies:

```bash
pyfloor . --install
```

## Commands

**Generation**  
The largest hurdle in starting a Python project, in my opinion, is the build system setup. You can either raw-dog scripts, include a `requirements.txt`, or use one of those feature-packed frameworks that bloat your storage. PyFloor comes with commands handling most of that, so all you need to do is generate the config and source files. Good news, PyFloor does that too! Here's what a basic generation looks like before installation:

```
my_project/                 # Your project
│
├─ src/                     # Source folder
│   └─ main.py              # Default starter code
│
├─ .gitignore               # Ignore rules for Git
└─ pyfloor.config           # Config for PyFloor
```

To finish initializing your project:

```bash
pyfloor . --install  # Creates files like .venv/ and requirements.txt
```

**Dependency management**  
PyFloor mostly wraps PIP so you will be installing packages from there. However, it also automatically syncs to `requirements.txt` since PIP doesn't do that automatically. If you ever manually install a package just run this command below to sync the new dependencies:

```bash
pyfloor . --sync
```

To install a dependency from PIP:

```bash
pyfloor . --install DEPENDENCY
```

To uninstall a dependency:

```bash
pyfloor . --uninstall DEPENDENCY
```

**Chaining**  
Different actions' arguments are handled in a very specific order so you can chain an entire setup in one command. This command below generates a project, installs `pillow`, and runs the currently empty code:

```bash
pyfloor . --install pillow --run
```

General argument execution order:
- Generation / Directory
- Installations
- Uninstallations
- Run

## Contribution

I don't usually write Python! Help at your own risk! Just follow the [MIT license](LICENSE.md) and we're good...