
#include "ArcballCamera.h"

using namespace std;
using namespace glm;




void ArcballCamera::calculateDerivedValues() {

	const float theta_ = glm::radians<float>(theta);
	const float phi_ = glm::radians<float>(phi);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -radius)) * glm::eulerAngleX(-theta_) * glm::eulerAngleY(-phi_);
	projectionMatrix = glm::perspective(glm::radians<float>(fovY), aspect, nearPlane, farPlane);
}



ArcballCamera::ArcballCamera() {

	theta = 0.0f;
	phi = 0.0f;
	radius = 15.0f;

	fovY = 55.0f;
	aspect = 1.0f;
	nearPlane = 0.1f;
	farPlane = 500.0f;



	calculateDerivedValues();

}



ArcballCamera::ArcballCamera(float theta, float phi, float radius, float fovY, float aspect, float nearPlane, float farPlane) {

	this->theta = theta;
	this->phi = phi;
	this->radius = std::max<float>(0.0f, radius);

	this->fovY = fovY;
	this->aspect = aspect;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;



	
	calculateDerivedValues();
	
}


#pragma region Accessor methods for stored values


float ArcballCamera::getTheta() {

	return theta;
}


float ArcballCamera::getPhi() {

	return phi;
}

void ArcballCamera::rotateCamera(float dTheta, float dPhi) {

	theta += dTheta;
	phi += dPhi;

	calculateDerivedValues();
}

float ArcballCamera::getRadius() {

	return radius;
}

void ArcballCamera::scaleRadius(float s) {

	radius *= s;
	calculateDerivedValues();
}

void ArcballCamera::incrementRadius(float i) {

	radius = std::max<float>(radius + i, 0.0f);
	calculateDerivedValues();
}

float ArcballCamera::getFovY() {

	return fovY;
}

void ArcballCamera::setFovY(float fovY) {

	this->fovY = fovY;
	calculateDerivedValues();
}

float ArcballCamera::getAspect() {

	return aspect;
}

void ArcballCamera::setAspect(float aspect) {

	this->aspect = aspect;
	calculateDerivedValues();
}

float ArcballCamera::getNearPlaneDistance() {

	return nearPlane;
}

void ArcballCamera::setNearPlaneDistance(float nearPlaneDistance) {

	this->nearPlane = nearPlaneDistance;
	calculateDerivedValues();
}

float ArcballCamera::getFarPlaneDistance() {

	return farPlane;
}

void ArcballCamera::setFarPlaneDistance(float farPlaneDistance) {

	this->farPlane = farPlaneDistance;
	calculateDerivedValues();
}

#pragma endregion


#pragma region Accessor methods for derived values


glm::mat4 ArcballCamera::viewTransform() {

	return viewMatrix;
}


glm::mat4 ArcballCamera::projectionTransform() {

	return projectionMatrix;
}

#pragma endregion