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
    class TransformationManager final
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

            /** transform_translateXYZ\n
             * Generates a translation transformation matrix with the given deltas for each axis.
             * @param mtx The pointer to save the generated translation transformation matrix to.
             * @param deltaX The amount to translate in the X axis. Positive delta right, negative delta left. Zero to ignore the axis.
             * @param deltaY The amount to translate in the Y axis. Positive delta up, negative delta down. Zero to ignore the axis.
             * @param deltaZ The amount to translate in the Z axis. Positive delta away from near plane, negative delta towards near plane. Zero to ignore the axis.
             */
            static void transform_translateXYZ(float* mtx, float deltaX, float deltaY, float deltaZ);

            /** transform_scaleXYZ\n
             * Generates a scaling transformation matrix with the given deltas for each axis.
             * @param mtx The pointer to save the generated scaling transformation matrix to.
             * @param deltaX The amount to scale in the X axis. Positive delta increases size, negative delta decreases size. Zero to ignore axis.
             * @param deltaY The amount to scale in the Y axis. Positive delta increases size, negative delta decreases size. Zero to ignore axis.
             * @param deltaZ The amount to scale in the Z axis. Positive delta increases size, negative delta decreases size. Zero to ignore axis.
             */
            static void transform_scaleXYZ(float* mtx, float deltaX, float deltaY, float deltaZ);

            /** transform_rotateX\n
             * Generates a rotation transformation matrix with the given delta angle around the X axis.
             * @warning Setting deltaTheta to 0 does not ignore the axis like in the scale or translate functions. Given that both sin and cos are used.
             * Do not call this function if there is no intention to apply a rotation.
             * @param mtx The pointer to save the generated rotation transformation matrix to.
             * @param deltaTheta The amount to rotate around the X axis. Positive delta theta counter clockwise, negative delta theta clockwise.
             */
            static void transform_rotateX(float *mtx, float deltaTheta);

            /** transform_rotateY\n
             * Generates a rotation transformation matrix with the given delta angle around the X axis.
             * @warning Setting deltaTheta to 0 does not ignore the axis like in the scale or translate functions. Given that both sin and cos are used.
             * Do not call this function if there is no intention to apply a rotation.
             * @param mtx The pointer to save the generated rotation transformation matrix to.
             * @param deltaTheta The amount to rotate around the Y axis. Positive delta theta counter clockwise, negative delta theta clockwise.
             */
            static void transform_rotateY(float *mtx, float deltaTheta);

            /** transform_rotateZ\n
             * Generates a rotation transformation matrix with the given delta angle around the Z axis.
             * @warning Setting deltaTheta to 0 does not ignore the axis like in the scale or translate functions. Given that both sin and cos are used.
             * Do not call this function if there is no intention to apply a rotation.
             * @param mtx The pointer to save the generated rotation transformation matrix to.
             * @param deltaTheta The amount to rotate around the Z axis. Positive delta theta counter clockwise, negative delta theta clockwise.
             */
            static void transform_rotateZ(float *mtx, float deltaTheta);

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
