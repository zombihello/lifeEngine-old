#include "AnimationManager.h"

//-------------------------------------------------------------------------//

le::AnimationManager::AnimationManager()
{

}

//-------------------------------------------------------------------------//

le::AnimationManager::~AnimationManager()
{
    mAnimList.clear();
}

//-------------------------------------------------------------------------//

void le::AnimationManager::LoadAnimation( string sRoute )
{
    TiXmlDocument animFile( sRoute.c_str() );

    if ( !animFile.LoadFile() ) return;

    TiXmlElement *head;
    head = animFile.FirstChildElement( "sprites" );

    TiXmlElement *animElement;
    animElement = head->FirstChildElement( "animation" );
    while ( animElement )
    {
        Animation anim;
        sCurrentAnim = animElement->Attribute( "title" );
        int delay = atoi( animElement->Attribute( "delay" ) );
        anim.SetSpeed( 1.0 / delay );

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

        mAnimList[ sCurrentAnim ] = anim;
        animElement = animElement->NextSiblingElement( "animation" );
    }
}

//-------------------------------------------------------------------------//

void le::AnimationManager::SetAnimation( string sName, bool loop )
{
    sCurrentAnim = sName;

    for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
        if ( it->first == sCurrentAnim )
            mAnimList[ sCurrentAnim ].SetLoop( loop );
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Flip( bool bFlip )
{
    for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
			mAnimList[ it->first ].GetFlip() = bFlip;
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Flip( const string NameAnimation, bool bFlip )
{
    for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
        if ( it->first == NameAnimation )
            mAnimList[ NameAnimation ].GetFlip() = bFlip;
}

//-------------------------------------------------------------------------//

void le::AnimationManager::UpdateAnimation( float Time, sf::Sprite& Sprite )
{
    for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
        if ( it->first == sCurrentAnim )
			mAnimList[ sCurrentAnim ].TickAnimation( Time, Sprite );
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Pause()
{
    for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
        if ( it->first == sCurrentAnim )
            mAnimList[ sCurrentAnim ].isPlaying() = false;
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Play()
{
    for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
        if ( it->first == sCurrentAnim )
            mAnimList[ sCurrentAnim ].isPlaying() = true;
}

//-------------------------------------------------------------------------//

bool le::AnimationManager::isPlaying()
{
    for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
        if ( it->first == sCurrentAnim )
            return mAnimList[ sCurrentAnim ].isPlaying();
}

//-------------------------------------------------------------------------//
