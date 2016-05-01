#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../leSystem/leSystem.h"
#include "BasicEntity.h"
#include "BasicBullet.h"
#include "BasicItems.h"
#include "BasicPersonages.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    class DLL_API EntityManager
    {
    public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		EntityManager( System& System );

        ////////////////
        /// ДЕСТРУКТОР
        ////////////////
        ~EntityManager();

        ////////////////////
        /// СОЗДАТЬ ЭНТИТИ
        ///////////////////
        void CreateEntity( BasicEntity* BasicEntity );

        ////////////////////
        /// СОЗДАТЬ ПУЛЮ
        ///////////////////
        void CreateBullet( BasicBullet* BasicBullet );

        /////////////////////
        /// СОЗДАТЬ ПРЕДМЕТ
        ////////////////////
        void CreateItem( BasicItems* BasicItems );

        ///////////////////////
        /// СОЗДАТЬ ПЕРСОНАЖА
        //////////////////////
        void CreatePersonage( BasicPersonages* BasicPersonages );

        ///////////////////////////////////////////////////
        /// ОБНОВИТЬ ЭНТИТИ ( ПРЕДМЕТЫ, ПЕРСОНАЖИ И Т.Д )
        //////////////////////////////////////////////////
        void UpdateEntity();

        ///////////////////////////////////////////////////
        /// УДАЛИТЬ ЭНТИТИ ( ПРЕДМЕТЫ, ПЕРСОНАЖИ И Т.Д )
        //////////////////////////////////////////////////
        void DeleteAll();

        ////////////////////////
        /// УДАЛИТЬ ВСЕ ЭНТИТИ
        ///////////////////////
        void DeleteAllEntity();

        ////////////////////////
        /// УДАЛИТЬ ВСЕ ПУЛИ
        ///////////////////////
        void DeleteAllBullets();

        //////////////////////////
        /// УДАЛИТЬ ВСЕ ПРЕДМЕТЫ
        /////////////////////////
        void DeleteAllItems();

        ////////////////////////////
        /// УДАЛИТЬ ВСЕХ ПЕРСОНАЖЕЙ
        ////////////////////////////
        void DeleteAllPersonages();

        ////////////////////
        /// ПОЛУЧИТЬ ЭНТИТИ
        ////////////////////
        BasicEntity* GetEntity( const string sName );

        ////////////////////
        /// ПОЛУЧИТЬ ПУЛЮ
        ////////////////////
        BasicBullet* GetBullet( const string sName );

        /////////////////////
        /// ПОЛУЧИТЬ ПРЕДМЕТ
        /////////////////////
        BasicItems* GetItem( const string sName );

        ///////////////////////
        /// ПОЛУЧИТЬ ПЕРСОНАЖА
        ///////////////////////
        BasicPersonages* GetPersonage( const string sName );

        ////////////////////////
        /// ПОЛУЧИТЬ ВСЕ ЭНТИТИ
        ///////////////////////
        vector<BasicEntity*>& GetAllEntity();

        ////////////////////////
        /// ПОЛУЧИТЬ ВСЕ ПУЛИ
        ///////////////////////
        vector<BasicBullet*>& GetAllBullets();

        ////////////////////////////
        /// ПОЛУЧИТЬ ВСЕ ПРЕДМЕТЫ
        ////////////////////////////
        vector<BasicItems*>& GetAllItems();

        ////////////////////////////
        /// ПОЛУЧИТЬ ВСЕХ ПЕРСОНАЖЕЙ
        ////////////////////////////
        vector<BasicPersonages*>& GetAllPersonages();
    private:
		View*							Camera;

        vector<BasicEntity*>            vEntity;
        vector<BasicBullet*>            vBullet;
        vector<BasicItems*>             vItems;
        vector<BasicPersonages*>        vPersonages;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // ENTITYMANAGER_H
