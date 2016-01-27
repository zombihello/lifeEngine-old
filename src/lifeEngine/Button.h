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
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | йнмярпсйрнп
        ///////////////////////////////////////////////////
        Button( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | деярпсйрнп он слнквюмхч
        ///////////////////////////////////////////////////
        ~Button();

        //////////////////////////////////////
        // UPDATE BUTTON  | намнбкъел ймнойс  
        /////////////////////////////////////
        void UpdateButton();

        //////////////////////////////////////
        // CREATE BUTTON  | янгдюел ймнойс  
        /////////////////////////////////////
        void CreateButton( const Text& Text );

        ///////////////////////////////////////////////
        // SET ID FOR BUTTON  | гюдюел мнлеп ймнойх
        //////////////////////////////////////////////
        void SetId( const int id );

        ///////////////////////////////////////////////
        // GET TEXT BUTTON  | онксвюел рейяр ймнойх
        // @return Text
        //////////////////////////////////////////////
        Text GetText() const;

        ///////////////////////////////////////////////
        // GET COLOR BUTTON  | онксвюел жбер ймнойх
        // @return яolor
        //////////////////////////////////////////////
        Color GetColor() const;

        //////////////////////////////////////////////////////
        // GET RECT BUTTON  | онксвюел опълнсцнкэмхй ймнойх
        // @return FloatRect
        //////////////////////////////////////////////////////
        FloatRect GetRectButton() const;

        //////////////////////////////////////////////////////
        // GET ID BUTTON  | онксвюел онпъдйнбши мнлеп ймнойх
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