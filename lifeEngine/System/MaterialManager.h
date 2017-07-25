#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "System.h"
#include "3D/ModelMesh.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API MaterialManager
	{
	public:
		///////////////////////////////
		/// ЗАГРУЗИТЬ OPENGL ТЕКСТУРУ
		//////////////////////////////
		static bool LoadGLTexture( string key, string route );

		//////////////////////////
		/// ЗАГРУЗИТЬ МЕШ МОДЕЛИ
		//////////////////////////
		static bool LoadModelMesh( string key, string route );

		//////////////////////////////////////
		/// СОЗДАТЬ ПУСТУЮ OPENGL ТЕКСТУРУ
		//////////////////////////////////////
		static GLuint CreateGLTexture( string key, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT,
									   float width, float height, GLint internalFormat, GLenum format, GLenum type );

		//////////////////////////////
		/// ПОЛУЧИТЬ OPENGL ТЕКСТУРУ
		//////////////////////////////
		static const GLuint GetGLTexture( string key );

		//////////////////////////
		/// ПОЛУЧИТЬ МЕШ МОДЕЛИ
		//////////////////////////
		static const ModelMesh* GetModelMesh( string key );

		//////////////////////////
		/// УДАЛИТЬ ВСЕ МАТЕРИАЛЫ
		//////////////////////////
		static void DeleteAllMaterials();

		////////////////////////
		/// УДАЛИТЬ МЕШ МОДЕЛИ
		////////////////////////
		static void DeleteModelMesh( string key );

		/////////////////////////////
		/// УДАЛИТЬ OPENGL ТЕКСТУРУ
		/////////////////////////////
		static void DeleteGlTexture( string key );

	private:
		static map<string, GLuint>				mGlTexture;
		static map<string, ModelMesh>			mModelMesh;
	};

	//-------------------------------------------------------------------------//
}

#endif // MATERIAL_MANAGER_H