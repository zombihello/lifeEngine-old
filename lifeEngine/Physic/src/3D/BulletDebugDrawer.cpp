#include "..\..\3D\BulletDebugDrawer.h"

//-------------------------------------------------------------------------//

le::BulletDebugDrawer::BulletDebugDrawer()
{
	DebugMode = 0;
}

//-------------------------------------------------------------------------//

void le::BulletDebugDrawer::drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
{
	float Vertex[6] = { from.x(), from.y(), from.z(), to.x(), to.y(), to.z() };

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, &Vertex );

	glColor3f( color.x(), color.y(), color.z() );
	glDrawArrays( GL_LINES, 0, 2 );

	glColor3f( 1, 1, 1 );
	glDisableClientState( GL_VERTEX_ARRAY );
}

//-------------------------------------------------------------------------//

void le::BulletDebugDrawer::setDebugMode( int DebugMode )
{
	this->DebugMode = DebugMode;
}

//-------------------------------------------------------------------------//

void le::BulletDebugDrawer::draw3dText( const btVector3& location, const char* textString )
{
	//glRasterPos3f(location.x(),  location.y(),  location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

//-------------------------------------------------------------------------//

void le::BulletDebugDrawer::reportErrorWarning( const char* warningString )
{
	cout << "[Bullet Physic]: " << warningString << endl;
}

//-------------------------------------------------------------------------//

void le::BulletDebugDrawer::drawContactPoint( const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
{
	/*btVector3 to=pointOnB+normalOnB*distance;
	const btVector3&from = pointOnB;
	glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

	BulletDebugDrawer::drawLine( from, to, color );

	glRasterPos3f(from.x(),  from.y(),  from.z());
	char buf[12];
	sprintf(buf," %d",lifeTime);
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);*/
}

//-------------------------------------------------------------------------//

int le::BulletDebugDrawer::getDebugMode() const
{
	return DebugMode;
}

//-------------------------------------------------------------------------//