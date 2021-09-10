// non cambiare l'ordine
#include <glad/glad.h>
// non cambiare l'ordine
#include <GLFW/glfw3.h>

#include <thread>

int main() {
  glfwInit();
  auto window = glfwCreateWindow(521, 521, "ciao Enrico", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwShowWindow(window);
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1, 0, 0, 0);
    glfwSwapBuffers(window);
  }
  std::this_thread::sleep_for(std::chrono::seconds(2));
  glfwDestroyWindow(window);
  glfwTerminate();
}
