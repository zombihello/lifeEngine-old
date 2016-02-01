#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

/////////////////////////
// LIFEENGINE
////////////////////////
#include "System.h"

namespace le
{
    class Animation
    {
    public:
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        Animation();

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~Animation();

        ////////////////////////////////////
        // TICK ANIMATION | ТИК АНИМАЦИИ
        ///////////////////////////////////
        void TickAnimation( float time );

        ////////////////////////////////////
        // GET SPRITE | ПОЛУЧИТЬ СПРАЙТ
        // @return Sprite
        ///////////////////////////////////
        Sprite& GetSprite();

        //////////////////////////////////////////////////////
        // GET SPEED ANIMATION | ПОЛУЧИТЬ СКОРОСТЬ АНИМАЦИИ
        // @return float
        /////////////////////////////////////////////////////
        float& GetSpeed();

        //////////////////////////////////////////////////////
        // GET FRAMES ANIMATION | ПОЛУЧИТЬ КАДРЫ АНИМАЦИИ
        // @return vector<IntRect>
        /////////////////////////////////////////////////////
        vector<IntRect>& GetFrames();

        /////////////////////////////////////////////////////////////////////
        // GET FRAMES FLIP ANIMATION | ПОЛУЧИТЬ ОТЗЕРКАЛЕНЫЕ КАДРЫ АНИМАЦИИ
        // @return vector<IntRect>
        /////////////////////////////////////////////////////////////////////
        vector<IntRect>& GetFramesFlip();

        //////////////////////////////
        // GET FLIP | ПОЛУЧИТЬ ФЛИП  
        // @return bool
        /////////////////////////////
        bool& GetFlip();

        /////////////////////////////////////////////////////////
        // GET IS PLAYING ANIMATION | ПОЛУЧИТЬ ИГРАЕТ АНИМАЦИЯ
        // @return bool
        //////////////////////////////////////////////////////////
        bool& isPlaying();
    private:
        float                       fCurrentFrame;
        float                       fSpeed;

        bool                        bLoop;
        bool                        bFlip;
        bool                        bIsPlaying;

        vector<IntRect>             vframes;
        vector<IntRect>             vframes_flip;

        Sprite                      Sprite;
    };

    class AnimationManager
    {
    public:
        ////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР  
        ////////////////////////////////
        AnimationManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~AnimationManager();

        ///////////////////////////////////////////////////////
        // LOAD ANIMATION FROM XML | ЗАГРУЗИТЬ АНИМАЦИЮ С XML
        ///////////////////////////////////////////////////////
        void LoadAnimation( string fileName , Texture &t );

        /////////////////////////////////////
        // SET ANIMATION | ЗАДАТЬ  АНИМАЦИЮ 
        /////////////////////////////////////
        void SetAnimation( String name );

        //////////////////////////////////////////
        // DRAW ANIMATION | НАРИСОВАТЬ АНИМАЦИЮ 
        /////////////////////////////////////////
        void DrawAnimation( FloatRect RectObject );

        ///////////////////////////////////////////////
        // MIRROR THE SPRITE | ОТРАЗИТЬ ЗЕРКАЛЬНО СПРАЙТ
        ///////////////////////////////////////////////
        void Flip( bool bFlip = true );

        ////////////////////////////////////
        // TICK ANIMATION | ТИК АНИМАЦИИ
        ///////////////////////////////////
        void TickAnimation();

        ////////////////////////////////////
        //  SET PAUSE | ПОСТАВИТЬ НА ПАУЗУ 
        ///////////////////////////////////
        void Pause();

        ////////////////////////////////////////////////////
        // PLAY CURRENT ANIMATION  | ИГРАТЬ ТЕК. АНИМАЦИЮ   
        ///////////////////////////////////////////////////
        void Play();

        ///////////////////////////////////////
        // PLAY ANIMATION  | ИГРАТЬ АНИМАЦИЮ   
        //////////////////////////////////////
        void Play( String name );

        ///////////////////////////////////////
        // PLAY ANIMATION  | ИГРАТЬ АНИМАЦИЮ   
        // @return true or false
        //////////////////////////////////////
        bool isPlaying();

        //////////////////////////////////////////////
        // GET SIZE SPRITE | ПОЛУЧИТЬ РАЗМЕР СПРАЙТА  
        // @return Vector2f
        //////////////////////////////////////////////
        Vector2f GetSize();
    private:
        float*                          fTime;

        String                          sCurrentAnim;
        map<String , Animation>         animList;
        RenderWindow*                   RenderWindow;
        Console*                        Console;
    };
}

#endif //ANIMATION_MANAGER_H