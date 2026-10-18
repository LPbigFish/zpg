#pragma once

#include "models/bushes.h"
#include "models/clock.h"
#include "models/plain.h"
#include "models/sphere.h"
#include "models/tree.h"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <memory>

namespace zg = zpg::graphics;

namespace zc = zpg::core;

struct SceneBuilder {
    static auto create_triangles_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program
    ) -> std::unique_ptr<zc::Scene>;
    static auto
    create_ball_scene(const std::shared_ptr<zg::ShaderProgram>& shader_program)
        -> std::unique_ptr<zc::Scene>;
    static auto create_basic_forest_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& shader_program_2
    ) -> std::unique_ptr<zc::Scene>;
    static auto
    create_login_scene(const std::shared_ptr<zg::ShaderProgram>& shader_program)
        -> std::unique_ptr<zc::Scene>;
    static auto create_solar_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;
    static auto create_slenderman_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_forest_clock_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program,
        const std::shared_ptr<zg::ShaderProgram>& clock_shader_program
    ) -> std::unique_ptr<zc::Scene>;

  private:
    static auto add_signature(
        const std::unique_ptr<zc::Scene>& scene,
        const std::shared_ptr<zg::ShaderProgram>& shader_program
    ) -> void;
};
