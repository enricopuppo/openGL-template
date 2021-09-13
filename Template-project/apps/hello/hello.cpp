// non cambiare l'ordine
#include <glad/glad.h>
// non cambiare l'ordine
#include <GLFW/glfw3.h>
// non cambiare l'ordine
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <stdio.h>

void draw_widgets() {}

void draw_window(int width, int height)
{
  glViewport(0, 0, width, height);
  glClearColor(1, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main()
{
  // open window
  if (!glfwInit())
    printf("bad glfw\n");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  auto window = glfwCreateWindow(521, 521, "ciao Enrico", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync
  glfwShowWindow(window);

  // load opengl
  if (!gladLoadGL())
    printf("bad glad\n");

  printf("Supported GLSL version: %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

  // widgets
  auto widgets = true;
  if (widgets)
  {
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetStyle().WindowRounding = 0;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifndef __APPLE__
    ImGui_ImplOpenGL3_Init();
#else
    ImGui_ImplOpenGL3_Init("#version 330");
#endif
    if (widgets)
    {
      ImGui::CreateContext();
      ImGui::GetIO().IniFilename = nullptr;
      ImGui::GetStyle().WindowRounding = 0;
      ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifndef __APPLE__
      ImGui_ImplOpenGL3_Init();
#else
      ImGui_ImplOpenGL3_Init("#version 330");
#endif
    }

    // run GUI loop
    while (!glfwWindowShouldClose(window))
    {
      // window drawing
      auto width = 0, height = 0;
      glfwGetFramebufferSize(window, &width, &height);
      draw_window(width, height);

      // widgets
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      if (ImGui::Begin("ciao", nullptr, ImGuiWindowFlags_NoSavedSettings))
        draw_widgets();
      ImGui::End();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      // swap
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}
