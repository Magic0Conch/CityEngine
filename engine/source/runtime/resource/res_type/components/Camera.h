#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "glm/fwd.hpp"
#include "runtime/include/EngineWindow.h"
namespace EasyEngine {
	enum class CameraType{
		PERSPECTIVE,ORTHOGONAL
	};

	class Camera
	{
	public:
		glm::vec3 cameraPosition;
		//yaw pitch roll
		glm::vec3 cameraEulerRotation;
		float fov,nearPlane=0.1f,farPlane=1000.0f;
		CameraType cameraType;
		//cameraDirection is actually the reverse direction in the reverse direction of what it is targeting.
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		glm::vec3 cameraRight;
		float aspect = EngineWindow::getInstance().viewportWidth / EngineWindow::getInstance().viewportHeight;
	private:
		const glm::vec3 world_up = glm::vec3(.0f,1.0f,.0f);
		glm::vec3 targetPosition;
		glm::mat4 cameraPoseMatrix;
		glm::mat4 projectionMatrix;
	public:
		Camera(const CameraType cameraType);
		Camera(const glm::vec3& initialPose, const glm::vec3& initialDirection = glm::vec3(0));
		virtual ~Camera();
		Camera& operator=(const Camera&);
		Camera() = delete;

		void setCameraDirectionByTarget(const glm::vec3 targetPosition);
		void setCameraPosition(const glm::vec3 cameraPosition);
		void setCameraRotationByEuler(const glm::vec3 eulerRotation);
		void setCameraRotation(const glm::vec3& cameraFront,const glm::vec3& cameraUp);
		const glm::mat4 getCameraPoseMatrix();
		const glm::mat4 getProjectionMatrix();
	};

}