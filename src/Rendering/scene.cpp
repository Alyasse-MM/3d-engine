#include "scene.hpp"
#include "Core/MeshInstance.hpp"

namespace al3d
{
    namespace Rendering {
        Scene::Scene() {
        }
        std::shared_ptr<Mesh> Scene::importMesh(const std::string& path) {
            auto newMesh = std::make_shared<Mesh>(path);
            m_meshes.push_back(newMesh);
            return newMesh;
        }
        void Scene::loadMeshInstance(const std::string& path, Vector3f position, Vector3f rotations, float scale) {
            auto newMeshInstance = std::make_shared<MeshInstance>(importMesh(path), position, scale, rotations);
        }
        void Scene::loadMeshInstance(const std::string& path) {
            auto newMeshInstance = std::make_shared<MeshInstance>(importMesh(path));
        }
    }
}