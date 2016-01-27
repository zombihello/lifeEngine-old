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
        // CONSTRUCTOR | КОНСТРУКТОР
        ////////////////////////////////
        ButtonManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~ButtonManager();

        ///////////////////////////////////
        // CREATE BUTTON | СОЗДАТЬ КНОПКУ  
        //////////////////////////////////
        void CreateButton( const sf::Text& Text );

        //////////////////////////////////////////
        // UPDATE BUTTONS | ОБНОВЛЯЕМ ВСЕ КНОПКИ  
        /////////////////////////////////////////
        void UpdateButtons();

        /////////////////////////////////////////////
        // DELETE ALL BUTTONS | УДАЛЯЕМ ВСЕ КНОПКИ  
        ////////////////////////////////////////////
        void DeleteAllButtons();

        ///////////////////////////////////////////////
        // DELETE BUTTON BY ID | УДАЛЯЕМ КНОПКУ ПО ID  
        //////////////////////////////////////////////
        void DeleteButton( const int id );

        ///////////////////////////////////////////////////
        // GET TEMP ID BUTTON | ПОЛУЧАЕМ ТЕКУЩИЙ ID КНОПКИ 
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