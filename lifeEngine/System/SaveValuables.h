#ifndef SAVEVALUABLES_H
#define SAVEVALUABLES_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

///////////////
/// СИСТЕМА
///////////////
#include <map>
using namespace std;

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API SaveValuables
	{
		//////////////////////////////////
		/// ПОЛУЧИТЬ СВОЙСТВО ТИПА INT
		/////////////////////////////////
		int GetPropertyInt( string sName );

		//////////////////////////////////
		/// ПОЛУЧИТЬ СВОЙСТВО ТИПА FLOAT
		/////////////////////////////////
		float GetPropertyFloat( string sName );

		//////////////////////////////////
		/// ПОЛУЧИТЬ СВОЙСТВО ТИПА STRING
		/////////////////////////////////
		string GetPropertyString( string sName );

		//////////////////////////////////
		/// ПОЛУЧИТЬ ВЛОЖЕНЫЙ ТЭГ
		/////////////////////////////////
		SaveValuables GetTag( string sNameTag );

		bool							bIsLoadSave;

		string							sNameClass;

		map<string , string>			mProperties;
		map<string , SaveValuables>		mSaveValuables;
	};

	//-------------------------------------------------------------------------//
}

#endif // SAVEVALUABLES_H
