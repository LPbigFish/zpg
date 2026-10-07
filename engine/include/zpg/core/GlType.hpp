#pragma once
#include <concepts>
#include <cstdint>
#include <glad/gl.h>
#include <type_traits>

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

template<> struct gl_t<std::bool_constant<true>> {
    static constexpr GLenum V = GL_TRUE;
};

template<> struct gl_t<std::bool_constant<false>> {
    static constexpr GLenum V = GL_FALSE;
};

// concept pro matchnutí GL types pro overloading
template<typename T>
concept GlTypeConstrain = std::same_as<std::remove_cvref_t<T>, GLint>
                       || std::same_as<std::remove_cvref_t<T>, GLuint>
                       || std::same_as<std::remove_cvref_t<T>, GLfloat>;
} // namespace zpg::core
