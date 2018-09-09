#include <Physics/Body.h>

//-------------------------------------------------------------------------//

void le::Body::Move( const glm::vec3& FactorMove )
{
	Ray.Start = Ray.End;
	Ray.End = Ray.Start + FactorMove;
}

//-------------------------------------------------------------------------//

void le::Body::SetPosition( const glm::vec3& Position )
{
	Ray.Start = Ray.End = Position;
}

//-------------------------------------------------------------------------//

void le::Body::SetTraceType( TraceType TraceType, float TraceRadius )
{
	Ray.TraceType = TraceType;
	Ray.TraceRadius = TraceRadius;
}

//-------------------------------------------------------------------------//

le::Ray& le::Body::GetRay()
{
	return Ray;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Body::GetPosition()
{
	return Ray.End;
}

//-------------------------------------------------------------------------//

bool le::Body::operator==( Body& Body )
{
	return Ray == Body.Ray;
}

//-------------------------------------------------------------------------//
