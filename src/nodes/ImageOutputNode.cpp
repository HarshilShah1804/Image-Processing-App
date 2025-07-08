#include "ImageOutputNode.h"
#include <imgui.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

ImageOutputNode::ImageOutputNode() {
    filepath.clear();
    image = cv::Mat();
    textureID = 0;
    textureValid = false;
    memset(inputBuffer, 0, sizeof(inputBuffer));
    name = "Image Output Node";
}

ImageOutputNode::~ImageOutputNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void ImageOutputNode::setInputImage(const cv::Mat &img) {
    image = img.clone();
}

std::string ImageOutputNode::getName() const {
    return name;
}

cv::Mat ImageOutputNode::getImage() const {
    return image;
}

void ImageOutputNode::process() {
    // Nothing to do here — image is set via setInputImage
}

void ImageOutputNode::loadImage() {
    if (image.empty()) {
        return;
    }

    // Delete previous texture if any
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    // Convert to RGBA
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

    // Upload to texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, displayImage.cols, displayImage.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, displayImage.ptr());
    glBindTexture(GL_TEXTURE_2D, 0);

    textureValid = true;
}

void ImageOutputNode::saveImage(const std::string &path) {
    if (image.empty()) {
        saveStatus = "No image to save.";
        return;
    }
    if (cv::imwrite(path, image)) {
        saveStatus = "Saved successfully.";
    } else {
        saveStatus = "Save failed.";
    }
    filepath = path;
}

void ImageOutputNode::renderUI() {
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::Begin(("Image Output " + std::to_string(id)).c_str());
    loadImage();
    ImGui::InputText("Save path", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    if (ImGui::Button("Save")) {
        filepath = inputBuffer;
        saveImage(filepath);
    }

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(128, 128));
    }

    ImGui::End();
}
