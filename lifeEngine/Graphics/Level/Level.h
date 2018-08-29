﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef LEVEL_H
#define LEVEL_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////////
// LIFEENGINE
//////////////////
#include <System\VAO.h>
#include <System\Bitset.h>
#include <Graphics\Level\Entityes\EntityParser.h>
#include <Graphics\Level\Skybox.h>
#include <Graphics\Level\Plane.h>
#include <Graphics\Level\Entityes\BaseEntity.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class LightManager;
	class Scene;
	class Model;
	class PointLight;
	class SpotLight;

	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс для работы с уровнем
	//////////////////////////////////////////////////////////////////////
	class DLL_API Level
	{
	public:

		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		//////////////////////////////////////////////////////////////////////
		Level( System& System );

		//////////////////////////////////////////////////////////////////////
		/// \brief Деструктор
		//////////////////////////////////////////////////////////////////////
		~Level();

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить карту в формате BSP (Версия Quake 3)
		///
		/// \param[in] Route Путь к файлу
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		bool LoadLevel( const string& Route );

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
		/// \brief Очистить уровень
		//////////////////////////////////////////////////////////////////////
		void ClearLevel();

		//////////////////////////////////////////////////////////////////////
		/// \brief Посчитать видимые плоскости
		///	
		/// \param[in] Camera Камера игрока
		//////////////////////////////////////////////////////////////////////
		void CalculateVisablePlanes( Camera& Camera );

		//////////////////////////////////////////////////////////////////////
		/// \brief Посчитать видимые плоскости
		///	
		/// \param[in] Position Позиция игрока
		/// \param[in] Frustum Пирамида отсечения
		//////////////////////////////////////////////////////////////////////
		void CalculateVisablePlanes( const glm::vec3& Position, Frustum& Frustum );

		//////////////////////////////////////////////////////////////////////
		/// \brief Посчитать видимые модели
		///	
		/// \param[in] Models Массив моделей для проверки
		/// \param[in] Frustum Пирамида отсечения
		//////////////////////////////////////////////////////////////////////
		void CalculateVisableModels( vector<Model*>& Models, Frustum& Frustum );

		//////////////////////////////////////////////////////////////////////
		/// \brief Посчитать видимые источники света
		///	
		/// \param[in] Lights Массив источников света для проверки
		/// \param[in] Frustum Пирамида отсечения
		/// \return Был ли увиден хотя бы один источник света
		//////////////////////////////////////////////////////////////////////
		bool CalculateVisableLights( vector<PointLight>& Lights, Frustum& Frustum );

		//////////////////////////////////////////////////////////////////////
		/// \brief Посчитать видимые источники света
		///	
		/// \param[in] Lights Массив источников света для проверки
		/// \param[in] Frustum Пирамида отсечения
		/// \return Был ли увиден хотя бы один источник света
		//////////////////////////////////////////////////////////////////////
		bool CalculateVisableLights( vector<SpotLight>& Lights, Frustum& Frustum );

		//////////////////////////////////////////////////////////////////////
		/// \brief Проверить на видимость модель
		///	
		/// \param[in] Models Проверяемая модель
		/// \param[in] Frustum Пирамида отсечения
		/// \return True - видна модель, False - не видна
		//////////////////////////////////////////////////////////////////////
		bool CalculateVisableModel( Model& Model, Frustum& Frustum );

		//////////////////////////////////////////////////////////////////////
		/// \brief Проверить на видимость источник света
		///	
		/// \param[in] Light Проверяемый источник света
		/// \param[in] Frustum Пирамида отсечения
		/// \return True - виден источник, False - не видн
		//////////////////////////////////////////////////////////////////////
		bool CalculateVisableLight( PointLight& Light, Frustum& Frustum );

		//////////////////////////////////////////////////////////////////////
		/// \brief Проверить на видимость источник света
		///	
		/// \param[in] Light Проверяемый источник света
		/// \param[in] Frustum Пирамида отсечения
		/// \return True - виден источник, False - не видн
		//////////////////////////////////////////////////////////////////////
		bool CalculateVisableLight( SpotLight& Light, Frustum& Frustum );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить скайбокс уровня
		///		
		/// \return Skybox&.
		//////////////////////////////////////////////////////////////////////
		Skybox& GetSkybox();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить VAO уровня
		///		
		/// \return Массив буфером уровня в OpenGL
		//////////////////////////////////////////////////////////////////////
		GLuint& GetArrayBuffer();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить энтити по названию
		///		
		/// \param[in] NameEntity Название энтити
		/// \return Указатель на энтити. Если не нашли энтити,то вернет NULL
		//////////////////////////////////////////////////////////////////////
		template<typename T> T* GetEntity( const string& NameEntity );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить массив энтити с одинаковым названием
		///		
		/// \param[in] NameEntity Название энтити
		/// \return Массив указатилей на энтити. Если не нашли энтити,то вернет NULL
		//////////////////////////////////////////////////////////////////////
		template<typename T> vector<T*> GetEntityes( const string& NameEntity );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить буфер видимых плоскостей
		///
		/// \return Буфер видимых плоскостей на уровне
		//////////////////////////////////////////////////////////////////////
		vector<InfoBSPPolygon>& GetVisablePlanes();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить буфер видимых моделей
		///
		/// \return Буфер видимых моделей на уровне
		//////////////////////////////////////////////////////////////////////
		vector<InfoBSPModel>& GetVisableModels();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить список энтити-объектов которые не созданы
		///
		/// \return Список энтити-объектов которые не созданы
		//////////////////////////////////////////////////////////////////////
		vector<Entity>&	GetNoInitEntity();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить все энтити на уровне
		///		
		/// \return vector<BaseEntity*>&
		//////////////////////////////////////////////////////////////////////
		vector<BaseEntity*>& GetAllEntitys();

	private:

		//////////////////////////////////////////////////////////////////////
		/// \brief Изменить уровень гаммы на текстуре
		///		
		/// \param[in] ImageBits Массив байт изображения
		/// \param[in] Size Размер изображения
		/// \param[in] Factor Фактор гаммы
		//////////////////////////////////////////////////////////////////////
		void ChangeGamma( byte *ImageBits, int Size, float Factor );

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить в OpenGL карту освещения с памяти
		///		
		/// \param[in] ImageBits Массив байт изображения
		/// \param[in] Width Длина изображения
		/// \param[in] Height Высота изображения
		//////////////////////////////////////////////////////////////////////
		void CreateLightmapTexture( byte *ImageBits, int Width, int Height );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить индекс листа в котором нах. камера
		///		
		/// \return Индекс листа
		//////////////////////////////////////////////////////////////////////
		int FindLeaf( const glm::vec3& Position );

		//////////////////////////////////////////////////////////////////////
		/// \brief Виден ли тестовый кластер из текущего кластера
		///		
		/// \return True - Виден тестовый кластер из текущего, False - нет
		//////////////////////////////////////////////////////////////////////
		bool IsClusterVisible( int CurrentCluster, int TestCluster );

		Scene*								Scene;				///< Сцена на котором находится уровень
		Skybox*								Skybox;				///< Скайбокс уровня
		Bitset								FacesDrawn;			///< Хранит нарисованные фейсы в битах
		BSPVisData							Сlusters;			///< Кластеры

		int									CameraCluster;		///< Кластер в котором находится камера (обновляется при вызове CalculateVisablePlanes() )
		GLuint								VertexBuffer;		///< Вершиный буфер
		GLuint								IndexBuffer;		///< Индексный буфер
		GLuint								ArrayBuffer;		///< VAO

		vector<Plane*> 						ArrayPlanes;		///< Массив плоскостей уровня
		vector<BSPModel>					ArrayModels;		///< Массив моделей уровня (движущиеся браши)
		vector<InfoBSPPolygon>				VisablePlanes;		///< Буфер видимых плоскостей (обновляется при вызове CalculateVisablePlanes() )
		vector<InfoBSPModel>				VisableModels;		///< Буфер видимых моделей (обновляется при вызове CalculateVisablePlanes() )

		vector<BSPNode>						ArrayNodes;			///< Массив веток BSP дерева
		vector<BSPLeaf>						ArrayLeafs;			///< Массив листьев BSP дерева
		vector<BSPPlane>					ArrayBSPPlanes;		///< Массив секущих плоскостей
		vector<BaseEntity*>					ArrayEntitys;		///< Массив энтити-объектов		
		vector<int>							ArrayLeafsFaces;	///< Массив индексов фейсов в листе
		vector<GLuint>						ArrayLightmaps;		///< Массив идентификаторов карт освещения
		vector<Entity>						NoInitEntity;		///< Энтити-объекты которые не созданы

		map<BSPModel*, BaseEntity*>			ArrayModelEntitys;	///< Массив энтити-объектов с моделью		
	};

	//-------------------------------------------------------------------------//

	template<typename T> inline T* Level::GetEntity( const string& NameEntity )
	{
		for ( size_t IdEntity = 0; IdEntity < ArrayEntitys.size(); IdEntity++ )
			return ( T* ) ArrayEntitys[ IdEntity ];

		return NULL;
	}

	//-------------------------------------------------------------------------//

	template<typename T> inline vector<T*> Level::GetEntityes( const string& NameEntity )
	{
		vector<T*> ArrayEntity;

		for ( size_t IdEntity = 0; IdEntity < ArrayEntitys.size(); IdEntity++ )
			ArrayEntity.push_back( ( T* ) ArrayEntitys[ IdEntity ] );

		return ArrayEntity;
	}

	//-------------------------------------------------------------------------//
}

#endif //LEVEL_H

