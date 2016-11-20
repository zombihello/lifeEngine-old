#include "../SaveValuables.h"

//-------------------------------------------------------------------------//

int le::SaveValuables::GetPropertyInt( string sName )
{
	return atoi( mProperties[ sName ].c_str() );
}

//-------------------------------------------------------------------------//

float le::SaveValuables::GetPropertyFloat( string sName )
{
	return strtod( mProperties[ sName ].c_str() , NULL );
}

//-------------------------------------------------------------------------//

string le::SaveValuables::GetPropertyString( string sName )
{
	return mProperties[ sName ];
}

//-------------------------------------------------------------------------//

le::SaveValuables le::SaveValuables::GetTag( string sNameTag )
{
	return mSaveValuables[ sNameTag ];
}

//-------------------------------------------------------------------------//