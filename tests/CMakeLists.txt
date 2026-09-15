find_package(GTest CONFIG REQUIRED)

add_executable(zpg_tests src/EngineTests.cpp)
target_link_libraries(zpg_tests PRIVATE ZPG::Engine GTest::gtest_main)
zpg_configure_target(zpg_tests)

if(ZPG_ENABLE_SANITIZERS AND ZPG_DISABLE_LEAK_DETECTION_FOR_TESTS)
    # LeakSanitizer cannot run when the test process is traced by a debugger
    # or managed test runner. ASan and UBSan remain enabled.
    set_property(TARGET zpg_tests PROPERTY TEST_LAUNCHER
        "${CMAKE_COMMAND};-E;env;LSAN_OPTIONS=detect_leaks=0"
    )
endif()

include(GoogleTest)
gtest_discover_tests(zpg_tests
    DISCOVERY_MODE PRE_TEST
)
