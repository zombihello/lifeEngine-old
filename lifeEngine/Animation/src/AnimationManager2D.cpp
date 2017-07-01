#include "../AnimationManager2D.h"

//-------------------------------------------------------------------------//

le::AnimationManager2D::AnimationManager2D()
{

}

//-------------------------------------------------------------------------//

le::AnimationManager2D::~AnimationManager2D()
{
	mAnimList.clear();
}

//-------------------------------------------------------------------------//

void le::AnimationManager2D::LoadAnimation( string sRoute )
{
	TiXmlDocument animFile( sRoute.c_str() );

	if ( !animFile.LoadFile() ) return;

	TiXmlElement *head;
	head = animFile.FirstChildElement( "sprites" );

	TiXmlElement *animElement;
	animElement = head->FirstChildElement( "animation" );
	while ( animElement )
	{
		Animation2D anim;
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

void le::AnimationManager2D::SetAnimation( string sName , bool loop )
{
	sCurrentAnim = sName;

	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == sCurrentAnim )
	{
		mAnimList[ sCurrentAnim ].SetLoop( loop );
		mAnimList[ sCurrentAnim ].IsPlaying() = true;
		return;
	}
}

//-------------------------------------------------------------------------//

void le::AnimationManager2D::Flip( bool bFlip )
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
		mAnimList[ it->first ].GetFlip() = bFlip;
}

//-------------------------------------------------------------------------//

void le::AnimationManager2D::Flip( const string NameAnimation , bool bFlip )
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == NameAnimation )
	{
		mAnimList[ NameAnimation ].GetFlip() = bFlip;
		return;
	}
}

//-------------------------------------------------------------------------//

void le::AnimationManager2D::UpdateAnimation( float Time , sf::Sprite& Sprite )
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == sCurrentAnim )
	{
		mAnimList[ sCurrentAnim ].TickAnimation( Time , Sprite );
		return;
	}
}

//-------------------------------------------------------------------------//

void le::AnimationManager2D::Pause()
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == sCurrentAnim )
	{
		mAnimList[ sCurrentAnim ].IsPlaying() = false;
		return;
	}
}

//-------------------------------------------------------------------------//

void le::AnimationManager2D::Play()
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == sCurrentAnim )
	{
		mAnimList[ sCurrentAnim ].IsPlaying() = true;
		return;
	}
}

//-------------------------------------------------------------------------//

bool le::AnimationManager2D::IsPlaying()
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == sCurrentAnim )
		return mAnimList[ sCurrentAnim ].IsPlaying();

	return false;
}

//-------------------------------------------------------------------------//

bool le::AnimationManager2D::IsFlip()
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == sCurrentAnim )
		return mAnimList[ sCurrentAnim ].GetFlip();

	return false;
}

//-------------------------------------------------------------------------//

bool le::AnimationManager2D::IsFlip( string NameAnimation )
{
	for ( auto it = mAnimList.begin(); it != mAnimList.end(); it++ )
	if ( it->first == NameAnimation )
		return mAnimList[ NameAnimation ].GetFlip();

	return false;
}

//-------------------------------------------------------------------------//

string le::AnimationManager2D::GetNameCurrentAnim()
{
	return sCurrentAnim;
}

//-------------------------------------------------------------------------//
