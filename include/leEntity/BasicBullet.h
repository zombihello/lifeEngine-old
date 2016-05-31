#ifndef BASICBULLET_H
#define BASICBULLET_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../leSystem/leSystem.h"
#include "BasicEntity.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    class DLL_API BasicBullet : public BasicEntity
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
		BasicBullet( le::System& System, le::Physic& Physic );

        ////////////////
        /// ДЕСТРУКТОР
        ////////////////
       virtual ~BasicBullet();

        ///////////////////
        /// ПОЛУЧИТЬ УРОН
        ///////////////////
        int GetDamage();
    protected:
        void InitBullet( int Damage );

        int             iDamage;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // BASICBULLET_H
