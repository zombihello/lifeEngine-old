#ifndef ANIMATION_H
#define ANIMATION_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////////////
// LIFEENGINE
////////////////////////
#include "../System/System.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    class DLL_API Animation
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        Animation();

        //////////////////
        /// ТИК АНИМАЦИИ
        //////////////////
        void TickAnimation( float time, Sprite& Sprite );

        ////////////////////////
        /// ЗАЦИКЛИТЬ АНИМАЦИЮ
        ////////////////////////
        void SetLoop( bool loop );

        //////////////////////////////
        /// ЗАДАТЬ СКОРОСТЬ АНИМАЦИИ
        /////////////////////////////
        void SetSpeed( float speed );

        ///////////////////////
        /// СКОРОСТЬ АНИМАЦИИ
        ///////////////////////
        float& GetSpeed();

        /////////////////////////////
        /// ПОЛУЧИТЬ КАДРЫ АНИМАЦИИ
        /////////////////////////////
        vector<IntRect>& GetFrames();

        //////////////////////////////////////////
        /// ПОЛУЧИТЬ ОТЗЕРКАЛЕНЫЕ КАДРЫ АНИМАЦИИ
        //////////////////////////////////////////
        vector<IntRect>& GetFramesFlip();

        ///////////////////
        /// ПОЛУЧИТЬ ФЛИП
        //////////////////
        bool& GetFlip();

        ////////////////////////
        /// ИГРАЕТ ЛИ АНИМАЦИЯ
        ///////////////////////
		bool& IsPlaying();
    private:
        float                       fCurrentFrame;
        float                       fSpeed;

        bool                        bLoop;
        bool                        bFlip;
        bool                        bIsPlaying;

        vector<IntRect>             vFrames;
        vector<IntRect>             vFrames_flip;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // ANIMATION_H
