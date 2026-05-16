#pragma once
#include <SFML/Graphics.hpp>
#include "Maths/Vector3.hpp"
#include "Core/Mesh.hpp"
#include "Core/MeshInstance.hpp"

namespace al3d {
    using Mesh = Core::Mesh;
    using MeshInstance = Core::MeshInstance;
    using Vector3f = Maths::Vector3<float>;

    namespace Rendering {
        struct Face {
            std::vector<unsigned> indices;
            sf::Color color;
        };

        class Scene {
        public:
            std::vector<std::shared_ptr<Mesh>> m_meshes;
            std::vector<std::shared_ptr<MeshInstance>> m_meshInstances;

            Scene();
            std::shared_ptr<Mesh> importMesh(const std::string& path);
            void loadMeshInstance(const std::string& path, Vector3f position, Vector3f rotations, float scale);
            void loadMeshInstance(const std::string& path);
        };
    }
}