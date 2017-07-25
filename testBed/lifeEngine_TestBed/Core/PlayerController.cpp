#include "PlayerController.h"

//---------------------------------------------------------

PlayerController::PlayerController( System& System, Physic3D& Physic, Vector2f SizePlayer )
{
	this->Physic = &Physic;
	this->SizePlayer = SizePlayer;

	CameraPlayer = new le::Camera( System );

	Body3D_ConstructionInfo constructionInfo( Body3D_ConstructionInfo::Dinamic, 10, Vector3f(), Vector3f() );
	BodyPlayer = new le::Body3D( Physic, &constructionInfo, ShapeType_Capsule( SizePlayer.x / 2, SizePlayer.y ) );
	BodyPlayer->SetFreezeRotation( true, true, true );
}

//---------------------------------------------------------

PlayerController::~PlayerController()
{
	delete CameraPlayer;
	delete BodyPlayer;
}

//---------------------------------------------------------

void PlayerController::SetPosition( Vector3f Position )
{
	BodyPlayer->SetPosition( Position );
	CameraPlayer->SetPosition( Position );
	this->Position = Position;
}

//---------------------------------------------------------

void PlayerController::Update()
{
	if ( Keyboard::isKeyPressed( Keyboard::W ) )
		BodyPlayer->Move( Body3D::Impulse, CameraPlayer->GetVectorMove( Camera::Forward, 10) );

	if ( Keyboard::isKeyPressed( Keyboard::S ) )
		BodyPlayer->Move( Body3D::Impulse, CameraPlayer->GetVectorMove( Camera::Back, 10 ) );

	if ( Keyboard::isKeyPressed( Keyboard::A ) )
	{
		BodyPlayer->Move( Body3D::Impulse, CameraPlayer->GetVectorMove( Camera::Left, 10 ) );

		if ( CameraPlayer->GetInclinationCamera() > -5 )
			CameraPlayer->TiltCamera( -0.2f );
	}

	if ( Keyboard::isKeyPressed( Keyboard::D ) )
	{
		BodyPlayer->Move( Body3D::Impulse, CameraPlayer->GetVectorMove( Camera::Right, 10 ) );

		if ( CameraPlayer->GetInclinationCamera() < 5 )
			CameraPlayer->TiltCamera( 0.2f );
	}

	Vector3f BodyPosition = BodyPlayer->GetPosition();
	BodyPosition.y += SizePlayer.y / 2;

	CameraPlayer->SetPosition( BodyPosition );
	CameraPlayer->UpdateCamera();

	if ( !Keyboard::isKeyPressed( Keyboard::A ) && !Keyboard::isKeyPressed( Keyboard::D ) )
	{
		if ( CameraPlayer->GetInclinationCamera() + 0.5f < 0 )
			CameraPlayer->TiltCamera( 0.5f );
		else if ( CameraPlayer->GetInclinationCamera() - 0.5f > 0 )
			CameraPlayer->TiltCamera( -0.5f );
	}
}

//---------------------------------------------------------

le::Camera* PlayerController::GetPlayerCamera()
{
	return CameraPlayer;
}

//---------------------------------------------------------