#ifndef BASIC_HUD
#define BASIC_HUD

////////////////
// LIFEENGINE
///////////////
#include "BasicEntity.h"
#include "WeaponManager.h"
#include "System.h"
#include "TextManager.h"

namespace le
{
    class BasicHUD
    {
    public:
        //////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР
        /////////////////////////////
        BasicHUD( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~BasicHUD();

        //////////////////////////////
        //  INIT HUD | ИНИЦ. ЭНТИТИ
        //////////////////////////////
        void InitHUD( BasicEntity& BasicEntity , WeaponManager& WeaponManager , const string sRouteToFont );

        ////////////////////////////////
        //  UPDATE HUD | ОБНОВИТЬ ХУД
        ///////////////////////////////
        void UpdateHUD( View GameCamera );
    protected:
        ///////////////////////////////////////////////////
        //  INIT ENTITY HUD | ИНИЦ. ХУД ЭНТИТИ
        ///////////////////////////////////////////////////
        virtual void InitEntityHUD() = 0;

        ///////////////////////////////////////////////////
        //  INIT WEAPON HUD | ИНИЦ. ХУД ОРУЖИЯ
        ///////////////////////////////////////////////////
        virtual void InitWeaponHUD() = 0;

        ///////////////////////////////////////////////////
        //  UPDATE ENTITY HUD | ОБНОВИТЬ ХУД ЭНТИТИ
        ///////////////////////////////////////////////////
        virtual void UpdateEntityHUD() = 0;

        ///////////////////////////////////////////////////
        //  UPDATE WEAPON HUD | ОБНОВИТЬ ХУД ОРУЖИЯ
        ///////////////////////////////////////////////////
        virtual void UpdateWeaponHUD() = 0;

        RenderWindow*           RenderWindow;
        TextManager*            TextManager;
        WeaponManager*          WeaponManager;
        BasicEntity*            BasicEntity;
        View                    Hud;
    };
}

#endif //BASIC_HUD