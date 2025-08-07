#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/vec3.hpp>
#include <vector>

#include "texture.h"

namespace frengine {
using Material = struct Material {
  glm::vec3 ambient_color;
  glm::vec3 emission_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
  float shininess;

  std::vector<frengine::Texture> textures;
};

const Material default_material = Material{
  .ambient_color = glm::vec3(0.5F, 0.5F, 0.5F),
  .emission_color = glm::vec3(0.0F, 0.0F, 0.0F),
  .diffuse_color = glm::vec3(0.0F, 0.0F, 1.0F),
  .specular_color= glm::vec3(1.0F, 1.0F, 1.0F),
  .shininess = 0.0F,
  .textures = {}
};

}  // namespace frengine

#endif  // MATERIAL_H
