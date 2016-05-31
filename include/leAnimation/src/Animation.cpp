#include "Animation.h"

//-------------------------------------------------------------------------//

le::Animation::Animation()
{
    fCurrentFrame = fSpeed = 0;

    bLoop = false;
    bFlip = false;
    bIsPlaying = true;
}

//-------------------------------------------------------------------------//

void le::Animation::TickAnimation( float time, sf::Sprite& Sprite )
{
    if ( !bIsPlaying )
        return;

    fCurrentFrame += fSpeed * time;

    if ( fCurrentFrame > vFrames.size() )
    {
        fCurrentFrame = 0;

        if ( !bLoop )
        {
            bIsPlaying = false;
            return;
        }
    }

    int i = fCurrentFrame;
    Sprite.setTextureRect( vFrames[ i ] );

    if ( bFlip )
        Sprite.setTextureRect( vFrames_flip[ i ] );
}

//-------------------------------------------------------------------------//

void le::Animation::SetLoop( bool loop )
{
    bLoop = loop;
}

//-------------------------------------------------------------------------//

void le::Animation::SetSpeed( float speed )
{
    fSpeed = speed;
}

//-------------------------------------------------------------------------//

float &le::Animation::GetSpeed()
{
    return fSpeed;
}

//-------------------------------------------------------------------------//

vector<IntRect> &le::Animation::GetFrames()
{
    return vFrames;
}

//-------------------------------------------------------------------------//

vector<IntRect> &le::Animation::GetFramesFlip()
{
    return vFrames_flip;
}

//-------------------------------------------------------------------------//

bool &le::Animation::GetFlip()
{
    return bFlip;
}

//-------------------------------------------------------------------------//

bool &le::Animation::IsPlaying()
{
    return bIsPlaying;
}

//-------------------------------------------------------------------------//
