#include "../AI.h"

//-------------------------------------------------------------------------//

le::AI::AI(le::System& System) : ViewAI(System) {}

//-------------------------------------------------------------------------//

void le::AI::UpdateAI( vector<BasicPersonages*> vPersonage )
{
	UpdateViewAI( vPersonage );
	Actions();
}

//-------------------------------------------------------------------------//

bool le::AI::IsLook()
{
	return bIsLook;
}

//-------------------------------------------------------------------------//

le::BasicPersonages*le::AI::GetLookPersonage()
{
	return LookPersonage;
}

//-------------------------------------------------------------------------//

void le::AI::InitAI( le::BasicPersonages* Personage )
{
	InitViewAI( Personage );
}

//-------------------------------------------------------------------------//
