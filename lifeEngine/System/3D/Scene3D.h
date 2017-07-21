#ifndef SCENE_H
#define SCENE_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"
#include "../LoaderVAO.h"
#include "Camera.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API SceneInfoMesh
	{
		int					CountIndexs;

		GLuint				VertexArray;

		glm::mat4*			MatrixTransformation;
	};

	//-------------------------------------------------------------------------//

	/*struct DLL_API SceneRenderMesh
	{
		Shader*						Shader;
		glm::mat4					MatrixTransformation;

		//vector<SceneInfoMesh>		vInfoMesh;
	};*/

	//-------------------------------------------------------------------------//

	class DLL_API Scene3D
	{
	public:

		////////////////////////////////////
		/// КОНСТРУКТОР
		///////////////////////////////////
		Scene3D( System& System );
		Scene3D( System& System, Camera& PlayerCamera );

		////////////////////////////////////
		/// ЗАДАТЬ КАМЕРУ ИГРОКА
		///////////////////////////////////
		void SetPlayerCamera( Camera& PlayerCamera );

		////////////////////////////////////
		/// ДОБАВИТЬ МЕШ НА СЦЕНУ
		///////////////////////////////////
		void AddMeshToScene( GLuint Texture, SceneInfoMesh Mesh );
		void AddMeshToScene( map<GLuint, SceneInfoMesh> Mesh );

		///////////////////////////
		/// ОТРЕНДЕРИТЬ СЦЕНУ
		//////////////////////////
		void RenderScene();

		///////////////////////////
		/// ОЧИСТИТЬ СЦЕНУ
		//////////////////////////
		void ClearScene();

	private:

		Shader										ShaderRender;
		glm::mat4									ViewMatrix;

		System*										System;
		Camera*										PlayerCamera;
		glm::mat4*									ProjectionMatrix;
		
		map<GLuint, vector<SceneInfoMesh>>			mRenderBuffer;
	};

	//-------------------------------------------------------------------------//
}

#endif // SCENE_H
