#include "painterRenderer.hpp"
#include "Maths/Maths.hpp"
#include "Rendering/Clipping.hpp"
#include <algorithm>
#include <iostream>

namespace al3d
{
    namespace Rendering
    {
        using namespace Maths;
        using Face = Rendering::Face;

        inline void PainterRenderer::paintersAlgorithm(std::vector<RenderFace>&facesToDraw) {
            std::sort(facesToDraw.begin(), facesToDraw.end(), [](const RenderFace& a, const RenderFace& b) {
                return a.avgZ > b.avgZ;
                });
        }

        void PainterRenderer::render() {
            prepareFacesToDraw();

            paintersAlgorithm(m_drawList);

            m_window.clear(sf::Color(148, 140, 140, 1));

            for (const auto& face : m_drawList) {
                sf::ConvexShape poly(face.points.size());
                for (size_t i = 0; i < face.points.size(); ++i) poly.setPoint(i, face.points[i]);
                poly.setFillColor(face.color);
                m_window.draw(poly);
            }
        }
    }
}