#pragma once

#include "zpg/utils/is_unique.hpp"
#include <concepts>
#include <functional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace zpg::core::gfobserver {

namespace events {
struct KeyEvent {
    int key;
    int scancode;
    int action;
    int mods;
};

struct WindowFocusEvent {
    int focused;
};

struct WindowIconifyEvent {
    int iconified;
};

struct WindowSizeEvent {
    int width;
    int height;
};

struct CursorEvent {
    double x;
    double y;
};

struct ButtonEvent {
    int button;
    int action;
    int mode;
};

struct Error {
    int error;
    std::string description;
};
} // namespace events

template<typename... Events>
    requires(sizeof...(Events) > 0 && (utils::is_unique<Events...>))
class Observer {
    template<typename E> using Handler = std::function<void(const E&)>;

    std::tuple<std::vector<Handler<Events>>...> _callbacks;

  public:
    Observer() = default;

    static auto instance() -> Observer& {
        static Observer instance;
        return instance;
    }

    template<class E, class F>
        requires(std::same_as<E, Events> || ...)
             && (std::invocable<F, const E&>)
    auto subscribe(F&& function) -> void {
        std::get<std::vector<Handler<E>>>(_callbacks)
            .emplace_back(std::forward<F>(function));
    }

    template<class E>
        requires(std::same_as<E, Events> || ...)
    auto notify(const E& event) -> void {
        for (auto& fn : std::get<std::vector<Handler<E>>>(_callbacks)) {
            fn(event);
        }
    }
};

using GlfwObserver = Observer<
    events::KeyEvent,
    events::WindowFocusEvent,
    events::WindowIconifyEvent,
    events::WindowSizeEvent,
    events::CursorEvent,
    events::ButtonEvent,
    events::Error>;
} // namespace zpg::core::gfobserver
