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
		void WriteText( const String sText , const int iSize , Vector2f PositionText , Color ColorText );

		//////////////////////////////////
		/// НАПИСАТЬ ТЕКСТ С ПЕРЕМЕННОЙ
		/////////////////////////////////
		template<typename T> void WriteText( const String sText , const int iSize , Vector2f PositionText , Color ColorText , T Value );

		/////////////////////
		/// ОБНОВИТЬ ТЕКСТ
		/////////////////////
		void UpdateText();

		//////////////////
		/// ЗАДАТЬ ШРИФТ
		//////////////////
		void SetFont( const Font Font );

		//////////////////
		/// ЗАДАТЬ ТЕКСТ
		//////////////////
		void SetText( String Text );
		template<typename T> void SetText( String Text, T Value );

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
	
	template<typename T> void Text::WriteText( const String sText , const int iSize , Vector2f PositionText , Color ColorText , T Value )
	{
		wostringstream wostringstream;
		wostringstream << sText.toWideString() << " " << Value;

		text.setFont( Font );
		text.setString( wostringstream.str() );
		text.setCharacterSize( iSize );
		text.setColor( ColorText );
		text.setPosition( PositionText );
	}

	//-------------------------------------------------------------------------//

	template<typename T> void Text::SetText( String Text, T Value )
	{
		wostringstream wostringstream;
		wostringstream << Text.toWideString() << " " << Value;

		text.setString( wostringstream.str() );
	}

	//-------------------------------------------------------------------------//
}

#endif // TEXT_H
