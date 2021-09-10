#include <GLFW/glfw3.h>
#include <thread>

int main() {
  glfwInit();
  auto window = glfwCreateWindow(521, 521, "ciao Enrico", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwShowWindow(window);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  glfwDestroyWindow(window);
  glfwTerminate();
}
