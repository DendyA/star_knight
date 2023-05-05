// Created on: 26/04/23.
// Author: DendyA

#ifndef STAR_KNIGHT_INITIALIZER_H
#define STAR_KNIGHT_INITIALIZER_H

#include "SDL.h"

namespace star_knight
{
    /** Initializer class\n
     * The Initializer class is responsible for the initialization / destruction of bgfx and any of its sub-components.
     * It inits the bgfx subsystem as a whole and also sets up the views for use in the SDL window. It also destroys bgfx.
     * @note The destruction of this class is the responsibility of the class creating the instance.
     */
    class Initializer final
    {
        public:
            // Typing this as a basic int because this may need to be returned as the return value in main() in the case of an error.
            // And main's signature is obviously an int return type.
            enum SKRendererInitErrCodes: int
            {
                kNoErr = 0,
                kSDLNoManagementWindowInfoErr,
                kbgfxInitErr
            };

            /** Constructor\n
             * The default constructor of the Initializer class. Does @b NOT call initbgfx.
             * That is the responsibility of the class instantiating this one.
             */
            Initializer();

            /** Constructor\n
             * The main constructor of the Initializer class. Calls initbgfx.
             * @param pwindow The SDL_Window* to pull window information from.
             */
            explicit Initializer(SDL_Window* pwindow);

            /** Destructor\n
             * The default destructor.
             * @note NOT calling destroybgfx here because if bgfx didn't init properly, then a fatal error is returned.
             * @note m_errorMessage will be set to kbgfxInitErr in the case where bgfx isn't init-ed properly and can be used to check whether destroybgfx can be called.
             */
            ~Initializer();

            /** getErrorCode\n
             * Returns the value stored in m_errorCode.
             * The value stored in m_errorCode will always be the resulting status of the most recent failing function call.
             * @return m_errorCode
             */
            star_knight::Initializer::SKRendererInitErrCodes getErrorCode();

             /** getErrorMessage\n
              * Returns the value stored in m_errorMessage.
              * The value stored in m_errorCode will always be the resulting message of the most recent failing function call.
              * @return m_errorMessage.
              */
            std::string getErrorMessage();

            /** initbgfx\n
             * Initializes various bgfx objects like the Init struct, retrieves window information from SDL,
             * and initializes the bgfx system as a whole etc.
             * @param pwindow Pointer to the window bgfx is to render in.
             * @return Status of the function call. 0 if successful, 1 if failure.
             */
            void initbgfx(SDL_Window* pwindow);

             /** initbgfxView\n
              * Initializes bgfx view objects.
              * Specifically used to set debug flags, reset the backbuffer size, and inits view objects among other things.
              */
            void initbgfxView();

            /** destroybgfx\n
             *  Destroys the bgfx system as a whole. bgfx MUST be initialized before shutdown is called. Otherwise a fatal error occurs.
             */
            void destroybgfx();
        private:
            star_knight::Initializer::SKRendererInitErrCodes m_errorCode;
            std::string m_errorMessage;

            /** saveError\n
             * Saves error status and message.
            * If any of the functions in this class encounter an error, this is called to set the specific message and the isError var.
            * @param prependedToError Error message to save. Expected to be \n and null-terminated.
            * @param errorCode Error code to save. Expected to be one of SKRendererInitErrCodes.
            */
            void saveError(const std::string& prependedToError, SKRendererInitErrCodes errorCode);
    };

} // star_knight

#endif //STAR_KNIGHT_INITIALIZER_H
