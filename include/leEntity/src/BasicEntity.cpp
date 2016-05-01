#include "../BasicEntity.h"
#include "../BasicPersonages.h"

//-------------------------------------------------------------------------//

le::BasicEntity::BasicEntity( le::System &System, le::Physic& Physic )
{
	RenderWindow = &System.GetWindow();
	this->System = &System;
	this->Physic = &Physic;
	EntityBody = NULL;

	AnimationManager = new le::AnimationManager;
}

//-------------------------------------------------------------------------//

le::BasicEntity::~BasicEntity()
{
	if (  EntityBody != NULL )
	{
		Physic->DestroyBody( EntityBody );
		delete EntityBody;
		EntityBody = NULL;
	}

	delete AnimationManager;
}

//-------------------------------------------------------------------------//

void le::BasicEntity::Kill()
{
	bLife = false;
}

//-------------------------------------------------------------------------//

FloatRect le::BasicEntity::GetRect()
{
	if ( EntityBody != NULL )
	{
		EntityRect.left = EntityBody->body->GetPosition().x * 30.f;
		EntityRect.top = EntityBody->body->GetPosition().y * 30.f;
	}

	return EntityRect;
}

//-------------------------------------------------------------------------//

bool le::BasicEntity::GetTick()
{
	return bTick;
}

//-------------------------------------------------------------------------//

bool le::BasicEntity::GetLife()
{
	return bLife;
}

//-------------------------------------------------------------------------//

string le::BasicEntity::GetNameEntity()
{
	return sNameEntity;
}

//-------------------------------------------------------------------------//

le::AnimationManager& le::BasicEntity::GetAnimationManager()
{
	return *AnimationManager;
}

//-------------------------------------------------------------------------//

le::Body& le::BasicEntity::GetBody()
{
	return *EntityBody;
}

//-------------------------------------------------------------------------//

le::Physic& le::BasicEntity::GetPhysic()
{
	return *Physic;
}

//-------------------------------------------------------------------------//

void le::BasicEntity::InitEntity( float fX, float fY, int MaxHealtch, string sName )
{
	iTempHealtch = iMaxHealtch = MaxHealtch;

	bTick = false;
	bLife = true;
	bOnGround = false;

	sNameEntity = sName;
	EntityRect = FloatRect( fX, fY, Texture.getSize().x, Texture.getSize().y );

	Sprite.setOrigin( Texture.getSize().x/2, Texture.getSize().y/2 );
}

//-------------------------------------------------------------------------//

void le::BasicEntity::Render()
{
	AnimationManager->UpdateAnimation( System->GetConfiguration().fTime, Sprite );

	if ( EntityRect.width != Sprite.getTextureRect().width || EntityRect.height != Sprite.getTextureRect().height )
	{
		EntityRect.width = Sprite.getTextureRect().width;
		EntityRect.height = Sprite.getTextureRect().height;
		Sprite.setOrigin( abs( EntityRect.width )/2, abs( EntityRect.height )/2 );

		EntityBody->SetSize( Vector2f( abs( EntityRect.width ), abs( EntityRect.height ) ) );
	}

	if ( EntityBody != NULL )
		if ( EntityBody->body != NULL )
		{
			b2Body* BodyTmp = EntityBody->body;
			Sprite.setPosition( BodyTmp->GetPosition().x*30.f, BodyTmp->GetPosition().y*30.f );
			Sprite.setRotation( BodyTmp->GetAngle()*57.29577f );
		}

	RenderWindow->draw( Sprite );
}

//-------------------------------------------------------------------------//
