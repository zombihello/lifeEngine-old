#include "EntityManager.h"



le::EntityManager::EntityManager( System& System )
{
    Console = &System.GetConsole();
}


le::EntityManager::~EntityManager()
{
    DeleteAllEntity();
}

void le::EntityManager::CreateEntity( BasicEntity * BasicEntity )
{
    vEntity.push_back( BasicEntity );
}

void le::EntityManager::UpdateAllEntity( vector<Object> obj , vector<BasicEntity*> vEntity )
{
    for ( int i = 0; i < this->vEntity.size(); )
    {
        BasicEntity* BasicEntity = this->vEntity[ i ];

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

    Console->WriteToConsole( "Error: Entity [" + sName + "] Not Found" , Color::Red );
    return NULL;
}

vector<le::BasicEntity*> le::EntityManager::GetAllEntity() const
{
    return vEntity;
}
