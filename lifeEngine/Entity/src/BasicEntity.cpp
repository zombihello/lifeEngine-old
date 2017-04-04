#include "../BasicEntity.h"
#include "../BasicPersonages.h"

//-------------------------------------------------------------------------//

le::BasicEntity::BasicEntity( le::System &System , le::Physic& Physic )
{
	RenderWindow = &System.GetWindow();
	this->System = &System;
	this->Physic = &Physic;
	EntityBody = NULL;

	BoxDebug.setFillColor( Color( 0 , 0 , 0 , 0 ) );
	BoxDebug.setOutlineColor( Color::Green );
	BoxDebug.setOutlineThickness( 1 );

	AnimationManager2D = new le::AnimationManager2D;
}

//-------------------------------------------------------------------------//

le::BasicEntity::~BasicEntity()
{
	if ( EntityBody != NULL )
	{
		Physic->DestroyBody( EntityBody );
		delete EntityBody;
		EntityBody = NULL;
	}

	delete AnimationManager2D;
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
		EntityRect.left = EntityBody->body->GetPosition().x * 30.f - EntityRect.width / 2;
		EntityRect.top = EntityBody->body->GetPosition().y * 30.f - EntityRect.height / 2;
	}

	return EntityRect;
}

//-------------------------------------------------------------------------//

int &le::BasicEntity::GetTempHealtch()
{
	return iTempHealtch;
}

//-------------------------------------------------------------------------//

bool& le::BasicEntity::GetTick()
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

le::AnimationManager2D& le::BasicEntity::GetAnimationManager2D()
{
	return *AnimationManager2D;
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

void le::BasicEntity::InitEntity( float fX , float fY , int MaxHealtch , string sName )
{
	iTempHealtch = iMaxHealtch = MaxHealtch;

	bTick = false;
	bLife = true;
	bOnGround = false;

	sNameEntity = sName;
	EntityRect = FloatRect( fX , fY , Texture.getSize().x , Texture.getSize().y );
	BoxDebug.setSize( Vector2f( EntityRect.width , EntityRect.height ) );

	Sprite.setOrigin( Texture.getSize().x / 2 , Texture.getSize().y / 2 );
	BoxDebug.setOrigin( Sprite.getOrigin() );
}

//-------------------------------------------------------------------------//

void le::BasicEntity::Render()
{
	AnimationManager2D->UpdateAnimation( System->GetConfiguration().fTime , Sprite );

	if ( EntityRect.width != abs( Sprite.getTextureRect().width ) || EntityRect.height != abs( Sprite.getTextureRect().height ) )
	{
		EntityRect.width = abs( Sprite.getTextureRect().width );
		EntityRect.height = abs( Sprite.getTextureRect().height );

		Sprite.setOrigin( EntityRect.width / 2 , EntityRect.height / 2 );
		BoxDebug.setOrigin( Sprite.getOrigin() );

		EntityBody->SetSize( Vector2f( EntityRect.width , EntityRect.height ) );
		BoxDebug.setSize( Vector2f( EntityRect.width , EntityRect.height ) );
	}

	if ( EntityBody != NULL )
		if ( EntityBody->body != NULL )
		{
			b2Body* BodyTmp = EntityBody->body;
			Sprite.setPosition( BodyTmp->GetPosition().x*30.f , BodyTmp->GetPosition().y*30.f );
			Sprite.setRotation( BodyTmp->GetAngle()*57.29577f );

			BoxDebug.setPosition( Sprite.getPosition() );
			BoxDebug.setRotation( Sprite.getRotation() );
		}

	if ( System->GetConfiguration().bDebug )
		RenderWindow->draw( BoxDebug );

	RenderWindow->draw( Sprite );
}

//-------------------------------------------------------------------------//

void le::BasicEntity::Save()
{

}

//-------------------------------------------------------------------------//

void le::BasicEntity::Load()
{

}

//-------------------------------------------------------------------------//