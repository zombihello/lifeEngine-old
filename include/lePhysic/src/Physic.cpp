#include "../Physic.h"

//-------------------------------------------------------------------------//

le::Physic::Physic( Gravity Gravity )
{
    World = new b2World( b2Vec2( Gravity.fX, Gravity.fY ) );
}

//-------------------------------------------------------------------------//

le::Physic::~Physic()
{
    DestroyAllBody();
    delete World;
}

//-------------------------------------------------------------------------//

void le::Physic::CreateBody( le::Body* Body )
{
    Body->body = World->CreateBody( &Body->bDef );
    vBody.push_back( Body );
}

//-------------------------------------------------------------------------//

void le::Physic::DestroyAllBody()
{
    for ( int i = 0; i < vBody.size(); i++ )
    {
        Body* body = vBody[ i ];

        World->DestroyBody( body->body );
        body->body = NULL;
    }

    vBody.clear();
}

//-------------------------------------------------------------------------//

void le::Physic::DestroyBody( le::Body* Body )
{
    if ( Body->body != NULL )
        for ( int i = 0; i < vBody.size(); i++ )
            if ( Body == vBody[ i ] )
            {
                vBody.erase( vBody.begin() + i );
                World->DestroyBody( Body->body );
                Body->body = NULL;
                return;
            }
}

//-------------------------------------------------------------------------//

void le::Physic::UpdatePhysic( float timeStep, int velocityIterations, int positionIterations )
{
    World->Step( timeStep, velocityIterations, positionIterations );
}

//-------------------------------------------------------------------------//

void le::Physic::SetGravity( le::Gravity Gravity )
{
    World->SetGravity( b2Vec2( Gravity.fX, Gravity.fY  ) );
}

//-------------------------------------------------------------------------//

le::Body *le::Physic::GetBody( string sNameBody )
{
    for ( int i = 0; i < vBody.size(); i++ )
    {
        Body* body = vBody[ i ];

        if ( body->GetNameBody() == sNameBody )
            return body;
    }

    return NULL;
}

//-------------------------------------------------------------------------//

b2World *le::Physic::GetWorld()
{
    return World;
}

//-------------------------------------------------------------------------//

vector<le::Body *> le::Physic::GetAllBody()
{
    return vBody;
}

//-------------------------------------------------------------------------//

le::Gravity::Gravity( float fX, float fY )
{
    this->fX = fX;
    this->fY = fY;
}

//-------------------------------------------------------------------------//
