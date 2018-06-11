//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef BRUSH_PLANE_H
#define BRUSH_PLANE_H
#pragma warning( push )

#pragma warning( disable: 4251 )

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

//////////////////
// LIFEENGINE
//////////////////
#include <Graphics\BrushVertex.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class Brush;

	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Структура хранит информацию плоскости браша (LightMap,
	/// текстура и т.д)
	//////////////////////////////////////////////////////////////////////
	struct DLL_API BrushPlane
	{
		int						CountIndexs; ///< Количество индексов вершин
		Brush*					Brush; ///< Указатель на браш

		GLuint					LightMap; ///< Карта освещения плоскости браша
		GLuint					ArrayBuffer; ///< VAO
		GLuint					IndexBuffer; ///< Индексный буффер

		vector<BrushVertex>		Vertexes; ///< Массив вершин
	};

	//-------------------------------------------------------------------------//
}

#pragma warning( pop )
#endif // BRUSH_PLANE_H