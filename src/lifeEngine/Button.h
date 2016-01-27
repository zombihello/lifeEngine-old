#ifndef BUTTON_H
#define BUTTON_H

/////////////////
// LIFEENGINE
/////////////////
#include "System.h"

namespace le
{
    class Button
    {
    public:
        ///////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР
        //////////////////////////////
        Button( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~Button();

        //////////////////////////////////////
        // UPDATE BUTTON  | ОБНОВИТЬ КНОПКУ 
        /////////////////////////////////////
        void UpdateButton();

        //////////////////////////////////////
        // CREATE BUTTON  | СОЗДАТЬ КНОПКУ 
        /////////////////////////////////////
        void CreateButton( const sf::Text& Text );

        ///////////////////////////////////////////////
        // SET ID FOR BUTTON  | ЗАДАТЬ ID ДЛЯ КНОПКИ
        //////////////////////////////////////////////
        void SetId( const int id );

        ///////////////////////////////////////////////
        // GET TEXT BUTTON  | ПОЛУЧИТЬ ТЕКСТ КНОПКИ
        // @return Text
        //////////////////////////////////////////////
        Text GetText() const;

        ///////////////////////////////////////////////
        // GET COLOR BUTTON  | ПОЛУЧИТЬ ЦВЕТ КНОПКИ
        // @return �olor
        //////////////////////////////////////////////
        Color GetColor() const;

        //////////////////////////////////////////////////////
        // GET RECT BUTTON  | ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК КНОПКИ
        // @return FloatRect
        //////////////////////////////////////////////////////
        FloatRect GetRectButton() const;

        //////////////////////////////////////////////////////
        // GET ID BUTTON  | ПОЛУЧИТЬ ID КНОПКИ
        // @return int
        //////////////////////////////////////////////////////
        int GetIdButton() const;
    private:
        int                 iId;
        Text                Text;
        Color               Color;
        FloatRect           RectButton;
        RenderWindow*       RenderWindow;
        MouseCursor*        MouseCursor;
    };
}

#endif // BUTTON_H