#pragma once
#include "MeshInstance.hpp"

namespace al3d::Core {
    void MeshInstance::updateMatrix() {
        Matrix4f translation4 = Matrix4f::getTranslation(m_position);
        Matrix4f rotation4 = Matrix4f::getRotationZ(m_rotation.z) * Matrix4f::getRotationY(m_rotation.y) * Matrix4f::getRotationX(m_rotation.x);
        Matrix4f scale4 = Matrix4f::getHomothety(m_scale);

        m_transformMatrix4 = translation4 * rotation4 * scale4;


        Matrix3f rotation3 = Matrix3f::getRotationZ(m_rotation.z) * Matrix3f::getRotationY(m_rotation.y) * Matrix3f::getRotationX(m_rotation.x);
        Matrix3f scale3 = Matrix3f::getHomothety(m_scale);
        m_transformMatrix3 = rotation3 * scale3;
    }

    MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh, Vector3f position, float scale, Vector3f rotations) :
        m_mesh(mesh),
        m_position(position),
        m_rotation(rotations),
        m_scale(scale)
    {
        updateMatrix();
    }

    MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh) :
        m_mesh(mesh),
        m_position({ 0.f,0.f,0.f }),
        m_rotation({ 0.f,0.f,0.f }),
        m_scale(1.f)
    {
        updateMatrix();
    }

    void MeshInstance::setPosition(const al3d::Maths::Vector3<float>& newPos) {
        m_position = newPos;
        updateMatrix();
    }

    void MeshInstance::setRotation(const al3d::Maths::Vector3<float>& newRot) {
        m_rotation = newRot;
        updateMatrix();
    }

    void MeshInstance::setScale(float newScale) {
        m_scale = newScale;
        updateMatrix();
    }

    const al3d::Core::MeshInstance::Matrix4f& MeshInstance::getTransformMatrix4() const {
        return m_transformMatrix4;
    }

    const al3d::Core::MeshInstance::Matrix3f& MeshInstance::getTransformMatrix3() const {
        return m_transformMatrix3;
    }

    const std::shared_ptr<Mesh> MeshInstance::getMesh() const {
        return m_mesh;
    }
}