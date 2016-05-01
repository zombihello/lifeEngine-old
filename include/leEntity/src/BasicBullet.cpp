#include "BasicBullet.h"

//-------------------------------------------------------------------------//

le::BasicBullet::BasicBullet( le::System &System, le::Physic &Physic ) : BasicEntity( System, Physic )
{

}

//-------------------------------------------------------------------------//

le::BasicBullet::~BasicBullet()
{
}

//-------------------------------------------------------------------------//

int le::BasicBullet::GetDamage()
{
    return iDamage;
}

//-------------------------------------------------------------------------//

void le::BasicBullet::InitBullet( int Damage )
{
    iDamage = Damage;
}

//-------------------------------------------------------------------------//
