#include "Player.hpp"
#include "Entity.hpp"
#include "TexturedModel.hpp"
#include "Light.hpp"
#include "./glm/glm.hpp"
#include <GLFW/glfw3.h>
#include "DisplayManager.hpp"

Player::Player(TexturedModel &model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 poolScale) : Entity(model, position, rotation, scale)
{
    // initializes the player characteristics
    this->movementSpeed = 2.5;
    this->turnSpeed = 120.0;
    this->upSpeed = 1.0;
    this->currentMovementSpeed = 0.0;
    this->currentTurnSpeed = 0.0;
    this->currentUpSpeed = 0.0;
    this->gravity = -0.125;
    this->floorHeight = -poolScale[1] + 0.1;
    this->waterHeight = 0.0;
    this->xLimit = poolScale[0] * 0.5 - 0.25;
    this->zLimit = poolScale[2] * 0.5 - 0.25;
}

glm::vec3 Player::move(DisplayManager &dm)
{
    // moves the submarine according to the inputs given by the user
    // and limitating the mobility to the pool volume.
    // if the submarine is not on the water surface or on the floor it slowly sinks
    checkInputs(dm.getWindow());
    editRotation(glm::vec3(0.0, currentTurnSpeed * dm.getDeltaTime(), 0.0));

    float distance = currentMovementSpeed * dm.getDeltaTime();
    float deltaX = distance * sin(glm::radians(getRotation()[1]));
    float deltaY = currentUpSpeed * dm.getDeltaTime();
    float deltaZ = distance * cos(glm::radians(getRotation()[1]));

    // pool boundaries
    if (getPosition()[0] >= xLimit && deltaX > 0)
        deltaX = 0.0;
    else if (getPosition()[0] <= -xLimit && deltaX < 0)
        deltaX = 0.0;
    if (getPosition()[2] >= zLimit && deltaZ > 0)
        deltaZ = 0.0;
    else if (getPosition()[2] <= -zLimit && deltaZ < 0)
        deltaZ = 0.0;
    editPosition(glm::vec3(deltaX, deltaY, deltaZ));

    if (getPosition()[1] >= waterHeight || getPosition()[1] <= floorHeight) // if on surface on on the floor the submarine remains still
        currentUpSpeed = 0.0;
    else // otherwise it slightly falls on the bottom of the pool
        currentUpSpeed += gravity * dm.getDeltaTime();
    return glm::vec3(deltaX, deltaY, deltaZ);
}

void Player::checkInputs(GLFWwindow *window)
{
    // reads the player's inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        currentMovementSpeed = movementSpeed;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        currentMovementSpeed = -movementSpeed;
    else
        currentMovementSpeed = 0.0;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        currentTurnSpeed = -turnSpeed;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        currentTurnSpeed = turnSpeed;
    else
        currentTurnSpeed = 0.0;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    { // if not on top ascends
        if (getPosition()[1] < waterHeight)
            currentUpSpeed = upSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    { // if not on bottom descends
        if (getPosition()[1] > floorHeight)
            currentUpSpeed = -upSpeed;
    }
    else
    {
        if (getPosition()[1] > floorHeight && getPosition()[1] < waterHeight)
            currentUpSpeed = gravity;
    }
}