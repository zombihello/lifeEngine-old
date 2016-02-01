#include "AnimationManager.h"


le::AnimationManager::AnimationManager( System & System )
{
    RenderWindow = &System.GetWindow();
    fTime = &System.GetConfiguration().fTime;
    Console = &System.GetConsole();
}

le::AnimationManager::~AnimationManager()
{
    animList.clear();
}

void le::AnimationManager::LoadAnimation( string fileName , Texture & t )
{
    TiXmlDocument animFile( fileName.c_str() );

    if ( !animFile.LoadFile() )
    {
        Console->WriteToConsole( "Error: File [" + fileName + "] Not Found" , Color::Red );
        return;
    }

    TiXmlElement *head;
    head = animFile.FirstChildElement( "sprites" );

    TiXmlElement *animElement;
    animElement = head->FirstChildElement( "animation" );
    while ( animElement )
    {

        Animation anim;
        sCurrentAnim = animElement->Attribute( "title" );
        int delay = atoi( animElement->Attribute( "delay" ) );
        anim.GetSpeed() = 1.0 / delay; anim.GetSprite().setTexture( t );

        TiXmlElement *cut;
        cut = animElement->FirstChildElement( "cut" );
        while ( cut )
        {
            int x = atoi( cut->Attribute( "x" ) );
            int y = atoi( cut->Attribute( "y" ) );
            int w = atoi( cut->Attribute( "w" ) );
            int h = atoi( cut->Attribute( "h" ) );

            anim.GetFrames().push_back( IntRect( x , y , w , h ) );
            anim.GetFramesFlip().push_back( IntRect( x + w , y , -w , h ) );
            cut = cut->NextSiblingElement( "cut" );
        }

        anim.GetSprite().setOrigin( 0 , anim.GetFrames()[ 0 ].height );

        animList[ sCurrentAnim ] = anim;
        animElement = animElement->NextSiblingElement( "animation" );
    }
}

void le::AnimationManager::SetAnimation( String name )
{
    sCurrentAnim = name;
    animList[ sCurrentAnim ].GetFlip() = 0;
}

void le::AnimationManager::DrawAnimation( FloatRect RectObject )
{
    animList[ sCurrentAnim ].GetSprite().setPosition( RectObject.left , RectObject.top + RectObject.height );
    RenderWindow->draw( animList[ sCurrentAnim ].GetSprite() );
}

void le::AnimationManager::Flip( bool bFlip )
{
    animList[ sCurrentAnim ].GetFlip() = bFlip;
}

void le::AnimationManager::TickAnimation()
{
    animList[ sCurrentAnim ].TickAnimation( *fTime );
}

void le::AnimationManager::Pause()
{
    animList[ sCurrentAnim ].isPlaying() = false;
}

void le::AnimationManager::Play()
{
    animList[ sCurrentAnim ].isPlaying() = true;
}

void le::AnimationManager::Play( String name )
{
    animList[ name ].isPlaying() = true;
}

bool le::AnimationManager::isPlaying()
{
    return animList[ sCurrentAnim ].isPlaying();
}

Vector2f le::AnimationManager::GetSize()
{
    return Vector2f( animList[ sCurrentAnim ].GetFrames()[ 0 ].width , animList[ sCurrentAnim ].GetFrames()[ 0 ].height );
}

le::Animation::Animation()
{
    fCurrentFrame = 0;
    bIsPlaying = true;
    bFlip = false;
}

le::Animation::~Animation()
{
}

void le::Animation::TickAnimation( float time )
{
    if ( !bIsPlaying ) return;

    fCurrentFrame += fSpeed * time;

    if ( fCurrentFrame > vframes.size() )
    {
        fCurrentFrame = 0;//-= frames.size();

        if ( !bLoop )
        {
            bIsPlaying = false;
            return;
        }
    }

    int i = fCurrentFrame;
    Sprite.setTextureRect( vframes[ i ] );
    if ( bFlip ) Sprite.setTextureRect( vframes_flip[ i ] );
}

Sprite& le::Animation::GetSprite()
{
    return Sprite;
}

float & le::Animation::GetSpeed()
{
    return fSpeed;
}

vector<IntRect>& le::Animation::GetFrames()
{
    return vframes;
}

vector<IntRect>& le::Animation::GetFramesFlip()
{
    return vframes_flip;
}

bool & le::Animation::GetFlip()
{
    return bFlip;
}

bool& le::Animation::isPlaying()
{
    return bIsPlaying;
}
