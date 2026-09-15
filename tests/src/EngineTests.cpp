#include <gtest/gtest.h>

#include "zpg/core/Application.hpp"

TEST(EngineScaffold, ApplicationCanBeConstructed) {
    zpg::core::Application{};
    SUCCEED();
}

TEST(EngineScaffold, NamespaceIsAvailable) {
    EXPECT_TRUE(true);
}
