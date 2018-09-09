﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef PHYSICS_H
#define PHYSICS_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////////
// LIFEENGINE
//////////////////
#include <System\System.h>
#include <Physics\Body.h>

namespace le
{
	//-------------------------------------------------------------------------//
	
	class Level;

	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief  Класс для симуляции физики в движке
	//////////////////////////////////////////////////////////////////////
	class DLL_API Physics
	{
	public:

		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		///
		/// \param[in] Gravity Сила гравитации
		//////////////////////////////////////////////////////////////////////
		Physics( const glm::vec3& Gravity = glm::vec3( 0.f, 9.8f, 0.f ) );

		//////////////////////////////////////////////////////////////////////
		/// \brief Деструктор
		//////////////////////////////////////////////////////////////////////
		~Physics();

		//////////////////////////////////////////////////////////////////////
		/// \brief Обновить симуляцию физики
		//////////////////////////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////////////////////////
		/// \brief Добавить уровень в симуляцию
		///
		/// \param[in] Level Уровень
		//////////////////////////////////////////////////////////////////////
		void AddLevel( Level& Level );

		//////////////////////////////////////////////////////////////////////
		/// \brief Убрать уровень из симул¤ции
		//////////////////////////////////////////////////////////////////////
		void RemoveLevel();

		//////////////////////////////////////////////////////////////////////
		/// \brief Создать тело для симул¤ции
		///
		/// \return Указатель на физическое тело в массиве
		//////////////////////////////////////////////////////////////////////
		Body* CreateBody();

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить тело из симуляции
		///
		/// \param[in] Body Физическое тело которое нужно удалить из симулции
		//////////////////////////////////////////////////////////////////////
		void DeleteBody( Body* Body );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все тела из симуляции
		//////////////////////////////////////////////////////////////////////
		void DeleteAllBodyes();

		//////////////////////////////////////////////////////////////////////
		/// \brief Очистить симуляцию физики
		//////////////////////////////////////////////////////////////////////
		void Clear();

		//////////////////////////////////////////////////////////////////////
		/// \brief Задать силу гравитации
		///
		/// \param[in] Gravity Сила гравитации
		//////////////////////////////////////////////////////////////////////
		void SetGravity( const glm::vec3& Gravity );

	private:

		Level*					Level;		///< Уровень

		vector<Body>			Bodyes;		///< Массив физических тел
	};

	//-------------------------------------------------------------------------//
}

#endif // PHYSICS_H
