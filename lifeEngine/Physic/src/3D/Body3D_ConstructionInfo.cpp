#include "..\..\3D\Body3D_ConstructionInfo.h"

//-------------------------------------------------------------------------//

le::Body3D_ConstructionInfo::Body3D_ConstructionInfo( TypeBody typeBody, float Mass, Vector3f Position, Vector3f Rotation, Vector3f Inertia )
{
	if ( typeBody == TypeBody::Static )
		fMass = 0;
	else
		fMass = Mass;

	this->Position.setValue( Position.x, Position.y, Position.z );
	this->Rotation.setValue( Rotation.x, Rotation.y, Rotation.z, 1 );
	this->Inertia.setValue( Inertia.x, Inertia.y, Inertia.z );
}

//-------------------------------------------------------------------------//