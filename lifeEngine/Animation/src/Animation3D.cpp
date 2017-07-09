#include "..\Animation3D.h"
#include "..\..\System\3D\Skeleton.h"

//-------------------------------------------------------------------------//

float lerpFrames( float tmpTime, float frame, float Y1, float Y2 )
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

	for ( auto it = mAnimationBone.begin(); it != mAnimationBone.end(); it++ )
	{
		if ( iNextFrame < it->second.vFrames.size() )
		{
			glm::mat4x4 Matrix;
			glm::vec4 tmpVec;

			for ( int id = 0, axis = 1; id < 4; axis++ )
			{
				switch ( axis )
				{
				case 1:
					tmpVec.x = lerpFrames( fNowFrame, iNextFrame, it->second.vFrames[iNextFrame - 1][id].x, it->second.vFrames[iNextFrame][id].x );
					break;

				case 2:
					tmpVec.y = lerpFrames( fNowFrame, iNextFrame, it->second.vFrames[iNextFrame - 1][id].y, it->second.vFrames[iNextFrame][id].y );
					break;

				case 3:
					tmpVec.z = lerpFrames( fNowFrame, iNextFrame, it->second.vFrames[iNextFrame - 1][id].z, it->second.vFrames[iNextFrame][id].z );
					break;

				case 4:
					tmpVec.w = lerpFrames( fNowFrame, iNextFrame, it->second.vFrames[iNextFrame - 1][id].w, it->second.vFrames[iNextFrame][id].w );
					Matrix[id] = tmpVec;

					axis = 0;
					id++;
					break;
				}
			}

			Skeleton->SetMatrixBone( it->first, Matrix );
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
						tmpVec.x = atof( tmp.c_str( ) );
						break;

					case 2:
						tmpVec.y = atof( tmp.c_str( ) );
						break;

					case 3:
						tmpVec.z = atof( tmp.c_str( ) );
						break;

					case 4:
						tmpVec.w = atof( tmp.c_str( ) );

						Matrix[id] = tmpVec;
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

		mAnimationBone[sNameBone] = AnimBone;

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