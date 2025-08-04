#include "EdgeDetection.h"
#include <imgui.h>
#include <imnodes.h>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <GL/gl.h>

EdgeDetectionNode::EdgeDetectionNode() {
    name = "Edge Detection";
    position = ImVec2(100, 100); // Optional positioning
    textureID = 0;
    textureValid = false;
    edgeType = 0; // Default edge detection type

    id = Node::generateUniqueId();
    inputs.push_back({generateUniqueId(), "In"});
    outputs.push_back({generateUniqueId(), "Out"});
}

EdgeDetectionNode::~EdgeDetectionNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void EdgeDetectionNode::setInputImage(const cv::Mat &img) {
    inputImage = img.clone();
    textureValid = false;  // Mark texture as invalid
}

cv::Mat EdgeDetectionNode::getImage() const {
    return outputImage;
}

void EdgeDetectionNode::resetInput() {
    inputImage.release();
    textureValid = false;
}

std::string EdgeDetectionNode::getName() const {
    return name;
}

void EdgeDetectionNode::process() {
    updateTexture();
}

void EdgeDetectionNode::renderUI() {
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(name.c_str());
    ImNodes::EndNodeTitleBar();

    if (!inputs.empty()) {
        ImNodes::BeginInputAttribute(inputs[0].id);
        ImGui::Text("In");
        ImNodes::EndInputAttribute();
    }

    if (!outputs.empty()) {
        ImNodes::BeginOutputAttribute(outputs[0].id);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();
    }
    const char* edgeTypes[] = { "Canny", "Sobel", "Laplacian" };
    ImGui::Text("Edge Type:");
    ImGui::Combo("Edge Type", &edgeType, edgeTypes, IM_ARRAYSIZE(edgeTypes));
    if (ImGui::Button("Process")) {
        if (inputImage.empty()) {
            std::cerr << "[EdgeDetectionNode] No input image.\n";
            return;
        }
        if (edgeType == 0) {
            cv::Canny(inputImage, outputImage, 100, 200); // Example parameters
        } else if (edgeType == 1) {
            cv::Sobel(inputImage, outputImage, CV_8U, 1, 1); // Example parameters
        } else if (edgeType == 2) {
            cv::Laplacian(inputImage, outputImage, CV_8U); // Example parameters
        } else {
            std::cerr << "[EdgeDetectionNode] Unknown edge type: " << edgeType << "\n";
            return;
        }
        if (outputImage.empty()) {
            std::cerr << "[EdgeDetectionNode] Output image is empty after processing.\n";
            textureValid = false;
            return;
        }
        textureValid = true; // Mark texture as valid after processing
    }

    if (textureValid && textureID != 0) {
        ImGui::Text("Preview:");
        ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(128, 128));
    }

    ImNodes::EndNode();
}

void EdgeDetectionNode::updateTexture() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    if (outputImage.empty()) return;

    cv::Mat displayImage;
    if (outputImage.channels() == 1)
        cv::cvtColor(outputImage, displayImage, cv::COLOR_GRAY2RGBA);
    else if (outputImage.channels() == 3)
        cv::cvtColor(outputImage, displayImage, cv::COLOR_BGR2RGBA);
    else if (outputImage.channels() == 4)
        displayImage = outputImage.clone();
    else
        return; // Unsupported channel count

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, displayImage.cols, displayImage.rows,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, displayImage.data);

    textureValid = true; // Mark texture as valid after updating
}