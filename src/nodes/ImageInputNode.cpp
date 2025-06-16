// ImageInputNode.cpp
#include "ImageInputNode.h"
#include <imgui.h>

ImageInputNode::ImageInputNode() {
    position = ImVec2(100, 100);
}

void ImageInputNode::renderUI() {
    ImGui::Begin(getName().c_str());

    ImGui::InputText("Path", imagePathBuffer, IM_ARRAYSIZE(imagePathBuffer));
    if (ImGui::Button("Load Image")) {
        image = cv::imread(imagePathBuffer);
        loaded = !image.empty();
        if (loaded) updateTexture(image);
    }

    if (loaded && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((void*)(intptr_t)textureID, ImVec2(150, 150));
    }

    ImGui::End();
}


void ImageInputNode::process() {
    if (loaded) updateTexture(image);
}
