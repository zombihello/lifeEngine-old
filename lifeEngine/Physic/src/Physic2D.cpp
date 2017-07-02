#include "../Physic2D.h"

//-------------------------------------------------------------------------//

le::Physic2D::Physic2D( Gravity2D Gravity )
{
    World = new b2World( b2Vec2( Gravity.fX, Gravity.fY ) );
}

//-------------------------------------------------------------------------//

le::Physic2D::~Physic2D( )
{
    DestroyAllBody();
    delete World;
}

//-------------------------------------------------------------------------//

void le::Physic2D::CreateBody( le::Body2D* Body )
{
    Body->body = World->CreateBody( &Body->bDef );
    vBody.push_back( Body );
}

//-------------------------------------------------------------------------//

void le::Physic2D::DestroyAllBody( )
{
    for ( int i = 0; i < vBody.size(); i++ )
    {
        Body2D* body = vBody[ i ];

        World->DestroyBody( body->body );
        body->body = NULL;
    }

    vBody.clear();
}

//-------------------------------------------------------------------------//

void le::Physic2D::DestroyBody( le::Body2D* Body )
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

void le::Physic2D::UpdatePhysic( float timeStep, int velocityIterations, int positionIterations )
{
    World->Step( timeStep, velocityIterations, positionIterations );
}

//-------------------------------------------------------------------------//

void le::Physic2D::SetGravity( le::Gravity2D Gravity )
{
    World->SetGravity( b2Vec2( Gravity.fX, Gravity.fY  ) );
}

//-------------------------------------------------------------------------//

le::Body2D* le::Physic2D::GetBody( string sNameBody )
{
    for ( int i = 0; i < vBody.size(); i++ )
    {
        Body2D* body = vBody[ i ];

        if ( body->GetNameBody() == sNameBody )
            return body;
    }

    return NULL;
}

//-------------------------------------------------------------------------//

vector<le::Body2D*> le::Physic2D::GetAllBody( )
{
    return vBody;
}

//-------------------------------------------------------------------------//

le::Gravity2D::Gravity2D( float fX, float fY )
{
    this->fX = fX;
    this->fY = fY;
}

//-------------------------------------------------------------------------//
