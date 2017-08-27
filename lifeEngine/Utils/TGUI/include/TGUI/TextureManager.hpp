/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2017 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_TEXTURE_MANAGER_HPP
#define TGUI_TEXTURE_MANAGER_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TGUI/TextureData.hpp>
#include <TGUI/Config.hpp>

#include <list>
#include <map>
#include <memory>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    class Texture;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API TextureManager
    {
    public:
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads a texture.
        ///
        /// @param texture    The texture object to store the loaded image.
        /// @param filename   Filename of the image to load.
        /// @param partRect   Load only part of the image. Don't pass this parameter if you want to load the full image.
        ///
        /// The second time you call this function with the same filename, the previously loaded image will be reused.
        ///
        /// @return False when the image could not be loaded, true otherwise.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static bool getTexture(Texture& texture, const std::string& filename, const sf::IntRect& partRect = sf::IntRect(0, 0, 0, 0));


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Share the image with another texture.
        ///
        /// @param textureDataToCopy  The original texture data that will now be reused.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void copyTexture(std::shared_ptr<TextureData> textureDataToCopy);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes the texture.
        ///
        /// @param textureDataToRemove  The texture data that should be removed.
        ///
        /// When no other texture is using the same image then the image will be removed from memory.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void removeTexture(std::shared_ptr<TextureData> textureDataToRemove);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        static std::map<std::string, std::list<TextureDataHolder>> m_imageMap;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TEXTURE_MANAGER_HPP
