#pragma once

#include "Maths/Vector3.hpp"
#include <vector>
#include "Core/Vertex.hpp"
#include <string>

namespace al3d {
	namespace Core {
		class Mesh {
		private:
			std::vector<Vertex> m_vertices;
			std::vector<Maths::Vector3<float>> m_normals;
			std::vector<uint32_t> m_vertices_indices, m_normals_indices;
		public:
			Mesh(const std::string filePath);
			~Mesh();
		};
	}
}