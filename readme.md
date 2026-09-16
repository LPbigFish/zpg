# ZPG

## Root

cmake --preset debug
  cmake --build --preset debug

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
