#include "Application.hpp"
#include "ModelManager.hpp"
#include "SceneBuilder.hpp"
#include "zpg/core/GFObserver.hpp"
#include "zpg/core/Renderer.hpp"
#include "zpg/core/Scene.hpp"
#include "zpg/graphics/Shader.hpp"
#include "zpg/graphics/ShaderProgram.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>
#include <utility>
#define GLAD_GL_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <memory>
#include <print>

namespace zpg::core {

// NOLINTNEXTLINE
namespace zg = zpg::graphics;

namespace zc = zpg::core;

Application::Application():
    glfw_context{GLFWContext()}, window{std::make_unique<Window>()} {}

Application::~Application() {
    scenes.clear();
    ModelManager::instance().clear();
}

auto Application::print_info() noexcept -> void {
    const auto* version
        // NOLINTNEXTLINE
        = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const auto* vendor
        // NOLINTNEXTLINE
        = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const auto* renderer
        // NOLINTNEXTLINE
        = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    // NOLINTNEXTLINE
    const auto* shading_language_version = reinterpret_cast<const char*>(
        glGetString(GL_SHADING_LANGUAGE_VERSION)
    );

    std::println("OpenGL Version: {}", version);
    std::println("Vendor {}", vendor);
    std::println("Renderer {}", renderer);
    std::println("GLSL {}", shading_language_version);
    int major{};
    int minor{};
    int revision{};
    glfwGetVersion(&major, &minor, &revision);
    std::println("Using GLFW {}.{}.{}", major, minor, revision);
}

auto Application::init() -> void {
    Application::create_callbacks();
    window->make_context();

    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        throw std::runtime_error{"GLAD initialization failed"};
    }

    int width{};
    int height{};
    glfwGetFramebufferSize(window->get(), &width, &height);

    Renderer::set_viewport(width, height);

    double cursor_x{};
    double cursor_y{};
    glfwGetCursorPos(window->get(), &cursor_x, &cursor_y);
    input_manager.on_cursor({cursor_x, cursor_y});

    Application::print_info();
    Application::refresh_window_title();
}

auto Application::create_callbacks() -> void {
    namespace gfo = zpg::core::gfobserver;
    namespace events = gfo::events;

    auto& observer = gfo::GlfwObserver::instance();
    input_manager.subscribe(observer);

    // ERRORs
    observer.subscribe<events::Error>([](const events::Error& event) -> void {
        std::println(
            stderr, "error_callback [{}]: {}", event.error, event.description
        );
    });

    // KEY EVENTS
    observer.subscribe<events::KeyEvent>(
        [this](const events::KeyEvent& event) -> void {
            if (event.key > GLFW_KEY_0 && event.key <= GLFW_KEY_9
                && event.action == GLFW_PRESS) {
                auto index = static_cast<std::size_t>(event.key - GLFW_KEY_1);
                this->switch_scene(index);
                std::println("Switched to scene {}", index);
            }

            if (event.action != GLFW_PRESS) {
                return;
            }

            switch (event.key) {
                case GLFW_KEY_F3:
                    this->update_viewport_on_resize
                        = !this->update_viewport_on_resize;
                    if (this->update_viewport_on_resize) {
                        int width{};
                        int height{};
                        glfwGetFramebufferSize(
                            this->window->get(), &width, &height
                        );
                        Renderer::set_viewport(width, height);
                    }
                    std::println(
                        "Viewport update on resize: {}",
                        this->update_viewport_on_resize ? "ON" : "OFF"
                    );
                    this->refresh_window_title();
                    break;
                case GLFW_KEY_F4:
                    this->update_projection_on_resize
                        = !this->update_projection_on_resize;
                    if (this->update_projection_on_resize
                        && !this->scenes.empty()) {
                        int width{};
                        int height{};
                        glfwGetFramebufferSize(
                            this->window->get(), &width, &height
                        );
                        this->scenes[this->current_scene_index]
                            ->get_camera()
                            .resize(width, height);
                    }
                    std::println(
                        "Projection update on resize: {}",
                        this->update_projection_on_resize ? "ON" : "OFF"
                    );
                    this->refresh_window_title();
                    break;
                default: break;
            }
        }
    );

    // FRAME BUFFER RESIZE
    observer.subscribe<events::FrameBufferSizeEvent>(
        [this](const events::FrameBufferSizeEvent& event) -> void {
            if (this->update_viewport_on_resize) {
                Renderer::set_viewport(event.width, event.height);
            }

            // Task 2c/2d: break/restore the resize handling on demand.
            if (this->update_projection_on_resize && !this->scenes.empty()) {
                this->scenes[this->current_scene_index]->get_camera().resize(
                    event.width, event.height
                );
            }
        }
    );
}

auto Application::add_scene(std::unique_ptr<zc::Scene> scene) -> void {
    scenes.push_back(std::move(scene));
}

auto Application::switch_scene(std::size_t index) -> void {
    if (index >= scenes.size()) {
        return;
    }

    current_scene_index = index;
    input_manager.reset_cursor();

    int width{};
    int height{};
    glfwGetFramebufferSize(window->get(), &width, &height);

    auto& scene = *scenes[current_scene_index];

    if (this->update_projection_on_resize) {
        scene.get_camera().resize(width, height);
    }
    scene.activate();
}

auto Application::refresh_window_title() const -> void {
    glfwSetWindowTitle(
        this->window->get(),
        std::format(
            "ZPG  [F3 viewport:{}]  [F4 perspektiva:{}]",
            this->update_viewport_on_resize ? "on" : "off",
            this->update_projection_on_resize ? "on" : "off"
        )
            .c_str()
    );
}

auto Application::create_scenes() -> void {
    if (auto result = ModelManager::instance().preload(); !result) {
        throw std::runtime_error{result.error()};
    }

    auto vs = zg::VertexShader::create("shaders/transCam.vert");
    auto vs_fixed = zg::VertexShader::create("shaders/trans.vert");
    auto fs = zg::FragmentShader::create("shaders/transformation.frag");
    auto fs_solid = zg::FragmentShader::create("shaders/solidColor.frag");
    auto vs_clock = zg::VertexShader::create("shaders/clock.vert");
    auto fs_clock = zg::FragmentShader::create("shaders/clock.frag");

    auto fs_tree = zg::FragmentShader::create("shaders/tree.frag");
    auto vs_lighting = zg::VertexShader::create("shaders/lighting.vert");
    auto fs_ambient = zg::FragmentShader::create("shaders/ambient.frag");
    auto fs_lambert = zg::FragmentShader::create("shaders/lambert.frag");
    auto fs_phong = zg::FragmentShader::create("shaders/phong.frag");
    auto fs_blinn = zg::FragmentShader::create("shaders/blinn.frag");
    auto fs_normals = zg::FragmentShader::create("shaders/normals.frag");
    auto fs_phong_wrong = zg::FragmentShader::create("shaders/phongWrong.frag");
    auto fs_forest = zg::FragmentShader::create("shaders/forestNight.frag");
    auto fs_unlit = zg::FragmentShader::create("shaders/unlit.frag");

    auto vs_texturing = zg::VertexShader::create("shaders/lighting.vert");
    auto fs_texturing = zg::FragmentShader::create("shaders/texturing.frag");

    if (!vs) {
        std::println(stderr, "{}", vs.error());
        return;
    }
    if (!vs_fixed) {
        std::println(stderr, "{}", vs_fixed.error());
        return;
    }
    if (!fs) {
        std::println(stderr, "{}", fs.error());
        return;
    }
    if (!fs_solid) {
        std::println(stderr, "{}", fs_solid.error());
        return;
    }
    if (!vs_clock) {
        std::println(stderr, "{}", vs_clock.error());
        return;
    }
    if (!fs_clock) {
        std::println(stderr, "{}", fs_clock.error());
        return;
    }
    if (!fs_tree) {
        std::println(stderr, "{}", fs_tree.error());
        return;
    }
    if (!vs_lighting) {
        std::println(stderr, "{}", vs_lighting.error());
        return;
    }
    if (!fs_ambient) {
        std::println(stderr, "{}", fs_ambient.error());
        return;
    }
    if (!fs_lambert) {
        std::println(stderr, "{}", fs_lambert.error());
        return;
    }
    if (!fs_phong) {
        std::println(stderr, "{}", fs_phong.error());
        return;
    }
    if (!fs_blinn) {
        std::println(stderr, "{}", fs_blinn.error());
        return;
    }
    if (!fs_normals) {
        std::println(stderr, "{}", fs_normals.error());
        return;
    }
    if (!fs_phong_wrong) {
        std::println(stderr, "{}", fs_phong_wrong.error());
        return;
    }
    if (!fs_forest) {
        std::println(stderr, "{}", fs_forest.error());
        return;
    }
    if (!fs_unlit) {
        std::println(stderr, "{}", fs_unlit.error());
        return;
    }
    if (!vs_texturing) {
        std::println(stderr, "{}", vs_texturing.error());
        return;
    }
    if (!fs_texturing) {
        std::println(stderr, "{}", fs_texturing.error());
        return;
    }

    auto shader_program = std::make_shared<zg::ShaderProgram>(*vs, *fs);
    auto fixed_shader_program
        = std::make_shared<zg::ShaderProgram>(*vs_fixed, *fs);
    auto tree_program = std::make_shared<zg::ShaderProgram>(*vs, *fs_tree);
    auto shader_program_2 = std::make_shared<zg::ShaderProgram>(*vs, *fs_solid);
    auto clock_shader_program
        = std::make_shared<zg::ShaderProgram>(*vs_clock, *fs_clock);

    auto ambient_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_ambient);
    auto lambert_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_lambert);
    auto phong_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_phong);
    auto blinn_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_blinn);

    auto texturing_program
        = std::make_shared<zg::ShaderProgram>(*vs_texturing, *fs_texturing);

    auto skybox_program = std::make_shared<zg::ShaderProgram>(
        *zg::VertexShader::create("shaders/skybox.vert"),
        *zg::FragmentShader::create("shaders/skybox.frag")
    );

    // Wrong/Correct normals
    auto normals_wrong_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_normals);
    auto normals_right_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_normals);

    normals_wrong_program->set_shader_program();
    normals_wrong_program->set_uniform(
        normals_wrong_program->get_uniform_location("wrongNormals"), 1
    );
    normals_wrong_program->unset_shader_program();

    auto specular_wrong_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_phong_wrong);
    auto specular_right_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_phong);

    for (const auto& program :
         {specular_wrong_program, specular_right_program}) {
        program->set_shader_program();
        program->set_uniform(program->get_uniform_location("shininess"), 1.f);
        program->set_uniform(
            program->get_uniform_location("lightPosition"), 0.f, 0.f, -6.f
        );
        program->set_uniform(
            program->get_uniform_location("color"), 0.85f, 0.25f, 0.15f
        );
        program->unset_shader_program();
    }

    // Task 3: night forest programs
    auto forest_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_forest);
    auto ground_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_forest);
    auto firefly_program
        = std::make_shared<zg::ShaderProgram>(*vs_lighting, *fs_unlit);

    // Scene 1
    //     auto scene1 =
    //     SceneBuilder::create_triangles_scene(fixed_shader_program);
    //     this->add_scene(std::move(scene1));

    // Scene 2
    //     auto scene2 = SceneBuilder::create_ball_scene(fixed_shader_program);
    //     this->add_scene(std::move(scene2));

    // SCENE 3
    //     auto scene3 = SceneBuilder::create_four_ball_scene(
    //         ambient_program,
    //         lambert_program,
    //         phong_program,
    //         blinn_program,
    //         fixed_shader_program
    //     );
    //     this->add_scene(std::move(scene3));

    // Scene 4 OBJECTS
    //     auto scene4 = SceneBuilder::create_login_scene(fixed_shader_program);
    //     this->add_scene(std::move(scene4));

    // SCENE 5
    //     auto scene5 = SceneBuilder::create_solar_scene(
    //         ambient_program, phong_program, blinn_program,
    //         fixed_shader_program
    //     );
    //     this->add_scene(std::move(scene5));

    // SCENE 7 - clock
    //     auto scene7 = SceneBuilder::create_forest_clock_scene(
    //         shader_program, tree_program, fixed_shader_program,
    //         clock_shader_program
    //     );
    //     this->add_scene(std::move(scene7));

    // SCENE 7 - normals: wrong vs correct transform
    //     auto scene7_normals = SceneBuilder::create_normals_scene(
    //         normals_wrong_program, normals_right_program,
    //         fixed_shader_program
    //     );
    //     this->add_scene(std::move(scene7_normals));

    // SCENE 8
    //     auto scene8 = SceneBuilder::create_specular_scene(
    //         specular_wrong_program, specular_right_program,
    //         fixed_shader_program
    //     );
    //     this->add_scene(std::move(scene8));

    // SCENE 9 - forest with firefly
    //     auto scene9 = SceneBuilder::create_night_forest_scene(
    //         forest_program, ground_program, firefly_program,
    //         fixed_shader_program
    //     );
    //     this->add_scene(std::move(scene9));

    auto scene10 = SceneBuilder::create_plain_scene(
        texturing_program, skybox_program, fixed_shader_program
    );
    this->add_scene(std::move(scene10));

    // SCENE 11 - EB408 classroom
    auto scene_eb408 = SceneBuilder::create_eb408_scene(
        texturing_program, fixed_shader_program
    );
    this->add_scene(std::move(scene_eb408));

    this->current_scene_index = this->scenes.size() - 1;
}

auto Application::run() -> void {
    Renderer::setup_rendering();

    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window->get())) {
        glfwPollEvents();

        const double now = glfwGetTime();
        const auto dt = static_cast<float>(now - last_time);
        last_time = now;

        auto& scene = *scenes.at(current_scene_index);
        input_manager.update(scene.get_camera(), dt);
        scene.update(dt);

        Renderer::setup_frame();

        Renderer::render(scene);

        int window_width{};
        int window_height{};
        int framebuffer_width{};
        int framebuffer_height{};
        glfwGetWindowSize(window->get(), &window_width, &window_height);
        glfwGetFramebufferSize(
            window->get(), &framebuffer_width, &framebuffer_height
        );
        input_manager.process_stencil_click(
            {window_width, window_height},
            {framebuffer_width, framebuffer_height}
        );

        glfwSwapBuffers(window->get());
    }
}
} // namespace zpg::core
