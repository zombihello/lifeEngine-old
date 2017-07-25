#ifndef LOADER_VAO
#define LOADER_VAO

#define COMPILING_LIBRARY
#define VERT_POSITION 0
#define VERT_NORMAL 1
#define VERT_TEXCOORD 2
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API LoaderVAO
	{
	public:

		/////////////////
		/// СОЗДАТЬ VAO
		////////////////
		static GLuint CreateVAO( );

		////////////////////////////////
		/// СОЗДАТЬ БУФЕР
		////////////////////////////////
		template<typename T> static GLuint CreateBuffer( GLenum TypeBuffer, vector<T> Data, GLenum Usage );

		////////////////////////////////
		/// ПРИСОЕДИНИТЬ БУФЕР К VAO
		////////////////////////////////
		template<typename T> static GLuint AtachBuffer( GLenum TypeBuffer, vector<T> Data, GLenum Usage );
		static void AtachBuffer( GLenum TypeBuffer, GLuint Buffer );

		//////////////////////////////////////////////
		/// ЗАДАТЬ ИНДЕКС АТРИБУТА ВЕРШИНЫ В ШЕЙДЕРЕ
		//////////////////////////////////////////////
		static void SetVertexAttribPointer( GLuint Index, GLint Size, GLenum Type, GLboolean Normalized, GLsizei Stride, const void* Pointer );

		//////////////////////////////////////////////
		/// РАЗРЕШИТЬ ИСПОЛЬЗОВАТЬ ВЕРШИНЫЙ АТРИБУТ
		/// ПОД ЗАДАНЫМ ИНДЕКСОМ
		//////////////////////////////////////////////
		static void EnableVertexPointer( GLuint Index );

		//////////////////////////////////////////////
		/// ЗАПРЕТИТЬ ИСПОЛЬЗОВАТЬ ВЕРШИНЫЙ АТРИБУТ
		/// ПОД ЗАДАНЫМ ИНДЕКСОМ
		//////////////////////////////////////////////
		static void DisableVertexPointer( GLuint Index );

		///////////////////////////
		/// СДЕЛАТЬ АКТИВНЫМ VAO
		///////////////////////////
		static void BindVAO( GLuint VAO );
		
		//////////////////////////////////
		/// ДЕАКТИВИРОВАТЬ ТЕКУЩИЙ VAO
		//////////////////////////////////
		static void UnbindVAO( );

		//////////////////////////////////
		/// ДЕАКТИВИРОВАТЬ ТЕКУЩИЙ БУФЕР
		//////////////////////////////////
		static void UnbindBuffer( GLenum TypeBuffer );

		///////////////////
		/// УДАЛИТЬ БУФЕР
		//////////////////
		static void DeleteBuffer( GLuint* Buffer );

		//////////////////
		/// УДАЛИТЬ VAO
		/////////////////
		static void DeleteVAO( GLuint* VAO );
	};

	//-------------------------------------------------------------------------//

	template<typename T> inline GLuint LoaderVAO::AtachBuffer( GLenum TypeBuffer, vector<T> Data, GLenum Usage )
	{
		GLuint Buffer;

		glGenBuffers( 1, &Buffer );
		glBindBuffer( TypeBuffer, Buffer );
		glBufferData( TypeBuffer, Data.size( ) * sizeof( T ), Data.data( ), Usage );

		return Buffer;
	}

	//-------------------------------------------------------------------------//

	template<typename T> inline GLuint LoaderVAO::CreateBuffer( GLenum TypeBuffer, vector<T> Data, GLenum Usage )
	{
		GLuint Buffer;

		glGenBuffers( 1, &Buffer );
		glBindBuffer( TypeBuffer, Buffer );
		glBufferData( TypeBuffer, Data.size( ) * sizeof( T ), Data.data( ), Usage );

		return Buffer;
	}

	//-------------------------------------------------------------------------//
}

#endif // LOADER_VAO