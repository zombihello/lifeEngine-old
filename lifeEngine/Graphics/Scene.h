﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef SCENE_H
#define SCENE_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

///////////////////////////
// СИСТЕМНЫЕ БИБЛИОТЕКИ
///////////////////////////
#include <vector>
#include <map>
using namespace std;

//////////////
// OPENGL
//////////////
#include <glew\glew.h>
#include <glm\glm.hpp>

//////////////////
// SFML
//////////////////
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
using namespace sf;

////////////////
// LIFEENGINE
///////////////
#include <System\System.h>
#include <System\GBuffer.h>
#include <Graphics\LightQuad.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class Model;
	class BoundingBox;
	class Camera;
	class Frustum;
	class Level;
	struct Plane;
	struct InfoBSPPolygon;
	class Skeleton;
	class PointLight;
	class DirectionalLight;
	class SpotLight;
	class LightManager;
	class Skybox;

	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс для работы со сценой
	///
	/// Данный класс необходим для рендера графики на экран
	//////////////////////////////////////////////////////////////////////
	class DLL_API Scene
	{
	public:

		//-------------------------------------------------------------------------//

		//////////////////////////////////////////////////////////////////////
		/// \brief Структура хранения меша для сцены
		//////////////////////////////////////////////////////////////////////
		struct DLL_API InfoMesh
		{
			//////////////////////////////////////////////////////////////////////
			/// \brief Конструктор
			//////////////////////////////////////////////////////////////////////
			InfoMesh();

			int					CountIndexs; ///< Количество индексов вершин
			bool*				IsRender; ///< Рендерить ли этот меш

			GLuint				VertexArray; ///< VAO
			Skeleton*			Skeleton; ///< Скелет меша
			BoundingBox*		BoundingBox; ///< Ограничивающее тело меша

			glm::vec3*			Position; ///< Позиция меша на сцене
			glm::mat4*			MatrixTransformation; ///< Матрица трансформации меша
		};

		//-------------------------------------------------------------------------//

		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		//////////////////////////////////////////////////////////////////////
		Scene();

		//////////////////////////////////////////////////////////////////////
		/// \brief Деструктор
		//////////////////////////////////////////////////////////////////////
		~Scene();

		//////////////////////////////////////////////////////////////////////
		/// \brief Добавить модель на сцену
		///		
		/// \param[in] Model Указатель на модель
		//////////////////////////////////////////////////////////////////////
		void AddModel( Model* Model );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить модель с сцены
		///		
		/// \param[in] Model Указатель на модель
		//////////////////////////////////////////////////////////////////////
		void RemoveModel( Model* Model );

		//////////////////////////////////////////////////////////////////////
		/// \brief Добавить уровень на сцену
		///		
		/// \param[in] Level Указатель на уровень
		//////////////////////////////////////////////////////////////////////
		void AddLevel( Level* Level );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить уровень со сцены
		//////////////////////////////////////////////////////////////////////
		void RemoveLevel();

		//////////////////////////////////////////////////////////////////////
		/// \brief Добавить менеджер света на сцену
		///		
		/// \param[in] LightManager Указатель на менеджер света
		//////////////////////////////////////////////////////////////////////
		void AddLightManager( LightManager* LightManager );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить менеджер света со сцены
		///		
		/// \param[in] LightManager Указатель на менеджер света
		//////////////////////////////////////////////////////////////////////
		void RemoveLightManager( LightManager* LightManager );

		//////////////////////////////////////////////////////////////////////
		/// \brief Убрать камеру со сцены
		///
		/// \param[in] NameCamera Идентификатор камеры
		//////////////////////////////////////////////////////////////////////
		void RemoveCamera( const string& NameCamera );

		//////////////////////////////////////////////////////////////////////
		/// \brief Убрать все камеры со сцены
		//////////////////////////////////////////////////////////////////////
		void RemoveAllCameras();

		//////////////////////////////////////////////////////////////////////
		/// \brief Отрендерить сцену
		//////////////////////////////////////////////////////////////////////
		void Render();

		//////////////////////////////////////////////////////////////////////
		/// \brief Очистить сцену
		//////////////////////////////////////////////////////////////////////
		void Clear();

		//////////////////////////////////////////////////////////////////////
		/// \brief Задать камеру
		///	
		/// \param[in] NameCamera Идентификатор камеры
		/// \param[in] Camera Камера
		//////////////////////////////////////////////////////////////////////
		void SetCamera( const string& NameCamera, Camera& Camera );

		//////////////////////////////////////////////////////////////////////
		/// \brief Задать активную камеру
		///		
		/// \param[in] NameCamera Идентификатор камеры
		/// \return Активная камера на сцене
		//////////////////////////////////////////////////////////////////////
		Camera* SetActiveCamera( const string& NameCamera );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить G-Буффер сцены
		///		
		/// \return G-Буффер сцены
		//////////////////////////////////////////////////////////////////////
		GBuffer& GetGBuffer();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить карту наход. на сцене
		///		
		/// \return Указать на уровень, если его нет, то вернет NULL
		//////////////////////////////////////////////////////////////////////
		Level* GetLevel();

	private:

		//////////////////////////////////////////////////////////////////////
		/// \brief Построить карты теней
		//////////////////////////////////////////////////////////////////////
		void BuildShadowMaps();

		//////////////////////////////////////////////////////////////////////
		/// \brief Отрендерить геометрию сцены
		//////////////////////////////////////////////////////////////////////
		void GeometryRender();

		//////////////////////////////////////////////////////////////////////
		/// \brief Отрендерить геометрию сцены с использованием
		/// GBuffer'а
		//////////////////////////////////////////////////////////////////////
		void GeometryRender_GBuffer();
		
		//////////////////////////////////////////////////////////////////////
		/// \brief Просчитать динамическое освещение
		//////////////////////////////////////////////////////////////////////
		void CalculateLight();

		Shader*								AnimationModelsRender;					///< Шейдер рендера анимируемых моделей
		Shader*								StaticModelsRender;						///< Шейдер рендера статичных моделей
		Shader*								LevelRender;							///< Шейдер рендера уровня
		Shader*								AnimationModelsRender_GBuffer;			///< Шейдер рендера анимируемых моделей (с исп. GBuffer)
		Shader*								StaticModelsRender_GBuffer;				///< Шейдер рендера статичных моделей (с исп. GBuffer)
		Shader*								LevelRender_GBuffer;					///< Шейдер рендера уровня (с исп. GBuffer)
		Shader*								TestRender;								///< Шейдер тестового рендера на перекрытия
		Shader*								PointLightRender;						///< Шейдер точечного света
		Shader*								DirectionalLightRender;					///< Шейдер направленого света
		Shader*								SpotLightRender;						///< Шейдер прожекторного света

		glm::vec3*							PositionCamera;							///< Позиция камеры
		glm::mat4*							ViewMatrix;								///< Матрица вида
		glm::mat4*							ProjectionMatrix;						///< Матрица проекции
		glm::mat4							PVMatrix;								///< Матрица Projection * View
		glm::mat4							PVTMatrix;								///< Матрица Projection * View * Transformation

		Frustum*							Frustum;								///< Пирамида усечения
		Camera*								ActiveCamera;							///< Активная камера на сцене
		Level*								Level;									///< Уровень который нах. на сцене
		LightManager*						LightManager;							///< Менеджер света который прикреплен к сцене
		Skybox*								Skybox;									///< Скайбокс сцены
		GBuffer								GBuffer;								///< G-Буфер	
		LightQuad							LightQuad;								///< Квадрат для рендера света

		vector<Model*>						Models;									///< Массив моделей на сцене
		vector<PointLight>*					PointLights;							///< Массив точечный источников которые нах. на сцене
		vector<SpotLight>*					SpotLights;								///< Массив прожекторных источников которые нах. на сцене
		vector<DirectionalLight>*			DirectionalLights;						///< Массив направленных источников света которые нах. на сцене

		map<string, le::Camera*>			CamerasInScene;							///< Массив камер на сцене
		vector<InfoBSPPolygon>*				RenderBuffer_Level;						///< Буффер рендера карты
			
		map<GLuint, vector<InfoMesh*> >		RenderBuffer_AnimationModel;			///< Буффер рендера анимируемых моделей
		map<GLuint, vector<InfoMesh*> >		RenderBuffer_StaticModel;				///< Буффер рендера статичных моделей (не анимируемых)	
	};

	//-------------------------------------------------------------------------//
}

#endif // SCENE_H
