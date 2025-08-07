#ifndef PROGRAM_H
#define PROGRAM_H
#include <expected>
#include <fstream>
#include <glm/mat4x4.hpp>
#include <sstream>

#include "error.h"

namespace frengine {
class Program {
 private:
  unsigned int program_;
  unsigned int vertex_shader_;
  unsigned int fragment_shader_;
  static auto read_file(const std::string& filename)
      -> std::expected<std::string, Error> {
    const auto input_file = std::ifstream{filename};
    if (!input_file || !input_file.is_open()) {
      return std::unexpected(
          Error{.message = std::format("Could not open file '{}'", filename)});
    }

    std::stringstream file_contents_stream;
    file_contents_stream << input_file.rdbuf();
    return file_contents_stream.str();
  }

 public:
  Program(const unsigned int program, const unsigned int vertex_shader,
          const unsigned int fragment_shader)
      : program_(program),
        vertex_shader_(vertex_shader),
        fragment_shader_(fragment_shader) {}

  ~Program() {
    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);
    glDeleteProgram(program_);
  }

  // Delete copy constructors
  Program(const Program&) = delete;
  Program& operator=(const Program&) = delete;

  static auto Create(const std::string& vertex_shader_filename,
                     const std::string& fragment_shader_filename)
      -> std::expected<std::unique_ptr<Program>, Error> {
    const auto compile_shader = [](const unsigned int shader_type,
                                   const std::string& shader_filename)
        -> std::expected<unsigned int, Error> {
      const auto shader_source = read_file(shader_filename);
      if (!shader_source) {
        return std::unexpected(Error{
            .message = std::format("Could not read file contents from '{}'",
                                   shader_filename)});
      }

      auto shader = glCreateShader(shader_type);
      const auto shader_source_ptr = shader_source->c_str();
      glShaderSource(shader, 1, &shader_source_ptr, nullptr);
      glCompileShader(shader);

      int shader_compiled;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compiled);
      if (shader_compiled != GL_TRUE) {
        int log_length = 0;
        char message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        return std::unexpected(Error{.message = std::string(message)});
      }
      return shader;
    };

    const auto vertex_shader =
        compile_shader(GL_VERTEX_SHADER, vertex_shader_filename);
    if (!vertex_shader) {
      return std::unexpected(
          Error{.message = std::format(
                    "Could not compile vertex shader '{}': {}",
                    vertex_shader_filename, vertex_shader.error().message)});
    }
    const auto fragment_shader =
        compile_shader(GL_FRAGMENT_SHADER, fragment_shader_filename);
    if (!fragment_shader) {
      return std::unexpected(Error{
          .message = std::format("Could not compile fragment shader '{}': {}",
                                 fragment_shader_filename,
                                 fragment_shader.error().message)});
    }

    const auto program = glCreateProgram();
    glAttachShader(program, *vertex_shader);
    glAttachShader(program, *fragment_shader);
    glLinkProgram(program);

    int link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
      int log_length = 0;
      char message[1024];
      glGetProgramInfoLog(program, 1024, &log_length, message);
      return std::unexpected(
          Error{.message = std::format(
                    "Could not link program {} with shaders '{}' and '{}': {}",
                    program, vertex_shader_filename, fragment_shader_filename,
                    std::string(message))});
    }

    return std::make_unique<Program>(program, *vertex_shader, *fragment_shader);
  }

  auto Use() const -> void { glUseProgram(program_); }

  auto SetMat4(const std::string& uniform_name, const glm::mat4& mat) const
      -> std::expected<void, Error> {
    glUseProgram(program_);
    const auto location = glGetUniformLocation(program_, uniform_name.c_str());
    if (location == -1) {
      return std::unexpected(
          Error{.message = std::format("Could not get uniform location of '{}'",
                                       uniform_name)});
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    return {};
  }

  auto SetVec3(const std::string& uniform_name, const glm::vec3& vec) const
      -> std::expected<void, Error> {
    glUseProgram(program_);
    const auto location = glGetUniformLocation(program_, uniform_name.c_str());
    if (location == -1) {
      return std::unexpected(
          Error{.message = std::format("Could not get uniform location of '{}'",
                                       uniform_name)});
    }
    glUniform3fv(location, 1, &vec[0]);
    return {};
  }

  auto Set1i(const std::string& uniform_name, const int value) const
      -> std::expected<void, Error> {
    glUseProgram(program_);
    const auto location = glGetUniformLocation(program_, uniform_name.c_str());
    if (location == -1) {
      return std::unexpected(
          Error{.message = std::format("Could not get uniform location of '{}'",
                                       uniform_name)});
    }
    glUniform1i(location, value);
    return {};
  }
};

}  // namespace frengine

#endif  // PROGRAM_H
