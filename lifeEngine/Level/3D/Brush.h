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
		///  ŒÕ—“–” “Œ–
		////////////////
		Brush( System& System );

		/////////////////
		/// ƒ≈—“–” “Œ–
		////////////////
		~Brush();

		//////////////////////////
		/// »Õ»÷»¿À»«¿÷»ﬂ ¡–¿ÿ¿
		/////////////////////////
		void InitBrush( PrimitivesType Type, GLuint Texture, vector<Vector3f> Vertex, vector<Vector2f> TextureCoords );

		///////////////////
		/// –≈Õƒ≈– ¡–¿ÿ¿
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
