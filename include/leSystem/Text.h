#ifndef TEXT_H
#define TEXT_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "System.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API Text
	{
	public:
		//////////////////
		/// КОНСТРУКТОР
		/////////////////
		Text( System& System );

		//////////////////////
		/// ЗАГРУЗИТЬ ШРИФТ
		/////////////////////
		void LoadFont( const string sRoute );

		////////////////////
		/// НАПИСАТЬ ТЕКСТ
		///////////////////
		void WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText );

		//////////////////////////////////
		/// НАПИСАТЬ ТЕКСТ С ПЕРЕМЕННОЙ
		/////////////////////////////////
		template<typename T> void WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText , T Value );

		/////////////////////
		/// ОБНОВИТЬ ТЕКСТ
		/////////////////////
		void UpdateText();

		//////////////////
		/// ЗАДАТЬ ШРИФТ
		//////////////////
		void SetFont( const Font Font );

		////////////////////
		/// ПОЛУЧИТЬ ТЕКСТ
		////////////////////
		sf::Text& GetText();

		/////////////////////
		/// ПОЛУЧИТЬ ШРИФТ
		/////////////////////
		Font GetFont() const;
	private:
		RenderWindow*           RenderWindow;
		Font                    Font;
		sf::Text                text;
	};

	//-------------------------------------------------------------------------//
	
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

	//-------------------------------------------------------------------------//
}

#endif // TEXT_H
