// non cambiare l'ordine
#include <glad/glad.h>
// non cambiare l'ordine
#include <GLFW/glfw3.h>

void draw(int width, int height) {
  glViewport(0, 0, width, height);
  glClearColor(1, 0, 0, 0);
}

int main() {
  // open window
  glfwInit();
  auto window = glfwCreateWindow(521, 521, "ciao Enrico", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync
  glfwShowWindow(window);

  // load opengl
  gladLoadGL();

  // run GUI loop
  while (!glfwWindowShouldClose(window)) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    draw(width, height);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
