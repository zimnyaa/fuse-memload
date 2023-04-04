# `fuse-memfile`
a way to run ELF in-memory without allowing people to read the file (from the beginning)
better explained at https://tishina.in/execution/replacing-memfd-with-fuse

# deps&usage
the `makefile` makes everything straightforward:
```
~# apt install libfuse-dev python3

~# make # to build and run
~# make clean # to unmount and remove the artifacts
```

# references
https://github.com/fntlnz/fuse-example
