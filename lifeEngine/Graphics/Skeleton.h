﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef SKELETON_H
#define SKELETON_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////
// XML
//////////
#include <tinyxml.h>

//////////////////
// LIFEENGINE
//////////////////
#include <Graphics\MeshVertex.h>
#include <System\System.h>

namespace le
{
	//////////////////////////////////////////////////////////////////////
	/// \brief Класс для работы со скелетом модели
	//////////////////////////////////////////////////////////////////////
	class DLL_API Skeleton
	{
	public:

		//-------------------------------------------------------------------------//

		//////////////////////////////////////////////////////////////////////
		/// \brief Структура для хранения информации о кости
		//////////////////////////////////////////////////////////////////////
		struct DLL_API Bone
		{
			int					IdParent; ///< ID родительской кости
			string				Name; ///< Имя кости
			string				ParentName; ///< Имя родительской кости
			glm::mat4			TransformMatrix; ///< Матрица трансформации
			glm::mat4			InvertMatrix; ///< Инверт. матрица
			glm::mat4			Realese; ///< Итоговая матрица
			glm::mat4			InterpolatedMatrix; ///< Интерполированая матрица
			vector<int>			IdChild; ///< ID дочерних костей
			map<int, float>		Weights; ///< Вес кости на вершину
		};

		//-------------------------------------------------------------------------//

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить скелет модели
		///
		/// \param[in] Skeleton Указатель на xml тег Skeleton 
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		bool LoadSkeleton( TiXmlElement* Skeleton );

		//////////////////////////////////////////////////////////////////////
		/// \brief Обновить матрицу кости
		///
		/// \param[in] NameBone Название кости
		/// \param[in] Matrix Матрица кости
		//////////////////////////////////////////////////////////////////////
		void UpdateMatrixBone( const string& NameBone, const glm::mat4& Matrix );

		//////////////////////////////////////////////////////////////////////
		/// \brief Обновить скелет модели
		//////////////////////////////////////////////////////////////////////
		void UpdateSkeleton();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить все кости
		///
		/// \return const vector<Bone>*
		//////////////////////////////////////////////////////////////////////
		vector<Bone>* GetAllBones();

	private:
		//////////////////////////////////////////////////////////////////////
		/// \brief Считать кость
		/// \details Это рекурсивный метод который нужен для считывания
		/// иерархии костей у модели
		///
		/// \param[in] Node Указатель на xml тег Node
		/// \param[out] Bone Указатель на кость
		//////////////////////////////////////////////////////////////////////
		void ReadingBone( TiXmlElement* Node, Bone* Bone );

		//////////////////////////////////////////////////////////////////////
		/// \brief Инициализировать матрицу кости
		/// \details Это рекурсивный метод который нужен для инициализации
		/// матрицы кости
		///
		/// \param[out] Bone Указатель на кость
		//////////////////////////////////////////////////////////////////////
		void InitMatrixBone( Bone& Bone );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить ID кости по имени
		///
		/// \param[in] NameBone имя кости
		/// \return int. Если такой кости нету, то вернет -1
		//////////////////////////////////////////////////////////////////////
		int GetIdBone( const string& NameBone );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить ID кости по самой кости
		///
		/// \param[in] Bone кость
		/// \return int. Если такой кости нету, то вернет -1
		//////////////////////////////////////////////////////////////////////
		int GetIdBone( const Bone& Bone );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить кость по имени
		///
		/// \param[in] NameBone имя кости
		/// \return Bone*. Если такой кости нету, то вернет NULL
		//////////////////////////////////////////////////////////////////////
		Bone* GetBone( const string& NameBone );

		glm::mat4					BindShape; ///< Матрица биндига модели	
		vector<Bone>				Bones; ///< Массив костей у модели
		vector<int>					IdRootBones; ///< ID главных костей
	};

	//-------------------------------------------------------------------------//
}

#endif //SKELETON_H

