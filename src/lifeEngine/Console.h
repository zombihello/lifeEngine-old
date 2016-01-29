#ifndef CONSOLE_H
#define CONSOLE_H

/////////////////
// SFML
////////////////
#include <SFML\Graphics.hpp>
using namespace sf;

/////////////
// SYSTEM 
////////////
#include <vector>
#include <sstream>
using namespace std;

namespace le
{
    class Console
    {
    public:
        ///////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР
        //////////////////////////////
        Console( const int iWindowWidth , const int iWindowHeight );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~Console();

        ////////////////////////////////
        // LOAD FONT | ЗАГРУЗИТЬ ШРИФТ  
        ////////////////////////////////
        void LoadFont( const string sRoute );

        //////////////////////////////////////////
        // WRITE TO CONSOLE | НАПИСАТЬ В КОНСОЛЬ   
        //////////////////////////////////////////
        void WriteToConsole( const string sText , Color ColorText );
        // void WriteToConsole( sf::Text Text );

        ///////////////////////////////////////
        // UPDATE CONSOLE  | ОБНОВИТЬ КОНСОЛЬ  
        //////////////////////////////////////
        void UpdateCondole( RenderWindow& RenderWindow );

        //////////////////////////////////////
        // CLEAR CONSOLE  | ОЧИСТИТЬ КОНСОЛЬ  
        /////////////////////////////////////
        void ClearConsole();
    private:
        float                       fLastY;

        RectangleShape              Background;
        Color                       ColorBackground;
        Font                        Font;
        vector<Text*>               vConsole;
    };
}

#endif //CONSOLE_H