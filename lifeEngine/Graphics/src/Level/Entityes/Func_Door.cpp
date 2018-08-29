#include <Graphics\Level\Entityes\Func_Door.h>
#include <System\System.h>

//-------------------------------------------------------------------------//

le::Func_Door::Func_Door( map<string, string>& Values ) :
	BaseEntity( "func_door" ),
	Angle( 0 ),
	IdModel( -1 ),
	Model( NULL )
{
	IdModel = StringToNumber<int>( Values[ "model" ], -1 );
	AngleMax = glm::radians( StringToNumber<float>( Values[ "AngleMax" ], 90.f ) );
	Position = ValueToVec3( Values[ "origin" ] );

	Transformation = glm::translate( Position );

	StartQuat = TempQuat = glm::quat( glm::vec3( 0.f, 0.f, 0.f ) );
	EndQuat = glm::quat( glm::vec3( 0.f, AngleMax, 0.f ) );
}

//-------------------------------------------------------------------------//

void le::Func_Door::Update()
{
	if ( !Model ) return;

	if ( Keyboard::isKeyPressed( Keyboard::F ) && TempQuat != EndQuat )
	{
		TempQuat = glm::lerp( TempQuat, EndQuat, 100.f * System::Configuration.Time / 1000.f  );
		Transformation = glm::translate( Position ) * glm::mat4_cast( TempQuat );
	}
	else if ( !Keyboard::isKeyPressed( Keyboard::F ) && TempQuat != StartQuat )
	{
		TempQuat = glm::lerp( TempQuat, StartQuat, 100.f * System::Configuration.Time / 1000.f );
		Transformation = glm::translate( Position ) * glm::mat4_cast( TempQuat );
	}
}

//-------------------------------------------------------------------------//

void le::Func_Door::SetModel( BSPModel& BSPModel )
{
	Model = &BSPModel;
	Model->Max += Position;
	Model->Min += Position;
}

//-------------------------------------------------------------------------//

int le::Func_Door::GetIdModel()
{
	return IdModel;
}

//-------------------------------------------------------------------------//