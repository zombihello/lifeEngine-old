#include "../System.h"
#include <HaffmanCode.h>
using namespace le;

//-------------------------------------------------------------------------//

System::System( const string FileConfiguration, int argc, char** argv )
{
	sRouteFileConfiguration = FileConfiguration;

	if ( FileExists( FileConfiguration ) )
	{
		Configuration.iWindowWidth = ReadTextFile<int>( FileConfiguration, "iWindowWidth" );
		Configuration.iWindowHeight = ReadTextFile<int>( FileConfiguration, "iWindowHeight" );
		Configuration.iVolumeMusic = ReadTextFile<int>( FileConfiguration, "iVolumeMusic" );
		Configuration.iVolumeSound = ReadTextFile<int>( FileConfiguration, "iVolumeSound" );
		Configuration.iFrameLimit = ReadTextFile<int>( FileConfiguration, "iFrameLimit" );
		Configuration.iAntialiasingLevel = ReadTextFile<int>( FileConfiguration, "iAntialiasingLevel" );
		Configuration.fRatioView = ReadTextFile<float>( FileConfiguration, "fRatioView" );

		Configuration.bFullscreen = ReadTextFile<int>( FileConfiguration, "bFullscreen" );
		Configuration.bMusic = ReadTextFile<int>( FileConfiguration, "bMusic" );
		Configuration.bSound = ReadTextFile<int>( FileConfiguration, "bSound" );
		Configuration.bV_Sinc = ReadTextFile<int>( FileConfiguration, "bV_Sinc" );

		Configuration.sLanguage = ReadTextFile<string>( FileConfiguration, "sLanguage" );
		Configuration.fGameSpeed = ReadTextFile<float>( FileConfiguration, "fGameSpeed" );
		Configuration.fGameTick = ReadTextFile<float>( FileConfiguration, "fGameTick" );
	}
	else
	{
		cout << "Error: File [" << FileConfiguration << "] Not Found\n";

		UpdateFileConfiguration();
	}

	if ( Configuration.fRatioView < 0 )
		GameCamera.reset( FloatRect( 0, 0, Configuration.iWindowWidth * Configuration.fRatioView, Configuration.iWindowHeight * Configuration.fRatioView ) );
	else
		GameCamera.reset( FloatRect( 0, 0, Configuration.iWindowWidth / Configuration.fRatioView, Configuration.iWindowHeight / Configuration.fRatioView ) );

	MenuCamera.reset( FloatRect( 0, 0, Configuration.iWindowWidth, Configuration.iWindowHeight ) );

	if ( argc > 1 )
	for ( int id = 1; id < argc; id++ )
	{
		if ( strstr( argv[id], "-dev" ) )
			SetDebug( true );

		if ( strstr( argv[id], "-window" ) )
			Configuration.bFullscreen = false;

		if ( strstr( argv[id], "-fullscreen" ) )
			Configuration.bFullscreen = true;

		if ( strstr( argv[id], "-width" ) )
			Configuration.iWindowWidth = atoi( argv[id + 1] );

		if ( strstr( argv[id], "-height" ) )
			Configuration.iWindowHeight = atoi( argv[id + 1] );
	}

	if ( Configuration.bFullscreen )
		WindowCreate( Style::Fullscreen );
	else
		WindowCreate();
}

//-------------------------------------------------------------------------//

le::System::System( const string FileConfiguration, const string ShadersDir, int argc, char ** argv )
{
	sRouteFileConfiguration = FileConfiguration;
	Configuration.sShadersDir = ShadersDir;

	if ( FileExists( FileConfiguration ) )
	{
		Configuration.iWindowWidth = ReadTextFile<int>( FileConfiguration, "iWindowWidth" );
		Configuration.iWindowHeight = ReadTextFile<int>( FileConfiguration, "iWindowHeight" );
		Configuration.iVolumeMusic = ReadTextFile<int>( FileConfiguration, "iVolumeMusic" );
		Configuration.iVolumeSound = ReadTextFile<int>( FileConfiguration, "iVolumeSound" );
		Configuration.iFrameLimit = ReadTextFile<int>( FileConfiguration, "iFrameLimit" );
		Configuration.iAntialiasingLevel = ReadTextFile<int>( FileConfiguration, "iAntialiasingLevel" );
		Configuration.fRatioView = ReadTextFile<float>( FileConfiguration, "fRatioView" );

		Configuration.bFullscreen = ReadTextFile<int>( FileConfiguration, "bFullscreen" );
		Configuration.bMusic = ReadTextFile<int>( FileConfiguration, "bMusic" );
		Configuration.bSound = ReadTextFile<int>( FileConfiguration, "bSound" );
		Configuration.bV_Sinc = ReadTextFile<int>( FileConfiguration, "bV_Sinc" );

		Configuration.sLanguage = ReadTextFile<string>( FileConfiguration, "sLanguage" );
		Configuration.fGameSpeed = ReadTextFile<float>( FileConfiguration, "fGameSpeed" );
		Configuration.fGameTick = ReadTextFile<float>( FileConfiguration, "fGameTick" );
	}
	else
	{
		cout << "Error: File [" << FileConfiguration << "] Not Found\n";

		UpdateFileConfiguration();
	}

	if ( Configuration.fRatioView < 0 )
		GameCamera.reset( FloatRect( 0, 0, Configuration.iWindowWidth * Configuration.fRatioView, Configuration.iWindowHeight * Configuration.fRatioView ) );
	else
		GameCamera.reset( FloatRect( 0, 0, Configuration.iWindowWidth / Configuration.fRatioView, Configuration.iWindowHeight / Configuration.fRatioView ) );

	MenuCamera.reset( FloatRect( 0, 0, Configuration.iWindowWidth, Configuration.iWindowHeight ) );

	if ( argc > 1 )
		for ( int id = 1; id < argc; id++ )
		{
			if ( strstr( argv[ id ], "-dev" ) )
				SetDebug( true );

			if ( strstr( argv[ id ], "-window" ) )
				Configuration.bFullscreen = false;

			if ( strstr( argv[ id ], "-fullscreen" ) )
				Configuration.bFullscreen = true;

			if ( strstr( argv[ id ], "-width" ) )
				Configuration.iWindowWidth = atoi( argv[ id + 1 ] );

			if ( strstr( argv[ id ], "-height" ) )
				Configuration.iWindowHeight = atoi( argv[ id + 1 ] );
		}

	if ( Configuration.bFullscreen )
		WindowCreate( Style::Fullscreen );
	else
		WindowCreate();
}

//-------------------------------------------------------------------------//

string System::ReadXmlFile( const string sRoute, const string sTagMain, const string sTag )
{
	if ( FileExists( sRoute ) )
	{
		TiXmlDocument File( sRoute.c_str() );
		File.LoadFile();

		TiXmlElement* Head;
		TiXmlElement* Tag;

		if ( !File.FirstChildElement( sTagMain.c_str() ) )
		{
			cout << "Error: In File [" << sRoute << "] Not Found FirstChildElement " << sTagMain << "\n";
			return "ERROR";
		}

		Head = File.FirstChildElement( sTagMain.c_str() );

		if ( !Head->FirstChildElement( sTag.c_str() ) )
		{
			cout << "Error: In File [" << sRoute << "] Not Found FirstChildElement |" << sTag << "| in Element |" << sTagMain << "|\n";
			return "ERROR";
		}

		Tag = Head->FirstChildElement( sTag.c_str() );

		string sString;

		while ( Tag )
		{
			sString = Tag->GetText();
			Tag = Tag->NextSiblingElement( sTag.c_str() );
		}

		return sString;
	}
	else
		cout << "Error: File [" << sRoute << "] Not Found\n";

	return "ERROR";
}

//-------------------------------------------------------------------------//

bool System::FileExists( const string sRouteToFile )
{
	ifstream fis( sRouteToFile );

	if ( !fis.fail() )
		return true;

	return false;
}

//-------------------------------------------------------------------------//

bool System::DirectoryExists( const string sRouteToDirectory )
{
	ofstream fos( sRouteToDirectory + "/tmp" );
	fos.close();

	ifstream fis( sRouteToDirectory + "/tmp" );

	if ( !fis.fail() )
	{
		remove( string( sRouteToDirectory + "/tmp" ).c_str() );
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------//

void System::WindowCreate( int iStyle )
{
	RenderWindow.close();

	if ( iStyle == Style::Default && Configuration.bFullscreen )
		iStyle = Style::Fullscreen;

	string sTitleWindow = Configuration.sWindowName;
	if ( Configuration.sGameVersion != "" )
		sTitleWindow += " | " + Configuration.sGameVersion;

	ContextSettings ContextSettings;
	ContextSettings.depthBits = 24;
	ContextSettings.stencilBits = 8;
	ContextSettings.majorVersion = 3;
	ContextSettings.minorVersion = 0;
	ContextSettings.antialiasingLevel = Configuration.iAntialiasingLevel;

	RenderWindow.create( VideoMode( Configuration.iWindowWidth, Configuration.iWindowHeight ), sTitleWindow, iStyle, ContextSettings );
	RenderWindow.setMouseCursorVisible( false );
	RenderWindow.setFramerateLimit( Configuration.iFrameLimit );
	RenderWindow.setVerticalSyncEnabled( Configuration.bV_Sinc );

	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK )
		exit( -1 );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthMask( GL_TRUE );
	glClearDepth( 1.f );
	glCullFace( GL_BACK );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	Configuration.ProjectionMatrix = glm::perspective( glm::radians( 75.f ), ( float ) Configuration.iWindowWidth / ( float ) Configuration.iWindowHeight, 0.1f, 1500.f );
	glLoadMatrixf( glm::value_ptr( Configuration.ProjectionMatrix ) );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

//-------------------------------------------------------------------------//

void System::MainLoop( BasicStagesGame& BasicStagesGame )
{
	while ( RenderWindow.isOpen() )
	{
		Clock();
		while ( RenderWindow.pollEvent( Event ) )
		{
			if ( Event.type == Event::Closed )
				RenderWindow.close();
		}

		if ( Event.type != Event::LostFocus )
		{
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			RenderWindow.setView( GameCamera );
			MouseCursor.UpdatePosition( RenderWindow );
			BasicStagesGame.CheckStages();

			RenderWindow.display();
		}
	}
}

//-------------------------------------------------------------------------//

void System::UpdateFileConfiguration()
{
	SaveInFile( sRouteFileConfiguration, "[SCREAN]", "", true );
	SaveInFile( sRouteFileConfiguration, "iWindowWidth", Configuration.iWindowWidth );
	SaveInFile( sRouteFileConfiguration, "iWindowHeight", Configuration.iWindowHeight );
	SaveInFile( sRouteFileConfiguration, "iFrameLimit", Configuration.iFrameLimit );
	SaveInFile( sRouteFileConfiguration, "iAntialiasingLevel", Configuration.iAntialiasingLevel );
	SaveInFile( sRouteFileConfiguration, "fRatioView", Configuration.fRatioView );
	SaveInFile( sRouteFileConfiguration, "bFullscreen", Configuration.bFullscreen );
	SaveInFile( sRouteFileConfiguration, "bV_Sinc", Configuration.bV_Sinc );

	SaveInFile( sRouteFileConfiguration, "\n[AUDIO]", "" );
	SaveInFile( sRouteFileConfiguration, "bSound", Configuration.bSound );
	SaveInFile( sRouteFileConfiguration, "bMusic", Configuration.bMusic );
	SaveInFile( sRouteFileConfiguration, "iVolumeSound", Configuration.iVolumeSound );
	SaveInFile( sRouteFileConfiguration, "iVolumeMusic", Configuration.iVolumeMusic );

	SaveInFile( sRouteFileConfiguration, "\n[GAME]", "" );
	SaveInFile( sRouteFileConfiguration, "sLanguage", Configuration.sLanguage );
	SaveInFile( sRouteFileConfiguration, "fGameSpeed", Configuration.fGameSpeed );
	SaveInFile( sRouteFileConfiguration, "fGameTick", Configuration.fGameTick );
}

//-------------------------------------------------------------------------//

void System::SetDebug( bool debug )
{
	Configuration.bDebug = debug;

	if ( debug )
		cout << "Info: Debug mode enabled\n";
	else
		cout << "Info: Debug mode disabled\n";
}

//-------------------------------------------------------------------------//

void System::SetGameSpeed( const float fGameSpeed )
{
	Configuration.fGameSpeed = fGameSpeed;
}

//-------------------------------------------------------------------------//

void System::SetFrameLimit( const int FrameLimit )
{
	Configuration.iFrameLimit = FrameLimit;

	RenderWindow.setFramerateLimit( FrameLimit );
}

//-------------------------------------------------------------------------//

void System::SetWindowTitle( const String WindowName, const String GameVersion )
{
	Configuration.sWindowName = WindowName;
	Configuration.sGameVersion = GameVersion;

	string sTitleWindow = WindowName;

	if ( GameVersion != "" )
		sTitleWindow += " | " + GameVersion;

	RenderWindow.setTitle( sTitleWindow );
}

//-------------------------------------------------------------------------//

void System::SetGameTick( const float fGameTick )
{
	Configuration.fGameTick = fGameTick;
}

//-------------------------------------------------------------------------//

Configuration& System::GetConfiguration()
{
	return Configuration;
}

//-------------------------------------------------------------------------//

RenderWindow& System::GetWindow()
{
	return RenderWindow;
}

//-------------------------------------------------------------------------//

Event& System::GetEvent()
{
	return Event;
}

//-------------------------------------------------------------------------//

MouseCursor& System::GetMouseCursor()
{
	return MouseCursor;
}

//-------------------------------------------------------------------------//

View& System::GetMenuCamera()
{
	return MenuCamera;
}

//-------------------------------------------------------------------------//

le::Localization &System::GetLocalization()
{
	return Localization;
}

//-------------------------------------------------------------------------//

View& System::GetGameCamera()
{
	return GameCamera;
}

//-------------------------------------------------------------------------//

void System::Clock()
{
	Configuration.fTime = clock.getElapsedTime().asMicroseconds();
	clock.restart();

	Configuration.fTime /= Configuration.fGameSpeed;

	if ( Configuration.fTime > Configuration.fGameTick )
		Configuration.fTime = Configuration.fGameTick;
}

//-------------------------------------------------------------------------//

Configuration::Configuration()
{
	iWindowWidth = VideoMode::getDesktopMode().width;
	iWindowHeight = VideoMode::getDesktopMode().height;
	iVolumeMusic = iVolumeSound = 100;
	iFrameLimit = 60;
	iAntialiasingLevel = 0;

	fRatioView = 1.f;
	fGameSpeed = 300.f;
	fGameTick = 40.f;
	fTime = 0.f;

	bSound = bMusic = bFullscreen = true;
	bV_Sinc = bDebug = false;

	sLanguage = "EN";
	sWindowName = ENGINE;
	sGameVersion = ENGINE_VERSION;
}

//-------------------------------------------------------------------------//
