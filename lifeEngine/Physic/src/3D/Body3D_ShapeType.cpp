#include "..\..\3D\Body3D_ShapeType.h"

//-------------------------------------------------------------------------//

le::ShapeType_StaticPlane::ShapeType_StaticPlane( Vector3f PlaneNormal, float PlaneConstant )
{
	this->PlaneNormal = PlaneNormal;
	fPlaneConstant = PlaneConstant;
}

//-------------------------------------------------------------------------//

le::ShapeType_Sphere::ShapeType_Sphere( float Radius )
{
	fRadius = Radius;
}

//-------------------------------------------------------------------------//

le::ShapeType_Box::ShapeType_Box( Vector3f Size )
{
	this->Size = Size;
}

//-------------------------------------------------------------------------//

le::ShapeType_Cylinder::ShapeType_Cylinder( Vector3f Size )
{
	this->Size = Size;
}

//-------------------------------------------------------------------------//

le::ShapeType_Capsule::ShapeType_Capsule( float Radius, float Height )
{
	fRadius = Radius;
	fHeight = Height;
}

//-------------------------------------------------------------------------//

le::ShapeType_Cone::ShapeType_Cone( float Radius, float Height )
{
	fRadius = Radius;
	fHeight = Height;
}

//-------------------------------------------------------------------------//

le::ShapeType_Mesh::ShapeType_Mesh( vector<float>* CollisionVertexs, vector<int>* CollisionIdVertexs )
{
	vVertexs = CollisionVertexs;
	vIdVertexs = CollisionIdVertexs;
}

//-------------------------------------------------------------------------//