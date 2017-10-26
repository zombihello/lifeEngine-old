﻿//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef BOUNDING_CONE_H
#define BOUNDING_CONE_H
#pragma warning( push )

#pragma warning( disable: 4251 ) 

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////
// OPENGL
//////////////
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>

////////////////
// LIFEENGINE
///////////////
#include <System\Query.h>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс ограничивающего тела (конус)
	///
	/// Данный класс используеться для оптимального (быстрого) отсичения
	/// невидимой геометрии и описания объемов света
	//////////////////////////////////////////////////////////////////////
	class DLL_API BoundingCone
	{
	public:
		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		//////////////////////////////////////////////////////////////////////
		BoundingCone();

		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор копирывания
		//////////////////////////////////////////////////////////////////////
		BoundingCone( BoundingCone& Copy );

		//////////////////////////////////////////////////////////////////////
		/// \brief Деструктор
		//////////////////////////////////////////////////////////////////////
		~BoundingCone();

		//////////////////////////////////////////////////////////////////////
		/// \brief Инициализировать ограничивающее тело
		///		
		/// \param[in] Height Высота конуса
		/// \param[in] Radius Радиус конуса
		//////////////////////////////////////////////////////////////////////
		void InitCone( const float& Height, const float& Radius );

		//////////////////////////////////////////////////////////////////////
		/// \brief Провести тест на перекртие объекта
		//////////////////////////////////////////////////////////////////////
		void QueryTest();

		//////////////////////////////////////////////////////////////////////
		/// \brief Отрендерить объект
		//////////////////////////////////////////////////////////////////////
		void RenderCone();

		//////////////////////////////////////////////////////////////////////
		/// \brief Задать позицию конуса
		///		
		/// \param[in] Position Позиция сферы
		//////////////////////////////////////////////////////////////////////
		void SetPosition( const glm::vec3& Position );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить матрицу трансформации
		///		
		/// \return  glm::mat4&
		//////////////////////////////////////////////////////////////////////
		glm::mat4& GetTransformation();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить позицию
		///		
		/// \return  glm::vec3&
		//////////////////////////////////////////////////////////////////////
		glm::vec3& GetPosition();

		BoundingCone& operator=( const BoundingCone& Copy );

		Query				Query; ///< Объект для проверки перекрытия

	private:
		//////////////////////////////////////////////////////////////////////
		/// \brief Создать сферу
		///		
		/// \param[in] Height Высота конуса
		/// \param[in] Radius Радиус конуса
		/// \return  vector<glm::vec3>&
		//////////////////////////////////////////////////////////////////////
		vector<glm::vec3>	CreateCone( const float& Height, const float& Radius );

		float				Height; ///< Высота конуса
		float				Radius; ///< Радиус конуса

		GLuint				ArrayBuffer; ///< VAO буффер
		GLuint				VertexBuffer; ///< Вершиный буффер
		GLuint				IndexBuffer; ///< Индексный буффер

		glm::vec3			Position; ///< Позиция сферы
		glm::mat4			Transformation; ///< Матрица трансформации
	};

	//-------------------------------------------------------------------------//
}

#pragma warning( pop )
#endif // BOUNDING_CONE_H

