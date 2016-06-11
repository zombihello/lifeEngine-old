#include "Localization.h"
#include "../codecvt/codecvt.h"

//-------------------------------------------------------------------------//

void le::Localization::LoadFileLocalization( string Route, string LanguageName )
{
	wifstream fis( Route );
	fis.imbue( locale( fis.getloc(), new utf8_conversion ) );

	if ( fis.is_open() )
	{
		map<String,String> mTemp;
		wstring sTagName;
		wstring sValue;

		for ( int i = 0; !fis.eof(); i++ )
		{
			fis >> sTagName;
			fis >> sValue;

			if ( sValue[ 0 ] == '"' )
			{
				wstring sTempString;
				sValue.erase( sValue.begin() );

				for ( int j = 0; sValue[ sValue.size()-1 ] != '"'; j++ )
				{
					sValue += L' ';
					fis >> sTempString;
					sValue += sTempString;
				}

				sValue.erase( sValue.size()-1 + sValue.begin() );
			}

			mTemp[ sTagName ] = sValue;
		}

		mLocalization[ LanguageName ] = mTemp;
	}
	else
		cout << "Error: File [" << Route << "] Not Found\n";
}

//-------------------------------------------------------------------------//

String le::Localization::GetString( string LanguageName, string TagString )
{
	if ( mLocalization.count( LanguageName ) != 0 )
		if ( mLocalization[ LanguageName ].count( TagString ) != 0 )
			return mLocalization[ LanguageName ][ TagString ];
	return "ERROR";
}

//-------------------------------------------------------------------------//
