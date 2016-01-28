#include "System.h"
using namespace le;


System::System()
{
    if ( FileExists( "config.cfg" ) ) // if file configuration exists
    {
        Configuration.iWindowWidth = ReadFile<int>( "config.cfg" , "iWindowWidth" );
        Configuration.iWindowHeight = ReadFile<int>( "config.cfg" , "iWindowHeight" );
        Configuration.iVolumeMusic = ReadFile<int>( "config.cfg" , "iVolumeMusic" );
        Configuration.iVolumeSound = ReadFile<int>( "config.cfg" , "iVolumeSound" );

        Configuration.bFullscreen = ReadFile<int>( "config.cfg" , "bFullscreen" );
        Configuration.bMusic = ReadFile<int>( "config.cfg" , "bMusic" );
        Configuration.bSound = ReadFile<int>( "config.cfg" , "bSound" );
        Configuration.bV_Sinc = ReadFile<int>( "config.cfg" , "bV_Sinc" );
    }
    else // otherwise write file config.cfg
    {
        cout << "Error: File [config.cfg] Not Found\n";
        UpdateConfig();
    }

    // Create Window
    if ( Configuration.bFullscreen )
        WindowCreate( Style::Fullscreen );
    else
        WindowCreate();
}


System::~System()
{
}

string le::System::ReadXMLFile( const string sRoute , const string sTagMain , const string sTag )
{
    string sString;

    if ( FileExists( sRoute ) ) // if exists file
    {
        TiXmlDocument File( sRoute.c_str() );
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
            cout << "Error: In File [" << sRoute << "] Not Found FirstChildElement " << sTag << "\n";
            return "ERROR";
        }

        Tag = Head->FirstChildElement( sTag.c_str() );

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

bool le::System::FileExists( const string sRouteToFile )
{
    ifstream fis( sRouteToFile ); // Open file

    if ( !fis.fail() ) // if no error, then all well
    {
        fis.close(); // close file
        return true;
    }

    // otherwise not all well
    return false;
}

bool le::System::DirectoryExists( const string sRouteToDirectory )
{
    ofstream fos( sRouteToDirectory + "/tmp" ); // Create tmp file
    fos.close(); // Close file

    ifstream fis( sRouteToDirectory + "/tmp" ); // Open This File

    if ( !fis.fail() ) // if no error, then all well
    {
        fis.close(); // close file
        remove( string( sRouteToDirectory + "/tmp" ).c_str() ); // remove tmp file
        return true;
    }

    // otherwise not all well
    return false;
}

void le::System::UpdateConfig()
{
    SaveFile( "config.cfg" , "[SCREAN]" , "" , true );
    SaveFile( "config.cfg" , "iWindowWidth" , Configuration.iWindowWidth );
    SaveFile( "config.cfg" , "iWindowHeight" , Configuration.iWindowHeight );
    SaveFile( "config.cfg" , "bFullscreen" , Configuration.bFullscreen );
    SaveFile( "config.cfg" , "bV_Sinc" , Configuration.bV_Sinc );

    SaveFile( "config.cfg" , "\n[AUDIO]" , "" );
    SaveFile( "config.cfg" , "bSound" , Configuration.bSound );
    SaveFile( "config.cfg" , "bMusic" , Configuration.bMusic );
    SaveFile( "config.cfg" , "iVolumeSound" , Configuration.iVolumeSound );
    SaveFile( "config.cfg" , "iVolumeMusic" , Configuration.iVolumeMusic );
}

void le::System::WindowCreate( const int iStyle )
{
    RenderWindow.close();

    RenderWindow.create( VideoMode( Configuration.iWindowWidth , Configuration.iWindowHeight ) , Configuration.sWindowName + " | " + Configuration.sGameVersion , iStyle );
    RenderWindow.setMouseCursorVisible( false );
    RenderWindow.setFramerateLimit( 60 );
    RenderWindow.setVerticalSyncEnabled( Configuration.bV_Sinc );
}

void le::System::MainLoop( BasicStageGame & BasicStageGame )
{
    while ( RenderWindow.isOpen() )
    {
        clock();
        while ( RenderWindow.pollEvent( Event ) )
        {
            TypeEvent = Event.type;
            if ( TypeEvent == Event::Closed ) RenderWindow.close();
            MouseCursor.MousePosition( RenderWindow );
        }

        //////////////
        // RENDER
        /////////////
        RenderWindow.clear();

        if ( Event.type != Event::LostFocus )
            BasicStageGame.CheckStages();

        RenderWindow.display();
    }
}

void le::System::SetWindowTitle( const String WindowName , const String GameVersion )
{
    Configuration.sWindowName = WindowName;
    Configuration.sGameVersion = GameVersion;

    RenderWindow.setTitle( WindowName + " | " + GameVersion );
}

void le::System::SetGameSpeed( const float fGameSpeed )
{
    Configuration.fGameSpeed = fGameSpeed;
}

void le::System::SetGameTick( const float fGameTick )
{
    Configuration.fGameTick = fGameTick;
}

Configuration& le::System::GetConfiguration()
{
    return Configuration;
}

Event::EventType le::System::GetTypeEvent() const
{
    return TypeEvent;
}

RenderWindow & le::System::GetWindow()
{
    return RenderWindow;
}

Event le::System::GetEvent() const
{
    return Event;
}

MouseCursor & le::System::GetMouseCursor()
{
    return MouseCursor;
}

void le::System::clock()
{
    Configuration.fTime = Clock.getElapsedTime().asMicroseconds();
    Clock.restart();

    Configuration.fTime /= Configuration.fGameSpeed;

    if ( Configuration.fTime > Configuration.fGameTick )
        Configuration.fTime = Configuration.fGameTick;
}

le::Configuration::Configuration()
{
    iWindowWidth = 800;
    iWindowHeight = 600;
    iVolumeSound = 100;
    iVolumeMusic = 100;

    fGameSpeed = 300.f;
    fGameTick = 40.f;
    fTime = 0.f;

    bSound = true;
    bMusic = true;
    bFullscreen = false;
    bV_Sinc = false;

    sWindowName = "Window Name";
    sGameVersion = "Game Version";
}
