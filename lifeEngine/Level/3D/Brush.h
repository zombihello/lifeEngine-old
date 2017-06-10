#ifndef BASICBRUSH_H
#define BASICBRUSH_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../../System/System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Brush
	{
	public:
		//-------------------------------------------------------------------------//

		enum PrimitivesType
		{
			Cube,
			Sphere,
			Plane
		};

		//-------------------------------------------------------------------------//

		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Brush( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Brush();

		//////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ БРАША
		/////////////////////////
		void InitBrush( PrimitivesType Type, GLuint Texture, vector<Vector3f> Vertex, vector<Vector2f> TextureCoords );

		///////////////////
		/// РЕНДЕР БРАША
		//////////////////
		void RenderBrush();

	private:

		System*					    System;
		GLuint						TextureBrush;

		vector<Vector3f>			vVertex;
		vector<Vector2f>			vTextureCoords;
		vector<int>					vIdVertex;
	};

	//-------------------------------------------------------------------------//
}

#endif
