// Created on: 01/05/23.
// Author: DendyA

#ifndef STAR_KNIGHT_TRANSFORMATION_MANAGER_H
#define STAR_KNIGHT_TRANSFORMATION_MANAGER_H

#include "bgfx/bgfx.h"

// IMPORTANT: This MUST be included before SDL_syswm.h since in that header None is defined as 0L which breaks bx/math.h
// Moving it here, bx/math.h defines the None tag instead and SDL_syswm.h has an ifndef guard to check if None is not defined.
// IMPORTANT: This also causes issues when using uint32_t since bx has its own home-grown uint32_t header. If sk_global_defines.h
// is included before this header, it clobbers the uint32_t included in the C++ STL. To fix this, cstdint.h is included in sk_global_defines.h
// meaning sk_global_defines.h can be included before or after math.h.
#include "bx/math.h"

namespace star_knight
{
    /** TransformationManager class.\n
     * The TransformationManager class is responsible for the initialization and updating of the matrices used for rendering.
     * The matrices in question being the projection, view, and transformation matrices. The workflow of using the class is to
     * call one of the transformation update functions to update the parameters or vectors of the projection or view matrix respectively.
     * @todo Then the game loop will call the generic update function which will conditionally check if either matrix was updated, and then
     * update the view transform.
     * @todo Need to add matrix "zero-ing" functions once the matrices have been sent to the bgfx system.
     */
    class TransformationManager
    {
        public:
            /** Constructor\n
             * The main constructor of the TransformationManager class. Inits the View Matrix's vectors to defaults.
             * Also sets the parameters for the projection matrix to starting defaults.
             */
            TransformationManager();

            /** Destructor\n
             * The default destructor
             */
            ~TransformationManager();

            /** updateViewTransform\n
             * Updates the view transform, which takes both the view and projection matrix, and ties it to the
             * given viewId.
             * @todo This should conditionally check if either the view or projection matrix updated (by use of a member flag)
             *  and then if it did, only then update the relevant matrix and update the view transform. This should be
             *  updated to be a generic update function (through which any updates needed during game are performed) and then
             *  be called from GameLoop's main loop.
             * @param viewID The viewID to tie the view transform to.
             */
            void updateViewTransform(bgfx::ViewId viewID);

            /** view_translateX\n
             * Updates the eyePosition and lookingAt vector for the View matrix. Essentially, "moves" the camera
             * delta amount in the X direction.
             * @param delta The amount the "position" needs to change. Positive moves left, negative moves right.
             */
            void view_translateX(float delta);

            /** view_translateY\n
             * Updates the eyePosition and lookingAt vector for the View matrix. Essentially, "moves" the camera
             * delta amount in the Y direction.
             * @param delta The amount the "position" needs to change. Positive moves up, negative moves down.
             */
            void view_translateY(float delta);

            /** setTransformMatrix\n
             * Updates the transform matrix and also gives the transform matrix to bgfx.
             * @todo The transform matrix will be per model so this is just for the default plane being rendered. WILL need to be refactored.
             */
            void setTransformMatrix();

        private:
            // Parameters related to the projection matrix.
            float m_fov;
            float m_aspectRatio;
            float m_nearPlane;
            float m_farPlane;
            float m_projMat[16]{}; // Should be all zeroes given the empty initializer list sets all elements to default; numeric type default is zero.

//          The "space" for these sets of vectors (for the View Matrix) is considered World Space.
            bx::Vec3 m_lookingAt; // Point in space the camera is "looking at". In the case of {0, 0, 0} (like in the initializer list), the origin.
            bx::Vec3 m_eyePosition; // Point in space the camera is physically located.
            bx::Vec3 m_coordinateSysUp; // Initializer list has the default vector for mtxLookAt. Orienting the positive Y axis to be "up" {0, 1, 0}.
            float m_viewMat[16]{};

            /** setProjMatrix\n
             * Creates a projection matrix out of the member parameters. The resulting projection matrix is saved in m_projMat.
             */
            void setProjMatrix();

            /** setViewMatrix\n
             * Creates a view matrix out of the member parameters. The resulting view matrix is saved in m_viewMat.
             */
            void setViewMatrix();
    };

} // star_knight

#endif //STAR_KNIGHT_TRANSFORMATION_MANAGER_H
