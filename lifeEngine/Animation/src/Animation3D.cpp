#include "..\Animation3D.h"
#include "..\..\System\3D\Skeleton.h"

//-------------------------------------------------------------------------//

inline float lerpFrames( float tmpTime, float frame, float Y1, float Y2 )
{
	return Y1 + ( ( Y2 - Y1 ) / ( frame - ( frame - 1 ) ) ) * ( tmpTime - ( frame - 1 ) );
}

//-------------------------------------------------------------------------//

le::Animation3D::Animation3D()
{
	iNextFrame = 1;
	fNowFrame = 0;
	fMaxFrames = 0;
	fFps = 30;
	fDeltaFrame = ( iNextFrame - fNowFrame ) / fFps;
	bIsLoop = bIsPlaying = false;
}

//-------------------------------------------------------------------------//

le::Animation3D::Animation3D( le::Skeleton& Skeleton, float MaxFrames )
{
	iNextFrame = 1;
	fNowFrame = 0;
	fMaxFrames = MaxFrames;
	fFps = 30;
	fDeltaFrame = ( iNextFrame - fNowFrame ) / fFps;
	bIsLoop = bIsPlaying = false;

	this->Skeleton = &Skeleton;
}

//-------------------------------------------------------------------------//

le::Animation3D::~Animation3D()
{
	mAnimationBone.clear();
}

//-------------------------------------------------------------------------//

void le::Animation3D::Play( bool isLoop )
{
	bIsPlaying = true;
	bIsLoop = isLoop;
}

//-------------------------------------------------------------------------//

void le::Animation3D::Pause()
{
	bIsPlaying = false;
}

//-------------------------------------------------------------------------//

void le::Animation3D::Stop()
{
	bIsPlaying = false;

	iNextFrame = 1;
	fNowFrame = 0;
}

//-------------------------------------------------------------------------//

void le::Animation3D::UpdateAnimation()
{
	if ( fNowFrame >= fMaxFrames )
	{
		fNowFrame = 0;
		iNextFrame = 1;
	}

	glm::mat4x4 Interpolated;
	glm::mat4x4 NowFrame;
	glm::mat4x4 NextFrame;

	for ( auto it = mAnimationBone.begin(); it != mAnimationBone.end(); it++ )
	{
		if ( iNextFrame < it->second.vFrames.size() )
		{
			NowFrame = it->second.vFrames[ iNextFrame - 1 ];
			NextFrame = it->second.vFrames[ iNextFrame ];

			Interpolated[ 0 ].x = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 0 ].x, NextFrame[ 0 ].x );
			Interpolated[ 0 ].y = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 0 ].y, NextFrame[ 0 ].y );
			Interpolated[ 0 ].z = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 0 ].z, NextFrame[ 0 ].z );
			Interpolated[ 0 ].w = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 0 ].w, NextFrame[ 0 ].w );

			Interpolated[ 1 ].x = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 1 ].x, NextFrame[ 1 ].x );
			Interpolated[ 1 ].y = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 1 ].y, NextFrame[ 1 ].y );
			Interpolated[ 1 ].z = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 1 ].z, NextFrame[ 1 ].z );
			Interpolated[ 1 ].w = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 1 ].w, NextFrame[ 1 ].w );

			Interpolated[ 2 ].x = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 2 ].x, NextFrame[ 2 ].x );
			Interpolated[ 2 ].y = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 2 ].y, NextFrame[ 2 ].y );
			Interpolated[ 2 ].z = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 2 ].z, NextFrame[ 2 ].z );
			Interpolated[ 2 ].w = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 2 ].w, NextFrame[ 2 ].w );

			Interpolated[ 3 ].x = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 3 ].x, NextFrame[ 3 ].x );
			Interpolated[ 3 ].y = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 3 ].y, NextFrame[ 3 ].y );
			Interpolated[ 3 ].z = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 3 ].z, NextFrame[ 3 ].z );
			Interpolated[ 3 ].w = lerpFrames( fNowFrame, iNextFrame, NowFrame[ 3 ].w, NextFrame[ 3 ].w );

			Skeleton->UpdateMatrixBone( it->first, Interpolated );
		}
	}

	fNowFrame += fDeltaFrame;

	if ( fNowFrame >= iNextFrame )
		iNextFrame++;

	Skeleton->UpdateMesh();
}

//-------------------------------------------------------------------------//

void le::Animation3D::SetFPS( float fps )
{
	fFps = fps;
	fDeltaFrame = ( iNextFrame - fNowFrame ) / fFps;
}

//-------------------------------------------------------------------------//

bool le::Animation3D::LoadAnimation( TiXmlElement* animation )
{
	// Работаем с контейнером animation для опред. кости 
	TiXmlElement *animationBone;
	animationBone = animation->FirstChildElement( "animation" );

	if ( animationBone == NULL ) return false;

	while ( animationBone )
	{
		string sNameBone = animationBone->Attribute( "name" );
		AnimationBone AnimBone;

		// Работаем с контейнером times
		TiXmlElement *times;
		times = animationBone->FirstChildElement( "times" );

		string sTmp = string( times->GetText() );
		stringstream ss( sTmp );

		while ( !ss.eof() )
		{
			sTmp.clear();
			ss >> sTmp;
			AnimBone.vTimes.push_back( atof( sTmp.c_str() ) );
		}

		// Работаем с контейнером matrix
		TiXmlElement *matrix;
		matrix = animationBone->FirstChildElement( "matrix" );

		int tmpMaxFrames = 0;

		while ( matrix )
		{
			if ( matrix->Attribute( "frame" ) != NULL ) // TODO: ИСРПАВИТЬ
			{
				glm::mat4x4 Matrix;
				glm::vec4 tmpVec;
				sTmp = matrix->GetText();

				ss.seekg( 0 );
				ss.str( sTmp );

				for ( int id = 0, axis = 1; id < 4 && !ss.eof(); axis++ )
				{
					string tmp;
					ss >> tmp;

					switch ( axis )
					{
					case 1:
						tmpVec.x = atof( tmp.c_str() );
						break;

					case 2:
						tmpVec.y = atof( tmp.c_str() );
						break;

					case 3:
						tmpVec.z = atof( tmp.c_str() );
						break;

					case 4:
						tmpVec.w = atof( tmp.c_str() );

						Matrix[ id ] = tmpVec;
						axis = 0;
						id++;
						break;
					}
				}

				AnimBone.vFrames.push_back( Matrix );
			}

			matrix = matrix->NextSiblingElement();
		}

		// Работаем с контейнером TypeInterpolation
		TiXmlElement *typeInterpolation;
		typeInterpolation = animationBone->FirstChildElement( "TypeInterpolation" );

		sTmp = typeInterpolation->GetText();
		ss.seekg( 0 );
		ss.str( sTmp );

		while ( !ss.eof() )
		{
			sTmp.clear();
			ss >> sTmp;
			AnimBone.vTypeInterpolation.push_back( sTmp );
		}

		mAnimationBone[ sNameBone ] = AnimBone;

		animationBone = animationBone->NextSiblingElement();
	}

	return true;
}

//-------------------------------------------------------------------------//

bool le::Animation3D::IsPlaying()
{
	return bIsPlaying;
}

//-------------------------------------------------------------------------//

void le::Animation3D::SetSkeleton( le::Skeleton& Skeleton )
{
	this->Skeleton = &Skeleton;
}

//-------------------------------------------------------------------------//