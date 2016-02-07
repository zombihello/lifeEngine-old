#include "BasicEntity.h"

le::BasicEntity::BasicEntity( System & System )
{
    RenderWindow = &System.GetWindow();
    fTime = &System.GetConfiguration().fTime;

    WeaponManager = NULL;
}

int& le::BasicEntity::GetHealtch()
{
    return iHealtch;
}

int& le::BasicEntity::GetArmor()
{
    return iArmor;
}

bool& le::BasicEntity::GetLife()
{
    return bLife;
}

bool le::BasicEntity::GetTick() const
{
    return bTick;
}

string le::BasicEntity::GetNameEntity() const
{
    return sNameEntity;
}

FloatRect le::BasicEntity::GetRect() const
{
    return Rect;
}

le::BasicEntity::TYPES_ENTITY le::BasicEntity::GetTypeEntity() const
{
    return TypeEntity;
}

le::WeaponManager & le::BasicEntity::GetWeaponManager()
{
    return *WeaponManager;
}

void le::BasicEntity::Option( const string sName , const int Healtch , const int Armor , TYPES_ENTITY Type )
{
    sNameEntity = sName;
    iHealtch = Healtch;
    iArmor = Armor;
    TypeEntity = Type;
    fTimer = 0;
    bLife = true;
    bTick = false;
}
