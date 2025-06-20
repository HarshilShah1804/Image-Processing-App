#include "GrayScaleNode.h"
#include <opencv2/imgproc.hpp>
#include <GL/gl.h>
#include <iostream>
#include <imgui.h>

GrayScaleNode::GrayScaleNode() {
    name = "GrayScale";
    position = ImVec2(300, 100);  // Fixes your position error
    textureID = 0;
    textureValid = false;
}

void GrayScaleNode::renderUI() {
    ImGui::Begin(name.c_str());
    ImGui::Text("GrayScale Node");

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(128, 128));
    }

    ImGui::End();
}

void GrayScaleNode::process() {

    if (inputImage.empty()) {
        std::cerr << "No input Image" << std::endl;
        return;
    }

    if (!inputImage.empty()) {
        cv::cvtColor(inputImage, outputImage, cv::COLOR_BGR2GRAY);
    }

    // Delete previous texture if any
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    // Convert to RGBA
    cv::Mat displayImage;
    if (outputImage.channels() == 1)
        cv::cvtColor(outputImage, displayImage, cv::COLOR_GRAY2RGBA);
    else if (outputImage.channels() == 3)
        cv::cvtColor(outputImage, displayImage, cv::COLOR_BGR2RGBA);
    else if (outputImage.channels() == 4)
        displayImage = outputImage.clone();
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

std::string GrayScaleNode::getName() const {
    return name;
}

cv::Mat GrayScaleNode::getImage() const {
    return outputImage;
}

void GrayScaleNode::setInputImage(const cv::Mat& input) {
    inputImage = input;
}
