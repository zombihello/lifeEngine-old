#include "..\ConfigFile.h"

//-------------------------------------------------------------------------//

bool le::ConfigFile::LoadFromFile( const string& RouteToFile )
{
	ifstream FileInput( RouteToFile );

	if ( !FileInput.is_open() )
		return false;

	bool IsValue = false, IsString = false;
	string LineConfig, TmpString, NameValue;
	istringstream StrStream;

	while ( getline( FileInput, LineConfig ) )
	{
		StrStream.clear();
		StrStream.str( LineConfig );

		while ( !StrStream.eof() )
		{
			StrStream >> TmpString;
			
			if ( TmpString[ 0 ] == '#' )
				break;

			if ( TmpString == "\"" )
			{
				IsString = !IsString;
				continue;
			}
			else if ( TmpString[ 0 ] == '[' && TmpString[ TmpString.size() - 1 ] == ']' ) 
				continue;
			else if ( TmpString[ 0 ] == '"' && TmpString[ TmpString.size() - 1 ] == '"' )
			{
				TmpString.erase( 0, 1 );
				TmpString.erase( TmpString.size() - 1, 1 );
			}
			else
			{
				if ( TmpString[ 0 ] == '"' )
				{
					TmpString.erase( 0, 1 );
					IsString = true;
				}
				else if ( TmpString[ TmpString.size() - 1 ] == '"' )
				{
					TmpString.erase( TmpString.size() - 1 );
					TmpString = " " + TmpString;
					IsString = false;
				}
			}

			if ( !IsValue )
			{
				NameValue = TmpString;
				IsValue = true;
			}
			else
			{
				if ( IsString && !Values[ NameValue ].empty() )
					Values[ NameValue ] += " " + TmpString;
				else
					Values[ NameValue ] += TmpString;

				if ( !IsString ) IsValue = false;
			}
		}

	}

	return true;
}

//-------------------------------------------------------------------------//

void le::ConfigFile::SaveInFile( const string& RouteToFile )
{
	ofstream FileOutput( RouteToFile );
	FileOutput << SaveBuffer.str();
	FileOutput.close();
}

//-------------------------------------------------------------------------//

void le::ConfigFile::WriteValue( const string& NameValue, const string& Value )
{
	SaveBuffer << NameValue << " \"" << Value << "\"" << endl;
}

//-------------------------------------------------------------------------//

void le::ConfigFile::WriteValue( const string & NameValue, const int& Value )
{
	SaveBuffer << NameValue << " " << Value << endl;
}

//-------------------------------------------------------------------------//

void le::ConfigFile::WriteValue( const string & NameValue, const float& Value )
{
	SaveBuffer << NameValue << " " << Value << endl;
}

//-------------------------------------------------------------------------//

void le::ConfigFile::WriteValue( const string & NameValue, const bool& Value )
{
	if ( Value )
		SaveBuffer << NameValue << " true\n";
	else
		SaveBuffer << NameValue << " false\n";
}

//-------------------------------------------------------------------------//

void le::ConfigFile::WriteComent( const string& Coment )
{
	SaveBuffer << "#" << Coment << endl;
}

//-------------------------------------------------------------------------//

void le::ConfigFile::WriteGroup( const string & NameGroup )
{
	SaveBuffer << "[" << NameGroup << "]\n";
}

//-------------------------------------------------------------------------//

void le::ConfigFile::ClearSaveBuffer()
{
	SaveBuffer.clear();
}

//-------------------------------------------------------------------------//

string le::ConfigFile::GetValueString( const string& NameValue )
{
	return Values[ NameValue ];
}

//-------------------------------------------------------------------------//

int le::ConfigFile::GetValueInt( const string& NameValue )
{
	return atoi( Values[ NameValue ].c_str() );
}

//-------------------------------------------------------------------------//

float le::ConfigFile::GetValueFloat( const string& NameValue )
{
	return static_cast< float >( atof( Values[ NameValue ].c_str() ) );
}

//-------------------------------------------------------------------------//

bool le::ConfigFile::GetValueBool( const string& NameValue )
{
	string StringValue = Values[ NameValue ];

	if ( StringValue == "True" || StringValue == "true" || StringValue == "1" )
		return true;

	return false;
}

//-------------------------------------------------------------------------//
