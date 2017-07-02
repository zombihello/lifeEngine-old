#ifndef BASICITEM_H
#define BASICITEM_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "BasicEntity.h"
//#include "BasicPersonages.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    class BasicPersonages;

	//-------------------------------------------------------------------------//
	
    class DLL_API BasicItems : public BasicEntity
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        BasicItems( le::System& System, le::Physic2D& Physic );

        ////////////////
        /// ДЕСТРУКТОР
        ////////////////
       virtual ~BasicItems();

        //////////////////
        /// ДАТЬ ПРЕДМЕТ
        //////////////////
        virtual void GiveItem( BasicPersonages& Personage ) = 0;

		/////////////////////////
		/// ПОЛУЧИТЬ КОЛИЧЕСТВО 
		/////////////////////////
		int GetValue();
    protected:
        ////////////////////////////
        /// ИНИЦИАЛИЗАЦИЯ ПРЕДМЕТА
        ////////////////////////////
        void InitItem( int Value );

        int             iValue;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // BASICITEM_H
