#include <Graphics\Level\Entityes\Func_Rotating.h>
#include <System\System.h>

//-------------------------------------------------------------------------//

le::Func_Rotating::Func_Rotating( map<string, string>& Values ) :
	BaseEntity( "func_rotating" ),
	IdModel( -1 ),
	Model( NULL )
{
	IdModel = StringToNumber<int>( Values[ "model" ], -1 );
	Speed = StringToNumber<float>( Values[ "Speed" ], 120.f );
	Position = ValueToVec3( Values[ "origin" ] );

	Transformation = glm::translate( Position );
}

//-------------------------------------------------------------------------//

void le::Func_Rotating::Update()
{
	// TODO: [zombiHello] Проверить на правильность работу врещения Min и Max вершин

	if ( !Model ) return;

	glm::quat Rotation = glm::quat( glm::vec3( 0.f, glm::radians( System::Configuration.Time * Speed ), 0.f ) );

	Transformation *= glm::mat4_cast( Rotation );
	Model->Max = Rotation * Local_Max + Position;
	Model->Min = Rotation * Local_Min + Position;

}

//-------------------------------------------------------------------------//

void le::Func_Rotating::SetModel( BSPModel& BSPModel )
{
	Model = &BSPModel;
	Local_Max = Model->Max;
	Local_Min = Model->Min;

	Model->Max += Position;
	Model->Min += Position;
}

//-------------------------------------------------------------------------//

int le::Func_Rotating::GetIdModel()
{
	return IdModel;
}

//-------------------------------------------------------------------------//