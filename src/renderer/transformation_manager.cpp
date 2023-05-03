// Created on: 01/05/23.
// Author: DendyA

#include "sk_global_defines.h"

#include "transformation_manager.h"

namespace star_knight
{
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
    star_knight::TransformationManager::updateViewTransform(bgfx::ViewId viewID)
    {
        setProjMatrix();
        setViewMatrix();

        bgfx::setViewTransform(viewID, m_viewMat, m_projMat);
    }

    void
    star_knight::TransformationManager::setTransformMatrix()
    {

        float mtx[16];
        bx::mtxRotateY(mtx, 0.0f);

        // position x,y,z
        mtx[12] = 0.0f;
        mtx[13] = 0.0f;
        mtx[14] = 0.0f;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);
    }
} // star_knight