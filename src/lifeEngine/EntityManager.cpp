#include "EntityManager.h"



le::EntityManager::EntityManager()
{
}


le::EntityManager::~EntityManager()
{
    DeleteAllEntity();
}

void le::EntityManager::CreateEntity( BasicEntity * BasicEntity )
{
    vEntity.push_back( BasicEntity );
}

void le::EntityManager::UpdateAllEntity( vector<Object> obj , View Camera )
{
    for ( int i = 0; i < this->vEntity.size(); )
    {
        BasicEntity* BasicEntity = this->vEntity[ i ];

        if ( FloatRect( Camera.getCenter().x - Camera.getSize().x / 2 , Camera.getCenter().y - Camera.getSize().y / 2 , Camera.getSize().x , Camera.getSize().y ).intersects( BasicEntity->GetRect() ) )
            BasicEntity->UpdateEntity( obj , vEntity );

        if ( !BasicEntity->GetLife() )
        {
            this->vEntity.erase( this->vEntity.begin() + i );
            delete BasicEntity;
        }
        else i++;
    }
}

void le::EntityManager::DeleteAllEntity()
{
    for ( int i = 0; i < vEntity.size(); i++ )
        delete vEntity[ i ];

    vEntity.clear();
}

le::BasicEntity * le::EntityManager::GetEntity( const string sName )
{
    for ( int i = 0; i < vEntity.size(); i++ )
        if ( vEntity[ i ]->GetNameEntity() == sName )
            return vEntity[ i ];

    return NULL;
}

vector<le::BasicEntity*> le::EntityManager::GetAllEntity() const
{
    return vEntity;
}
