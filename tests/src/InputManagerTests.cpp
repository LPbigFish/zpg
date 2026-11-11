#define GLFW_INCLUDE_NONE
#include "zpg/core/InputManager.hpp"
#include <glad/gl.h>
#include <gtest/gtest.h>
#include <vector>

namespace {

struct PixelRead {
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
    GLenum format;
    GLenum type;
};

class StencilInputTest: public testing::Test {
    PFNGLREADPIXELSPROC original_read_pixels{glad_glReadPixels};
    PFNGLREADBUFFERPROC original_read_buffer{glad_glReadBuffer};

  protected:
    inline static std::vector<PixelRead> reads;
    inline static std::vector<GLenum> read_buffers;
    zpg::core::InputManager input;

    auto SetUp() -> void override {
        reads.clear();
        read_buffers.clear();
        glad_glReadPixels = [](GLint x,
                               GLint y,
                               GLsizei width,
                               GLsizei height,
                               GLenum format,
                               GLenum type,
                               void*) -> void {
            reads.push_back({x, y, width, height, format, type});
        };
        glad_glReadBuffer
            = [](GLenum buffer) -> void { read_buffers.push_back(buffer); };
    }

    auto TearDown() -> void override {
        glad_glReadPixels = original_read_pixels;
        glad_glReadBuffer = original_read_buffer;
    }
};

TEST_F(
    StencilInputTest, StationaryClickReadsOnceEvenIfReleasedBeforeRendering
) {
    input.on_cursor({10.0, 20.0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0});

    auto camera = zpg::core::Camera{};
    input.update(camera, 0.f);
    EXPECT_TRUE(reads.empty()); // No reads before the frame is rendered.

    input.process_stencil_click({800, 600}, {800, 600});

    ASSERT_EQ(reads.size(), 3U);
    EXPECT_EQ(read_buffers, std::vector<GLenum>{GL_BACK});
    for (const auto& read : reads) {
        EXPECT_EQ(read.x, 10);
        EXPECT_EQ(read.y, 579);
        EXPECT_EQ(read.width, 1);
        EXPECT_EQ(read.height, 1);
    }
    EXPECT_EQ(reads[0].format, GL_RGBA);
    EXPECT_EQ(reads[0].type, GL_UNSIGNED_BYTE);
    EXPECT_EQ(reads[1].format, GL_DEPTH_COMPONENT);
    EXPECT_EQ(reads[1].type, GL_FLOAT);
    EXPECT_EQ(reads[2].format, GL_STENCIL_INDEX);
    EXPECT_EQ(reads[2].type, GL_UNSIGNED_INT);

    input.process_stencil_click({800, 600}, {800, 600});
    EXPECT_EQ(reads.size(), 3U);
    EXPECT_EQ(read_buffers.size(), 1U);
}

TEST_F(StencilInputTest, HoldingLeftButtonDoesNotRepeatReadback) {
    input.on_cursor({10.0, 20.0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.process_stencil_click({800, 600}, {800, 600});
    input.process_stencil_click({800, 600}, {800, 600});
    EXPECT_EQ(reads.size(), 3U);

    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.process_stencil_click({800, 600}, {800, 600});
    EXPECT_EQ(reads.size(), 6U);
}

TEST_F(StencilInputTest, ScalesClickPositionAndKeepsItWhenCursorMoves) {
    input.on_cursor({10.75, 20.25});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.on_cursor({200.0, 300.0});
    input.process_stencil_click({800, 600}, {1600, 1200});

    ASSERT_EQ(reads.size(), 3U);
    EXPECT_EQ(reads[2].x, 21);
    EXPECT_EQ(reads[2].y, 1159);
}

TEST_F(StencilInputTest, MapsWindowCornersToValidFramebufferPixels) {
    input.on_cursor({0.0, 0.0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.process_stencil_click({800, 600}, {1600, 1200});

    ASSERT_EQ(reads.size(), 3U);
    EXPECT_EQ(reads.back().x, 0);
    EXPECT_EQ(reads.back().y, 1199);

    input.on_cursor({799.9, 599.9});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.process_stencil_click({800, 600}, {1600, 1200});

    ASSERT_EQ(reads.size(), 6U);
    EXPECT_EQ(reads.back().x, 1599);
    EXPECT_EQ(reads.back().y, 0);
}

TEST_F(StencilInputTest, IgnoresOutOfWindowClicks) {
    const std::vector<zpg::core::events::CursorEvent> positions{
      {-1.0, 20.0}, {10.0, -1.0}, {800.0, 20.0}, {10.0, 600.0}
    };
    for (const auto& position : positions) {
        input.on_cursor(position);
        input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
        input.process_stencil_click({800, 600}, {1600, 1200});
    }
    EXPECT_TRUE(reads.empty());
    EXPECT_TRUE(read_buffers.empty());
}

TEST_F(StencilInputTest, DiscardsClicksWhenWindowOrFramebufferIsMinimized) {
    input.on_cursor({10.0, 20.0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.process_stencil_click({0, 600}, {800, 600});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.process_stencil_click({800, 600}, {800, 0});
    input.process_stencil_click({800, 600}, {800, 600});
    EXPECT_TRUE(reads.empty());
}

TEST_F(StencilInputTest, CancelsPendingClickOnFocusLossOrSceneSwitch) {
    input.on_cursor({10.0, 20.0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.on_focus(false);
    input.process_stencil_click({800, 600}, {800, 600});
    EXPECT_TRUE(reads.empty());

    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.reset_cursor();
    input.process_stencil_click({800, 600}, {800, 600});
    EXPECT_TRUE(reads.empty());
}

TEST(InputManager, LeftClicksDoNotInterruptRightButtonLook) {
    auto input = zpg::core::InputManager{};
    auto camera = zpg::core::Camera{};
    auto expected = zpg::core::Camera{};

    input.on_button({GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, 0});
    input.on_cursor({10.0, 10.0});
    input.on_cursor({20.0, 5.0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0});
    input.on_button({GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0});
    input.on_cursor({30.0, 0.0});
    input.update(camera, 0.f);
    expected.look(20.f, 10.f);

    EXPECT_EQ(camera.front(), expected.front());

    input.on_button({GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE, 0});
    input.on_cursor({100.0, 100.0});
    input.on_cursor({200.0, 200.0});
    input.update(camera, 0.f);
    EXPECT_EQ(camera.front(), expected.front());
}

} // namespace
