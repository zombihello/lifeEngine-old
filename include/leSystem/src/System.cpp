#include "../System.h"
using namespace le;

//-------------------------------------------------------------------------//

System::System( const string FileConfiguration )
{
	sRouteFileConfiguration = FileConfiguration;

	if ( FileExists( FileConfiguration ) )
	{
		Configuration.iWindowWidth = ReadTextFile<int>( FileConfiguration , "iWindowWidth" );
		Configuration.iWindowHeight = ReadTextFile<int>( FileConfiguration , "iWindowHeight" );
		Configuration.iVolumeMusic = ReadTextFile<int>( FileConfiguration , "iVolumeMusic" );
		Configuration.iVolumeSound = ReadTextFile<int>( FileConfiguration , "iVolumeSound" );
		Configuration.iFrameLimit = ReadTextFile<int>( FileConfiguration, "iFrameLimit" );

		Configuration.bFullscreen = ReadTextFile<int>( FileConfiguration , "bFullscreen" );
		Configuration.bMusic = ReadTextFile<int>( FileConfiguration , "bMusic" );
		Configuration.bSound = ReadTextFile<int>( FileConfiguration , "bSound" );
		Configuration.bV_Sinc = ReadTextFile<int>( FileConfiguration , "bV_Sinc" );
	}
	else
	{
		cout << "Error: File [" << FileConfiguration << "] Not Found\n";

		UpdateFileConfiguration();
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

	RenderWindow.create( VideoMode( Configuration.iWindowWidth , Configuration.iWindowHeight ) , Configuration.sWindowName + " | " + Configuration.sGameVersion , iStyle );
	RenderWindow.setMouseCursorVisible( false );
	RenderWindow.setFramerateLimit( Configuration.iFrameLimit );
	RenderWindow.setVerticalSyncEnabled( Configuration.bV_Sinc );

	Camera.reset( FloatRect( 0,0, Configuration.iWindowWidth, Configuration.iWindowHeight ) );
}

//-------------------------------------------------------------------------//

void System::MainLoop( BasicStagesGame& BasicStagesGame )
{
	while ( RenderWindow.isOpen() )
	{
		Clock();
		while( RenderWindow.pollEvent( Event ) )
		{
			if ( Event.type == Event::Closed )
				RenderWindow.close();
		}

		RenderWindow.clear();
		RenderWindow.setView( Camera );
		MouseCursor.UpdatePosition( RenderWindow );
		BasicStagesGame.CheckStages();
		RenderWindow.display();
	}
}

//-------------------------------------------------------------------------//

void System::UpdateFileConfiguration()
{
	SaveInFile( sRouteFileConfiguration , "[SCREAN]" , "" , true );
	SaveInFile( sRouteFileConfiguration , "iWindowWidth" , Configuration.iWindowWidth );
	SaveInFile( sRouteFileConfiguration , "iWindowHeight" , Configuration.iWindowHeight );
	SaveInFile( sRouteFileConfiguration , "iFrameLimit" , Configuration.iFrameLimit );
	SaveInFile( sRouteFileConfiguration , "bFullscreen" , Configuration.bFullscreen );
	SaveInFile( sRouteFileConfiguration , "bV_Sinc" , Configuration.bV_Sinc );

	SaveInFile( sRouteFileConfiguration , "\n[AUDIO]" , "" );
	SaveInFile( sRouteFileConfiguration , "bSound" , Configuration.bSound );
	SaveInFile( sRouteFileConfiguration , "bMusic" , Configuration.bMusic );
	SaveInFile( sRouteFileConfiguration , "iVolumeSound" , Configuration.iVolumeSound );
	SaveInFile( sRouteFileConfiguration , "iVolumeMusic" , Configuration.iVolumeMusic );
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

	RenderWindow.setTitle( WindowName + " | " + GameVersion );
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

View& System::GetCamera()
{
	return Camera;
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
	iWindowWidth = 800;
	iWindowHeight = 600;
	iVolumeMusic = iVolumeSound = 100;
	iFrameLimit = 60;

	fGameSpeed = 300.f;
	fGameTick = 40.f;
	fTime = 0.f;

	bSound = bMusic = true;
	bV_Sinc = bFullscreen = false;

	sWindowName = "lifeEngine";
	sGameVersion = "v0.0.1";
}

//-------------------------------------------------------------------------//
