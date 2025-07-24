#ifndef MODEL_H
#define MODEL_H

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <vector>

#include "error.h"
#include "mesh.h"
#include "renderable.h"
#include "texture.h"

namespace frengine {

class Model : public IRenderable {
 private:
  std::vector<std::unique_ptr<Mesh>> meshes_;
  glm::mat4 model_matrix_ = glm::mat4(1.0F);

 public:
  explicit Model(std::vector<std::unique_ptr<Mesh>>&& meshes)
      : meshes_(std::move(meshes)) {}

  static auto process_node(const aiNode* node, const aiScene* scene)
      -> std::vector<std::unique_ptr<Mesh>> {
    std::vector<std::unique_ptr<Mesh>> meshes;
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
      const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      if (auto created_mesh = process_mesh(mesh, scene); !created_mesh) {
        std::println(std::cerr, "Could not load mesh {} from node {}", i,
                     node->mName.C_Str());
      } else {
        meshes.emplace_back(std::move(*created_mesh));
      }
    }

    for (size_t i = 0; i < node->mNumChildren; i++) {
      auto children_meshes = process_node(node->mChildren[i], scene);
      meshes.insert(meshes.end(),
                    std::make_move_iterator(children_meshes.begin()),
                    std::make_move_iterator(children_meshes.end()));
    }

    return meshes;
  }

  static auto process_mesh(const aiMesh* mesh, const aiScene* scene)
      -> std::expected<std::unique_ptr<Mesh>, Error> {
    // -> Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex{};
      vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                         mesh->mVertices[i].z};
      vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                       mesh->mNormals[i].z};
      if (mesh->mTextureCoords[0])
        vertex.uv = {mesh->mTextureCoords[0][i].x,
                     mesh->mTextureCoords[0][i].y};
      else
        vertex.uv = {0.0f, 0.0f};

      vertices.emplace_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
      const aiFace face = mesh->mFaces[i];
      for (size_t j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    // Textures
    std::vector<Texture> textures;

    if (mesh->mMaterialIndex >= 0) {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
      for (size_t i = 0;
           i < material->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
           i++) {
        aiString texture_path;
        material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, i,
                             &texture_path);

        auto texture =
            Texture::Create(std::format("models/cube/{}", texture_path.C_Str()),
                            Texture::Type::Diffuse);
        if (!texture) {
          return std::unexpected(Error{
              .message = std::format("Could not load texture from file '{}'",
                                     texture_path.C_Str())});
        }
        textures.emplace_back(*texture);
      }
    }

    return {Mesh::Create(vertices, indices, textures)};
  }

  static auto Create(const std::string& model_filename)
      -> std::expected<std::shared_ptr<Model>, Error> {
    Assimp::Importer importer;
    const auto scene = importer.ReadFile(
        model_filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
      return std::unexpected(
          Error{.message = std::format("Could not load scene from file '{}'",
                                       model_filename)});
    }

    // auto meshes = process_node(scene->mRootNode, scene);
    // return std::make_unique<Model>(meshes);
    return std::make_shared<Model>(process_node(scene->mRootNode, scene));
  }

  [[nodiscard]] auto meshes() const
      -> const std::vector<std::unique_ptr<Mesh>>& {
    return meshes_;
  }

  // IRenderable
  auto Draw(const Program& program, const unsigned int vao) const
      -> void override {
    // if (model_matrix_ != glm::mat4(1.0F)) {
    program.SetMat4("model", model_matrix_);
    // }
    for (const auto& mesh : meshes_) {
      mesh->Draw(program, vao);
    }
  }

  void SetModelMatrix(const glm::mat4& model) override {
    model_matrix_ = model;
  }
  const glm::mat4& GetModelMatrix() const override { return model_matrix_; }
};

}  // namespace frengine

#endif  // MODEL_H
