#include <Graphics\Level\Entityes\EntityParser.h>

//-------------------------------------------------------------------------//

le::EntityParser::EntityParser()
{}

//-------------------------------------------------------------------------//

le::EntityParser::EntityParser( const BSPEntities& BSPEntities, int Size )
{
	Parse( BSPEntities, Size );
}

//-------------------------------------------------------------------------//

void le::EntityParser::Parse( const BSPEntities& BSPEntities, int Size )
{
	bool						IsEntity = false, IsBracket = false, IsName = false, IsValue = false;
	size_t						IdStart_EntityData = 0, IdFinish_EntityData = 0;

	string						Name;
	string						Temp;
	Entity						TempEntity;
	map<string, string>			Values;

	// *************************************
	// Парсим информацию о энтити-объектах

	for ( int IdChar_Entities = 0; IdChar_Entities < Size; IdChar_Entities++ )
	{
		if ( BSPEntities.EntitiesData[ IdChar_Entities ] == '{' && !IsEntity )
		{
			IsEntity = true;
			IdStart_EntityData = IdChar_Entities+1;
		}
		else if ( BSPEntities.EntitiesData[ IdChar_Entities ] == '}' && IsEntity )
		{
			IsEntity = false;
			IdFinish_EntityData = IdChar_Entities-1;

			for ( size_t IdChar_Entity = IdStart_EntityData; IdChar_Entity < IdFinish_EntityData; IdChar_Entity++ )
			{
				if ( BSPEntities.EntitiesData[ IdChar_Entity ] == '\"' && !IsName && !IsValue && !IsBracket )
				{
					IsName = IsBracket = true;
					Temp.clear();
					continue;
				}
				else if ( BSPEntities.EntitiesData[ IdChar_Entity ] == '\"' && IsName && !IsValue && IsBracket )
				{
					Name = Temp;
					IsBracket = false;
					Temp.clear();
					continue;
				}
				else if ( BSPEntities.EntitiesData[ IdChar_Entity ] == '\"' && IsName && !IsValue && !IsBracket )
				{
					IsBracket = IsValue = true;
					Temp.clear();
					continue;
				}
				else if ( BSPEntities.EntitiesData[ IdChar_Entity ] == '\"' && IsName && IsValue && IsBracket )
				{
					IsBracket = IsValue = IsName = false;

					Values[ Name ] = Temp;
					Temp.clear();
					continue;
				}

				Temp += BSPEntities.EntitiesData[ IdChar_Entity ];
			}

			if ( Values.find( "classname" ) != Values.end() )
			{
				TempEntity.ClassName = Values[ "classname" ];
				TempEntity.Values = Values;
				Entityes.push_back( TempEntity );
			}

			Values.clear();
		}
	}
}

//-------------------------------------------------------------------------//