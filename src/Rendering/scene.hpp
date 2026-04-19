#pragma once
#include <SFML/Graphics.hpp>
#include "Maths/Vector3.hpp"

namespace al3d
{
    struct Face {
        std::vector<int> indices;
        sf::Color color;
    };

    class Scene {
    public:
        std::vector<Maths::Vector3<float>> m_vertices;
        std::vector<Face> m_faces;

        Scene();
    };
}