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
        void UpdateConsole( RenderWindow& RenderWindow , Event& Event );

        //////////////////////////////////////
        // CLEAR CONSOLE  | ОЧИСТИТЬ КОНСОЛЬ  
        /////////////////////////////////////
        void ClearConsole();
    private:
        ///////////////////////////////////////
        // INPUT IN CONSOLE  | ВВОД В КОНСОЛЬ
        //////////////////////////////////////
        void InputInConsole( RenderWindow & RenderWindow , Event& Event );

        ///////////////////////////////////////
        // PARSING COMAND | ПАРСИНГ КОМАНДЫ  
        //////////////////////////////////////
        void ParsingComand( const string sComand );
        float                       fLastY;

        bool                        bConsoleOpen;

        RectangleShape              Background;
        Color                       ColorBackground;
        Font                        Font;
        Text                        InputText;
        vector<Text*>               vConsole;
    };
}

#endif //CONSOLE_H