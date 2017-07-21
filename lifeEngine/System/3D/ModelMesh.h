#ifndef MODEL_MESH_H
#define MODEL_MESH_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "Skeleton.h"
#include "../System.h"
#include "../../Animation/AnimationManager3D.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API VBO_ModelVertex
	{
		glm::vec3			Position;
		glm::vec3			Normal;
		glm::vec2			TextureCoord;
		Color				VertexColor;

		bool operator==( VBO_ModelVertex& VBO_ModelVertex );
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ModelVertex
	{
		glm::mat4x4			BoneTransform;

		glm::vec3			Position;
		glm::vec3			Normal;

		vector<int>			vIdVBO_Vertex;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ModelMesh
	{
		/////////////////
		/// ДЕСТРУКТОР
		/////////////////
		~ModelMesh();

		//////////////////////////
		/// ЗАГРУЗИТЬ МЕШ МОДЕЛИ
		//////////////////////////
		bool LoadMesh( string route );

		/////////////////
		/// ОЧИСТИТЬ МЕШ
		/////////////////
		void Clear();

		bool									IsCollisionMesh;

		Vector3f								SizeModel;

		Skeleton								Skeleton;
		AnimationManager3D						AnimationManager3D;

		vector<VBO_ModelVertex>					vVBO_Vertexs;
		vector<float>							vCollision_Vertexs;
		vector<int>								vCollision_IdVertexs;
		map<string, vector<unsigned int> >		mIdIndexs;
		map<int, ModelVertex>					mVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif //MODEL_MESH_H