//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef VAO_H
#define VAO_H

#define COMPILING_LIBRARY
#define VERT_POSITION 0
#define VERT_NORMAL 1
#define VERT_TEXCOORD 2
#include <DllGlobal.h>

///////////////////////////
// СИСТЕМНЫЕ БИБЛИОТЕКИ
///////////////////////////
#include <vector>
using namespace std;

//////////////
// OPENGL
//////////////
#include <glew\glew.h>

//////////////////
// SFML
//////////////////
#include <SFML\OpenGL.hpp>

////////////////
// LIFEENGINE
///////////////
#include <System\Logger.h>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс для работы с VAO
	///
	/// Данный класс предназначен для работы с VAO
	/// (его создание, удалением и т.д)
	//////////////////////////////////////////////////////////////////////
	class VAO
	{
	public:

		//-------------------------------------------------------------------------//

		enum TypeBuffer
		{
			Vertex_Buffer = GL_ARRAY_BUFFER,
			Index_Buffer = GL_ELEMENT_ARRAY_BUFFER
		};

		//-------------------------------------------------------------------------//

		enum TypeUpdate
		{
			Static_Draw = GL_STATIC_DRAW,
			Dynamic_Draw = GL_DYNAMIC_DRAW,
			Stream_Draw = GL_STREAM_DRAW
		};

		//////////////////////////////////////////////////////////////////////
		/// \brief Создать VAO
		///		
		/// \return GLuint. Идентификатор VAO
		//////////////////////////////////////////////////////////////////////
		static GLuint CreateVAO();

		//////////////////////////////////////////////////////////////////////
		/// \brief Создать буффер для VAO
		///		
		/// \param[in] TypeBuffer Тип буффера
		/// \param[in] Data Массив данных
		///	\param[in] Usage Тип обновления
		/// \return GLuint. Идентификатор буффера
		//////////////////////////////////////////////////////////////////////
		template<typename T> static GLuint CreateBuffer( TypeBuffer TypeBuffer, vector<T> Data, TypeUpdate Usage );

		//////////////////////////////////////////////////////////////////////
		/// \brief Присоединить буффер к VAO
		///		
		/// \param[in] TypeBuffer Тип буффера
		///	\param[in] Buffer Идентификатор буффера
		//////////////////////////////////////////////////////////////////////
		static void AtachBuffer( TypeBuffer TypeBuffer, GLuint Buffer );

		//////////////////////////////////////////////////////////////////////
		/// \brief Задать индекс атрибута вершины в шейдере
		///		
		/// \param[in] Index Номер аргумента в шейдере
		///	\param[in] Size Размер аргумента в шейедре
		///	\param[in] Type Исп. тип данных
		///	\param[in] Normalized Нужно ли нормализовывать данные
		///	\param[in] Stride Расстояние между наборами данных
		///	\param[in] Pointer Смещение начала данных в буфере
		//////////////////////////////////////////////////////////////////////
		static void SetVertexAttribPointer( GLuint Index, GLint Size, GLenum Type, GLboolean Normalized, GLsizei Stride, const void* Pointer );

		//////////////////////////////////////////////////////////////////////
		/// \brief Разрешить использовать вершиный атрибут
		/// под заданым индексом
		///		
		/// \param[in] Index Номер аргумента в шейдере
		//////////////////////////////////////////////////////////////////////
		static void EnableVertexPointer( GLuint Index );

		//////////////////////////////////////////////////////////////////////
		/// \brief Запретить использовать вершиный атрибут
		/// под заданым индексом
		///		
		/// \param[in] Index Номер аргумента в шейдере
		//////////////////////////////////////////////////////////////////////
		static void DisableVertexPointer( GLuint Index );

		//////////////////////////////////////////////////////////////////////
		/// \brief Сделать активным буффер
		///		
		/// \param[in] TypeBuffer Тип буффера
		/// \param[in] Buffer Идентификатор буффера
		//////////////////////////////////////////////////////////////////////
		static void BindBuffer( TypeBuffer TypeBuffer, GLuint Buffer );

		//////////////////////////////////////////////////////////////////////
		/// \brief Сделать активным VAO
		///		
		/// \param[in] VAO Идентификатор VAO
		//////////////////////////////////////////////////////////////////////
		static void BindVAO( GLuint VAO );

		//////////////////////////////////////////////////////////////////////
		/// \brief Деактивировать текущий VAO
		//////////////////////////////////////////////////////////////////////
		static void UnbindVAO();

		//////////////////////////////////////////////////////////////////////
		/// \brief Деактивировать текущий буффер
		///		
		/// \param[in] TypeBuffer Тип буффера
		//////////////////////////////////////////////////////////////////////
		static void UnbindBuffer( TypeBuffer TypeBuffer );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить буффер
		///		
		/// \param[in] Buffer Указатель на идентификатор буффера
		//////////////////////////////////////////////////////////////////////
		static void DeleteBuffer( GLuint* Buffer );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить VAO
		///		
		/// \param[in] VAO Указатель на идентификатор VAO
		//////////////////////////////////////////////////////////////////////
		static void DeleteVAO( GLuint* VAO );
	};

	//-------------------------------------------------------------------------//

	template<typename T> inline GLuint VAO::CreateBuffer( TypeBuffer TypeBuffer, vector<T> Data, TypeUpdate Usage )
	{
		GLuint Buffer;

		glGenBuffers( 1, &Buffer );
		glBindBuffer( TypeBuffer, Buffer );
		glBufferData( TypeBuffer, Data.size() * sizeof( T ), Data.data(), Usage );

		return Buffer;
	}

	//-------------------------------------------------------------------------//
}

#endif // VAO_H
