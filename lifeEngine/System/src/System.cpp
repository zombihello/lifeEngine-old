#include "..\System.h"

//-------------------------------------------------------------------------//

Ogre::LogManager le::System::LogManager;

//-------------------------------------------------------------------------//

le::System::System( const string& ConfigFile, const string& LogFile, string PluginDir )
	: Window( NULL )
{
	Root = new Ogre::Root( "", ConfigFile, "" );

	// Создаем файл логов
	if ( LogFile != "" )
		LogManager.createLog( LogFile );

	LogManager.logMessage( ENGINE " " ENGINE_VERSION );

	// Загрузка плагинов для Ogre3D
	if ( PluginDir != "" && PluginDir[ PluginDir.size() - 1 ] != '/' )
		PluginDir += "/";

#ifdef _DEBUG
	Root->loadPlugin( PluginDir + "RenderSystem_GL_d" );
#else
	Root->loadPlugin( PluginDir + "RenderSystem_GL" );
#endif

	// Загрузка конфигураций
	if ( !Root->restoreConfig() )
	{
		Ogre::RenderSystem* RenderSystem = Root->getRenderSystemByName( "OpenGL Rendering Subsystem" );

		if ( RenderSystem->getName() != "OpenGL Rendering Subsystem" )
		{
			LogManager.logMessage( Ogre::LogMessageLevel::LML_CRITICAL, "Render System [OpenGL Rendering Subsystem] Not Found" );
			exit( -1 );
		}

		RenderSystem->setConfigOption( "Full Screen", "No" );
		RenderSystem->setConfigOption( "VSync", "No" );
		RenderSystem->setConfigOption( "Video Mode", "800 x 600 @ 32-bit" );

		Root->setRenderSystem( RenderSystem );
		Root->saveConfig();
	}

	Scene = Root->createSceneManager( Ogre::ST_GENERIC );
}

//-------------------------------------------------------------------------//

le::System::~System()
{
	delete Root;
}

//-------------------------------------------------------------------------//

void le::System::WindowCreate( const string& NameWindow )
{
	Window = Root->initialise( true, NameWindow );
}

//-------------------------------------------------------------------------//

void le::System::MainLoop( BasicApplication& Application )
{
	if ( Window == NULL )
	{
		LogManager.logMessage( "Window Not Created" );
		return;
	}

	while ( !Window->isClosed() )
	{
		Ogre::WindowEventUtilities::messagePump();
		Application.Update();
		Root->renderOneFrame();
	}
}

//-------------------------------------------------------------------------//

Ogre::Root* le::System::GetOgreRoot()
{
	return Root;
}

//-------------------------------------------------------------------------//

Ogre::RenderWindow* le::System::GetWindow()
{
	return Window;
}

//-------------------------------------------------------------------------//

Ogre::SceneManager* le::System::GetScene()
{
	return Scene;
}

//-------------------------------------------------------------------------//
