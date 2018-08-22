﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef ENTITY_H
#define ENTITY_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

///////////////////////////
// СИСТЕМНЫЕ БИБЛИОТЕКИ
///////////////////////////
#include <vector>
#include <map>
using namespace std;

//////////////
// OPENGL
//////////////
#include <glew\glew.h>
#include <glm\glm.hpp>

//////////
// XML
//////////
#include <tinyxml.h>

//////////////////
// LIFEENGINE
//////////////////
#include <System\System.h>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс для хранения энтити-объектов карты
	///
	/// Данный класс необходим для хранения энтити-объектов карты, например
	/// точка старта игрока, спавн npc и т.д
	//////////////////////////////////////////////////////////////////////
	class DLL_API Entity
	{
	public:
		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		///
		/// \param[in] EntityData Информация о энтити
		//////////////////////////////////////////////////////////////////////
		Entity( const string& EntityData );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить название энтити
		///
		/// \return string&
		//////////////////////////////////////////////////////////////////////
		string& GetNameEntity();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить значение энтити строкой
		///
		/// \param[in] NameValue Название значения
		/// \return string&. Если значения нету, то вернет пустую строку
		//////////////////////////////////////////////////////////////////////
		string GetValueString( const string& NameValue );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить значение энтити целочисленным числом
		///
		/// \param[in] NameValue Название значения
		/// \return int. Если значения нету, то вернет -1
		//////////////////////////////////////////////////////////////////////
		int GetValueInt( const string& NameValue );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить значение энтити дробью
		///
		/// \param[in] NameValue Название значения
		/// \return int. Если значения нету, то вернет -1
		//////////////////////////////////////////////////////////////////////
		float GetValueFloat( const string& NameValue );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить значение энтити массивом строк
		/// \details Данный метод возвращает массив чисел/слов которые
		/// были разделены пробелами, например [NameValue] "10 5 3", то вернет
		/// массив: "10", "5", "3"
		///
		/// \param[in] NameValue Название значения
		/// \return vector<string>
		//////////////////////////////////////////////////////////////////////
		vector<string> GetVelueVectorString( const string& NameValue );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить значение энтити массивом целочисленных чисел
		/// \details Данный метод возвращает массив чисел/слов которые
		/// были разделены пробелами, например [NameValue] "10 5 3", то вернет
		/// массив: 10, 5, 3
		///
		/// \param[in] NameValue Название значения
		/// \return vector<int>
		//////////////////////////////////////////////////////////////////////
		vector<int> GetVelueVectorInt( const string& NameValue );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить значение энтити массивом дробных чисел
		/// \details Данный метод возвращает массив чисел/слов которые
		/// были разделены пробелами, например [NameValue] "10.5 5.21 3.01", то вернет
		/// массив: 10.5, 5.21, 3.01
		///
		/// \param[in] NameValue Название значения
		/// \return vector<float>
		//////////////////////////////////////////////////////////////////////
		vector<float> GetVelueVectorFloat( const string& NameValue );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить позицию энтити в мире
		///
		/// \return glm::vec3&
		//////////////////////////////////////////////////////////////////////
		glm::vec3& GetPosition();

	private:

		string					NameEntity; ///< Название энтити
		glm::vec3				Position; ///< Позиция энтити в мире
		map<string, string>		Values; ///< Массив значений
	};

	//-------------------------------------------------------------------------//
}

#endif // ENTITY_H