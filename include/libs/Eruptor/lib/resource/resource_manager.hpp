#ifndef ERUPTOR_RESOURCE_RESOURCE_MANAGER_HPP
#define ERUPTOR_RESOURCE_RESOURCE_MANAGER_HPP

#include <Eruptor/lib/resource/model.hpp>
#include <Eruptor/lib/resource/material.hpp>
#include <assimp/material.h>
#include <filesystem>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace eruptor::hardware
{
    class Resource_manager;
    struct Texture_data;
}

namespace eruptor::resource
{

enum class Texture_type
{
    DIFFUSE,
    SPECULAR
};

class Resource_manager
{
public:
    void Init(hardware::Resource_manager & hw_resource_manager);

    void Load_models();

private:
    void Load_model(Model & model, const std::filesystem::path & path);

    void Process_node(aiNode * node, const aiScene * scene, Model & model);
    void Process_mesh(aiMesh * mesh, const aiScene * scene, Model & model);
    Texture_handle Load_material_texture(aiMaterial * mat, aiTextureType ai_type, Texture_type type);

    std::vector<Model> models{};
    std::vector<Material> materials{};
    std::vector<Texture_handle> textures_handles{};
    std::vector<Mesh_handle> mesh_handles{};

    hardware::Resource_manager * hw_resource_manager{};
};

}

#endif // ERUPTOR_RESOURCE_RESOURCE_MANAGER_HPP
