#pragma once

#include <SFML/Graphics.hpp>
#include "Maths/Vector3.hpp"
#include <vector>
#include "Core/Vertex.hpp"
#include <string>

namespace al3d {
	namespace Core {
		class Mesh {
		private:
			std::vector<Maths::Vector3<float>> m_vertices;
			std::vector<Maths::Vector3<float>> m_normals;
			std::vector<unsigned> m_vertices_indices, m_normals_indices;
			std::vector<sf::Color> m_faces_colors;
		public:
			Mesh(const std::string filePath);
			~Mesh();
			const std::vector<Maths::Vector3<float>> getVertices();
			const std::vector<unsigned> getFacesIndices();
			const std::vector<Maths::Vector3<float>> getNormals();
			const std::vector<unsigned> getNormalsIndices();
			const std::vector<sf::Color> getFacesColors();
		};
	}
}