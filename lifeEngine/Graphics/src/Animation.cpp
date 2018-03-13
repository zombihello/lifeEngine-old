#include "..\Animation.h"
#include <Graphics\Skeleton.h>

#define LERP_FRAME( NowTime, NextFrame, Y1, Y2 ) ( Y1 + ( ( Y2 - Y1 ) / ( NextFrame - ( NextFrame - 1 ) ) ) * ( NowTime - ( NextFrame - 1 ) ) )

//-------------------------------------------------------------------------//

le::Animation::Animation() :
	Skeleton( NULL ),
	NextFrame( 1 ),
	NowFrame( 0 ),
	MaxFrames( 0 ),
	DeltaFrame( ( NextFrame - NowFrame ) / 30.f ),
	Loop( false ),
	Playing( false )
{}

//-------------------------------------------------------------------------//

bool le::Animation::LoadAnimation( TiXmlElement* Animation, int MaxFrames )
{
	this->MaxFrames = MaxFrames;

	// ****************************
	// Загружаем каждую анимацию кости
	// ****************************

	TiXmlElement* AnimationBone;
	AnimationBone = Animation->FirstChildElement( "animation" );

	stringstream StrStream;
	string TempString, NameBone;
	glm::mat4 TempMatrix4;
	glm::vec4 TempVector4;

	while ( AnimationBone )
	{
		Animation::AnimationBone AnimBone;
		NameBone = AnimationBone->Attribute( "name" );

		// ****************************
		// Загружаем матрицы кости на каждый кадр
		// ****************************

		TiXmlElement* Matrix;
		Matrix = AnimationBone->FirstChildElement( "matrix" );

		while ( Matrix )
		{
			StrStream.clear();
			StrStream.str( Matrix->GetText() );

			for ( int id = 0, axis = 1; id < 4 && !StrStream.eof(); axis++ )
			{
				StrStream >> TempString;

				switch ( axis )
				{
				case 1: // x
					TempVector4.x = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
					break;

				case 2: // y
					TempVector4.y = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
					break;

				case 3: // z
					TempVector4.z = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
					break;

				case 4: // w
					TempVector4.w = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );

					TempMatrix4[ id ] = TempVector4;
					axis = 0;
					id++;
					break;
				}

				TempString.clear();
			}

			AnimBone.Frames.push_back( TempMatrix4 );
			Matrix = Matrix->NextSiblingElement();
		}

		// ****************************
		// Проверяем тип интерполяции который поддерживает движок
		// ****************************

		TiXmlElement* TypeInterpolation;
		TypeInterpolation = AnimationBone->FirstChildElement( "TypeInterpolation" );

		if ( TypeInterpolation != NULL )
		{
			StrStream.clear();
			StrStream.str( TypeInterpolation->GetText() );

			while ( !StrStream.eof() )
			{
				StrStream >> TempString;

				if ( TempString != "LINEAR" )
				{
					Logger::Log( Logger::Warning, "Not Supported Type Interpolation In Animation Mesh. May Be Not Correct Animation [Supported Type Interpolation - LINEAR]" );
					break;
				}

				TempString.clear();
			}
		}

		AnimationBones[ NameBone ] = AnimBone;
		AnimationBone = AnimationBone->NextSiblingElement();
	}

	return true;
}

//-------------------------------------------------------------------------//

void le::Animation::Play( bool IsLoop )
{
	Playing = true;
	Loop = IsLoop;
}

//-------------------------------------------------------------------------//

void le::Animation::Pause()
{
	Playing = false;
}

//-------------------------------------------------------------------------//

void le::Animation::Stop()
{
	Playing = false;
	NextFrame = 1;
	NowFrame = 0.f;
}

//-------------------------------------------------------------------------//

void le::Animation::Update()
{
	if ( NowFrame >= MaxFrames )
	{
		if ( Loop )
		{
			NowFrame = 0.f;
			NextFrame = 1;
		}
		else
			Stop();
	}

	for ( auto it = AnimationBones.begin(); it != AnimationBones.end(); it++ )
	{
		if ( ( size_t ) NextFrame < it->second.Frames.size()-1 )
		{
			MatrixNowFrame = &it->second.Frames[ NextFrame - 1 ];
			MatrixNextFrame = &it->second.Frames[ NextFrame ];

			MatrixInterpolated[ 0 ].x = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 0 ].x, ( *MatrixNextFrame )[ 0 ].x );
			MatrixInterpolated[ 0 ].y = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 0 ].y, ( *MatrixNextFrame )[ 0 ].y );
			MatrixInterpolated[ 0 ].z = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 0 ].z, ( *MatrixNextFrame )[ 0 ].z );
			MatrixInterpolated[ 0 ].w = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 0 ].w, ( *MatrixNextFrame )[ 0 ].w );

			MatrixInterpolated[ 1 ].x = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 1 ].x, ( *MatrixNextFrame )[ 1 ].x );
			MatrixInterpolated[ 1 ].y = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 1 ].y, ( *MatrixNextFrame )[ 1 ].y );
			MatrixInterpolated[ 1 ].z = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 1 ].z, ( *MatrixNextFrame )[ 1 ].z );
			MatrixInterpolated[ 1 ].w = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 1 ].w, ( *MatrixNextFrame )[ 1 ].w );

			MatrixInterpolated[ 2 ].x = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 2 ].x, ( *MatrixNextFrame )[ 2 ].x );
			MatrixInterpolated[ 2 ].y = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 2 ].y, ( *MatrixNextFrame )[ 2 ].y );
			MatrixInterpolated[ 2 ].z = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 2 ].z, ( *MatrixNextFrame )[ 2 ].z );
			MatrixInterpolated[ 2 ].w = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 2 ].w, ( *MatrixNextFrame )[ 2 ].w );

			MatrixInterpolated[ 3 ].x = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 3 ].x, ( *MatrixNextFrame )[ 3 ].x );
			MatrixInterpolated[ 3 ].y = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 3 ].y, ( *MatrixNextFrame )[ 3 ].y );
			MatrixInterpolated[ 3 ].z = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 3 ].z, ( *MatrixNextFrame )[ 3 ].z );
			MatrixInterpolated[ 3 ].w = LERP_FRAME( NowFrame, NextFrame, ( *MatrixNowFrame )[ 3 ].w, ( *MatrixNextFrame )[ 3 ].w );

			Skeleton->UpdateMatrixBone( it->first, MatrixInterpolated );
		}
	}

	NowFrame += DeltaFrame * System::Configuration.Time;

	if ( NowFrame >= NextFrame )
		NextFrame++;

	Skeleton->UpdateSkeleton();
}

//-------------------------------------------------------------------------//

void le::Animation::SetSkeleton( le::Skeleton& Skeleton )
{
	this->Skeleton = &Skeleton;
}

//-------------------------------------------------------------------------//

void le::Animation::SetFPS( float FPS )
{
	DeltaFrame = ( NextFrame - NowFrame ) / FPS;
}

//-------------------------------------------------------------------------//

bool le::Animation::IsPlaying()
{
	return Playing;
}

//-------------------------------------------------------------------------//
