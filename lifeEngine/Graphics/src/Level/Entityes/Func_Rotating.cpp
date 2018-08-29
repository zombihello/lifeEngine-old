#include <Graphics\Level\Entityes\Func_Rotating.h>
#include <System\System.h>

//-------------------------------------------------------------------------//

le::Func_Rotating::Func_Rotating( map<string, string>& Values ) :
	BaseEntity( "func_rotating" ),
	IdModel( -1 ),
	Model( NULL )
{
	IdModel = StringToNumber<int>( Values[ "model" ], -1 );
	Speed = StringToNumber<float>( Values[ "Speed" ], 2.f );
	Position = ValueToVec3( Values[ "origin" ] );

	Transformation = glm::translate( Position );
}

//-------------------------------------------------------------------------//

void le::Func_Rotating::Update()
{
	if ( !Model ) return;

	Transformation *= glm::mat4_cast( glm::quat( glm::vec3( 0.f, glm::radians( System::Configuration.Time * Speed ), 0.f ) ) );
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