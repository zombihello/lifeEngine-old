﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef RAY_H
#define RAY_H

//////////////////
// LIFEENGINE
//////////////////
#include <System\System.h>

namespace le
{
	//-------------------------------------------------------------------------//

	enum TraceType
	{
		TRACE_TYPE_RAY,				///< Тип трасировки лучем
		TRACE_TYPE_SPHERE			///< Тип трасировки сферой
	};

	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Структура хранящая информацию о луче
	//////////////////////////////////////////////////////////////////////
	struct Ray
	{
		TraceType			TraceType;			///< Тип трасировки луча
		float				TraceRadius;		///< Радиус трасировки луча

		glm::vec3			Start;				///< Начало луча
		glm::vec3			End;				///< Конец луча

		bool operator==( Ray& Ray );
	};

	//-------------------------------------------------------------------------//
}

#endif // RAY_H