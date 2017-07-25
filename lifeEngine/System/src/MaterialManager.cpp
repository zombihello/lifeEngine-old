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

		mGlTexture[ key ] = texture;

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

		mModelMesh[ key ] = modelMesh;

		return true;
	}

	return true;
}

//-------------------------------------------------------------------------//

GLuint le::MaterialManager::CreateGLTexture( string key, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, float width, float height, GLint internalFormat, GLenum format, GLenum type )
{
	if ( mGlTexture.find( key ) == mGlTexture.end() )
	{
		GLuint texture = 0;
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );

		glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL );
		glBindTexture( GL_TEXTURE_2D, 0 );

		if ( key != "" )
			mGlTexture[ key ] = texture;

		return texture;
	}

	cout << "Error: texture with a key [" << key << "] already exists. The function le::MaterialManager::CreateGLTexture() returned 0\n";
	return 0;
}

//-------------------------------------------------------------------------//

const GLuint le::MaterialManager::GetGLTexture( string key )
{
	if ( mGlTexture.find( key ) != mGlTexture.end() )
		return mGlTexture[ key ];

	cout << "Error: texture with a key [" << key << "] not found. The function le::MaterialManager::GetGLTexture() returned 0\n";
	return 0;
}

//-------------------------------------------------------------------------//

const le::ModelMesh* le::MaterialManager::GetModelMesh( string key )
{
	if ( mModelMesh.find( key ) != mModelMesh.end() )
		return &mModelMesh[ key ];

	cout << "Error: model mesh with a key [" << key << "] not found. The function le::MaterialManager::GetModelMesh() returned NULL\n";
	return NULL;
}

//-------------------------------------------------------------------------//

void le::MaterialManager::DeleteAllMaterials()
{
	for ( auto it = mGlTexture.begin(); it != mGlTexture.end(); it++ )
		glDeleteTextures( 1, &mGlTexture[ it->first ] );

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
	{
		glDeleteTextures( 1, &mGlTexture[ key ] );
		mGlTexture.erase( mGlTexture.find( key ) );
	}
}

//-------------------------------------------------------------------------//