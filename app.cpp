// src/app.cpp
#include <imgui.h>
#include <imnodes.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "nodes/graph.h"

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void DrawNeonGrid(ImDrawList* draw_list, ImVec2 canvas_p0, ImVec2 canvas_p1, float grid_spacing = 50.0f) {
    ImU32 neon_color = IM_COL32(0, 255, 255, 128);
    float thickness = 1.0f;
    for (float x = canvas_p0.x; x <= canvas_p1.x; x += grid_spacing)
        draw_list->AddLine(ImVec2(x, canvas_p0.y), ImVec2(x, canvas_p1.y), neon_color, thickness);
    for (float y = canvas_p0.y; y <= canvas_p1.y; y += grid_spacing)
        draw_list->AddLine(ImVec2(canvas_p0.x, y), ImVec2(canvas_p1.x, y), neon_color, thickness);
}

int main() {
    Graph g;
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return -1;

    const char* glsl_version = "#version 130";
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Image Graph App", nullptr, nullptr);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while (!glfwWindowShouldClose(window)) {
        ImNodes::CreateContext();
        ImNodes::StyleColorsDark();
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Background grid
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("##Canvas", nullptr,
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoBackground);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 canvas_p0 = ImGui::GetWindowPos();
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + io.DisplaySize.x, canvas_p0.y + io.DisplaySize.y);
        DrawNeonGrid(draw_list, canvas_p0, canvas_p1, 50.0f);
        ImGui::End();
        ImGui::PopStyleVar(2);

        g.renderAddNodeUI();  // UI to add nodes dynamically
        ImNodes::BeginNodeEditor();
        g.renderUI();         // Render node UIs
        ImNodes::EndNodeEditor();

        int startAttr, endAttr;
        if (ImNodes::IsLinkCreated(&startAttr, &endAttr)) {
            g.addEdge(startAttr, endAttr);
        }
        
        g.process();

        // Render everything
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
