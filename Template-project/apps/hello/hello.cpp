// non cambiare l'ordine
#include <glad/glad.h>
// non cambiare l'ordine
#include <GLFW/glfw3.h>

void draw() { glClearColor(1, 0, 0, 0); }

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
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
