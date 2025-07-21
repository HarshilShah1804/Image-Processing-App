#include "ImageOutputNode.h"
#include <imgui.h>
#include <imnodes.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <GL/gl.h>

ImageOutputNode::ImageOutputNode() {
    filepath.clear();
    image = cv::Mat();
    textureID = 0;
    textureValid = false;
    saveStatus.clear();
    imageChanged = false;
    memset(inputBuffer, 0, sizeof(inputBuffer));
    name = "Image Output";

    id = Node::generateUniqueId();

    inputs.push_back({generateUniqueId(), "In"});
}

ImageOutputNode::~ImageOutputNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void ImageOutputNode::setInputImage(const cv::Mat &img) {
    image = img.clone();
    imageChanged = true;  // Mark that the image has changed
}

cv::Mat ImageOutputNode::getImage() const {
    return image;
}

void ImageOutputNode::resetInput() {
    image.release();
    textureValid = false;
}

std::string ImageOutputNode::getName() const {
    return name;
}

void ImageOutputNode::process() {
    if (imageChanged) {
        updateTexture();
        imageChanged = false;
    }
}

void ImageOutputNode::loadImage() {
    if (image.empty()) return;

    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    cv::Mat displayImage;
    if (image.channels() == 1)
        cv::cvtColor(image, displayImage, cv::COLOR_GRAY2RGBA);
    else if (image.channels() == 3)
        cv::cvtColor(image, displayImage, cv::COLOR_BGR2RGBA);
    else if (image.channels() == 4)
        displayImage = image.clone();
    else
        return;

    cv::flip(displayImage, displayImage, 0);

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
    if (id == 0) return;

    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(name.c_str());
    ImNodes::EndNodeTitleBar();

    if (!inputs.empty()) {
        ImNodes::BeginInputAttribute(inputs[0].id);
        ImGui::Text("In");
        ImNodes::EndInputAttribute();
    }

    ImGui::InputText("Save path", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    if (ImGui::Button("Save")) {
        filepath = std::string(inputBuffer);
        saveImage(filepath);
    }

    if (!saveStatus.empty()) {
        ImGui::TextWrapped("%s", saveStatus.c_str());
    }

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(128, 128));
    }

    ImNodes::EndNode();
}

void ImageOutputNode::updateTexture() {
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