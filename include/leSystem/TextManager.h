#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "System.h"
#include "Text.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API TextManager
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		TextManager( System& System );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~TextManager();

		////////////////////
		/// ЗАГРУЗИТЬ ШРИФТ
		////////////////////
		void LoadFont( const string sRoute );

		///////////////////
		/// НАПИСАТЬ ТЕКСТ
		///////////////////
		void WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText );

		/////////////////////////////////
		/// НАПИСАТЬ ТЕКСТ С ПЕРЕМЕННОЙ
		/////////////////////////////////
		template<typename T> void WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText , T Value );

		///////////////////////
		/// ОБНОВЛЕНИЕ ТЕКСТОВ
		///////////////////////
		void UpdateText();

		///////////////////////
		/// УДАЛИТЬ ВЕСЬ ТЕКСТ
		///////////////////////
		void DeleteAllText();

		/////////////////////////
		/// УДАЛИТЬ ТЕКСТ ПО ID
		/////////////////////////
		void DeleteText( int ID );

		/////////////////////////
		/// ПОЛУЧИТЬ ТЕКСТ ПО ID
		/////////////////////////
		le::Text GetText( int ID );

		////////////////////
		/// ПОЛУЧИТЬ ШРИФТ
		///////////////////
		Font GetFont() const;
	private:
		System*					System;
		Font					Font;
		vector<le::Text*>       vText;
	};

	//-------------------------------------------------------------------------//
	
	template<typename T> void TextManager::WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText , T Value )
	{
		le::Text* Text = new le::Text( *System );

		Text->SetFont( Font );
		Text->WriteText( sText, iSize, PositionText, ColorText, Value );

		vText.push_back( Text );
	}

	//-------------------------------------------------------------------------//
}

#endif // TEXTMANAGER_H
