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
		Vector3f Position;
		Vector3f Normal;
		Vector2f TextureCoord;
		Color    VertexColor;

		bool operator==( VBO_ModelVertex& VBO_ModelVertex );
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ModelVertex
	{
		Matrix				MatrixBone;
		Matrix				MatrixBoneTmp;
		Matrix				MatrixVertex;

		Vector3f			Position;

		vector<int>			vIdVBO_Vertex;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ModelMesh
	{
		/////////////////
		/// ƒ≈—“–” “Œ–
		/////////////////
		~ModelMesh();

		//////////////////////////
		/// «¿√–”«»“‹ Ã≈ÿ ÃŒƒ≈À»
		//////////////////////////
		bool LoadMesh( string route );

		/////////////////
		/// Œ◊»—“»“‹ Ã≈ÿ
		/////////////////
		void Clear();

		Skeleton								Skeleton;
		AnimationManager3D						AnimationManager3D;

		vector<VBO_ModelVertex>					vVBO_Vertexs;
		map<string, vector<unsigned int> >		mIdIndexs;
		map<string, int>						mCountIndexs;
		map<int, ModelVertex>					mVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif //MODEL_MESH_H