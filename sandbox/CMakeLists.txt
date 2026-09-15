add_executable(zpg_sandbox src/main.cpp)
target_link_libraries(zpg_sandbox PRIVATE ZPG::Engine)
zpg_configure_target(zpg_sandbox)
