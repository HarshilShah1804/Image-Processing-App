#include "ImageOutputNode.h"
#include <imgui.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <GL/gl.h>
#include <iostream>

ImageOutputNode::ImageOutputNode()
    : filepath(""),
      textureID(0),
      textureValid(false),
      imageChanged(false),
      saveStatus("") {
    memset(inputBuffer, 0, sizeof(inputBuffer));
    name = "Image Output Node";
}

ImageOutputNode::~ImageOutputNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void ImageOutputNode::setInputImage(const cv::Mat& input) {
    inputImage = input.clone();
    imageChanged = true;
}

void ImageOutputNode::process() {
    image = inputImage.clone();  // Optional: apply any transformations
    if (imageChanged) {
        saveImage(filepath);
        updateTextureFromImage();
        imageChanged = false;
    }
}

std::string ImageOutputNode::getName() const {
    return name;
}

cv::Mat ImageOutputNode::getImage() const {
    return image;
}

void ImageOutputNode::saveImage(const std::string& path) {
    if (image.empty()) {
        std::cerr << "No image loaded to save." << std::endl;
        saveStatus = "❌ No image to save.";
        return;
    }

    if (path.empty()) {
        std::cerr << "No save path specified." << std::endl;
        saveStatus = "❌ No save path specified.";
        return;
    }

    if (!cv::imwrite(path, image)) {
        std::cerr << "Failed to save image to: " << path << std::endl;
        saveStatus = "❌ Failed to save.";
    } else {
        std::cout << "Image saved successfully to: " << path << std::endl;
        saveStatus = "✅ Saved to: " + path;
    }
}

void ImageOutputNode::updateTextureFromImage() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    if (image.empty()) return;

    cv::Mat displayImage;
    if (image.channels() == 1)
        cv::cvtColor(image, displayImage, cv::COLOR_GRAY2RGBA);
    else if (image.channels() == 3)
        cv::cvtColor(image, displayImage, cv::COLOR_BGR2RGBA);
    else if (image.channels() == 4)
        displayImage = image.clone();
    else
        return;

    cv::flip(displayImage, displayImage, 0);  // Flip for OpenGL

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, displayImage.cols, displayImage.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, displayImage.ptr());
    glBindTexture(GL_TEXTURE_2D, 0);

    textureValid = true;
}

void ImageOutputNode::renderUI() {
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin(("Image Output Node " + std::to_string(id)).c_str());

    ImGui::InputText("Save Path", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    if (ImGui::Button("Save")) {
        filepath = std::string(inputBuffer);
        saveImage(filepath);
    }

    if (!saveStatus.empty()) {
        ImGui::TextWrapped("%s", saveStatus.c_str());
    }

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        static float previewSize = 128.0f;
        ImGui::SliderFloat("Preview Size", &previewSize, 64.0f, 512.0f);
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(previewSize, previewSize));
    }

    ImGui::End();
}
