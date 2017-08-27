#include "..\ResourcesManager.h"

//-------------------------------------------------------------------------//

map<string, GLuint> le::ResourcesManager::GlTextures = map<string, GLuint>();

//-------------------------------------------------------------------------//

bool le::ResourcesManager::LoadGlTexture( const string & NameTexture, const string & RouteToFile )
{
	Logger::Log( Logger::Info, "Loading Texture [" + RouteToFile + "] With Name [" + NameTexture + "]" );

	if ( GlTextures.find( NameTexture ) == GlTextures.end() )
	{
		Image Image;

		if ( !Image.loadFromFile( RouteToFile ) )
		{
			Logger::Log( Logger::Error, "Texture [" + RouteToFile + "] Not Found" );
			return false;
		}

		Image.flipVertically();

		GLuint Texture = 0;
		glGenTextures( 1, &Texture );
		glBindTexture( GL_TEXTURE_2D, Texture );
		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, Image.getSize().x, Image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsPtr() );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		GlTextures[ NameTexture ] = Texture;

		Logger::Log( Logger::Info, "Texture With Name [" + NameTexture + "] Loaded" );
		return true;
	}

	Logger::Log( Logger::Info, "Texture With Name [" + NameTexture + "] Already Loaded" );
	return true;
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteGlTexture( const string& NameTexture )
{
	if ( GlTextures.find( NameTexture ) != GlTextures.end() )
	{
		glDeleteTextures( 1, &GlTextures[NameTexture] );
		GlTextures.erase( NameTexture );
		Logger::Log( Logger::Info, "Texture With Name [" + NameTexture + "] Deleted" );
	}
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteAllGlTexture()
{
	for ( auto it = GlTextures.begin(); it != GlTextures.end(); it++ )
	{
		glDeleteTextures( 1, &it->second );
		GlTextures.erase( it->first );
		Logger::Log( Logger::Info, "Texture With Name [" + it->first + "] Deleted" );
	}
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteAllResources()
{
	DeleteAllGlTexture();
}

//-------------------------------------------------------------------------//

const GLuint le::ResourcesManager::GetGlTexture( const string& NameTexture )
{
	if ( GlTextures.find( NameTexture ) != GlTextures.end() )
		return GlTextures[ NameTexture ];

	return 0;
}

//-------------------------------------------------------------------------//
