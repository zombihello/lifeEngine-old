#include <System\ResourcesManager.h>
#include "..\System.h"

//-------------------------------------------------------------------------//

le::Configuration le::System::Configuration = le::Configuration();

//-------------------------------------------------------------------------//

le::System::System( int argc, char** argv, const string& ConfigFile, const string& LogFile ) :
	IsLostFocus( false )
{
	Logger::SetLogFile( LogFile );
	Logger::Log( Logger::None, ENGINE " " ENGINE_VERSION );
	Logger::Log( Logger::None, "*** " ENGINE " Start ***" );
	Logger::Log( Logger::Info, "Loading File Configurations [" + LogFile + "]" );

	le::ConfigFile Config;

	if ( Config.LoadFromFile( ConfigFile ) )
	{
		Logger::Log( Logger::Info, "Loaded File Configurations [" + LogFile + "]" );

		Configuration.WindowSize = glm::vec2( Config.GetValueInt( "Width" ), Config.GetValueInt( "Height" ) );
		Configuration.FrameLimit = Config.GetValueInt( "FrameLimit" );
		Configuration.AntialiasingLevel = Config.GetValueInt( "AntialiasingLevel" );
		Configuration.RenderDistance = Config.GetValueInt( "RenderDistance" );
		Configuration.QualityShadows = Config.GetValueInt( "QualityShadows" );
		Configuration.FOV = Config.GetValueInt( "FOV" );
		Configuration.SensitivityMouse = Config.GetValueFloat( "SensitivityMouse" );
	}
	else
	{
		Logger::Log( Logger::Info, "File Configurations [" + LogFile + "] Not Found. Created Default Configuration File" );

		Config.WriteGroup( "SCREAN" );
		Config.WriteValue( "Width", Configuration.WindowSize.x );
		Config.WriteValue( "Height", Configuration.WindowSize.y );
		Config.WriteValue( "FrameLimit", Configuration.FrameLimit );
		Config.WriteValue( "SensitivityMouse", Configuration.SensitivityMouse );

		Config.WriteGroup( "GRAPHICS" );
		Config.WriteValue( "AntialiasingLevel", Configuration.AntialiasingLevel );
		Config.WriteValue( "RenderDistance", Configuration.RenderDistance );
		Config.WriteValue( "QualityShadows", Configuration.QualityShadows );
		Config.WriteValue( "FOV", Configuration.FOV );
		Config.SaveInFile( ConfigFile );
	}

	Configuration.ProjectionMatrix = glm::perspective( glm::radians( NUMBER_TO_FLOAT( Configuration.FOV ) ), Configuration.WindowSize.x / Configuration.WindowSize.y, 0.1f, NUMBER_TO_FLOAT( Configuration.RenderDistance ) );
}

//-------------------------------------------------------------------------//

le::System::~System()
{
	ResourcesManager::DeleteAllResources();
	Logger::Log( Logger::None, "*** " ENGINE " End ***" );
}

//-------------------------------------------------------------------------//

void le::System::WindowCreate( const string& NameWindow, int Style, bool IsMouseCursorVisible )
{
	glewExperimental = GL_TRUE;

	ContextSettings ContextSettings;
	ContextSettings.depthBits = 24;
	ContextSettings.stencilBits = 8;
	ContextSettings.antialiasingLevel = Configuration.AntialiasingLevel;

	RenderWindow.create( VideoMode( NUMBER_TO_UINT( Configuration.WindowSize.x ), NUMBER_TO_UINT( Configuration.WindowSize.y ) ), NameWindow, Style, ContextSettings );
	RenderWindow.setFramerateLimit( Configuration.FrameLimit );
	RenderWindow.setMouseCursorVisible( IsMouseCursorVisible );

	Logger::Log( Logger::Info, "Window Created (" + to_string( ( int ) Configuration.WindowSize.x ) + "x" + to_string( ( int ) Configuration.WindowSize.y ) + ")" );
	Logger::Log( Logger::None, "*** OpenGL Info ***" );

	stringstream GLInfo;
	int MaxUniformComponents;

	GLInfo << "  OpenGL Version: " << glGetString( GL_VERSION ) << endl;
	GLInfo << "  OpenGL Vendor: " << glGetString( GL_VENDOR ) << endl;
	GLInfo << "  OpenGL Renderer: " << glGetString( GL_RENDERER ) << endl;
	GLInfo << "  OpenGL GLSL Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << endl;

	glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &MaxUniformComponents );
	GLInfo << "  Max Floats in Fragment Shader: " << MaxUniformComponents << endl;

	glGetIntegerv( GL_MAX_VERTEX_UNIFORM_COMPONENTS, &MaxUniformComponents );
	GLInfo << "  Max Floats in Vertex Shader: " << MaxUniformComponents << endl;

	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &MaxUniformComponents );
	GLInfo << "  Max Texture Image Units: " << MaxUniformComponents;

	Logger::Log( Logger::None, GLInfo.str() );
	Logger::Log( Logger::None, "*** OpenGL Info End ***" );

	if ( glewInit() != GLEW_OK )
	{
		Logger::Log( Logger::Error, "OpenGL Context Is Broken" );
		exit( -1 );
	}
	else if ( !GLEW_VERSION_3_0 )
	{
		Logger::Log( Logger::Error, "OpenGL 3.0+ Not Available" );
		exit( -1 );
	}
	else if ( !Shader::isAvailable() )
	{
		Logger::Log( Logger::Error, "System Not Supported Shaders" );
		exit( -1 );
	}	
}

//-------------------------------------------------------------------------//

void le::System::MainLoop( BasicApplication& Application )
{
	while ( RenderWindow.isOpen() )
	{
		Configuration.Time = Clock.restart().asSeconds() * 60; // 60 - 1 тик

		while ( RenderWindow.pollEvent( Event ) )
		{
			switch ( Event.type )
			{
			case Event::Closed:
				RenderWindow.close();
				break;

			case Event::LostFocus:
				IsLostFocus = true;
				break;

			case Event::GainedFocus:
				IsLostFocus = false;
				break;
			}
		}

		if ( !IsLostFocus )
		{
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			Application.Update();
			RenderWindow.display();
		}
	}
}

//-------------------------------------------------------------------------//

void le::System::SetWireframeRender( bool Enable )
{
	if ( Enable )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		Configuration::IsWireframeRender = true;
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		Configuration::IsWireframeRender = false;
	}
}

//-------------------------------------------------------------------------//

bool le::System::FileExists( const string& RouteToFile )
{
	bool IsExists = false;
	ifstream FileInput( RouteToFile );

	if ( FileInput.is_open() )
		IsExists = true;

	FileInput.close();
	return IsExists;
}

//-------------------------------------------------------------------------//

bool le::System::DirectoryExists( const string& RouteToDirectory )
{
	string RouteToTmpFile = RouteToDirectory + "/tmp";
	ofstream FileOutput( RouteToTmpFile );
	FileOutput.close();

	ifstream FileInput( RouteToTmpFile );

	if ( FileInput.is_open() )
	{
		remove( RouteToTmpFile.c_str() );
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------//

RenderWindow& le::System::GetWindow()
{
	return RenderWindow;
}

//-------------------------------------------------------------------------//
