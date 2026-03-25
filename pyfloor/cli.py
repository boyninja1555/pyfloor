import argparse
from pyfloor.project import PyFloorProject


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("directory")
    parser.add_argument("--install", nargs="?", const=True)
    parser.add_argument("--uninstall")
    parser.add_argument("--sync", action="store_true")
    parser.add_argument("--run", nargs=argparse.REMAINDER)
    args = parser.parse_args()
    project = PyFloorProject(args.directory, args.install is not None)

    if args.install is not None:
        project.install(None if args.install is True else args.install)

    if args.uninstall is not None:
        project.uninstall(args.uninstall)

    if args.sync:
        project.sync_depends()

    if args.run is not None:
        project.run(args.run)


if __name__ == "__main__":
    main()
