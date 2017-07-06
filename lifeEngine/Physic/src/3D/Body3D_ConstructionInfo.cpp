#include "..\..\3D\Body3D_ConstructionInfo.h"

//-------------------------------------------------------------------------//

le::Body3D_ConstructionInfo::Body3D_ConstructionInfo( float Mass, Vector3f Position, Vector3f Rotation, bool IsStatic, Vector3f Inertia )
{
	if ( !IsStatic )
		fMass = Mass;
	else
		fMass = 0;

	Static = IsStatic;
	this->Position.setValue( Position.x, Position.y, Position.z );
	this->Rotation.setValue( Rotation.x, Rotation.y, Rotation.z, 1 );
	this->Inertia.setValue( Inertia.x, Inertia.y, Inertia.z );
}

//-------------------------------------------------------------------------//