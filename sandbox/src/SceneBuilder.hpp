#pragma once

#include "ModelManager.hpp"
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
        const std::shared_ptr<zg::ShaderProgram>& ambient_program,
        const std::shared_ptr<zg::ShaderProgram>& phong_program,
        const std::shared_ptr<zg::ShaderProgram>& blinn_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;
    static auto create_slenderman_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_forest_clock_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& tree_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program,
        const std::shared_ptr<zg::ShaderProgram>& clock_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_four_ball_scene(
        const std::shared_ptr<zg::ShaderProgram>& ambient_program,
        const std::shared_ptr<zg::ShaderProgram>& lambert_program,
        const std::shared_ptr<zg::ShaderProgram>& phong_program,
        const std::shared_ptr<zg::ShaderProgram>& blinn_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_normals_scene(
        const std::shared_ptr<zg::ShaderProgram>& wrong_program,
        const std::shared_ptr<zg::ShaderProgram>& right_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_specular_scene(
        const std::shared_ptr<zg::ShaderProgram>& wrong_program,
        const std::shared_ptr<zg::ShaderProgram>& right_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_night_forest_scene(
        const std::shared_ptr<zg::ShaderProgram>& forest_program,
        const std::shared_ptr<zg::ShaderProgram>& ground_program,
        const std::shared_ptr<zg::ShaderProgram>& firefly_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_plain_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& skybox_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

    static auto create_eb408_scene(
        const std::shared_ptr<zg::ShaderProgram>& shader_program,
        const std::shared_ptr<zg::ShaderProgram>& fixed_shader_program
    ) -> std::unique_ptr<zc::Scene>;

  private:
    static auto model(ModelId id) -> std::shared_ptr<const zg::Model> {
        return ModelManager::instance().get(id);
    }

    static auto add_signature(
        const std::unique_ptr<zc::Scene>& scene,
        const std::shared_ptr<zg::ShaderProgram>& shader_program
    ) -> void;
};
