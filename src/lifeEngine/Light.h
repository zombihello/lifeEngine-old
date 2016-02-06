#ifndef LIGHT_H
#define LIGHT_H

///////////////
// LIFEENGINE
//////////////
#include "System.h"

namespace le
{
    class Light
    {
    public:
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        Light();

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~Light();

        ///////////////////////////////////////////////////
        // LOAD LIGHT MASK  | ЗАГРУЗИТЬ МАСКУ СВЕТА
        ///////////////////////////////////////////////////
        void SetMask( Texture& Texture );

        ////////////////////////////////
        // CREATE LIGHT | СОЗДАТЬ СВЕТ
        ///////////////////////////////
        void CreateLight( Vector2f PositionLight , const float fRadius , Color ColorLight );

        ////////////////////////////////
        // GET LIGHT | ПОЛУЧИТЬ СВЕТ
        // @return CircleShape
        ///////////////////////////////
        CircleShape GetLight() const;

        //////////////////////////////////////////////
        // GET RECT | ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК СВЕТА
        // @return Rect ( FloatRect )
        /////////////////////////////////////////////
        FloatRect GetRect() const;
    private:
        Texture                 Texture;
        CircleShape             CircleShape;
        FloatRect               Rect;
    };
}

#endif //LIGHT_H