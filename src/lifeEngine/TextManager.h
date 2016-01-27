#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

////////////////////
// LIFEENGINE
///////////////////
#include "System.h"
#include <ostream>

namespace le
{
    class TextManager
    {
    public:
        //////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР
        /////////////////////////////
        TextManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~TextManager();

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
        // UPDATE TEXTS  | ОБНОВЛЕНИЕ ТЕКСТОВ 
        //////////////////////////////////////
        void UpdateText();

        //////////////////////////////////////////
        // DELETE ALL TEXT  | УДАЛИТЬ ВЕСЬ ТЕКСТ   
        /////////////////////////////////////////
        void DeleteAllText();

        //////////////////////////////////////////////
        // DELETE TEXT BY ID  | УДАЛИТЬ ТЕКСТ ПО ID      
        //////////////////////////////////////////////
        void DeleteText( int ID );

        //////////////////////////////////////////////
        // GET TEXT BY ID | ПОЛУЧИТЬ ТЕКСТ ПО ID    
        // @return Text
        /////////////////////////////////////////////
        Text GetText( int ID );

        //////////////////////////////////////////////
        // GET FONT | ПОЛУЧИТЬ ШРИФТ     
        // @return Font
        /////////////////////////////////////////////
        Font GetFont() const;
    private:
        RenderWindow*       RenderWindow;
        Font                Font;
        vector<Text*>       vText;
    };


    template<typename T> void TextManager::WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText , T Value )
    {
        vText.push_back( new sf::Text );
        sf::Text* Text = vText[ vText.size() - 1 ];

        ostringstream ostringstream;
        ostringstream << sText << " " << Value;

        Text->setFont( Font );
        Text->setString( ostringstream.str() );
        Text->setCharacterSize( iSize );
        Text->setColor( ColorText );
        Text->setPosition( PositionText );
    }
}

#endif // TEXT_MANAGER_H