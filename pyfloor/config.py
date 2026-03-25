import os


class PyFloorConfig:
    path: str
    values: dict[str, str]
    loaded: bool

    def __init__(self, path: str, values: dict[str, str] = {}):
        object.__setattr__(self, "path", path)
        object.__setattr__(self, "values", values)
        object.__setattr__(self, "loaded", False)

    def __setattr__(self, name: str, value: str, save=True):
        if name in {"path", "values", "loaded"}:
            object.__setattr__(self, name, value)
            return

        self.values[name] = str(value)
        if save:
            self.save()

    def __getattr__(self, name: str):
        if not self.loaded:
            self.load()

        return self.values.get(name)

    def load(self, fallback_existing=False):
        if not os.path.isfile(self.path):
            print("Saving a default config...")
            self.save()

        if not fallback_existing:
            object.__setattr__(self, "values", {})

        with open(self.path, "r") as f:
            for rawline in f:
                line = rawline.strip()
                if not line or line.startswith(";") or line.startswith("#"):
                    continue

                if "=" not in line:
                    continue

                k, v = line.split("=", 1)
                self.values[k] = v

        object.__setattr__(self, "loaded", True)

    def save(self):
        with open(self.path, "w") as f:
            items = list(self.values.items())
            for i, (k, v) in enumerate(items):
                if i > 0:
                    f.write("\n")

                f.write(f"{k}={v}")
