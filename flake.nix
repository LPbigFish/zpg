{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      perSystem =
        { pkgs, system, ... }:
        let
          llvmPackages = pkgs.llvmPackages;
        in
        {
          devShells.default = pkgs.mkShell.override { stdenv = llvmPackages.stdenv; } {
            packages = with pkgs; [
              glfw
              glm
              libglvnd
              llvmPackages.clang
              llvmPackages.clang-tools
              llvmPackages.lldb
              llvmPackages.lld
              cmake
              ninja
              pkg-config
              renderdoc
              apitrace
              mesa-demos
              glslang
              spirv-tools
              valgrind
              heaptrack
              cppcheck
              include-what-you-use
              cmake-format
              gtest
            ];
            CC = "clang";
            CXX = "clang++";
            shellHook = ''
              export LD_LIBRARY_PATH="/run/opengl-driver/lib''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
              printf 'ZPG Space dev shell | clang %s | lldb %s | cmake %s\n' \
                "$(clang --version | sed -n '1s/.*version \([^ ]*\).*/\1/p')" \
                "$(lldb --version | sed -n '1s/.*version \([^ ]*\).*/\1/p')" \
                "$(cmake --version | sed -n '1s/.*version \([^ ]*\).*/\1/p')"
            '';
          };
        };
    };
}
