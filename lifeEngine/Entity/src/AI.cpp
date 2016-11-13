#include "../AI.h"

//-------------------------------------------------------------------------//

le::AI::AI( le::System& System , Level& Level )
{
	ViewAI = new le::AI_View( System );
	mRoutesAI = Level.GetRoutesAI();
}

//-------------------------------------------------------------------------//

le::AI::~AI()
{
	delete ViewAI;
}

//-------------------------------------------------------------------------//

void le::AI::UpdateAI( vector<BasicPersonages*> vPersonage )
{
	ViewAI->UpdateViewAI( vPersonage );
	Actions();
}

//-------------------------------------------------------------------------//

void le::AI::Walk( Vector2f Factor , Body::TYPE_MOVE typeMove )
{
	AI_Route RouteMove = mRoutesAI[ sTmpRouteAI ];

	if ( Personage->GetRect().left < RouteMove.GetRect().left )
	{
		Personage->GetBody().MoveBody( Vector2f( Factor.x , 0 ) , typeMove );
		Personage->GetAnimationManager().Flip( true );
	}
	else
	{
		Personage->GetBody().MoveBody( Vector2f( -Factor.x , 0 ) , typeMove );
		Personage->GetAnimationManager().Flip( false );
	}

	if ( RouteMove.IsUsedY() )
	{
		if ( Personage->GetRect().top < RouteMove.GetRect().top )
			Personage->GetBody().MoveBody( Vector2f( 0 , -Factor.y ) , typeMove );
		else
			Personage->GetBody().MoveBody( Vector2f( 0 , Factor.y ) , typeMove );
	}

	if ( Personage->GetRect().intersects( RouteMove.GetRect() ) )
	{
		sTmpRouteAI = RouteMove.GetNextRoute();
		RouteMove = mRoutesAI[ sTmpRouteAI ];
	}
}

//-------------------------------------------------------------------------//

void le::AI::MoveToObject( Vector2f Factor , Body::TYPE_MOVE typeMove , BasicPersonages *Personage , bool UseY )
{
	FloatRect TempRect = this->Personage->GetRect();

	if ( !UseY )
		TempRect.top = Personage->GetRect().top;

	if ( !TempRect.intersects( Personage->GetRect() ) )
	{
		if ( this->Personage->GetRect().left < Personage->GetRect().left )
		{
			this->Personage->GetBody().MoveBody( Vector2f( Factor.x , 0 ) , typeMove );
			this->Personage->GetAnimationManager().Flip( true );
		}
		else
		{
			this->Personage->GetBody().MoveBody( Vector2f( -Factor.x , 0 ) , typeMove );
			this->Personage->GetAnimationManager().Flip( false );
		}

		if ( UseY )
		{
			if ( this->Personage->GetRect().top < Personage->GetRect().top )
				this->Personage->GetBody().MoveBody( Vector2f( 0 , -Factor.y ) , typeMove );
			else
				this->Personage->GetBody().MoveBody( Vector2f( 0 , Factor.y ) , typeMove );
		}
	}
}

//-------------------------------------------------------------------------//

bool le::AI::IsLook()
{
	return ViewAI->IsLook();
}

//-------------------------------------------------------------------------//

le::BasicPersonages* le::AI::GetLookPersonage()
{
	return ViewAI->GetLookPersonage();
}

//-------------------------------------------------------------------------//

void le::AI::InitAI( le::BasicPersonages* Personage , le::GroupColision GroupColision )
{
	ViewAI->InitViewAI( Personage , GroupColision );
	this->Personage = Personage;
}

//-------------------------------------------------------------------------//

void le::AI::InitAI( BasicPersonages* Personage , string StartRoute , GroupColision GroupColision )
{
	ViewAI->InitViewAI( Personage , GroupColision );
	sTmpRouteAI = StartRoute;
	this->Personage = Personage;
}

//-------------------------------------------------------------------------//
