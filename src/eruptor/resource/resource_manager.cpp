#include <Eruptor/lib/resource/resource_manager.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <Eruptor/lib/resource/stb_image.h>
#include <Eruptor/lib/hardware/resources/resource_manager.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void eruptor::resource::Resource_manager::Init(hardware::Resource_manager & hw_resource_manager)
{
    this->hw_resource_manager = &hw_resource_manager;
}

void eruptor::resource::Resource_manager::Load_model(Model & model, const std::filesystem::path & path)
{
    Assimp::Importer importer{};
    const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw std::runtime_error{" ERROR::REOUURCE::RESOURCE_MANAGER::Failed to load model."};
    }
    auto directory = path.parent_path();

    Process_node(scene->mRootNode, scene, model);
}

void eruptor::resource::Resource_manager::Process_node(aiNode* node, const aiScene* scene, Model& model)
{
    for(auto i{0u}; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Process_mesh(mesh, scene, model);
    }

    for(auto i{0u}; i < node->mNumChildren; i++)
    {
        Process_node(node->mChildren[i], scene, model);
    }
}

void eruptor::resource::Resource_manager::Process_mesh(aiMesh* mesh, const aiScene* scene, Model& model)
{
    if(mesh->mMaterialIndex >= 0)
    {
        Material material{};
        aiMaterial * ai_material = scene->mMaterials[mesh->mMaterialIndex];

       material.diffuse_texture_handle = Load_material_texture(ai_material, aiTextureType_DIFFUSE, Texture_type::DIFFUSE);
       material.specular_texture_handle = Load_material_texture(ai_material, aiTextureType_SPECULAR, Texture_type::SPECULAR);

       this->materials.push_back(material);
       model.materials_handles.emplace_back( static_cast<uint32_t>(materials.size() - 1) ) ;
    }

    hardware::Mesh_data mesh_data{};
    for(auto i{0u}; i < mesh->mNumVertices; i++)
    {
        hardware::Vertex vertex{};

        glm::vec3 vector{};

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normals = vector;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec{};
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_cord = vec;
        }
        else
        {
            vertex.texture_cord = glm::vec2{0.0f, 0.0f};
        }

        mesh_data.vertecies.push_back( vertex );
    }

    for(auto i{0u}; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(auto j{0u}; j < face.mNumIndices; j++)
        {
            mesh_data.indices.push_back( face.mIndices[j] );
        }
    }

    mesh_data.material_id = model.materials_handles.size() - 1;

    Mesh_handle mesh_handle{ hw_resource_manager->Stage_mesh_data( mesh_data ) };
    mesh_handles.push_back(mesh_handle);
    model.Meshes_handles.push_back( mesh_handle );
}

eruptor::resource::Texture_handle eruptor::resource::Resource_manager::Load_material_texture(aiMaterial* mat, aiTextureType ai_type, Texture_type type)
{
    hardware::Texture_data tex_data{};

    aiString str{};
    mat->GetTexture(ai_type, 0, &str);
    tex_data.format = (type == Texture_type::DIFFUSE)? vk::Format::eR8G8B8A8Srgb : vk::Format::eR8Unorm;

    int loaded_chanels = (type == Texture_type::DIFFUSE)? STBI_rgb_alpha : STBI_grey;

    tex_data.pixels = stbi_load(str.C_Str(), &tex_data.width, &tex_data.height, &tex_data.tex_chanels, loaded_chanels);

    if(!tex_data.pixels)
    {
        throw std::runtime_error{"failed to load texture image!"};
    }

    tex_data.tex_chanels = (type == Texture_type::DIFFUSE)? 4 : 1;

    Texture_handle tex_handle{ hw_resource_manager->Stage_texture_data(tex_data)};
    textures_handles.push_back(tex_handle);

    return tex_handle;
}








