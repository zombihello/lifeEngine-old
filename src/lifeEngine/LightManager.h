#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

////////////////
// LIFEENGINE
////////////////
#include "System.h"
#include "Light.h"

namespace le
{
    class LightManager
    {
    public:
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        LightManager();

        ///////////////////////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР  
        ///////////////////////////////////////////////////
        LightManager( const int iMapWidth , const int iMapHeight );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~LightManager();

        ///////////////////////////////////////////////////
        // LOAD LIGHT MASK  | ЗАГРУЗИТЬ МАСКУ СВЕТА
        ///////////////////////////////////////////////////
        void LoadMask( const string sRoute , bool Smooth = false );

        ////////////////////////////////
        // CREATE LIGHT | СОЗДАТЬ СВЕТ
        ///////////////////////////////
        void CreateLight( Vector2f PositionLight , const float fRadius , Color ColorLight );

        //////////////////////////////////////////
        // DRAW All LIGHT | НАРИСОВАТЬ ВЕСЬ СВЕТ
        /////////////////////////////////////////
        void DrawAllLight( RenderWindow& RenderWindow );

        //////////////////////////////////////////
        // DELETE ALL LIGHT | УДАЛИТЬ ВЕСЬ СВЕТ
        /////////////////////////////////////////
        void DeleteAllLight();

        //////////////////////////////////////////
        // DELETE LIGHT BY ID | УДАЛИТЬ СВЕТ ПО ID
        /////////////////////////////////////////
        void DeleteLight( const int id );

        ///////////////////////////////////////
        // SET MAP SIZE | ЗАДАТЬ РАЗМЕРЫ КАРТЫ
        //////////////////////////////////////
        void SetMapSize( const int iMapWith , const int iMapHeight );

        ///////////////////////////////////////
        // SET OPENCIY | ЗАДАТЬ ПРОЗРАЧНОСТЬ
        //////////////////////////////////////
        void SetOpenciy( const float alpha );

        ///////////////////////////////////////
        // SET COLOR SHADOW | ЗАДАТЬ ЦВЕТ ТЕНИ
        //////////////////////////////////////
        void SetColorShadow( const float r , const float g , const float b );
    private:
        int                     iMapWidth;
        int                     iMapHeight;
        float                   color[ 3 ];
        RenderTexture           RenderTexture;
        Texture                 Texture;
        vector<Light*>          vLight;
    };
}

#endif //LIGHT_MANAGER_H