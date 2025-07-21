#include "Node.h"

int Node::generateUniqueId() {
    static int counter = 10000;
    return counter++;
}
