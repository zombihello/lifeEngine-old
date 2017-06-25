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
		/// гюцпсгхрэ OPENGL рейярспс
		//////////////////////////////
		static bool LoadGLTexture( string key, string route );

		//////////////////////////
		/// гюцпсгхрэ леь лндекх
		//////////////////////////
		static bool LoadModelMesh( string key, string route );

		//////////////////////////////
		/// онксвхрэ OPENGL рейярспс
		//////////////////////////////
		static const GLuint GetGLTexture( string key );

		//////////////////////////
		/// онксвхрэ леь лндекх
		//////////////////////////
		static const ModelMesh GetModelMesh( string key );

		//////////////////////////
		/// сдюкхрэ бяе люрепхюкш
		//////////////////////////
		static void DeleteAllMaterials( );

		////////////////////////
		/// сдюкхрэ леь лндекх
		////////////////////////
		static void DeleteModelMesh( string key );

		/////////////////////////////
		/// сдюкхрэ OPENGL рейярспс
		/////////////////////////////
		static void DeleteGlTexture( string key );

	private:
		static map<string, GLuint>				mGlTexture;
		static map<string, ModelMesh>			mModelMesh;
	};

	//-------------------------------------------------------------------------//
}

#endif // MATERIAL_MANAGER_H