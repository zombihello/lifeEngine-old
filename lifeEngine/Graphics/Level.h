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

//////////
// XML
//////////
#include <tinyxml.h>

//////////////////
// LIFEENGINE
//////////////////
#include <Graphics\Skybox.h>
#include <Graphics\Brush.h>
#include <Graphics\Entity.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class LightManager;

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
		/// \brief Загрузить карту
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
		/// \brief Сгенерировать карту освещения
		///
		/// \param[in] LightManager Манеджер освещения
		//////////////////////////////////////////////////////////////////////
		void GenerateLightMap( LightManager& LightManager );

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
		/// \brief Получить скайбокс уровня
		///		
		/// \return Skybox&.
		//////////////////////////////////////////////////////////////////////
		Skybox& GetSkybox();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить энтити по названию
		///		
		/// \param[in] NameEntity Название энтити
		/// \return Entity*. Если не нашли энтити,то вернет NULL
		//////////////////////////////////////////////////////////////////////
		Entity* GetEntity( const string& NameEntity );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить все браши на уровне
		///		
		/// \return vector<Brush*>&
		//////////////////////////////////////////////////////////////////////
		vector<Brush*>& GetAllBrushes();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить все энтити на уровне
		///		
		/// \return vector<Entity*>&
		//////////////////////////////////////////////////////////////////////
		vector<Entity>& GetAllEntitys();

	private:

		Scene*							Scene; ///< Сцена на котором находится уровень
		Skybox*							Skybox; ///< Скайбокс уровня
		string							NameMap; ///< Название карты
		string							DescriptionMap; ///< Описание карты
		string							SkyBoxName; ///< Название скайбокса
		vector<Brush*>					Brushes; ///< Массив брашей
		vector<Entity>					Entitys; ///< Массив энтити-объектов
	};

	//-------------------------------------------------------------------------//
}

#endif //LEVEL_H

