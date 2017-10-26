#include "..\ResourcesManager.h"

//-------------------------------------------------------------------------//

GLuint le::ResourcesManager::ErrorTexture = 0;

map<string, GLuint> le::ResourcesManager::GlTextures = map<string, GLuint>();
map<string, Shader>	le::ResourcesManager::Shaders = map<string, Shader>();
map<string, le::Mesh> le::ResourcesManager::Meshes = map<string, le::Mesh>();

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

bool le::ResourcesManager::LoadMesh( const string & NameMesh, const string & RouteToFile )
{
	if ( Meshes.find( NameMesh ) == Meshes.end() )
	{
		Mesh Mesh;

		if ( !Mesh.LoadMesh( RouteToFile ) )
			return false;

		Meshes[ NameMesh ] = Mesh;
		return true;
	}

	Logger::Log( Logger::Info, "Mesh With Name [" + NameMesh + "] Already Loaded" );
	return true;
}

//-------------------------------------------------------------------------//

bool le::ResourcesManager::LoadShader( const string& NameShader, const string& VertexShader, const string& GeometryShader, const string& FragmentShader )
{
	if ( Shaders.find( NameShader ) == Shaders.end() )
	{
		Logger::Log( Logger::Info, "Loading Shader With Name [" + NameShader + "]" );

		if ( !Shaders[ NameShader ].loadFromFile( VertexShader, GeometryShader, FragmentShader ) )
		{
			Logger::Log( Logger::Error, "*** Shader Error ***" );
			Logger::Log( Logger::Error, "Shader Name: " + NameShader );
			Logger::Log( Logger::Error, Shaders[ NameShader ].getErrorMessage().str() );
			Logger::Log( Logger::Error, "*** Shader Error End ***" );

			Shaders.erase( NameShader );
			return false;
		}

		Logger::Log( Logger::Info, "Shader With Name [" + NameShader + "] Loaded" );
		return true;
	}

	Logger::Log( Logger::Info, "Shader With Name [" + NameShader + "] Already Loaded" );
	return true;
}

//-------------------------------------------------------------------------//

bool le::ResourcesManager::LoadShader( const string& NameShader, const string& VertexShader, const string& FragmentShader )
{
	if ( Shaders.find( NameShader ) == Shaders.end() )
	{
		Logger::Log( Logger::Info, "Loading Shader With Name [" + NameShader + "]" );

		if ( !Shaders[ NameShader ].loadFromFile( VertexShader, FragmentShader ) )
		{
			Logger::Log( Logger::Error, "*** Shader Error ***" );
			Logger::Log( Logger::Error, "Shader Name: " + NameShader );
			Logger::Log( Logger::Error, Shaders[ NameShader ].getErrorMessage().str() );
			Logger::Log( Logger::Error, "*** Shader Error End ***" );

			Shaders.erase( NameShader );
			return false;
		}

		Logger::Log( Logger::Info, "Shader With Name [" + NameShader + "] Loaded" );
		return true;
	}

	Logger::Log( Logger::Info, "Shader With Name [" + NameShader + "] Already Loaded" );
	return true;
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteGlTexture( const string& NameTexture )
{
	if ( GlTextures.find( NameTexture ) != GlTextures.end() )
	{
		glDeleteTextures( 1, &GlTextures[NameTexture] );
		Logger::Log( Logger::Info, "Texture With Name [" + NameTexture + "] Deleted" );
		GlTextures.erase( NameTexture );	
	}
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteMesh( const string & NameMesh )
{
	if ( Meshes.find( NameMesh ) != Meshes.end() )
	{
		Logger::Log( Logger::Info, "Mesh With Name [" + NameMesh + "] Deleted" );
		Meshes.erase( NameMesh );		
	}
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteShader( const string& NameShader )
{
	if ( Shaders.find( NameShader ) != Shaders.end() )
	{
		Logger::Log( Logger::Info, "Shader With Name [" + NameShader + "] Deleted" );
		Shaders.erase( NameShader );		
	}
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteAllGlTexture()
{
	for ( auto it = GlTextures.begin(); it != GlTextures.end(); it++ )
	{
		glDeleteTextures( 1, &it->second );
		Logger::Log( Logger::Info, "Texture With Name [" + it->first + "] Deleted" );
	}

	GlTextures.clear();
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteAllMeshes()
{
	for ( auto it = Meshes.begin(); it != Meshes.end(); it++ )
		Logger::Log( Logger::Info, "Mesh With Name [" + it->first + "] Deleted" );

	Meshes.clear();
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteAllShaders()
{
	for ( auto it = Shaders.begin(); it != Shaders.end(); it++ )
		Logger::Log( Logger::Info, "Shader With Name [" + it->first + "] Deleted" );

	Shaders.clear();
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::DeleteAllResources()
{
	DeleteAllGlTexture();
	DeleteAllMeshes();
	DeleteAllShaders();
}

//-------------------------------------------------------------------------//

void le::ResourcesManager::SetErrorTexture( const string& ErrorTexture )
{
	if ( le::ResourcesManager::ErrorTexture != 0 )
		glDeleteTextures( 1, &le::ResourcesManager::ErrorTexture );

	Image Image;

	if ( !Image.loadFromFile( ErrorTexture ) )
		return;

	Image.flipVertically();

	glGenTextures( 1, &le::ResourcesManager::ErrorTexture );
	glBindTexture( GL_TEXTURE_2D, le::ResourcesManager::ErrorTexture );
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, Image.getSize().x, Image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsPtr() );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

//-------------------------------------------------------------------------//

const GLuint le::ResourcesManager::GetGlTexture( const string& NameTexture )
{
	if ( GlTextures.find( NameTexture ) != GlTextures.end() )
		return GlTextures[ NameTexture ];

	return ErrorTexture;
}

//-------------------------------------------------------------------------//

le::Mesh* le::ResourcesManager::GetMesh( const string& NameMesh )
{
	if ( Meshes.find( NameMesh ) != Meshes.end() )
		return &Meshes[ NameMesh ];

	return NULL;
}

//-------------------------------------------------------------------------//

Shader* le::ResourcesManager::GetShader( const string& NameShader )
{
	if ( Shaders.find( NameShader ) != Shaders.end() )
		return &Shaders[ NameShader ];

	return NULL;
}

//-------------------------------------------------------------------------//
