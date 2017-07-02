#include "../Body2D.h"
#include "../Physic2D.h"

//-------------------------------------------------------------------------//

le::Body2D::Body2D( Physic2D& Physic, Vector2f Position, string NameBody, TYPE_BODY TypeBody )
{
	Filter = new b2Filter;

	bDef.position.Set( Position.x/30.f, Position.y/30.f );
	bDef.type = (b2BodyType) TypeBody;

	fDensity = 1.f;
	fRestitution = 0.f;
	fFriction = 0.2f;
	sNameBody = NameBody;
	bMoveLeft = false;
	Physic.CreateBody( this );
}

//-------------------------------------------------------------------------//

le::Body2D::Body2D( Physic2D& Physic, Vector2f Position, float Angle, string NameBody, TYPE_BODY TypeBody )
{
	Filter = new b2Filter;

	bDef.position.Set( Position.x/30.f, Position.y/30.f );
	bDef.angle = Angle*57.29577f;
	bDef.type = (b2BodyType) TypeBody;

	fDensity = 1.f;
	fRestitution = 0.f;
	fFriction = 0.2f;
	sNameBody = NameBody;
	bMoveLeft = false;
	Physic.CreateBody( this );
}

//-------------------------------------------------------------------------//

le::Body2D::~Body2D( )
{
	for ( int i = 0; i < vShape.size(); i++ )
		delete vShape[ i ];

	for ( int i = 0; i < vCircle.size(); i++ )
		delete vCircle[ i ];

	delete Filter;

	vShape.clear();
	vCircle.clear();
}

//-------------------------------------------------------------------------//

void le::Body2D::CreateCircleShape( float Radius )
{
	if ( body != NULL )
	{
		CircleShape2D* Circle = new CircleShape2D( Radius );
		Circle->SetPropirtes( fDensity, fRestitution, fFriction );
		Circle->SetFixture( body->CreateFixture( &Circle->GetFixtureDef() ) );
		body->GetFixtureList()->SetFilterData( *Filter );
		vCircle.push_back( Circle );
	}
}

//-------------------------------------------------------------------------//

void le::Body2D::CreateCircleShape( float Radius, Vector2f Center )
{
	if ( body != NULL )
	{
		CircleShape2D* Circle = new CircleShape2D( Radius, Center );
		Circle->SetPropirtes( fDensity, fRestitution, fFriction );
		Circle->SetFixture( body->CreateFixture( &Circle->GetFixtureDef() ) );
		body->GetFixtureList()->SetFilterData( *Filter );
		vCircle.push_back( Circle );
	}
}

//-------------------------------------------------------------------------//

void le::Body2D::CreatePolygonShape( Vector2f SizeBody )
{
	if ( body != NULL )
	{
		PolygonShape2D* Shape = new PolygonShape2D( SizeBody );
		Shape->SetPropirtes( fDensity, fRestitution, fFriction );
		Shape->SetFixture( body->CreateFixture( &Shape->GetFixtureDef() ) );
		body->GetFixtureList()->SetFilterData( *Filter );
		vShape.push_back( Shape );
	}
}

//-------------------------------------------------------------------------//

void le::Body2D::CreatePolygonShape( Vector2f SizeBody, Vector2f Center, float Angle )
{
	if ( body != NULL )
	{
		PolygonShape2D* Shape = new PolygonShape2D( SizeBody, Center, Angle );
		Shape->SetPropirtes( fDensity, fRestitution, fFriction );
		Shape->SetFixture( body->CreateFixture( &Shape->GetFixtureDef() ) );
		body->GetFixtureList()->SetFilterData( *Filter );
		vShape.push_back( Shape );
	}
}

//-------------------------------------------------------------------------//

void le::Body2D::MoveBody( Vector2f Factor, TYPE_MOVE TypeMove )
{
	if (  TypeMove == FORCE )
		body->ApplyForceToCenter( b2Vec2( Factor.x, Factor.y ), true );
	else if ( TypeMove == LINEAR_IMPULSE )
		body->ApplyLinearImpulse( b2Vec2( Factor.x, Factor.y ), b2Vec2( 0,0 ), true );
	else if ( TypeMove == LINEAR_VELOCITY )
	{
		if ( Factor.x == 0 ) Factor.x = GetSpeedBody().x;
		if ( Factor.y == 0 ) Factor.y = GetSpeedBody().y;

		body->SetLinearVelocity( b2Vec2( Factor.x , Factor.y ) );
	}
}

//-------------------------------------------------------------------------//

void le::Body2D::SetSize( Vector2f SizeBody )
{
	for ( int i = 0; i < vShape.size(); i++ )
	{
		PolygonShape2D* Shape = vShape[ i ];
		body->DestroyFixture( &Shape->GetFixture() );
		Shape->SetSize( SizeBody );
		Shape->SetFixture( body->CreateFixture( &Shape->GetFixtureDef() ) );
	}

	for ( int i = 0; i < vCircle.size(); i++ )
	{
		CircleShape2D* Circle = vCircle[ i ];
		body->DestroyFixture( &Circle->GetFixture() );
		Circle->SetRadius( SizeBody.x/2 );
		Circle->SetFixture( body->CreateFixture( &Circle->GetFixtureDef() ) );
	}

	body->GetFixtureList()->SetFilterData( *Filter );
}

//-------------------------------------------------------------------------//

void le::Body2D::SetPropirtes( float Density, float Restitution, float Friction )
{
	for ( int i = 0; i < vShape.size(); i++)
	{
		PolygonShape2D* Shape = vShape[ i ];
		Shape->SetPropirtes( Density, Restitution, Friction );
	}

	for ( int i = 0; i < vCircle.size(); i++)
	{
		CircleShape2D* Circle = vCircle[ i ];
		Circle->SetPropirtes( Density, Restitution, Friction );
	}

	fDensity = Density;
	fRestitution = Restitution;
	fFriction = Friction;
}

//-------------------------------------------------------------------------//

void le::Body2D::SetPosition( Vector2f Position )
{
	body->SetType( b2BodyType::b2_staticBody );

	body->SetTransform( b2Vec2(Position.x/30.f,Position.y/30.f), bDef.angle );

	if ( bDef.type == b2_dynamicBody )
		body->SetType( b2BodyType::b2_dynamicBody );
}

//-------------------------------------------------------------------------//

void le::Body2D::SetGroupBody( int NumberGroup )
{
	Filter->groupIndex = NumberGroup;

	body->GetFixtureList()->SetFilterData( *Filter );
}

//-------------------------------------------------------------------------//

void le::Body2D::SetCategorieCollisions( uint16 Category )
{
	Filter->categoryBits = Category;

	body->GetFixtureList()->SetFilterData( *Filter );
}

//-------------------------------------------------------------------------//

void le::Body2D::SetIgnoreCategoryCollisions( uint16 Category )
{
	Filter->maskBits = ~Category;

	body->GetFixtureList()->SetFilterData( *Filter );
}

//-------------------------------------------------------------------------//

void le::Body2D::SetFixedRotation( bool FixedRotation )
{
	bDef.fixedRotation = FixedRotation;
}

//-------------------------------------------------------------------------//

void le::Body2D::SetSpeedBody( Vector2f Speed )
{
	body->SetLinearVelocity( b2Vec2( Speed.x, Speed.y ) );
}

//-------------------------------------------------------------------------//

bool le::Body2D::IsBodyColision( le::Body2D* Body )
{
	for ( auto ContactEdgeBody1 = body->GetContactList(); ContactEdgeBody1; ContactEdgeBody1 = ContactEdgeBody1->next )
		for ( auto ContactEdgeBody2 = Body->body->GetContactList(); ContactEdgeBody2; ContactEdgeBody2 = ContactEdgeBody2->next )
		{
			if ( ContactEdgeBody1->contact == ContactEdgeBody2->contact )
				return true;
		}

	return false;
}

//-------------------------------------------------------------------------//

bool le::Body2D::IsColision( )
{
	auto ContactEdgeBody = body->GetContactList();

	if ( ContactEdgeBody )
		return true;

	return false;
}

//-------------------------------------------------------------------------//

bool le::Body2D::IsOnGround( Vector2f SizeBody )
{
	b2Vec2 Position = body->GetPosition();
	Position.y += (SizeBody.y/2+1) / 30.f;

	for ( b2Body* it = body->GetWorld()->GetBodyList(); it != 0; it = it->GetNext()  )
		for ( b2Fixture* fixture = it->GetFixtureList(); fixture != 0; fixture = fixture->GetNext() )
			if ( fixture->TestPoint( Position ) )
				return true;

	return false;
}

//-------------------------------------------------------------------------//

uint16 le::Body2D::GetCategorieCollisions( )
{
	return Filter->categoryBits;
}

//-------------------------------------------------------------------------//

string le::Body2D::GetNameBody( )
{
	return sNameBody;
}

//-------------------------------------------------------------------------//

Vector2f le::Body2D::GetSpeedBody( )
{
	return Vector2f( body->GetLinearVelocity().x, body->GetLinearVelocity().y );
}

//-------------------------------------------------------------------------//

Vector2f le::Body2D::GetPositionBody( )
{
	return Vector2f( body->GetPosition().x * 30.f, body->GetPosition().y * 30.f );
}

//-------------------------------------------------------------------------//