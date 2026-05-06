#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Core/state.hpp"
#include "scene.hpp"
#include "Maths/Vector3.hpp"

namespace al3d
{
    namespace Rendering
    {
        using Face = Rendering::Face;
        using Vector3f = Maths::Vector3<float>;

        struct RenderFace {
            std::vector<sf::Vector2f> points;
            std::vector<float> zValues;
            sf::Color color;
            float avgZ;
        };

        class SoftwareRenderer {
        protected:
            EngineState& m_engineState;
            sf::RenderWindow& m_window;
            Rendering::Scene* m_scene;
            std::vector<RenderFace> m_drawList;
            std::vector<Vector3f> viewSpaceVertices;
            std::vector<Vector3f> verticesNormals;

            std::vector<unsigned> facesIds;
            std::vector<unsigned> normalsIds;
            std::vector<sf::Color> facesColors;

            void prepareFacesToDraw();
        public:
            virtual ~SoftwareRenderer() = default;

            void workerPrepareFacesLoop(unsigned refIdFaces, unsigned refIdNormals);

            SoftwareRenderer(sf::RenderWindow& w, EngineState& e) : m_engineState(e), m_window(w), m_scene(nullptr) {};
            virtual void render() = 0;
            void setScene(Rendering::Scene* s) { m_scene = s; }
        };
    }
}