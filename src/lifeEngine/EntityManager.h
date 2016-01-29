#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

///////////////
// LIFEENGINE
//////////////
#include "System.h"
#include "LevelManager.h"
#include "BasicEntity.h"

namespace le
{
    class EntityManager
    {
    public:
        ////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР 
        ////////////////////////////////
        EntityManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~EntityManager();

        ///////////////////////////////////
        // CREATE ENTITY | СОЗДАТЬ ЭНТИТИ
        //////////////////////////////////
        void CreateEntity( BasicEntity* BasicEntity );

        ////////////////////////////////////////////
        // UPDATE ALL ENTITY | ОБНОВИТЬ ВСЕ ЭНТИТИ
        ///////////////////////////////////////////
        void UpdateAllEntity( vector<Object> obj , vector<BasicEntity*> vEntity );

        ////////////////////////////////////////////
        // REMOVE ALL ENTITY | УДАЛИТЬ ВСЕ ЭНТИТИ
        ///////////////////////////////////////////
        void DeleteAllEntity();

        //////////////////////////////////////////////////////
        // GET ENTITY BY NAME | ПОЛУЧИТЬ ЭНТИТИ ПО ЕГО ИМЕНИ  
        // @return *BasicEntity
        /////////////////////////////////////////////////////
        BasicEntity* GetEntity( const string sName );

        ////////////////////////////////////////
        // GET ALL ENTITY| ПОЛУЧИТЬ ВСЕ ЭНТИТИ 
        // @return  vector<BasicEntity*>
        ////////////////////////////////////////
        vector<BasicEntity*> GetAllEntity() const;
    private:
        Console*                         Console;
        vector<BasicEntity*>            vEntity;
    };
}

#endif //ENTITY_MANAGER_H