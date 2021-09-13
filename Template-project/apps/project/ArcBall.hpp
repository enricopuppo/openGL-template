#ifndef ARCBALL_H
#define ARCBALL_H

#include "./glm/glm.hpp"
#include "Quaternion.hpp"

class ArcBall{
	private:
		glm::mat4 centerTranslationMatrix;
		glm::mat4 translationMatrix;
		glm::mat4 viewMatrix;
		glm::vec2 mouseStart;
		Quaternion qCurr;
		Quaternion qOld;

		void update();
		Quaternion screenToArcBall(glm::vec2 point);

	public:
        ArcBall() = default;
		ArcBall(glm::vec3 cameraPos, glm::vec3 center, glm::vec3 upDirection);
		void rotate(glm::vec2 mouseEnd);
		void zoom(float zoomAmount);
		glm::mat4 getViewMatrix();
		void setMouseStart(glm::vec2 point);
		void invertPitch();
		void resetPitch();
};

#endif