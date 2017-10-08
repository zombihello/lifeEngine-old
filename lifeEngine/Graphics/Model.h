﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef MODEL_H
#define MODEL_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

///////////////////////////
// СИСТЕМНЫЕ БИБЛИОТЕКИ
///////////////////////////
#include <map>
using namespace std;

////////////////
// LIFEENGINE
///////////////
#include <Graphics\Mesh.h>
#include <Graphics\Scene.h>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс для работы с моделью
	//////////////////////////////////////////////////////////////////////
	class DLL_API Model
	{
	public:
		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		//////////////////////////////////////////////////////////////////////
		Model();

		//////////////////////////////////////////////////////////////////////
		/// \brief Дуструктор
		//////////////////////////////////////////////////////////////////////
		~Model();

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить модель
		///		
		/// \param[in] NameModel Идентификатор модели
		/// \param[in] RouteToFile Путь к файлу
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		bool LoadModel( const string& NameModel, const string& RouteToFile );

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить модель с меша
		///		
		/// \param[in] Mesh меш для модели
		//////////////////////////////////////////////////////////////////////
		void LoadModel( Mesh& Mesh );

		//////////////////////////////////////////////////////////////////////
		/// \brief Добавить на сцену
		///		
		/// \param[in] Scene Сцена
		//////////////////////////////////////////////////////////////////////
		void AddToScene( Scene& Scene );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить с сцены
		//////////////////////////////////////////////////////////////////////
		void RemoveFromScene();

		//////////////////////////////////////////////////////////////////////
		/// \brief Указать сцену (необходима для уничтожения со сцены)
		///		
		/// \param[in] Scene Сцена
		//////////////////////////////////////////////////////////////////////
		void SetScene( Scene* Scene );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить информацию меша для рендера
		///		
		/// \return map<GLuint, Scene::InfoMesh>&
		//////////////////////////////////////////////////////////////////////
		map<GLuint, Scene::InfoMesh>& GetRenderMesh();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить менеджер анимаций
		///		
		/// \return AnimationManager*
		//////////////////////////////////////////////////////////////////////
		AnimationManager* GetAnimationManager();

		//////////////////////////////////////////////////////////////////////
		/// \brief Нет ли скелета
		///
		/// \return bool
		//////////////////////////////////////////////////////////////////////
		bool IsNoSkeleton();

	private:

		bool							NoSkeleton; ///< Нет ли скелета
		Scene*							Scene; ///< Сцена на котором находится модель
		Skeleton						Skeleton; ///< Скелет модели
		AnimationManager				AnimationManager; ///< Менеджер анимаций
		BoundingBox						BoundingBox; ///< Ограничивающее тело модели

		GLuint							VertexBuffer; ///< Вершиный буффер
		glm::vec3						Position; ///< Позиция модели в пространстве
		glm::mat4x4						MatrixPosition; ///< Матрица позиции
		glm::mat4x4						MatrixRotation; ///< Матрица вращения
		glm::mat4x4						MatrixScale; ///< Матрица масштабирования
		glm::mat4x4						MatrixTransformation; ///< Матрица трансформации (Pos+Scal+Rot)
		vector<GLuint>					IndexBuffers; ///< Массив индексных буфферов
		vector<GLuint>					ArrayBuffers; ///< Массив VAO буфферов
		map<GLuint, Scene::InfoMesh>	RenderMesh; ///< Меш для рендера
	};

	//-------------------------------------------------------------------------//
}

#endif // MODEL_H

