add_executable(zpg_game src/main.cpp)
target_link_libraries(zpg_game PRIVATE ZPG::Engine)
zpg_configure_target(zpg_game)
