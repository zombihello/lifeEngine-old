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
        // CONSTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        MouseCursor();

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~MouseCursor();

        //////////////////////////////////////////////////////////////
        // LOAD TEXTURE FOR CURSOR  | ЗАГРУЗИТЬ ТЕКСТУРУ ДЛЯ КУРСОРА
        //////////////////////////////////////////////////////////////
        void LoadTexture( const string sRoute );

        //////////////////////////////////////////////////////
        // UPDATE MOUSE POSITION  | ОБНОВЛЕНИЕ ПОЗИЦИИ МЫШИ
        /////////////////////////////////////////////////////
        void MousePosition( RenderWindow& RenderWindow );

        /////////////////////////////////////
        // DRAW CURSOR  | НАРИСОВАТЬ КУРСОР
        ////////////////////////////////////
        void DrawCursor( RenderWindow& RenderWindow );

        /////////////////////////////////////////////////////
        // GET RECT CURSOR  | ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК КУРСОРА  
        // @return RectCursor
        /////////////////////////////////////////////////////
        FloatRect& GetRectCursor();

        /////////////////////////////////////////////////////
        // GET VECTOR CURSOR  | ПОЛУЧИТЬ ВЕКТОР КУРСОРА  
        // @return Vector2i
        /////////////////////////////////////////////////////
        Vector2i& GetVectorCursor();
    private:
        FloatRect       RectCursor;
        Texture         Texture;
        Sprite          Sprite;
    };
}

#endif // MOUSE_CURSOR_H