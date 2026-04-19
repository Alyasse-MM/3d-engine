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
			std::vector<uint32_t> m_indices;
		public:
			Mesh(const std::string filePath);
			~Mesh();
			//const std::vector<Vertex>& getVertices() const;
		};
	}
}