#ifndef BASICBRUSH_H
#define BASICBRUSH_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include <System/System.h>
#include <Physic/3D/Physic3D.h>
#include <Physic/3D/Body3D.h>
#include <System/3D/Scene3D.h>

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API BrushVertex
	{
		glm::vec3			Vertex;
		glm::vec3			Normal;
		glm::vec2			TextureCoord;

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
		Brush( System& System, Scene3D& Scene, Physic3D& Physic );
		Brush( System& System, Physic3D& Physic );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Brush();

		/////////////////////////////
		/// ДОБАВИТЬ БРАШ НА СЦЕНУ
		/////////////////////////////
		void AddToScene( Scene3D& Scene );

		//////////////////
		/// СОЗДАТЬ БРАШ
		//////////////////
		void CreateBrush( PrimitivesType Type, GLuint Texture, vector<glm::vec3> Vertex, vector<glm::vec3> Normals, vector<glm::vec2> TextureCoords );

		///////////////////
		/// РЕНДЕР БРАША
		//////////////////
		void UpdateBrush( Shader* Shader = NULL );

	private:

		System*										System;
		Physic3D*									Physic;
		Body3D*										Body;
		Scene3D*									Scene;

		GLuint										VertexBuffer;
		GLuint										VertexArray;
		GLuint										IndexBuffer;

		glm::mat4x4									MatrixTransformation;

		vector<float>								vCollision_Vertex;
		vector<int>									vCollision_IdVertex;
		map<GLuint, SceneInfoMesh>					mRenderMesh;
	};

	//-------------------------------------------------------------------------//
}

#endif
