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

	struct DLL_API BrushVertex
	{
		Vector3f			Vertex;
		Vector2f			TextureCoord;

		bool operator==( BrushVertex& BrushVertex );
	};

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

		int							iCountIndex;

		System*					    System;

		GLuint						TextureBrush;
		GLuint						VertexBuffer;
		GLuint						IndexBuffer;
	};

	//-------------------------------------------------------------------------//
}

#endif
