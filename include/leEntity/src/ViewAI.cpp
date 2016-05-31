#include "../ViewAI.h"

//-------------------------------------------------------------------------//

le::ViewAI::ViewAI( le::System& System )
{
	Window = &System.GetWindow();

	PersonageAI = NULL;
	LookPersonage = NULL;
	Body = NULL;

	bIsLook = bDebug = false;
	iIdPersonage = 0;
}

//-------------------------------------------------------------------------//

le::ViewAI::~ViewAI()
{
	if ( Body != NULL )
	{
		PersonageAI->GetPhysic().DestroyBody( Body );
		delete Body;
		Body = NULL;
	}
}

//-------------------------------------------------------------------------//

void le::ViewAI::UpdateViewAI( vector<BasicPersonages*> vPersonage )
{
	if ( bDebug )
		DrawView();

	if ( iIdPersonage < vPersonage.size() )
	{
		BasicPersonages* Enemy = vPersonage[ iIdPersonage ];

		if ( PersonageAI->IsEnemy( Enemy->GetNameEntity() ) )
		{
			if ( Body->body->GetPosition().x*30.f < Enemy->GetRect().left )
				Body->MoveBody( Vector2f( 0.1,0 ), le::Body::FORCE );
			else
				Body->MoveBody( Vector2f( -0.1,0 ), le::Body::FORCE );

			if ( Body->body->GetPosition().y*30.f < Enemy->GetRect().top )
				Body->MoveBody( Vector2f( 0,0.1 ), le::Body::FORCE );
			else
				Body->MoveBody( Vector2f( 0,-0.1 ), le::Body::FORCE );

			if ( Body->IsBodyColision( &Enemy->GetBody() ) )
			{
				Body->SetPosition( Vector2f( PersonageAI->GetRect().left, PersonageAI->GetRect().top ) );

				LookPersonage = Enemy;
				bIsLook = true;

				return;
			}
		}
		else
			iIdPersonage++;
	}
	else
		iIdPersonage = 0;

	if ( Body->IsColision() )
	{
		Body->SetPosition( Vector2f( PersonageAI->GetRect().left, PersonageAI->GetRect().top ) );
		LookPersonage = NULL;
		bIsLook = false;

		iIdPersonage++;
	}
}

//-------------------------------------------------------------------------//

void le::ViewAI::InitViewAI( le::BasicPersonages *Personage, le::GroupColision GroupColision )
{
	this->PersonageAI = Personage;

	Body = new le::Body( Vector2f( Personage->GetRect().left, Personage->GetRect().top ), Personage->GetNameEntity()+"_view", le::Body::DINAMIC );
	Personage->GetPhysic().CreateBody( Body );

	Body->CreatePolygonShape( Vector2f( 2,2 ) );

	Body->SetPropirtes( 1, 0, 0 );
	Body->SetCategorieCollisions( GroupColision.ColisionGroup );
	Body->SetIgnoreCategoryCollisions( GroupColision.ColisionGroup | GroupColision.IgnoreGroup );

	View.setFillColor( Color::White );
	View.setSize( Vector2f( 2,2 ) );
}

//-------------------------------------------------------------------------//

void le::ViewAI::SetDebug( bool debug )
{
	bDebug = debug;
}

//-------------------------------------------------------------------------//

bool le::ViewAI::IsLook()
{
	return bIsLook;
}

//-------------------------------------------------------------------------//

le::BasicPersonages *le::ViewAI::GetLookPersonage()
{
	return LookPersonage;
}

//-------------------------------------------------------------------------//

void le::ViewAI::DrawView()
{
	View.setPosition( Vector2f( Body->body->GetPosition().x*30.f, Body->body->GetPosition().y*30.f ) );
	Window->draw( View );
}

//-------------------------------------------------------------------------//
