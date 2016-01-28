#include "BasicEntity.h"

int le::BasicEntity::GetHealtch() const
{
    return iHealtch;
}

int le::BasicEntity::GetArmor() const
{
    return iArmor;
}

bool le::BasicEntity::GetLife() const
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

void le::BasicEntity::Option( const string sName , const int Healtch , const int Armor )
{
    sNameEntity = sName;
    iHealtch = Healtch;
    iArmor = Armor;
    fTimer = 0;
    bLife = true;
    bTick = false;
}
