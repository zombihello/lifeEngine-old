#ifndef TEXT_H
#define TEXT_H

///////////////
// LIFEENGINE
//////////////
#include "System.h"

namespace le
{
    class Text
    {
    public:
        //////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР
        /////////////////////////////
        Text( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~Text();

        ///////////////////////////////////
        // LOAD FONT | ЗАГРУЗИТЬ ШРИФТ 
        //////////////////////////////////
        void LoadFont( const string sRoute );

        ///////////////////////////////////
        //  WRITE TEXT  | НАПИСАТЬ ТЕКСТ     
        ///////////////////////////////////
        void WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText );

        ///////////////////////////////////////////////////////
        //  WRITE TEXT WITH VALUE | НАПИСАТЬ ТЕКСТ С ПЕРЕМЕННОЙ    
        ///////////////////////////////////////////////////////
        template<typename T> void WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText , T Value );

        ///////////////////////////////////////
        // UPDATE TEXT  | ОБНОВИТЬ ТЕКСТ  
        //////////////////////////////////////
        void UpdateText();

        //////////////////////////////////////////////
        // SET FONT | ЗАДАТЬ ШРИФТ
        /////////////////////////////////////////////
        void SetFont( const Font Font );

        //////////////////////////////////////////////
        // GET TEXT | ПОЛУЧИТЬ ТЕКСТ   
        // @return Text
        /////////////////////////////////////////////
        sf::Text GetText() const;

        //////////////////////////////////////////////
        // GET FONT | ПОЛУЧИТЬ ШРИФТ      
        // @return Font
        /////////////////////////////////////////////
        Font GetFont() const;
    private:
        RenderWindow*           RenderWindow;
        Font                    Font;
        sf::Text                text;
    };

    template<typename T> void Text::WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText , T Value )
    {
        ostringstream ostringstream;
        ostringstream << sText << " " << Value;

        text.setFont( Font );
        text.setString( ostringstream.str() );
        text.setCharacterSize( iSize );
        text.setColor( ColorText );
        text.setPosition( PositionText );
    }

}

#endif // TEXT_H