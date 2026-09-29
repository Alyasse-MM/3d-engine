#include "scene.hpp"
#include "Core/MeshInstance.hpp"
#include "filesystem"
#include "memory"
#include <iostream>

namespace fs = std::filesystem;

namespace al3d
{
    namespace Rendering {
        Scene::Scene() {
        }
        std::shared_ptr<Mesh> Scene::importMesh(const std::string& path) {
            std::error_code ec;
            fs::path canonicalPath = fs::canonical(path, ec);

            if (ec) {
                std::cerr << "File not found or invalid: " << path << '\n';
                return nullptr;
            }

            std::string meshUniqueKey = canonicalPath.string();

            auto cachedMesh = m_meshes.find(meshUniqueKey);
            if (cachedMesh != m_meshes.end()) {
                std::cout << "Loading from cache: " << meshUniqueKey << '\n';
                return cachedMesh->second;
            }
            else {
                auto mesh = std::make_shared<Mesh>(path);
                m_meshes[meshUniqueKey] = mesh;
                return mesh;
            }

        }
        void Scene::loadMeshInstance(const std::string& path, Vector3f position, Vector3f rotations, float scale) {
            m_meshInstances.push_back(std::make_shared<MeshInstance>(importMesh(path), position, scale, rotations));
        }
        void Scene::loadMeshInstance(const std::string& path) {
            m_meshInstances.push_back(std::make_shared<MeshInstance>(importMesh(path)));
        }
    }
}