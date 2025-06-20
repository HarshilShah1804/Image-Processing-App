#pragma once
#include <vector>
#include <string>
#include <memory>
#include <opencv2/imgproc.hpp>
#include <imgui.h>
#include "Node.h"
#include "nodes/ImageInputNode.h"
#include "nodes/GrayScaleNode.h"
#include "nodes/ImageOutputNode.h"

class Graph {
    public:
        int numNodes = 0;
        Graph() = default;
        ~Graph() = default;
        std::vector<std::shared_ptr<Node>> nodes;
        std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>> edges;
    
        void addNode(std::shared_ptr<Node> node) {
            node->id = numNodes++;
            nodes.push_back(node);
        }

        void addEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
            edges.emplace_back(from, to);
        }

        void renderAddNodeUI() {
            static int selectedNodeType = 0;
            const char* nodeTypes[] = { "Image Input", "Grayscale", "Image Output" };

            ImGui::Begin("Add Node");

            ImGui::Text("Select node type:");
            ImGui::Combo("##NodeType", &selectedNodeType, nodeTypes, IM_ARRAYSIZE(nodeTypes));

            if (ImGui::Button("Add Node")) {
                std::shared_ptr<Node> newNode;
                switch (selectedNodeType) {
                    case 0:
                        newNode = std::make_shared<ImageInputNode>();
                        break;
                    case 1:
                        newNode = std::make_shared<GrayScaleNode>();
                        break;
                    case 2:
                        newNode = std::make_shared<ImageOutputNode>();
                        break;
                    default:
                        std::cerr << "Unknown node type selected." << std::endl;
                }

                if (newNode) {
                    addNode(newNode);
                }
            }

            ImGui::End();
        }

        void renderUI() {
            // std::cout << "Number of nodes: " << nodes.size() << std::endl;
            for (const auto& node : nodes) {
                node->renderUI();
            }
            // Additional rendering logic for edges can be added here
        }

        void process() {
            cv::Mat currImage;

            for (const auto& node : nodes) {
                const std::string& nodeName = node->getName();

                if (nodeName == "Image Input Node") {
                    // std::cout << "Processing Image Input Node: " << nodeName << std::endl;
                    currImage = node->getImage();
                }
                else if (nodeName == "GrayScale") {
                    // std::cout << "Processing Grayscale Node: " << nodeName << std::endl;

                    // Downcast to GrayScaleNode
                    auto grayNode = std::dynamic_pointer_cast<GrayScaleNode>(node);
                    if (grayNode) {
                        grayNode->setInputImage(currImage);
                        grayNode->process();
                        currImage = grayNode->getImage();
                    } else {
                        std::cerr << "Error: Node is labeled Grayscale but not of GrayScaleNode type\n";
                    }
                } else if (nodeName == "Image Output Node") {
                    // std::cout << "Processing Image Output Node: " << nodeName << std::endl;
                    
                    // Downcast to ImageOutputNode
                    auto outputNode = std::dynamic_pointer_cast<ImageOutputNode>(node);
                    if (outputNode) {
                        outputNode->setInputImage(currImage);
                        outputNode->process();
                    } else {
                        std::cerr << "Error: Node is labeled Image Output but not of ImageOutputNode type\n";
                    }
                }
                else {
                    std::cerr << "Unknown node type: " << nodeName << std::endl;
                }
            }
        }


        std::vector<std::shared_ptr<Node>> getTopologicalOrder() {
            // Placeholder for topological sorting logic
            // This should return nodes in a valid processing order
            return nodes; // For now, just return the nodes as is
        }
};
