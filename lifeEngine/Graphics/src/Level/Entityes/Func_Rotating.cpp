#include <Graphics\Level\Entityes\Func_Rotating.h>
#include <System\System.h>

//-------------------------------------------------------------------------//

le::Func_Rotating::Func_Rotating( map<string, string>& Values ) :
	BaseEntity( "func_rotating" ),
	IdModel( -1 ),
	Model( NULL )
{
	IdModel = StringToNumber<int>( Values[ "model" ], -1 );
	Speed = StringToNumber<float>( Values[ "Speed" ], 2.f ) / 100.f;
	Position = ValueToVec3( Values[ "origin" ] );

	Transformation = glm::translate( Position );
}

//-------------------------------------------------------------------------//

void le::Func_Rotating::Update()
{
	if ( !Model ) return;

	glm::vec3 Axis( sin( 0 ), sin( System::Configuration.Time * Speed ), sin( 0 ) );
	glm::vec3 Rotations( cos( 0 ), cos( System::Configuration.Time * Speed ), cos( 0 ) );

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );

	Transformation *= glm::mat4_cast( RotateX * RotateY * RotateZ );
}

//-------------------------------------------------------------------------//

void le::Func_Rotating::SetModel( BSPModel& BSPModel )
{
	Model = &BSPModel;
	Model->Max += Position;
	Model->Min += Position;
}

//-------------------------------------------------------------------------//

int le::Func_Rotating::GetIdModel()
{
	return IdModel;
}

//-------------------------------------------------------------------------//