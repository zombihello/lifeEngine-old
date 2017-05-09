#ifndef MODEL_H
#define MODEL_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"
#include "Skeleton.h"
#include "../../Animation/AnimationManager3D.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API Vertex
	{
		Vector3f			Position;
		Vector3f			DefaultPosition;
		vector<int>			vIdBones;
		vector<float>		vIdWeihgt;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API TextureCoord
	{
		TextureCoord();

		Vector2f		Coords;
		GLuint			gl_Texture;
		bool			TextureEmpty;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API IDsVNTC
	{
		IDsVNTC();

		int		idVertex;
		int		idNormal;
		int		idTextureCoord;
		int		idVertexColor;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API Mesh
	{
		/////////////////
		/// ОЧИСТИТЬ МЕШ
		/////////////////
		void Clear();

		vector<IDsVNTC>			vIDs;

		vector<Vertex>			vVertexs;
		vector<Vector3f>		vNormals;
		vector<TextureCoord>	vTextureCoords;
		vector<Color>			vVertexColors;
	};

	//-------------------------------------------------------------------------//

	class DLL_API Model
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Model( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		/////////////////
		~Model();

		//////////////////////
		/// ЗАГРУЗКА МОДЕЛИ
		//////////////////////
		bool LoadModel( string route );

		//////////////////////
		/// ОТРИСОВКА МОДЕЛИ
		//////////////////////
		void RenderModel();

		////////////////////////////////
		/// ПОЛУЧИТЬ МЕНЕДЖЕР АНИМАЦИЙ
		////////////////////////////////
		AnimationManager3D& GetAnimationManager();

	private:

		Mesh							Mesh;
		Skeleton*						Skeleton;
		AnimationManager3D*				AnimationManager3D;

		System*							System;
	};

	//-------------------------------------------------------------------------//
}

#endif // MODEL_H