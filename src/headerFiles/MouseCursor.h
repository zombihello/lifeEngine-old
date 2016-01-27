#ifndef MOUSE_CURSOR_H
#define MOUSE_CURSOR_H

//////////////////
// SYSTEM
/////////////////
#include <string>
using namespace std;

///////////////////
// SFML
//////////////////
#include <SFML\Graphics.hpp>
using namespace sf;

namespace le
{
    class MouseCursor
    {
    public:
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | йнмярпсйрнп он слнквюмхч
        ///////////////////////////////////////////////////
        MouseCursor();

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | деярпсйрнп он слнквюмхч
        ///////////////////////////////////////////////////
        ~MouseCursor();

        //////////////////////////////////////////////////////////////
        // LOAD TEXTURE FOR CURSOR  | гюцпсгхрэ рейярспс дкъ йспянпю
        //////////////////////////////////////////////////////////////
        void LoadTexture( const string sRoute );

        //////////////////////////////////////////////////////
        // UPDATE MOUSE POSITION  | намнбкемхе онгхжхх лшьх
        /////////////////////////////////////////////////////
        void MousePosition( RenderWindow& RenderWindow );

        /////////////////////////////////////
        // DRAW CURSOR  | мюпхянбюрэ йспянп
        ////////////////////////////////////
        void DrawCursor( RenderWindow& RenderWindow );

        /////////////////////////////////////////////////////
        // GET RECT CURSOR  | онксвхрэ опълнсцнкэмхй йспянпю  
        // @return RectCursor
        /////////////////////////////////////////////////////
        FloatRect GetRectCursor() const;

        /////////////////////////////////////////////////////
        // GET VECTOR CURSOR  | онксвхрэ бейрнп йспянпю  
        // @return Vector2i
        /////////////////////////////////////////////////////
        Vector2i GetVectorCursor() const;
    private:
        FloatRect       RectCursor;
        Texture         Texture;
        Sprite          Sprite;
    };
}

#endif // MOUSE_CURSOR_H