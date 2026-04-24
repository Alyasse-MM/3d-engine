#include "Mesh.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace al3d {
    namespace Core {
	    Mesh::Mesh(std::string filePath) {
            std::ifstream file(filePath);

            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filePath);
            }

            std::string line;
            std::vector<Maths::Vector3<float>> tempNormals;
            while (std::getline(file, line)) {
                std::istringstream lineStream(line);
                std::string lineType;
                lineStream >> lineType;

                if (lineType == "v")
                {
                    float x, y, z;
                    lineStream >> x >> y >> z;
                    m_vertices.push_back({ x, y, z });
                }
                else if (lineType == "vn") {
                    float x, y, z;
                    lineStream >> x >> y >> z;
                    m_normals.push_back({ x, y, z });
                }
                else if (lineType == "f") {
                    std::string vertexChunk;
                    std::vector<int> vertices_indices, normals_indices;
                    while (lineStream >> vertexChunk) {
                        std::stringstream ss(vertexChunk);
                        std::string segment;

                        // Only the vertex and normal indices are used. Texture indices are ignored for now.
                        if (std::getline(ss, segment, '/')) {
                            if (!segment.empty()) {
                                vertices_indices.push_back(std::stoi(segment) - 1);
                            }
                        }
                        if (std::getline(ss, segment, '/')) {
                            if (!segment.empty()) {
                                normals_indices.push_back(std::stoi(segment) - 1);
                            }
                        }
                    }

                    if (vertices_indices.size() > 3) {
                        for (size_t i = 1; i < vertices_indices.size() - 1; ++i) {
                            m_vertices_indices.push_back(vertices_indices[0]);
                            m_normals_indices.push_back(normals_indices[0]);
                            m_vertices_indices.push_back(vertices_indices[i]);
                            m_normals_indices.push_back(normals_indices[i]);
                            m_vertices_indices.push_back(vertices_indices[i + 1]);
                            m_normals_indices.push_back(normals_indices[i + 1]);
                        }
                    }
                    else if (vertices_indices.size() < 3) {
                        throw std::runtime_error("Failed to open file: " + filePath);
                    }
                    else {
                        for (unsigned i = 0; i < 3; i++){
                            m_vertices_indices.push_back(vertices_indices[i]);
                            m_normals_indices.push_back(normals_indices[i]);
                        }
                    }
                }
            }
	    }

	    Mesh::~Mesh() {};
    }
}