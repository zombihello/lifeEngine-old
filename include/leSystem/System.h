#ifndef LESYSTEM_H
#define LESYSTEM_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////////
/// СИСТЕМНЫЕ ЛИБЫ
////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//////////////
/// SFML
//////////////
#include <SFML/Graphics.hpp>
using namespace sf;

//////////
/// XML
//////////
#include <tinyxml.h>

////////////////
/// LIFEENGINE
///////////////
#include "BasicStagesGame.h"
#include "MouseCursor.h"
#include "FPS.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	struct DLL_API Configuration
	{
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Configuration();

		int           iWindowWidth;
		int           iWindowHeight;
		int           iVolumeSound;
		int           iVolumeMusic;
		int           iFrameLimit;

		float		  fKoefecientView;
		float         fGameSpeed;
		float         fGameTick;
		float         fTime;

		bool          bSound;
		bool          bMusic;
		bool          bV_Sinc;
		bool          bFullscreen;

		string        sWindowName;
		string        sGameVersion;
	};

	//-------------------------------------------------------------------------//
	
	class DLL_API System
	{
	public:
		/////////////////
		// КОНСТРУКТОР
		/////////////////
		System( const string FileConfiguration );

		///////////////////////////////
		/// ЧТЕНИЕ ПЕРЕМНОЙ ИЗ ФАЙЛА
		//////////////////////////////
		template<typename T> T ReadTextFile( const string sRoute, const string sTagName );

		//////////////////////////////////
		/// ЧТЕНИЕ ПЕРЕМНОЙ ИЗ XML ФАЙЛА
		/////////////////////////////////
		string ReadXmlFile( const string sRoute , const string sTagMain , const string sTag );

		///////////////////////////////
		/// СОХРАНЕНИЕ ПЕРЕМНОЙ В ФАЙЛ
		//////////////////////////////
		template<typename T> void SaveInFile( const string sRoute, const string sTagName, T Value, bool bClearFile = false ) const;

		/////////////////////////////////////
		/// ПРОВЕРКА НА СУЩЕСТВОВАНИЕ ФАЙЛА
		////////////////////////////////////
		bool FileExists( const string sRouteToFile );

		//////////////////////////////////////////
		/// ПРОВЕРКА НА СУЩЕСТВОВАНИЕ ДИРЕКТОРИИ
		/////////////////////////////////////////
		bool DirectoryExists( const string sRouteToDirectory );

		////////////////////
		/// СОЗДАНИЕ ОКНА
		///////////////////
		void WindowCreate( int iStyle = Style::Default );

		////////////////////
		/// ГЛАВНЫЙ ЦИКЛ
		///////////////////
		void MainLoop( BasicStagesGame& BasicStagesGame );

		///////////////////////////////
		/// ОБНОВИТЬ ФАЙЛ КОНФИГУРАЦИЙ
		///////////////////////////////
		void UpdateFileConfiguration();

		///////////////////////////
		/// ЗАДАТЬ СКОРОСТЬ ИГРЫ
		///////////////////////////
		void SetGameSpeed( const float fGameSpeed );

		///////////////////////////
		/// ЗАДАТЬ ЛИМИТ КАДРОВ
		///////////////////////////
		void SetFrameLimit( const int FrameLimit );

		///////////////////////////
		/// ЗАДАТЬ НАЗВАНИЕ ОКНА
		///////////////////////////
		void SetWindowTitle( const String WindowName , const String GameVersion );

		//////////////////////
		/// ЗАДАТЬ ТИК ИГРЫ
		//////////////////////
		void SetGameTick( const float fGameTick );

		///////////////////////////
		/// ПОЛУЧИТЬ КОНФИГУРАЦИИ
		///////////////////////////
		Configuration& GetConfiguration();

		//////////////////
		/// ПЛУЧИТЬ ОКНО
		/////////////////
		RenderWindow& GetWindow();

		//////////////////////
		/// ПОЛУЧИТЬ СОБЫТИЕ
		/////////////////////
		Event& GetEvent();

		//////////////////////////
		/// ПОЛУЧИТЬ КУРСОР МЫШИ
		//////////////////////////
		MouseCursor& GetMouseCursor();

		//////////////////////////////
		/// ПОЛУЧИТЬ КАМЕРУ ДЛЯ ИГРЫ
		/////////////////////////////
		View& GetGameCamera();

		//////////////////////////////
		/// ПОЛУЧИТЬ КАМЕРУ ДЛЯ МЕНЮ
		/////////////////////////////
		View GetMenuCamera();
	private:
		void Clock();

		string					  sRouteFileConfiguration;
		Configuration             Configuration;
		MouseCursor               MouseCursor;
		RenderWindow              RenderWindow;
		sf::Clock                 clock;
		Event                     Event;
		View				      MenuCamera;
		View				      GameCamera;
	};

	//-------------------------------------------------------------------------//
	
	template<typename T> void System::SaveInFile( const string sRoute, const string sTagName, T Value, bool bClearFile ) const
	{
		ofstream fos;

		if ( bClearFile )
			fos.open( sRoute );
		else
			fos.open( sRoute, ios::app );

		fos << sTagName << " " << Value << "\n";
	}

	//-------------------------------------------------------------------------//
	
	template<typename T> T System::ReadTextFile( const string sRoute, const string sTagName )
	{
		T Value = T();

		if ( FileExists( sRoute ) )
		{
			ifstream fis( sRoute );
			string sString;

			for ( int i = 0; !fis.eof(); i++ )
			{
				fis >> sString;

				if ( sString == sTagName )
				{
					fis >> Value;
					return Value;
				}

				if ( fis.eof() )
					cout << "Error: Tag [" << sTagName << "] Not Found In File [" << sRoute << "]\n";
			}
		}
		else
			cout << "Error: File [" << sRoute << "] Not Found\n";

		return Value;
	}

	//-------------------------------------------------------------------------//
}

#endif // LESYSTEM_H
