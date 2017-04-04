#include "../Animation2D.h"

//-------------------------------------------------------------------------//

le::Animation2D::Animation2D()
{
	fCurrentFrame = fSpeed = 0;

	bLoop = false;
	bFlip = false;
	bIsPlaying = true;
}

//-------------------------------------------------------------------------//

void le::Animation2D::TickAnimation( float time , sf::Sprite& Sprite )
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

void le::Animation2D::SetLoop( bool loop )
{
	bLoop = loop;
}

//-------------------------------------------------------------------------//

void le::Animation2D::SetSpeed( float speed )
{
	fSpeed = speed;
}

//-------------------------------------------------------------------------//

float &le::Animation2D::GetSpeed()
{
	return fSpeed;
}

//-------------------------------------------------------------------------//

vector<IntRect> &le::Animation2D::GetFrames()
{
	return vFrames;
}

//-------------------------------------------------------------------------//

vector<IntRect> &le::Animation2D::GetFramesFlip()
{
	return vFrames_flip;
}

//-------------------------------------------------------------------------//

bool &le::Animation2D::GetFlip()
{
	return bFlip;
}

//-------------------------------------------------------------------------//

bool &le::Animation2D::IsPlaying()
{
	return bIsPlaying;
}

//-------------------------------------------------------------------------//
