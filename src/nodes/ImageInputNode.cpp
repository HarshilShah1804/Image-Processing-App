#include "ImageInputNode.h"

#include <imgui.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <GL/gl.h>
#include <iostream>

ImageInputNode::ImageInputNode() {
    filepath = "";
    image = cv::Mat();
    textureID = 0;
    textureValid = false;
    memset(inputBuffer, 0, sizeof(inputBuffer));
    name = "Image Input Node";
}

ImageInputNode::~ImageInputNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void ImageInputNode::process() {
    // No processing for input node
}

std::string ImageInputNode::getName() const {
    return name;
}

cv::Mat ImageInputNode::getImage() const {
    return image;
}

void ImageInputNode::loadImage(const std::string& path) {
    filepath = path;
    image = cv::imread(path);

    if (image.empty()) {
        std::cerr << "Failed to load image from: " << path << std::endl;
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

void ImageInputNode::renderUI() {
    ImGui::Begin(("Image Input Node " + std::to_string(id)).c_str());

    ImGui::InputText("File Path", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    if (ImGui::Button("Load")) {
        filepath = std::string(inputBuffer);
        loadImage(filepath);
    }

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(128, 128));
    }

    ImGui::End();
}
