#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <opencv2/imgproc.hpp>
#include <imgui.h>
#include <imnodes.h>
#include "Node.h"
#include "nodes/ImageInputNode.h"
#include "nodes/GrayScaleNode.h"
#include "nodes/ImageOutputNode.h"
#include "nodes/BrightnessNode.h"
#include "nodes/RotateNode.h"
#include "nodes/BlurNode.h"
#include "nodes/ExposureNode.h"
#include "nodes/SharpenNode.h"
#include "nodes/ContrastNode.h"

struct Edge {
    int id;
    int fromAttrId;
    int toAttrId;
};


class Graph {
public:
    int numNodes = 0;
    int numLinks = 0;

    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Edge> edges;

    std::unordered_map<int, std::shared_ptr<Node>> idToNode;

    void addNode(std::shared_ptr<Node> node) {
        node->id = node->id + 1;
        nodes.push_back(node);
        idToNode[node->id] = node;
        // std::cout << node->id << " " << node->getName() << " added to graph.\n";
    }

    void addEdge(int fromAttrId, int toAttrId) {
        static int linkId = 1;
        // std::cout << linkId << " " << fromAttrId << " -> " << toAttrId << std::endl;
        edges.push_back({ linkId++, fromAttrId, toAttrId });
    }


    void renderAddNodeUI() {
        static int selectedNodeType = 0;
        const char* nodeTypes[] = { "Image Input", "Grayscale", "Brightness", "Rotate", "Blur", "Exposure", "Sharpen", "Contrast", "Image Output"};

        ImGui::Begin("Add Node");

        ImGui::Text("Select node type:");
        ImGui::Combo("##NodeType", &selectedNodeType, nodeTypes, IM_ARRAYSIZE(nodeTypes));

        if (ImGui::Button("Add Node")) {
            std::shared_ptr<Node> newNode;
            switch (selectedNodeType) {
                case 0: newNode = std::make_shared<ImageInputNode>(); break;
                case 1: newNode = std::make_shared<GrayScaleNode>(); break;
                case 2: newNode = std::make_shared<BrightnessNode>(); break;
                case 3: newNode = std::make_shared<RotateNode>(); break;
                case 4: newNode = std::make_shared<BlurNode>(); break;
                case 5: newNode = std::make_shared<ExposureNode>(); break;
                case 6: newNode = std::make_shared<SharpenNode>(); break;
                case 7: newNode = std::make_shared<ContrastNode>(); break;
                case 8: newNode = std::make_shared<ImageOutputNode>(); break;
                default: std::cerr << "Unknown node type\n";
            }

            if (newNode) {
                addNode(newNode);
            }
        }

        ImGui::End();
    }

    // In Graph::renderUI()
    void renderUI() {
        for (const auto& node : nodes) {
            node->renderUI();
        }

        for (const auto& edge : edges) {
            ImNodes::Link(edge.id, edge.fromAttrId, edge.toAttrId);
        }

    }

    void process() {
        // Reset input images
        for (auto& node : nodes) {
            node->resetInput();
        }

        // Propagate data through edges
        for (const auto& edge : edges) {
            auto fromNode = idToNode[edge.fromAttrId];
            auto toNode = idToNode[edge.toAttrId];

            if (fromNode && toNode) {
                cv::Mat img = fromNode->getImage();
                if (img.empty()) {
                    // std::cerr << "[Graph] No image data from node " << fromNode->getName() << "\n";
                    continue;
                }
                // std::cout << "Passing image from " << fromNode->getName() << " to " << toNode->getName() << "\n";
                toNode->setInputImage(img);
            }
        }

        // Process each node
        for (auto& node : nodes) {
            node->process();
        }
    }

    std::vector<std::shared_ptr<Node>> getTopologicalOrder() {
        // TODO: implement proper topological sort based on edges
        return nodes;
    }
};
