#pragma once
#include <SFML/Graphics.hpp>
#include "Maths/Vector3.hpp"
#include "Core/Mesh.hpp"

namespace al3d {
    using Mesh = Core::Mesh;
    namespace Rendering {
        struct Face {
            std::vector<unsigned> indices;
            sf::Color color;
        };

        class Scene {
        public:
            std::vector<std::shared_ptr<Mesh>> m_meshes;

            Scene();
            std::shared_ptr<Mesh> importMesh(const std::string& path) {
                auto newMesh = std::make_shared<Mesh>(path);
                m_meshes.push_back(newMesh);
                return newMesh;
            }
        };
    }
}