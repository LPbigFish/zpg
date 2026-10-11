#include "zpg/graphics/Rotation.hpp"
#include "zpg/graphics/RotationRepeat.hpp"
#include "zpg/graphics/Scale.hpp"
#include "zpg/graphics/TransformationComposite.hpp"
#include "zpg/graphics/Translation.hpp"
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <gtest/gtest.h>

namespace zg = zpg::graphics;

namespace {

auto matrices_equal(const glm::mat4& a, const glm::mat4& b) -> bool {
    for (auto column = 0; column < 4; ++column) {
        for (auto row = 0; row < 4; ++row) {
            if (std::abs(a[column][row] - b[column][row]) > 1e-5F) {
                return false;
            }
        }
    }
    return true;
}

const glm::vec3 x_axis{1.f, 0.f, 0.f};
const glm::vec3 y_axis{0.f, 1.f, 0.f};
const glm::vec3 z_axis{0.f, 0.f, 1.f};

auto rotation_angle_z(const glm::mat4& m) -> float {
    return std::atan2(m[1][0], m[0][0]);
}

} // namespace

// An empty composite is the identity matrix.
TEST(TransformationComposite, EmptyIsIdentity) {
    auto composite = zg::TransformationComposite{};
    EXPECT_TRUE(matrices_equal(composite.get_matrix(), glm::mat4{1.f}));
}

// Children multiply left to right in the order they were added: adding
// [translation, rotation] yields T * R, the classic model matrix order where
// the first added child is the outermost (applied to the vertex last).
TEST(TransformationComposite, MatchesManualMatrixComposition) {
    auto composite = zg::TransformationComposite{};
    composite.add(std::make_unique<zg::Translation>(glm::vec3{2.f, 0.f, 0.f}));
    composite.add(std::make_unique<zg::Rotation>(z_axis, 90.f));

    const auto expected
        = glm::translate(glm::mat4{1.f}, glm::vec3{2.f, 0.f, 0.f})
        * glm::rotate(glm::mat4{1.f}, glm::radians(90.f), z_axis);

    EXPECT_TRUE(matrices_equal(composite.get_matrix(), expected));
}

// The whole point of task 4: swapping translate and rotate must produce a
// different matrix (matrix multiplication is not commutative).
TEST(TransformationComposite, OrderSwapProducesDifferentMatrix) {
    auto translate_first = zg::TransformationComposite{};
    translate_first.add(
        std::make_unique<zg::Translation>(glm::vec3{2.f, 0.f, 0.f})
    );
    translate_first.add(std::make_unique<zg::Rotation>(z_axis, 90.f));

    auto rotate_first = zg::TransformationComposite{};
    rotate_first.add(std::make_unique<zg::Rotation>(z_axis, 90.f));
    rotate_first.add(
        std::make_unique<zg::Translation>(glm::vec3{2.f, 0.f, 0.f})
    );

    EXPECT_FALSE(
        matrices_equal(translate_first.get_matrix(), rotate_first.get_matrix())
    );
}

// add_front inserts the outermost transform (leftmost factor).
TEST(TransformationComposite, AddFrontPutsChildFirst) {
    auto composite = zg::TransformationComposite{};
    composite.add(std::make_unique<zg::Scale>(2.f));
    composite.add_front(
        std::make_unique<zg::Translation>(glm::vec3{1.f, 0.f, 0.f})
    );

    const auto expected
        = glm::translate(glm::mat4{1.f}, glm::vec3{1.f, 0.f, 0.f})
        * glm::scale(glm::mat4{1.f}, glm::vec3{2.f});

    EXPECT_TRUE(matrices_equal(composite.get_matrix(), expected));
}

// Composite is itself a component, so it can contain other composites.
TEST(TransformationComposite, SupportsNesting) {
    auto inner = std::make_unique<zg::TransformationComposite>();
    inner->add(std::make_unique<zg::Scale>(2.f));
    inner->add(std::make_unique<zg::Translation>(glm::vec3{1.f, 0.f, 0.f}));

    auto outer = zg::TransformationComposite{};
    outer.add(std::move(inner));
    outer.add(std::make_unique<zg::Rotation>(y_axis, 90.f));

    // Left-to-right product: (S * T) * R.
    const auto expected
        = glm::scale(glm::mat4{1.f}, glm::vec3{2.f})
        * glm::translate(glm::mat4{1.f}, glm::vec3{1.f, 0.f, 0.f})
        * glm::rotate(glm::mat4{1.f}, glm::radians(90.f), y_axis);

    EXPECT_TRUE(matrices_equal(outer.get_matrix(), expected));
}

// Cloning deep-copies the children and reproduces the same matrix.
TEST(TransformationComposite, CloneDeepCopies) {
    auto composite = std::make_unique<zg::TransformationComposite>();
    composite->add(std::make_unique<zg::Translation>(glm::vec3{2.f, 0.f, 0.f}));
    composite->add(std::make_unique<zg::Rotation>(z_axis, 90.f));

    const auto copy = composite->clone();

    composite->get_children().clear(); // original must not affect the copy

    const auto expected
        = glm::translate(glm::mat4{1.f}, glm::vec3{2.f, 0.f, 0.f})
        * glm::rotate(glm::mat4{1.f}, glm::radians(90.f), z_axis);
    EXPECT_TRUE(matrices_equal(copy->get_matrix(), expected));
}

// Static leaves precompute their matrix in the constructor.
TEST(StaticLeaves, ProduceExpectedMatrices) {
    auto translation = zg::Translation(glm::vec3{1.f, 2.f, 3.f});
    auto scale = zg::Scale(0.5f);
    auto rotation = zg::Rotation(x_axis, 30.f);

    EXPECT_TRUE(matrices_equal(
        translation.get_matrix(),
        glm::translate(glm::mat4{1.f}, glm::vec3{1.f, 2.f, 3.f})
    ));
    EXPECT_TRUE(matrices_equal(
        scale.get_matrix(), glm::scale(glm::mat4{1.f}, glm::vec3{0.5f})
    ));
    EXPECT_TRUE(matrices_equal(
        rotation.get_matrix(),
        glm::rotate(glm::mat4{1.f}, glm::radians(30.f), x_axis)
    ));
}

// Dynamic leaf: the angle grows with the elapsed time, so two consecutive
// evaluations must show a small non-negative angle increase (no artificial
// waiting, the calls are microseconds apart).
TEST(RotationRepeat, EvolvesWithTime) {
    auto spin = zg::RotationRepeat(z_axis, 45.f);

    const auto angle_before = rotation_angle_z(spin.get_matrix());
    const auto angle_after = rotation_angle_z(spin.get_matrix());

    EXPECT_GE(angle_after, angle_before - 1e-4F);
    EXPECT_LT(angle_after - angle_before, glm::radians(5.f));
}
