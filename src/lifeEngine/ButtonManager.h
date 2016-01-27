#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

//////////////////
// LIFEENGINE
/////////////////
#include "System.h"
#include "Button.h"

namespace le
{
    class ButtonManager
    {
    public:
        /////////////////////////////////
        // CONSTRUCTOR | йнмярпсйрнп
        ////////////////////////////////
        ButtonManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | деярпсйрнп он слнквюмхч
        ///////////////////////////////////////////////////
        ~ButtonManager();

        ///////////////////////////////////
        // CREATE BUTTON | янгдюрэ ймнойс  
        //////////////////////////////////
        void CreateButton( const Text& Text );

        //////////////////////////////////////////
        // UPDATE BUTTONS | намнбкъел бяе ймнойх  
        /////////////////////////////////////////
        void UpdateButtons();

        /////////////////////////////////////////////
        // DELETE ALL BUTTONS | сдюкъел бяе ймнойх  
        ////////////////////////////////////////////
        void DeleteAllButtons();

        ///////////////////////////////////////////////
        // DELETE BUTTON BY ID | сдюкъел ймнойс он ID  
        //////////////////////////////////////////////
        void DeleteButton( const int id );

        ///////////////////////////////////////////////////
        // GET TEMP ID BUTTON | онксвюел рейсыхи ID ймнойх 
        // @return int
        //////////////////////////////////////////////////
        int GetTmpIdButton() const;
    private:
        int                     iIdButton;
        RenderWindow*           RenderWindow;
        Vector2i*               CursorPosition;
        System*                 System;
        vector<Button*>         vButton;
    };
}

#endif // BUTTON_MANAGER_H