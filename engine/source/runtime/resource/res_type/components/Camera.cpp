#include "Camera.h"
#include "glm/geometric.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
namespace EasyEngine {

Camera::Camera(const CameraType& cameraType):cameraType(cameraType) {
	cameraPosition = glm::vec3(.0f, .0f, 3.0f);
	cameraFront = glm::vec3(0.0, 0.0, -1.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);
	cameraEulerRotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	fov = 30.0f;
}

Camera::Camera(const glm::vec3& initialPose, const glm::vec3& initialDirection) {
	cameraPosition = initialPose;
	cameraFront = glm::vec3(0.0, 0.0, -1.0);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);
	fov = 30.0f;
}

Camera& Camera::operator=(const Camera& other) {
	this->cameraPosition = other.cameraPosition;
	this->cameraFront = other.cameraFront;
	this->cameraUp = other.cameraUp;
	this->cameraPoseMatrix = other.cameraPoseMatrix;
	this->targetPosition = other.targetPosition;
	return *this;
}

Camera::~Camera() {

}

//void Camera::setCameraDirectionByTarget(const glm::vec3 targetPosition) {
//	glm::vec3 cameraDirection = glm::normalize(cameraPosition - targetPosition);
//	glm::vec3 cameraRight = glm::normalize(glm::cross(world_up, cameraDirection));
//	cameraUp = glm::cross(cameraDirection, cameraRight);
//	this->targetPosition = targetPosition;
//	
//}

void Camera::setCameraPosition(const glm::vec3 cameraPosition) {
	this->cameraPosition = cameraPosition;
}

void Camera:: setCameraRotation(const glm::vec3& cameraFront,const glm::vec3& cameraUp){
	this->cameraFront = glm::normalize(cameraFront);
	this->cameraUp = glm::normalize(cameraUp);
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::setCameraRotationByEuler(const glm::vec3 eulerRotation) {
	this->cameraEulerRotation = eulerRotation;
	glm::vec3 direction;
	direction.x = cos(glm::radians(eulerRotation.x)) * cos(glm::radians(eulerRotation.y));
	direction.y = sin(glm::radians(eulerRotation.y));
	direction.z = sin(glm::radians(eulerRotation.x)) * cos(glm::radians(eulerRotation.y));
	cameraFront = glm::normalize(direction);
	//std::cout << cameraFront.x << ":" << cameraFront.y << ":" << cameraFront.z << std::endl;
	cameraRight = glm::normalize(glm::cross(world_up, cameraFront));
	cameraUp = glm::cross(cameraFront, cameraRight);
}


const glm::mat4 Camera::getCameraPoseMatrix() {
	//specify a camera position, a target position and a vector that represents the up vector in world space
	return cameraPoseMatrix = glm::lookAt(cameraPosition, cameraPosition+cameraFront, cameraUp);
}



const glm::mat4 Camera::getProjectionMatrix() {
	if (cameraType==PERSPECTIVE) {
		return projectionMatrix = glm::perspective(glm::radians(fov), aspect,nearPlane, farPlane);
	}
	else{
		return projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	}
}
}