#include <Graphics\Level\Entityes\Func_Door.h>
#include <System\System.h>

//-------------------------------------------------------------------------//

le::Func_Door::Func_Door( map<string, string>& Values ) :
	BaseEntity( "func_door" ),
	InOpen( false ),
	InClose( true ),
	DonePercentag( 0.f ),
	IdModel( -1 ),
	Model( NULL )
{
	float AngleMax	= StringToNumber<float>( Values[ "AngleMax" ], 90.f );
	IdModel			= StringToNumber<int>( Values[ "model" ], -1 );
	SpeedOpen		= AngleMax / StringToNumber<float>( Values[ "SpeedOpen" ], 150.f );
	SpeedClose		= AngleMax / StringToNumber<float>( Values[ "SpeedClose" ], 100.f );
	Position		= ValueToVec3( Values[ "origin" ] );

	Transformation	= MatrixPosition = glm::translate( Position );

	StartRotation	= Rotation = glm::quat( glm::vec3( 0.f, 0.f, 0.f ) );
	EndRotation		= glm::quat( glm::vec3( 0.f, glm::radians( AngleMax ), 0.f ) );
}

//-------------------------------------------------------------------------//

void le::Func_Door::Update()
{
	if ( !Model ) return;

	// TODO: [zombiHello] Исправить вращение мин. и макс. вершин для отсечения по фрустуму

	if ( Keyboard::isKeyPressed( Keyboard::F ) )
	{
		InOpen = true;
		InClose = false;
	}
	else if ( !Keyboard::isKeyPressed( Keyboard::F ) && InOpen )
	{
		InOpen = false;
		InClose = true;
	}

	if ( InOpen && DonePercentag < 1.f )
	{
		DonePercentag = glm::min( 1.f, DonePercentag + ( System::Configuration.Time / SpeedOpen ) );
		Rotation = glm::slerp( StartRotation, EndRotation, DonePercentag );

		Transformation = MatrixPosition * glm::mat4_cast( Rotation );
		//Model->Max = Rotation * ( Local_Max + Position );
		//Model->Min = Rotation * ( Local_Min + Position );
	}
	else if ( InClose && DonePercentag > 0.f )
	{
		DonePercentag = glm::max( 0.f, DonePercentag - ( System::Configuration.Time / SpeedClose ) );
		Rotation = glm::slerp( StartRotation, EndRotation, DonePercentag );

		Transformation = MatrixPosition * glm::mat4_cast( Rotation );
		//Model->Max = Rotation * ( Local_Max + Position );
		//Model->Min = Rotation * ( Local_Min + Position );
	}
}

//-------------------------------------------------------------------------//

void le::Func_Door::SetModel( BSPModel& BSPModel )
{
	Model = &BSPModel;

	Local_Max = Model->Max;
	Local_Min = Model->Min;

	Model->Max += Position;
	Model->Min += Position;
}

//-------------------------------------------------------------------------//

int le::Func_Door::GetIdModel()
{
	return IdModel;
}

//-------------------------------------------------------------------------//