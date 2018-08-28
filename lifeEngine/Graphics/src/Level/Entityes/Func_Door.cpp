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
}

//-------------------------------------------------------------------------//

void le::Func_Door::Update()
{
	if ( !Model ) return;

	glm::vec3 Axis( sin( 0 ) );
	glm::vec3 Rotations( cos( 0 ) );

	if ( Keyboard::isKeyPressed( Keyboard::F ) && Angle < AngleMax )
	{
		Axis.y = sin( System::Configuration.Time * 0.02f );
		Rotations.y = cos( System::Configuration.Time * 0.02f );
	}
	else if ( !Keyboard::isKeyPressed( Keyboard::F ) && Angle > 0 )
	{
		Axis.y = sin( System::Configuration.Time * -0.02f );
		Rotations.y = cos( System::Configuration.Time * -0.02f );
	}

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );
	glm::quat Rotate = RotateX * RotateY * RotateZ;

	Angle += glm::eulerAngles( Rotate ).y;
	Transformation *= glm::mat4_cast( Rotate );
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