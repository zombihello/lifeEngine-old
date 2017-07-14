#ifndef BASICBRUSH_H
#define BASICBRUSH_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../../System/System.h"
#include <Physic/3D/Physic3D.h>
#include <Physic/3D/Body3D.h>

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
		Brush( System& System, Physic3D& Physic );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Brush();

		//////////////////
		/// СОЗДАТЬ БРАШ
		//////////////////
		void CreateBrush( PrimitivesType Type, GLuint Texture, vector<Vector3f> Vertex, vector<Vector2f> TextureCoords );

		///////////////////
		/// РЕНДЕР БРАША
		//////////////////
		void RenderBrush();

	private:

		int							iCountIndex;

		System*					    System;
		Physic3D*					Physic;
		Body3D*						Body;

		GLuint						TextureBrush;
		GLuint						VertexBuffer;
		GLuint						IndexBuffer;

		vector<float>				vCollision_Vertex;
		vector<int>					vCollision_IdVertex;
	};

	//-------------------------------------------------------------------------//
}

#endif
