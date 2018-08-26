﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef FUNC_DOOR_H
#define FUNC_DOOR_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////////
// LIFEENGINE
//////////////////
#include <Graphics\BaseEntity.h>
#include <Graphics\BSPInfo.h>

namespace le
{
	class Func_Door : public BaseEntity
	{
	public:

		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		///
		/// \param[in] Values Переменные энтити-объекта
		/// \param[in] Model Модель привязанная к энтити
		//////////////////////////////////////////////////////////////////////
		Func_Door( const map<string, string>& Values, BSPModel* Model );

		//////////////////////////////////////////////////////////////////////
		/// \brief Обновить энтити-объект
		//////////////////////////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////////////////////////
		/// \brief Выполнить действие энтити-объекта
		//////////////////////////////////////////////////////////////////////
		void Action();

	private:

		int					Count;
		bool				MovingRight;
		BSPModel*			Model;				///< Модель привязанная к энтити		
	};
}

#endif // FUNC_DOOR_H
