#include "..\AnimationManager.h"

//-------------------------------------------------------------------------//

le::AnimationManager::AnimationManager() :
	CurrentAnimation( NULL ),
	Skeleton( NULL )
{}

//-------------------------------------------------------------------------//

bool le::AnimationManager::LoadAnimations( TiXmlElement* Animations )
{
	Logger::Log( Logger::Info, "Loading Animations Mesh" );

	// ****************************
	// Загружаем каждую анимацию
	// ****************************

	TiXmlElement* Animation;
	Animation = Animations->FirstChildElement( "animation" );

	while ( Animation )
	{
		int MaxFrames = NUMBER_TO_INT( atof( Animation->Attribute( "MaxFrames" ) ) );
		float FPS = NUMBER_TO_FLOAT( atof( Animation->Attribute( "FrameRate" ) ) );

		le::Animation Anim;
		Anim.LoadAnimation( Animation, MaxFrames );
		Anim.SetFPS( FPS );

		this->Animations[ Animation->Attribute( "name" ) ] = Anim;
		Animation = Animation->NextSiblingElement();
	}

	Logger::Log( Logger::Info, "Loaded Animations Mesh" );
	return true;
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Play( const string& NameAnimation, bool IsLoop )
{
	if ( Animations.find( NameAnimation ) != Animations.end() )
	{
		CurrentAnimation = &Animations[ NameAnimation ];
		CurrentAnimation->Play( IsLoop );
	}
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Pause()
{
	if ( CurrentAnimation != NULL )
		CurrentAnimation->Pause();
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Stop()
{
	if ( CurrentAnimation != NULL )
		CurrentAnimation->Stop();
}

//-------------------------------------------------------------------------//

void le::AnimationManager::Update()
{
	if ( Skeleton != NULL && CurrentAnimation != NULL )
		CurrentAnimation->Update();
}

//-------------------------------------------------------------------------//

void le::AnimationManager::DeleteAnimation( const string& NameAnimation )
{
	if ( Animations.find( NameAnimation ) != Animations.end() )
	{
		if ( CurrentAnimation == &Animations[ NameAnimation ] )
			CurrentAnimation = NULL;

		Animations.erase( NameAnimation );
	}
}

//-------------------------------------------------------------------------//

void le::AnimationManager::DeleteAllAnimations()
{
	Animations.clear();
	CurrentAnimation = NULL;
}

//-------------------------------------------------------------------------//

void le::AnimationManager::SetSkeleton( le::Skeleton& Skeleton )
{
	this->Skeleton = &Skeleton;

	for ( auto it = Animations.begin(); it != Animations.end(); it++ )
		it->second.SetSkeleton( *this->Skeleton );
}

//-------------------------------------------------------------------------//

bool le::AnimationManager::IsPlaying()
{
	if ( CurrentAnimation != NULL )
		return CurrentAnimation->IsPlaying();

	return false;
}

//-------------------------------------------------------------------------//
