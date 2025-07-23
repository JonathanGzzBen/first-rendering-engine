#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <print>

#include "frengine/program.h"
#include "frengine/renderer.h"

auto glfw_error_callback(int error, const char *description) -> void {
  std::println(std::cerr, "GLFW error {}: {}", error, description);
}

void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam) {
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
    std::println(std::cerr, "glDebugCallback: {}: {}", type, message);
  }
}

auto main() -> int {
  if (glfwInit() != GLFW_TRUE) {
    std::println(std::cerr, "Could not initialize glfw");
    return 1;
  }

  glfwSetErrorCallback(glfw_error_callback);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
  if (window == nullptr) {
    std::println(std::cerr, "Could not create GLFW window");
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::println(std::cerr, "Could not initialize GLEW");
    glfwTerminate();
    return 1;
  }

  // Configure OpenGL debugging if Debug context was created
  []() {
    int context_flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugCallback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                            nullptr, GL_TRUE);
    }
  }();

  const auto program =
      frengine::Program::Create("shaders/vertex.glsl", "shaders/fragment.glsl");
  if (!program) {
    std::println(std::cerr, "Could not create program: {}",
                 program.error().message);
    glfwTerminate();
    return 1;
  }

  const auto renderer = frengine::Renderer::Create();
  if (!renderer) {
    std::println(std::cerr, "Could not create renderer: {}",
                 renderer.error().message);
    glfwTerminate();
    return 1;
  }

  const auto triangle_vertices =
      std::vector<Vertex>({Vertex{.position = glm::vec3(-1.0F, -1.0F, 0.0F),
                                  .normal = glm::vec3(1.0F, 1.0F, 1.0F),
                                  .uv = glm::vec2(0.0F, 0.0F)},
                           Vertex{.position = glm::vec3(0.0F, 1.0F, 0.0F),
                                  .normal = glm::vec3(1.0F, 1.0F, 1.0F),
                                  .uv = glm::vec2(0.0F, 0.0F)},
                           Vertex{.position = glm::vec3(1.0F, -1.0F, 0.0F),
                                  .normal = glm::vec3(1.0F, 1.0F, 1.0F),
                                  .uv = glm::vec2(0.0F, 0.0F)}});
  const auto triangle_indices = std::vector<unsigned int>({0, 1, 2});

  const auto triangle_mesh =
      frengine::Mesh::Create(triangle_vertices, triangle_indices);
  if (!triangle_mesh) {
    std::println(std::cerr, "Could not create mesh: {}",
                 triangle_mesh.error().message);
    glfwTerminate();
    return 1;
  }

  auto view_matrix = glm::mat4(1.0f);
  view_matrix = glm::translate(view_matrix, glm::vec3(0.0F, 0.0F, -3.0F));
  if (const auto res = program->get()->SetMat4("view", view_matrix); !res) {
    std::println(std::cerr, "Could not set view matrix: {}",
                 res.error().message);
    glfwTerminate();
    return 1;
  }
  const auto projection_matrix =
      glm::perspective(glm::radians(45.0F), 540.0F / 480.0F, 0.1F, 100.0F);
  if (const auto res = program->get()->SetMat4("projection", projection_matrix);
      !res) {
    std::println(std::cerr, "Could not set projection matrix: {}",
                 res.error().message);
    glfwTerminate();
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    renderer->get()->Draw(**program, **triangle_mesh, glm::mat4(1.0F));

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return 0;
}