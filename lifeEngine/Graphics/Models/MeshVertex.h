//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef MESH_VERTEX_H
#define MESH_VERTEX_H
#pragma warning( push )

#pragma warning( disable: 4251 ) 

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////
// OPENGL
//////////////
#include <glm\glm.hpp>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Структура хранит информацию вершины (позиция,
	/// нормаль и т.д )
	//////////////////////////////////////////////////////////////////////
	struct DLL_API MeshVertex
	{
		glm::vec3		Position; ///< Позиция вершины
		glm::vec3		Normal; ///< Нормаль вершины
		glm::vec2		TextureCoord; ///< Текстурная координата
		glm::vec4		Weights; ///< Веса костей
		glm::vec4		IdBones; ///< ID костей

		bool operator==( MeshVertex& MeshVertex );
	};

	//-------------------------------------------------------------------------//
}

#pragma warning( pop )
#endif // MESH_VERTEX_H

