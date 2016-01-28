#include "PhysicManager.h"

le::PhysicManager::PhysicManager( float& fTime )
{
    this->fTime = &fTime;
}

le::PhysicManager::~PhysicManager()
{
}

void le::PhysicManager::UpdatePhysic( vector<Object> obj , vector<BasicEntity*> vEntity , FloatRect& RectEntity , int num )
{
    if ( num == 0 )
    {
        if ( !bOnGround ) fDy += 0.0005 * *fTime;
        bOnGround = false;
    }

    // Check colision for objects in level
    for ( int i = 0; i < obj.size(); i++ )
    {
        if ( RectEntity.intersects( obj[ i ].rect ) )
        {
            if ( obj[ i ].type == "solid" )
            {
                if ( fDx < 0 && num == 0 )  RectEntity.left = obj[ i ].rect.left + obj[ i ].rect.width;
                if ( fDx > 0 && num == 0 )  RectEntity.left = obj[ i ].rect.left - RectEntity.width;
                if ( fDy < 0 && num == 1 ) { RectEntity.top = obj[ i ].rect.top + RectEntity.height; fDy = 0; }
                if ( fDy > 0 && num == 1 ) { RectEntity.top = obj[ i ].rect.top - RectEntity.height; bOnGround = true; fDy = 0; }
            }
        }
    }

    // Check colision for entity

}

void le::PhysicManager::Option()
{
    fDx = fDy = 0;
    bOnGround = false;
}
