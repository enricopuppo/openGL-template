#include <GLFW/glfw3.h>
#include <thread>

int main() {
  glfwInit();
  auto window = glfwCreateWindow(521, 521, "ciao Enrico", nullptr, nullptr);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  glfwDestroyWindow(window);
  glfwTerminate();
}
