import os
import subprocess
import venv
from pyfloor.config import PyFloorConfig

MAIN_SOURCE = """def main():
    pass


if __name__ == "__main__":
    main()
"""

GITIGNORE = """### Python ###
__pycache__/
.venv/

### IDEs ###
.idea/
.vscode/
.DS_Store/"""


class PyFloorProject:
    new: bool
    path: str
    venv_path: str
    gitignore_path: str
    depends_path: str
    sources_path: str
    main_source_path: str
    config: PyFloorConfig

    def __init__(self, path: str, install_used: bool):
        self.new = False
        self.path = path
        self.venv_path = os.path.join(self.path, ".venv")
        self.gitignore_path = os.path.join(self.path, ".gitignore")
        self.depends_path = os.path.join(self.path, "requirements.txt")
        self.sources_path = os.path.join(self.path, "src")
        self.main_source_path = os.path.join(self.sources_path, "main.py")
        if not os.path.exists(self.path):
            self.new = True
            print("Creating project directory...")
            os.makedirs(self.path)

        self.config = PyFloorConfig(
            os.path.join(self.path, "pyfloor.config"),
            {
                "name": os.path.basename(os.path.abspath(self.path)),
                "version": "1.0.1",
                "authors": "",
                "contributors": "",
            },
        )

        self.config.load()
        if self.new:
            self.__generate__(install_used)

    def __generate__(self, install_used: bool):
        if not os.path.exists(self.gitignore_path):
            print("Creating default gitignore...")
            with open(self.gitignore_path, "w") as f:
                f.write(GITIGNORE)

        if not os.path.exists(self.sources_path):
            print("Creating sources directory...")
            os.makedirs(self.sources_path)

        if not os.path.exists(self.main_source_path):
            print("Creating main source file...")
            with open(self.main_source_path, "w") as f:
                f.write(MAIN_SOURCE)

        print(
            f"\n  Project generated! You will find it here:\n  {os.path.abspath(self.path)}\n{'  Try running this command again with --install when you need real dependency management and a proper Python environment.\n' if not install_used else ''}"
        )

    def run(self, args: list[str]):
        if not os.path.exists(self.venv_path) or not os.path.exists(self.depends_path):
            print("Please --install before running the program")
            return

        self.__venv_command__(self.main_source_path, *args)

    def install(self, dependency: str | None = None):
        if not os.path.exists(self.venv_path):
            venv.create(self.venv_path, with_pip=True)

        if not os.path.exists(self.depends_path):
            open(self.depends_path, "w").close()

        if dependency is None:
            self.__venv_command__(
                "-m", "pip", "install", "-r", os.path.abspath(self.depends_path)
            )
        else:
            self.__venv_command__("-m", "pip", "install", dependency)
            self.sync_depends(False)

    def uninstall(self, dependency: str):
        if not os.path.exists(self.venv_path):
            venv.create(self.venv_path, with_pip=True)

        if not os.path.exists(self.depends_path):
            open(self.depends_path, "w").close()

        self.__venv_command__("-m", "pip", "uninstall", dependency)
        self.sync_depends(False)

    def sync_depends(self, log=True):
        output = self.__venv_command_output__("-m", "pip", "freeze")
        with open(self.depends_path, "w") as f:
            f.write(output)
            if log:
                print(f"Synced dependencies to {os.path.basename(self.depends_path)}")

    def __venv_command__(self, *args: str):
        subprocess.check_call(
            [
                os.path.join(
                    self.venv_path, "Scripts" if os.name == "nt" else "bin", "python"
                ),
                *args,
            ]
        )

    def __venv_command_output__(self, *args: str):
        return subprocess.check_output(
            [
                os.path.join(
                    self.venv_path, "Scripts" if os.name == "nt" else "bin", "python"
                ),
                *args,
            ],
            text=True,
        )
