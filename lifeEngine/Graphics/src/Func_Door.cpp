#include "../Func_Door.h"
#include <System\System.h>

//-------------------------------------------------------------------------//

le::Func_Door::Func_Door( const map<string, string>& Values, BSPModel* Model ) :
	BaseEntity( Values ),
	Count( 0 ),
	MovingRight( false ),
	Model( Model )
{}

//-------------------------------------------------------------------------//

void le::Func_Door::Update()
{
	// Данный код временный, он предназначен для 
	// тестирования механизма движения брашей

	glm::vec3 OffsetMove( System::Configuration.Time, 0, 0 );

	if ( MovingRight )
		Count += 1.f;
	else
	{
		OffsetMove.x = -OffsetMove.x;
		Count -= 1.f;
	}

	Model->Max += OffsetMove;
	Model->Min += OffsetMove;
	Transformation *= glm::translate( OffsetMove );

	if ( Count > 500 )
		MovingRight = false;
	else if ( Count < -500 )
		MovingRight = true;
}

//-------------------------------------------------------------------------//

void le::Func_Door::Action()
{}

//-------------------------------------------------------------------------//