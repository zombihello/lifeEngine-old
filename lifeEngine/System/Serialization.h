#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

///////////////
/// СИСТЕМА
///////////////
#include <map>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

///////////////
/// LIFEENGINE
///////////////
#include "SaveValuables.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Serialization
	{
	public:
		//////////////////////////////////
		/// КОНСТРУКТОР
		/////////////////////////////////
		Serialization( string sNameClass );

		//////////////////////////////////
		/// ДЕСТРУКТОР
		/////////////////////////////////
		~Serialization();

		//////////////////////////////////
		/// СОХРАНИТЬ ПЕРЕМЕННУЮ В БУФЕР
		/////////////////////////////////
		void Add( int value, string nameValue );
		void Add( float value , string nameValue );
		void Add( string value , string nameValue );

		//////////////////////////////////
		/// СОЗДАТЬ ВЛОЖЕНЫЙ ТЭГ
		/////////////////////////////////
		void CreateTag( string nameTag );

		/////////////////////////////////////
		/// ПОЛУЧИТЬ СОХРАНЕННЫЕ ПЕРЕМЕННЫЕ
		/////////////////////////////////////
		SaveValuables* GetValuables();

		//////////////////////////////////////////
		/// ПОЛУЧИТЬ СПИСОК СОХРАНЕННЫХ ПЕРЕМЕННЫХ
		//////////////////////////////////////////
		static vector<SaveValuables> GetSaveValuables();

		//////////////////////////////////////////////////////////////////////
		/// СОХРАНИТЬ В АТРИБУТЫ [SAVE] ТЕКУЩЮЮ КАРТУ И НАЗВАНИЕ СОХРАНЕНИЯ 
		/////////////////////////////////////////////////////////////////////
		static void SetAttributsSave( string titleSave, string levelSave );

		//////////////////////////////////////////
		/// ОЧИСТИТЬ БУФЕР СОХРАНЕНИЯ
		//////////////////////////////////////////
		static void Clear();

		//////////////////////////////////////////
		/// СОХРАНИТЬ БУФЕР В ФАЙЛ
		//////////////////////////////////////////
		static void SaveInFile( string route );

		//////////////////////////////////////////
		/// ЗАГРУЗИТЬ СОХРАНЕНИЯ С ФАЙЛА
		//////////////////////////////////////////
		static void LoadFromFile( string route );
	private:
		bool				isCreateTag;

		int					iIdClass;

		string				sNameClass;	
	};

	//-------------------------------------------------------------------------//
}

#endif //SERIALIZATION_H
