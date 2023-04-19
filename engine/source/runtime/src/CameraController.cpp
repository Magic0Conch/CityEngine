#include "../include/CameraController.h"
#include <iostream>
namespace EasyEngine {
CameraController::CameraController(){}
CameraController::~CameraController(){}
CameraController::CameraController(Camera& initialCamera) {
	this->camera = &initialCamera;
}

void CameraController::processInput() {
	InputHandler& inputHandler = InputHandler::getInstance();
	glm::vec3 translationVector = inputHandler.buttonInputAxis * moveSpeed * WindowTime::deltaTimeValue;
	glm::vec3 rotationVector = inputHandler.mouseInputAxis * rotationSpeed * WindowTime::deltaTimeValue;
	glm::vec3 deltaPosition = -translationVector.z * camera->cameraFront - translationVector.y * camera->cameraUp  -translationVector.x * camera->cameraRight;
	camera->setCameraPosition(camera->cameraPosition + deltaPosition);
	// std::cout << rotationVector.x << ":" << rotationVector.y << ":" << rotationVector.z << std::endl;
	camera->setCameraRotationByEuler(camera->cameraEulerRotation + rotationVector);
	camera->fov -= rotationVector.z;
	if (camera->fov < 1.0f)
		camera->fov = 1.0f;
	if (camera->fov > 45.0f)
		camera->fov = 45.0f;
}
}