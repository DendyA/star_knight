// Created on: 01/05/23.
// Author: DendyA

#include "sk_global_defines.h"

#include "transformation_manager.h"

//  m_coordinateSysUp, m_lookingAt, m_eyePosition HAVE to be initialized in an initializer list
//  because the default constructor of Vec3 is set to delete (aka Vec3() = delete).
star_knight::TransformationManager::TransformationManager() :
    m_coordinateSysUp(0.0f, 1.0f, 0.0f),
    m_lookingAt(0.0f, 0.0f, 0.0f),
    m_eyePosition(0.0f, 0.0f, 10.0f)
{
//      Setting projection matrix parameters. This won't change very frequently, so the projection matrix will be able to be pre-computed.
    m_fov = STARTING_FOV;
    m_aspectRatio = STARTING_ASPECT_RATIO;
    m_nearPlane = STARTING_NEAR_PLANE;
    m_farPlane = STARTING_FAR_PLANE;

    m_viewMatUpdated = false;
    m_projMatUpdated = false;
}

star_knight::TransformationManager::~TransformationManager() = default;

void
star_knight::TransformationManager::setProjMatrix()
{
    bx::mtxProj(m_projMat,
                m_fov,
                m_aspectRatio,
                m_nearPlane, m_farPlane,
                bgfx::getCaps()->homogeneousDepth);
}

void
star_knight::TransformationManager::setViewMatrix()
{
    bx::mtxLookAt(m_viewMat, m_eyePosition, m_lookingAt, m_coordinateSysUp);
}

// ViewId is just an uint16_t "under-the-hood".
void
star_knight::TransformationManager::update(bgfx::ViewId viewID)
{
    bool updates = false;

    if(m_projMatUpdated)
    {
        setProjMatrix();
        m_projMatUpdated = false;
        updates = true;
    }

    if(m_viewMatUpdated)
    {
        setViewMatrix();
        m_viewMatUpdated = false;
        updates = true;
    }

    if(updates)
    {
        bgfx::setViewTransform(viewID, m_viewMat, m_projMat);
    }
}

void
star_knight::TransformationManager::view_translateX(float delta)
{
    m_lookingAt.x += delta;
    m_eyePosition.x += delta;
    m_viewMatUpdated = true;
}

void
star_knight::TransformationManager::view_translateY(float delta)
{
    m_lookingAt.y += delta;
    m_eyePosition.y += delta;
    m_viewMatUpdated = true;
}

void
star_knight::TransformationManager::transform_translateXYZ(float* mtx, const float deltaX, const float deltaY, const float deltaZ)
{
    // The index ordering is 0 being top left and counting along the row. So top right index is 3.
    // Bottom left corner index is 12 and bottom right index is 15. Assuming a 4x4 matrix.
    mtx[0] = 1.0f;
    mtx[3] = deltaX;
    mtx[5] = 1.0f;
    mtx[7] = deltaY;
    mtx[10] = 1.0f;
    mtx[11] = deltaZ;
    mtx[15] = 1.0f;
}

void
star_knight::TransformationManager::transform_scaleXYZ(float* mtx, const float deltaX, const float deltaY, const float deltaZ)
{
    mtx[0] = deltaX;
    mtx[5] = deltaY;
    mtx[10] = deltaZ;
    mtx[15] = 1.0f;
}

void
star_knight::TransformationManager::transform_rotateX(float* mtx, const float deltaTheta)
{
    bx::mtxRotateX(mtx, deltaTheta);
}

void
star_knight::TransformationManager::transform_rotateY(float* mtx, const float deltaTheta)
{
    bx::mtxRotateZ(mtx, deltaTheta);
}

void
star_knight::TransformationManager::transform_rotateZ(float* mtx, const float deltaTheta)
{
    bx::mtxRotateZ(mtx, deltaTheta);
}
