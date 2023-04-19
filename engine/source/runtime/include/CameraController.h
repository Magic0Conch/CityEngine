#pragma once
#include "../resource/res_type/components/Camera.h"
#include "InputHandler.h"
#include "WindowTime.h"
namespace EasyEngine {
	class CameraController
	{
	private:
		mutable glm::dvec3 moveSpeed = { 2.5,1.5,-3 };
		mutable glm::dvec3 rotationSpeed = { 1.0f,1.0f,1.0f };
	public:
		Camera* camera;
	public:
		CameraController();
		virtual ~CameraController();
		CameraController(Camera& initialCamera);
		//horizontal mouse-movement affects the yaw and vertical mouse-movement affects the pitch
		void processInput();
	};
}