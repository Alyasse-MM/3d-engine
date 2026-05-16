#pragma once
#include "Maths/Vector3.hpp"
#include "Maths/Matrix3.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Utils.hpp"
#include "Mesh.hpp"

namespace al3d::Core {
    class MeshInstance {
        using Matrix4f = al3d::Maths::Matrix4<float>;
        using Vector3f = al3d::Maths::Vector3<float>;
    private:
        std::shared_ptr<Mesh> m_mesh;

        Vector3f m_position;
        Vector3f m_rotation;
        float m_scale;

        Matrix4f m_transformMatrix;

        void updateMatrix();

    public:
        MeshInstance(std::shared_ptr<Mesh> mesh, Vector3f position, float scale, Vector3f rotations);

        MeshInstance(std::shared_ptr<Mesh> mesh);

        void setPosition(const al3d::Maths::Vector3<float>& newPos);

        void setRotation(const al3d::Maths::Vector3<float>& newRot);

        void setScale(float newScale);

        const Matrix4f& getTransformMatrix() const;
    };
}