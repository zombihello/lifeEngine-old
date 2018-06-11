//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef BRUSH_VERTEX_H
#define BRUSH_VERTEX_H
#pragma warning( push )

#pragma warning( disable: 4251 )

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////
// OPENGL
//////////////
#include <glew\glew.h>
#include <glm\glm.hpp>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Структура хранит информацию вершины (позиция,
	/// нормаль и т.д ) для браша
	//////////////////////////////////////////////////////////////////////
	struct DLL_API BrushVertex
	{
		glm::vec3		Position; ///< Позиция вершины
		glm::vec3		Normal; ///< Нормаль вершины
		glm::vec2		TextureCoord_DiffuseMap; ///< Текстурная координата для основной текстуры
		glm::vec2		TextureCoord_LightMap; ///< Текстурная координата для карты освещения

		bool operator==( BrushVertex& BrushVertex );
	};

	//-------------------------------------------------------------------------//
}

#pragma warning( pop )
#endif // BRUSH_VERTEX_H

