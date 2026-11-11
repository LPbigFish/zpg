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

$ X_L -(modelMatrix * X_L)-> X_{World} -(ViewMatrix * X_W)-> X_{Camera} -(ProjectionMatrix * X_{Camera})-> X_S $

## Useful Links

[Affine](https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fi.ytimg.com%2Fvi%2FB1zCAibMcCM%2Fmaxresdefault.jpg&f=1&nofb=1&ipt=f47d4d026d6f73b0211e6c7365345e177ca67b4d7c7588a7d050579d030c12ff&ipo=images)

[Rotation Matrix](https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fimage2.slideserve.com%2F3643947%2Fthe-3-rotation-matrices-l.jpg&f=1&nofb=1&ipt=b7d54f31d2250c268a0868b7f47c2ed85501a5669ad625dbd628c355e7431f6a&ipo=images)

[Matrix Calc](https://matrixcalc.org)
