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


#ifndef TGUI_COLOR_HPP
#define TGUI_COLOR_HPP

#include <TGUI/Global.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Implicit converter for colors
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Color
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Create the object from an sf::Color
        ///
        /// @param color  Color to copy
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Color(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Create the object from an the RGB or RGBA values
        ///
        /// @param red   Red component
        /// @param green Green component
        /// @param blue  Blue component
        /// @param alpha Alpha component
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Color(sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue, sf::Uint8 alpha = 255);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Create the object from a string
        ///
        /// @param string  String to be deserialized as color
        ///
        /// The Deserializer class is used to convert the string into a color.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Color(const char* string);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Create the object from a string
        ///
        /// @param string  String to be deserialized as color
        ///
        /// The Deserializer class is used to convert the string into a color.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Color(const std::string& string);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Convert this object into an sf::Color object
        ///
        /// @return The color stored in this object
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        operator sf::Color() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:
        sf::Color m_color;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_COLOR_HPP

