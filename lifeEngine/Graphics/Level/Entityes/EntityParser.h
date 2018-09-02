﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef ENTITY_PARSER_H
#define ENTITY_PARSER_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

///////////////////////////
// СИСТЕМНЫЕ БИБЛИОТЕКИ
///////////////////////////
#include <string>
#include <vector>
#include <map>
using namespace std;

//////////////////
// LIFEENGINE
//////////////////
#include <Graphics\Level\BSPInfo.h>

namespace le
{
	//////////////////////////////////////////////////////////////////////
	/// \brief Структура для хранения распарсенных энтити-объектов
	//////////////////////////////////////////////////////////////////////
	struct Entity
	{
		string					ClassName;		///< Название энтити-объекта
		map<string, string>		Values;			///< Массив свойства энтити-объекта
	};

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс необходимый для парсинга энтити-объектов на карте
	//////////////////////////////////////////////////////////////////////
	class DLL_API EntityParser
	{
	public:

		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		//////////////////////////////////////////////////////////////////////
		EntityParser();

		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		///
		/// \param[in] BSPEntities Информация об энтитях которую нужно распарсить
		/// \param[in] Size Размер строки парсинга
		//////////////////////////////////////////////////////////////////////
		EntityParser( const BSPEntities& BSPEntities, int Size );

		//////////////////////////////////////////////////////////////////////
		/// \brief Распарсить информацию об энтитях
		///
		/// \param[in] BSPEntities Информация об энтитях которую нужно распарсить
		/// \param[in] Size Размер строки парсинга
		//////////////////////////////////////////////////////////////////////
		void Parse( const BSPEntities& BSPEntities, int Size );


		vector<Entity>			Entityes;		///< Распарсенная информация об энтитях
	};
}

#endif // ENTITY_PARSER_H