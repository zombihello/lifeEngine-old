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
#include "ModelMesh.h"
#include "../../Efects/3D/Light3D.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API SceneInfoMesh
	{
		int					CountIndexs;

		GLuint				VertexArray;
		Material			Material;

		glm::mat4*			MatrixTransformation;
	};

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
		/// ДЕСТРУКТОР
		///////////////////////////////////
		~Scene3D();

		////////////////////////////////////
		/// ЗАДАТЬ КАМЕРУ ИГРОКА
		///////////////////////////////////
		void SetPlayerCamera( Camera& PlayerCamera );

		////////////////////////////////////
		/// ДОБАВИТЬ МЕШ НА СЦЕНУ
		///////////////////////////////////
		void AddMeshToScene( GLuint Texture, SceneInfoMesh Mesh );
		void AddMeshToScene( map<GLuint, SceneInfoMesh> Mesh );

		/////////////////////////////////////////
		///// ДОБАВИТЬ ИСТОЧНИК СВЕТА НА СЦЕНУ
		/////////////////////////////////////////
		void AddLightToScene( Light3D Light );

		///////////////////////////
		/// ОТРЕНДЕРИТЬ СЦЕНУ
		//////////////////////////
		void RenderScene();

		///////////////////////////
		/// ОЧИСТИТЬ СЦЕНУ
		//////////////////////////
		void ClearScene();

	private:

		Shader										GeometryRender;
		glm::mat4									ViewMatrix;

		System*										System;
		Camera*										PlayerCamera;
		glm::mat4*									ProjectionMatrix;
		
		vector<Light3D>								vPointLights;
		map<GLuint, vector<SceneInfoMesh>>			mRenderBuffer;
	};

	//-------------------------------------------------------------------------//
}

#endif // SCENE_H
