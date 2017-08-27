#include "..\Logger.h"

//-------------------------------------------------------------------------//

ofstream le::Logger::FileSave = ofstream();

//-------------------------------------------------------------------------//

void le::Logger::Log( const Type & TypeLog, const string & Message )
{
	switch ( TypeLog )
	{
	case Type::None:
		FileSave <<  Message << endl;
		cout << Message << endl;
		break;

	case Type::Info:
		FileSave << "[Info] " << Message << endl;
		cout << "[Info] " << Message << endl;
		break;

	case Type::Warning:
		FileSave << "[Warning] " << Message << endl;
		cout << "[Warning] " << Message << endl;
		break;

	case Type::Error:
		FileSave << "[Error] " << Message << endl;
		cout << "[Error] " << Message << endl;
		break;
	}
}

//-------------------------------------------------------------------------//

void le::Logger::SetLogFile( const string & Route )
{
	FileSave.open( Route );
}

//-------------------------------------------------------------------------//
