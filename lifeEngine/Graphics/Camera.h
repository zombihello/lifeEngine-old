//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (��������� �����) ***
//					Copyright (C) 2017
//
// ����� �� ����:		https://vk.com/zombihello
// ����������� ������:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_H
#define CAMERA_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////
// OPENGL
//////////////
#include <glm\glm.hpp>

//////////////////
// SFML
//////////////////
#include <SFML\Graphics.hpp>
using namespace sf;

//////////////////
// LIFEENGINE
//////////////////
#include <System\System.h>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief ����� ��� ������ � �������
	//////////////////////////////////////////////////////////////////////
	class DLL_API Camera
	{
	public:

		//-------------------------------------------------------------------------//

		enum TypeMove
		{
			Forward,
			Back,
			Left,
			Right
		};

		//-------------------------------------------------------------------------//

		//////////////////////////////////////////////////////////////////////
		/// \brief �����������
		//////////////////////////////////////////////////////////////////////
		Camera( System& System );

		//////////////////////////////////////////////////////////////////////
		/// \brief ����������
		//////////////////////////////////////////////////////////////////////
		~Camera();

		//////////////////////////////////////////////////////////////////////
		/// \brief �������� ������
		/// \details ������ ����� ��������� ������� ������� ������
		//////////////////////////////////////////////////////////////////////
		void UpdateCamera();

		//////////////////////////////////////////////////////////////////////
		/// \brief �������� ������
		/// \details ������ ����� ���������� ������
		///
		/// \param[in] FactorMove ������ �������� (�� ������� ��������)
		//////////////////////////////////////////////////////////////////////
		void Move( const glm::vec3& FactorMove );

		//////////////////////////////////////////////////////////////////////
		/// \brief �������� ������
		/// \details ������ ����� ���������� ������ � ������ �� ��������
		///
		/// \param[in] typeMove ��� �������� (�����, ������ � �.�)
		/// \param[in] MoveSpeed �������� �����������
		//////////////////////////////////////////////////////////////////////
		void Move( TypeMove typeMove, float MoveSpeed );

		//////////////////////////////////////////////////////////////////////
		/// \brief ��������� ������ �� ��� Z
		/// \details ������ ����� ��������� ������ �� Z
		///
		/// \param[in] FactorTilt ������ �������
		//////////////////////////////////////////////////////////////////////
		void TiltCamera( float FactorTilt );

		//////////////////////////////////////////////////////////////////////
		/// \brief ������ ��������������� ����
		///
		/// \param[in] sensitivityMouse ��������������� ����
		//////////////////////////////////////////////////////////////////////
		void SetSensitivityMouse( float sensitivityMouse );

		//////////////////////////////////////////////////////////////////////
		/// \brief ������ ������� ����
		///
		/// \param[in] NewPosition ����� �������
		//////////////////////////////////////////////////////////////////////
		void SetPosition( const glm::vec3& NewPosition );

		//////////////////////////////////////////////////////////////////////
		/// \brief ������ ���������� ������ �� Z
		///
		/// \param[in] InclinationCamera ������ ������
		//////////////////////////////////////////////////////////////////////
		void SetInclinationCamera( float InclinationCamera );

		//////////////////////////////////////////////////////////////////////
		/// \brief �������� ������� ������
		///
		/// \return const glm::vec3&
		//////////////////////////////////////////////////////////////////////
		const glm::vec3& GetPosition();

		//////////////////////////////////////////////////////////////////////
		/// \brief �������� ������ ������ �� Z
		///
		/// \return float
		//////////////////////////////////////////////////////////////////////
		float GetInclinationCamera();

		//////////////////////////////////////////////////////////////////////
		/// \brief �������� ������ ��������
		/// \details ���������� �������� XYZ � ������ �������� ������
		///
		/// \param[in] typeMove ��� �������� (�����, ������ � �.�)
		/// \param[in] MoveSpeed �������� �����������
		/// \return glm::vec3&
		//////////////////////////////////////////////////////////////////////
		glm::vec3 GetVectorMove( TypeMove typeMove, float MoveSpeed );

		//////////////////////////////////////////////////////////////////////
		/// \brief �������� ������� �������
		///
		/// \return const glm::mat4&
		//////////////////////////////////////////////////////////////////////
		glm::mat4& GetViewMatrix();

	private:

		float			SensitivityMouse; ///< ��������������� ����
		float			InclinationCamera; ///< ������ ������
		glm::vec2		Angle; ///< ���� �������� ������
		glm::vec3		Position; ///< ������� ������		
		glm::mat4		ViewMatrix; ///< ������� ����
		glm::mat4		InclinationCameraMatrix; ///< ������� ������� ������

		RenderWindow*	RenderWindow; ///< ������� ����
		glm::vec2		CenterWindow; ///< ����� ����
		Vector2i		MousePosition; ///< ������� ����
		Vector2u		SizeWindow; ///< ������ ����
	};

	//-------------------------------------------------------------------------//
}

#endif //CAMERA_H