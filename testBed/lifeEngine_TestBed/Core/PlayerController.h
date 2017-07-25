#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include <Physic\3D\Physic3D.h>
#include <Physic\3D\Body3D.h>
#include <System\3D\Camera.h>
using namespace le;

class PlayerController
{
public:

	//---------------------------------------------------------

	PlayerController( System& System, Physic3D& Physic, Vector2f SizePlayer );

	//---------------------------------------------------------

	~PlayerController();

	//---------------------------------------------------------

	void SetPosition( Vector3f Position );

	//---------------------------------------------------------

	void Update();

	//---------------------------------------------------------

	Camera* GetPlayerCamera();

	//---------------------------------------------------------

private:

	Vector2f    SizePlayer;
	Vector3f    Position;

	Camera*		CameraPlayer;
	Body3D*		BodyPlayer;
	Physic3D*	Physic;
};

#endif // PLAYER_CONTROLLER