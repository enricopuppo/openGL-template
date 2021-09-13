#include "ArcBall.hpp"
#include "Quaternion.hpp"
#include "./glm/glm.hpp"

ArcBall::ArcBall(glm::vec3 cameraPos, glm::vec3 center, glm::vec3 upDirection){
	glm::vec3 dir = center - cameraPos;
	glm::vec3 z = glm::normalize(dir);
	glm::vec3 x = glm::normalize(glm::cross(z, glm::normalize(upDirection)));
	glm::vec3 y = glm::normalize(glm::cross(x, z));
	x = glm::normalize(glm::cross(z, y));

	centerTranslationMatrix = glm::inverse(glm::translate(glm::mat4(1.0), center));
	translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -glm::length(dir)));
	qCurr = Quaternion(glm::transpose(glm::mat3(x, y, -z)));
	mouseStart = glm::vec2(0.0, 0.0);
	qCurr.normalize();
}

// projects a point in normalized display coordinates on the sphere
Quaternion ArcBall::screenToArcBall(glm::vec2 point){
	float dist = glm::dot(point, point);
	if (dist <= 1.0)
		return Quaternion(0.0, point[0], point[1], std::sqrt(1.0 - dist));
	else{
		glm::vec2 proj = glm::normalize(point);
		return Quaternion(0.0, proj[0], proj[1], 0.0);
	}
}

// given two points on the sphere computes the corresponding quaternions and
// updates the global rotation
void ArcBall::rotate(glm::vec2 mouseEnd){
	mouseEnd = glm::clamp(mouseEnd, glm::vec2(-1.0, -1.0), glm::vec2(1.0, 1.0));
	mouseStart = glm::clamp(mouseStart, glm::vec2(-1.0, -1.0), glm::vec2(1.0, 1.0));
	Quaternion mouseCurBall = screenToArcBall(-mouseEnd);
	Quaternion mousePrevBall = screenToArcBall(-mouseStart);
	qCurr = mouseCurBall * mousePrevBall * qCurr;
	mouseStart = mouseEnd;
	update();
}

// updates the view matrix with a translation on the Z axis given by the zoom 
void ArcBall::zoom(float zoomAmount){
	glm::vec3 motion(0.0, 0.0, zoomAmount);
	translationMatrix = glm::translate(glm::mat4(1.0), motion) * translationMatrix;
	update();
}

glm::mat4 ArcBall::getViewMatrix(){
	return viewMatrix;
}

// updates the current view matrix with the new rotation and the translation
void ArcBall::update(){
	viewMatrix = translationMatrix * qCurr.toMatrix4() * centerTranslationMatrix;
}

// sets the first point of the rotation arc
void ArcBall::setMouseStart(glm::vec2 point){
	this->mouseStart = point;
}

// used to place the camera below the terrain to capture the image used for the reflection.
// saves the current quaternion for later use
void ArcBall::invertPitch(){
	qOld = qCurr;
	glm::vec4 qCurrVec = qCurr.getWxyz();
	qCurr = Quaternion(qCurrVec[0], -qCurrVec[1], qCurrVec[2], -qCurrVec[3]);
	update();
}

// resets the previous quaternion after acquiring the image for the reflection
void ArcBall::resetPitch(){
	qCurr = qOld;
	update();
}