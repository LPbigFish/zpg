# ZPG

---

## Build Root

Enter the development shell with `nix develop` (or `direnv reload`). After
updating `flake.lock`, run `cmake --fresh --preset debug` to replace cached
compiler and library paths. The shell's glibc must support the host graphics
driver; older nixpkgs can cause EGL/GLX initialization to fail even when host
OpenGL tools work.

```sh
cmake --preset debug
  cmake --build --preset debug
```

## Build specific targets

```sh
    cmake --build --preset debug --target zpg_game
    cmake --build --preset debug --target zpg_sandbox
    cmake --build --preset debug --target zpg_tests
```

## Run tests

```sh
  ctest --preset debug
```

## Run binaries

```sh
  ./build/debug/bin/zpg_game
  ./build/debug/bin/zpg_sandbox
  ./build/debug/bin/zpg_tests
```
