#include "..\MaterialManager.h"

//-------------------------------------------------------------------------//

map<string, GLuint> le::MaterialManager::mGlTexture = map<string, GLuint>();
map<string, le::ModelMesh> le::MaterialManager::mModelMesh = map<string, le::ModelMesh>();

//-------------------------------------------------------------------------//

bool le::MaterialManager::LoadGLTexture( string key, string route )
{
	if ( mGlTexture.find( key ) == mGlTexture.end() )
	{
		Image image;

		if ( !image.loadFromFile( route ) )
			return false;

		image.flipVertically();

		GLuint texture = 0;
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr() );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		mGlTexture[key] = texture;

		return true;
	}

	return true;
}

//-------------------------------------------------------------------------//

bool le::MaterialManager::LoadModelMesh( string key, string route )
{
	if ( mModelMesh.find( key ) == mModelMesh.end() )
	{
		ModelMesh modelMesh;

		if ( !modelMesh.LoadMesh( route ) )
			return false;

		mModelMesh[key] = modelMesh;

		return true;
	}

	return true;
}

//-------------------------------------------------------------------------//

const GLuint le::MaterialManager::GetGLTexture( string key )
{
	if ( mGlTexture.find( key ) != mGlTexture.end() )
		return mGlTexture[key];
	return 0;
}

//-------------------------------------------------------------------------//

const le::ModelMesh le::MaterialManager::GetModelMesh( string key )
{
	if ( mModelMesh.find( key ) != mModelMesh.end() )
		return mModelMesh[key];
}

//-------------------------------------------------------------------------//

void le::MaterialManager::DeleteAllMaterials()
{
	mGlTexture.clear();
	mModelMesh.clear();
}

//-------------------------------------------------------------------------//

void le::MaterialManager::DeleteModelMesh( string key )
{
	if ( mModelMesh.find( key ) != mModelMesh.end() )
		mModelMesh.erase( mModelMesh.find( key ) );
}

//-------------------------------------------------------------------------//

void le::MaterialManager::DeleteGlTexture( string key )
{
	if ( mGlTexture.find( key ) != mGlTexture.end() )
		mGlTexture.erase( mGlTexture.find( key ) );
}

//-------------------------------------------------------------------------//