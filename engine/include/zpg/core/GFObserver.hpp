#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>
#include <variant>
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

using Events = std::variant<
    KeyEvent,
    WindowFocusEvent,
    WindowIconifyEvent,
    WindowSizeEvent,
    CursorEvent,
    ButtonEvent>;
} // namespace events

template<typename... Events> class Obverser {
    template<typename E> using Handler = std::function<void(const E&)>;

    std::tuple<std::vector<Handler<Events>>...> _callbacks;

  public:
    template<class E, class F>
        requires(std::same_as<E, Events> || ...)
             && (std::invocable<F, const E&>)
    auto subscribe(F&& function) -> void {
        std::get<std::vector<Handler<E>>>(_callbacks)
            .emplace_back(std::forward(function));
    }

    template<class E>
        requires(std::same_as<E, Events> || ...)
    auto notify(const E& event) -> void {
        for (auto& fn : std::get<std::vector<Handler<E>>>(_callbacks)) {
            fn(event);
        }
    }
};
} // namespace zpg::core::gfobserver
