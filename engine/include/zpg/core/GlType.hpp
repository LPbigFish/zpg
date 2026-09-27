#pragma once
#include <cstdint>
#include <glad/gl.h>

namespace zpg::core {
// NOLINTNEXTLINE
template<typename T> struct gl_t {};

template<> struct gl_t<double> {
    static constexpr GLenum V = GL_DOUBLE;
};

template<> struct gl_t<float> {
    static constexpr GLenum V = GL_FLOAT;
};

template<> struct gl_t<std::int32_t> {
    static constexpr GLenum V = GL_INT;
};

template<> struct gl_t<std::uint32_t> {
    static constexpr GLenum V = GL_UNSIGNED_INT;
};

template<> struct gl_t<std::int16_t> {
    static constexpr GLenum V = GL_SHORT;
};

template<> struct gl_t<std::uint16_t> {
    static constexpr GLenum V = GL_UNSIGNED_SHORT;
};

} // namespace zpg::core
