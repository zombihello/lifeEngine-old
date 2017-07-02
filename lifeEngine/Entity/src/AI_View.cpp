#include "../AI_View.h"

//-------------------------------------------------------------------------//

le::AI_View::AI_View( le::System& System )
{
	this->System = &System;

	PersonageAI = NULL;
	LookPersonage = NULL;
	Body = NULL;

	bIsLook = false;
	iIdPersonage = 0;
}

//-------------------------------------------------------------------------//

le::AI_View::~AI_View()
{
	if ( Body != NULL )
	{
		PersonageAI->GetPhysic().DestroyBody( Body );
		delete Body;
		Body = NULL;
	}
}

//-------------------------------------------------------------------------//

void le::AI_View::UpdateViewAI( vector<BasicPersonages*> vPersonage )
{
	if ( System->GetConfiguration().bDebug )
		DrawView();

	Body->MoveBody( Vector2f( 0, -10 * Body->body->GetMass() ), le::Body2D::FORCE );

	if ( iIdPersonage < vPersonage.size() )
	{
		BasicPersonages* Enemy = vPersonage[iIdPersonage];

		if ( PersonageAI->IsEnemy( Enemy->GetNameEntity() ) )
		{
			if ( Body->body->GetPosition().x*30.f < Enemy->GetRect().left )
				Body->MoveBody( Vector2f( 0.1, 0 ), le::Body2D::FORCE );
			else
				Body->MoveBody( Vector2f( -0.1, 0 ), le::Body2D::FORCE );

			if ( Body->body->GetPosition().y*30.f < Enemy->GetRect().top )
				Body->MoveBody( Vector2f( 0, 0.1 ), le::Body2D::FORCE );
			else
				Body->MoveBody( Vector2f( 0, -0.1 ), le::Body2D::FORCE );

			if ( Body->IsBodyColision( &Enemy->GetBody() ) )
			{
				Body->SetPosition( Vector2f( PersonageAI->GetRect().left, PersonageAI->GetRect().top ) );

				LookPersonage = Enemy;
				bIsLook = true;

				return;
			}
		}
		else
		{
			iIdPersonage++;

			if ( LookPersonage != NULL )
			{
				LookPersonage = NULL;
				bIsLook = false;
			}
		}
	}
	else
	{
		iIdPersonage = 0;

		if ( LookPersonage != NULL )
		{
			LookPersonage = NULL;
			bIsLook = false;
		}
	}

	if ( Body->IsColision() )
	{
		Body->SetPosition( Vector2f( PersonageAI->GetRect().left, PersonageAI->GetRect().top ) );
		LookPersonage = NULL;
		bIsLook = false;

		iIdPersonage++;
	}
}

//-------------------------------------------------------------------------//

void le::AI_View::InitViewAI( le::BasicPersonages *Personage, le::GroupColision GroupColision )
{
	this->PersonageAI = Personage;

	Body = new le::Body2D( Personage->GetPhysic( ), Vector2f( Personage->GetRect( ).left, Personage->GetRect( ).top ), Personage->GetNameEntity( ) + "_view", le::Body2D::DINAMIC );


	Body->CreatePolygonShape( Vector2f( 2, 2 ) );

	Body->SetCategorieCollisions( GroupColision.ColisionGroup );
	Body->SetIgnoreCategoryCollisions( GroupColision.ColisionGroup | GroupColision.IgnoreGroup );

	View.setFillColor( Color::White );
	View.setSize( Vector2f( 2, 2 ) );
}

//-------------------------------------------------------------------------//

bool le::AI_View::IsLook()
{
	return bIsLook;
}

//-------------------------------------------------------------------------//

le::BasicPersonages *le::AI_View::GetLookPersonage()
{
	return LookPersonage;
}

//-------------------------------------------------------------------------//

void le::AI_View::DrawView()
{
	View.setPosition( Vector2f( Body->body->GetPosition().x*30.f, Body->body->GetPosition().y*30.f ) );
	System->GetWindow().draw( View );
}

//-------------------------------------------------------------------------//
