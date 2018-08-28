#include <Graphics\Level\Entityes\BaseEntity.h>

//-------------------------------------------------------------------------//

le::BaseEntity::BaseEntity( const string& ClassName ) :
	ClassName( ClassName )
{}

//-------------------------------------------------------------------------//

void le::BaseEntity::Update()
{}

//-------------------------------------------------------------------------//

void le::BaseEntity::Action()
{}

//-------------------------------------------------------------------------//

bool le::BaseEntity::ValueToBoolean( const string& Value )
{
	if ( Value == "True" || Value == "true" || Value == "1" )
		return true;

	return false;
}

//-------------------------------------------------------------------------//

glm::vec3 le::BaseEntity::ValueToVec3( const string& Value )
{
	if ( !Value.empty() )
	{
		stringstream		StringStream( Value );
		string				TempString;
		glm::vec3			Vec3;
		float				Temp;
		int					IndexValue = 0;

		while ( !StringStream.eof() && IndexValue < 3 )
		{
			StringStream >> TempString;
			Vec3[ IndexValue ] = StringToNumber<float>( TempString );
			TempString.clear();
			IndexValue++;
		}

		// Меняем значения Y и Z, и отрицаем новый Z, чтобы Y был вверх. 
		Temp = Vec3.y;
		Vec3.y = Vec3.z;
		Vec3.z = -Temp;

		return Vec3;
	}

	return glm::vec3();
}

//-------------------------------------------------------------------------//

glm::vec4 le::BaseEntity::ValueToVec4( const string& Value )
{
	if ( !Value.empty() )
	{
		stringstream		StringStream( Value );
		string				TempString;
		glm::vec4			Vec4;
		float				Temp;
		int					IndexValue = 0;

		while ( !StringStream.eof() && IndexValue < 4 )
		{
			StringStream >> TempString;
			Vec4[ IndexValue ] = StringToNumber<float>( TempString );
			TempString.clear();
			IndexValue++;
		}

		// Меняем значения Y и Z, и отрицаем новый Z, чтобы Y был вверх. 
		Temp = Vec4.y;
		Vec4.y = Vec4.z;
		Vec4.z = -Temp;

		return Vec4;
	}

	return glm::vec4();
}

//-------------------------------------------------------------------------//