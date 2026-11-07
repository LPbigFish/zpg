#include "ModelManager.hpp"
#include "models/Fiona.hpp"
#include "models/SM_Door.hpp"
#include "models/SM_DoorFrame.hpp"
#include "models/SM_Model.hpp"
#include "models/Shrek.hpp"
#include "models/bushes.hpp"
#include "models/clock.hpp"
#include "models/earth.hpp"
#include "models/moon.hpp"
#include "models/plain.hpp"
#include "models/satelite.hpp"
#include "models/sphere.hpp"
#include "models/sun.hpp"
#include "models/text.hpp"
#include "models/tree.hpp"
#include "models/uv_plain.hpp"
#include <array>
#include <glm/ext/vector_float3.hpp>
#include <span>
#include <vector>

namespace {

constexpr std::array<float, 18> TRIANGLE_A{
  -0.5f,
  -0.5f,
  0.f,
  1.f,
  0.f,
  0.f,
  0.5f,
  -0.5f,
  0.f,
  0.f,
  1.f,
  0.f,
  0.f,
  0.5f,
  0.f,
  0.f,
  0.f,
  1.f,
};

constexpr std::array<float, 18> TRIANGLE_B{
  -0.5f,
  -0.5f,
  0.f,
  0.f,
  1.f,
  1.f,
  0.5f,
  -0.5f,
  0.f,
  1.f,
  0.f,
  1.f,
  0.f,
  0.5f,
  0.f,
  1.f,
  1.f,
  0.f,
};

[[nodiscard]] auto make_colored_tree() -> std::vector<float> {
    constexpr std::size_t SOURCE_STRIDE = 6;
    constexpr std::size_t TRUNK_VERTICES = 36'444;
    const std::span<const float> source{models::tree.vertices};

    std::vector<float> result;
    result.reserve(source.size() / SOURCE_STRIDE * 9);

    for (std::size_t i = 0; i < source.size() / SOURCE_STRIDE; ++i) {
        const auto offset = i * SOURCE_STRIDE;
        const auto color = i < TRUNK_VERTICES ? glm::vec3{0.36f, 0.20f, 0.08f}
                                              : glm::vec3{0.07f, 0.38f, 0.12f};
        result.insert(
            result.end(),
            {source[offset],
             source[offset + 1],
             source[offset + 2],
             color.r,
             color.g,
             color.b,
             source[offset + 3],
             source[offset + 4],
             source[offset + 5]}
        );
    }
    return result;
}

[[nodiscard]] auto make_colored_bush() -> std::vector<float> {
    constexpr std::size_t SOURCE_STRIDE = 6;
    constexpr glm::vec3 COLOR{0.04f, 0.28f, 0.08f};
    const std::span<const float> source{models::bushes.vertices};

    std::vector<float> result;
    result.reserve(source.size() / SOURCE_STRIDE * 9);

    for (std::size_t i = 0; i < source.size(); i += SOURCE_STRIDE) {
        result.insert(
            result.end(),
            {source[i],
             source[i + 1],
             source[i + 2],
             COLOR.r,
             COLOR.g,
             COLOR.b,
             source[i + 3],
             source[i + 4],
             source[i + 5]}
        );
    }
    return result;
}

} // namespace

auto ModelManager::instance() -> ModelManager& {
    static ModelManager manager;
    return manager;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
auto ModelManager::preload() -> std::expected<void, std::string> {
    namespace zg = zpg::graphics;
    using zg::VertexLayout;

    clear();
    const auto tree_colored = make_colored_tree();
    const auto bush_colored = make_colored_bush();

    const auto load_all = [&]() -> std::expected<void, std::string> {
        if (auto r
            = load(ModelId::TriangleA, {TRIANGLE_A, VertexLayout::PosColor});
            !r) {
            return r;
        }
        if (auto r
            = load(ModelId::TriangleB, {TRIANGLE_B, VertexLayout::PosColor});
            !r) {
            return r;
        }
        if (auto r = load(ModelId::Text, models::text); !r) {
            return r;
        }
        if (auto r = load(ModelId::SphereColor, models::sphere_color); !r) {
            return r;
        }
        if (auto r = load(ModelId::SphereNormal, models::sphere); !r) {
            return r;
        }
        if (auto r = load(ModelId::Plain, models::plain); !r) {
            return r;
        }
        if (auto r = load(
                ModelId::TreeColored,
                {std::span{tree_colored},
                 VertexLayout::PosColorNormal,
                 tree_colored.size()}
            );
            !r) {
            return r;
        }
        if (auto r = load(ModelId::BushColor, models::bushes); !r) {
            return r;
        }
        if (auto r = load(
                ModelId::BushColored,
                {std::span{bush_colored},
                 VertexLayout::PosColorNormal,
                 bush_colored.size()}
            );
            !r) {
            return r;
        }
        if (auto r = load(ModelId::Clock, models::clock); !r) {
            return r;
        }
        if (auto r = load(ModelId::Slenderman, models::slenderman); !r) {
            return r;
        }
        if (auto r = load(ModelId::Sun, models::sun); !r) {
            return r;
        }
        if (auto r = load(ModelId::Earth, models::earth); !r) {
            return r;
        }
        if (auto r = load(ModelId::Moon, models::moon); !r) {
            return r;
        }
        if (auto r = load(ModelId::Satellite, models::satellite); !r) {
            return r;
        }
        if (auto r = load(ModelId::Fiona, models::fiona); !r) {
            return r;
        }
        if (auto r = load(ModelId::Shrek, models::shrek); !r) {
            return r;
        }
        if (auto r = load(ModelId::Door, models::sm_door); !r) {
            return r;
        }
        if (auto r = load(ModelId::DoorFrame, models::sm_doorframe); !r) {
            return r;
        }
        return load(ModelId::UvPlain, models::uv_plain);
    }();

    if (!load_all) {
        clear();
    }
    return load_all;
}

auto ModelManager::get(ModelId id) const
    -> const std::shared_ptr<const zpg::graphics::Model>& {
    return models.at(static_cast<std::size_t>(id));
}

auto ModelManager::clear() -> void {
    models.fill(nullptr);
}
