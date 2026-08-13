I can't bother to write down contribution instructions, so here's some for every user!

**Step 1**  
Download a file from the releases tab. (`*.exe` for most users, any other file for Linux. Check the arch!)

**Step 2**  
Place that file anywhere your OS's `PATH` variable can reach! Also rename it to `pyfloor.exe` (just `pyfloor` on Linux) for ease of use.

**Step 3**  
Run `pyfloor` and pass a directory/folder path, or let it set up a new project at your current location. Your choice!

**Step 4**  
If on Windows (aka Winslop), **PyFloor** has not been implemented yet.  
If you're on Linux, it's your lucky day!

Run these commands to finish initialization and run the test project:
```bash
./pyfloor init   # Pass --pro or -p if you don't want the test project
./pyfloor run    # Will not work if --pro/-p is passed!
```

You can also run this command to view all your yummy options!
```bash
./pyfloor
```

Be careful to not run your `PATH`'s `pyfloor`, as of now it only exists to copy the shell scripts.

**OPTIONAL Step 5**  
Open up `pyfloor.config` in your favorite text editor, and change some settings! `nvim` (neovim) is recommended, but obviously required.

**OPTIONAL Step 6**  
Run this command to build your program with PyInstaller!
```bash
./pyfloor build
```

Once you get bored of having a million build-junk files, you can run `./pyfloor clean` to remove them.
