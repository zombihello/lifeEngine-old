#include "..\..\3D\Body3D_ShapeType.h"

//-------------------------------------------------------------------------//

le::Body3D_ShapeType::Body3D_ShapeType( PrimitivesType PrimitiveType, float Radius )
{
	this->PrimitiveType = PrimitiveType;
	this->Radius = Radius;
}

//-------------------------------------------------------------------------//

le::Body3D_ShapeType::Body3D_ShapeType( PrimitivesType PrimitiveType, float Radius, float Height )
{
	this->PrimitiveType = PrimitiveType;
	this->Radius = Radius;
	this->Height = Height;
}

//-------------------------------------------------------------------------//

le::Body3D_ShapeType::Body3D_ShapeType( PrimitivesType PrimitiveType, Vector3f Size )
{
	this->PrimitiveType = PrimitiveType;
	this->Size = Size;
}

//-------------------------------------------------------------------------//

le::Body3D_ShapeType::Body3D_ShapeType( PrimitivesType PrimitiveType, vector<float> CollisionVertexs, vector<int> CollisionIdVertexs )
{
	this->PrimitiveType = PrimitiveType;
	vCollisionVertexs = CollisionVertexs;
	vCollisionIdVertexs = CollisionIdVertexs;
}

//-------------------------------------------------------------------------//