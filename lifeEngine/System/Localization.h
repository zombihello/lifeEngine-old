#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////////
/// СИСТЕМНЫЕ ЛИБЫ
////////////////////
#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

/////////////////////
/// SFML
////////////////////
#include <SFML/System.hpp>
using namespace sf;

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Localization
	{
		//-------------------------------------------------------------------------//

	public:
		////////////////////////////////
		/// ЗАГРУЗИТЬ ФАЙЛ ЛОКАЛИЗАЦИИ
		////////////////////////////////
		void LoadFileLocalization( string Route, string LanguageName );

		////////////////////////////////
		/// ПОЛУЧИТЬ ПЕРЕВЕДЕНУЮ СТРОКУ
		////////////////////////////////
		String GetString( string LanguageName, string TagString );
	private:
		map< string, map<String,String> >			mLocalization;

		//-------------------------------------------------------------------------//
	};

	//-------------------------------------------------------------------------//
}

#endif // LOCALIZATION_H
