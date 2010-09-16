#ifndef LINK_H
#define LINK_H

#include "node.h"

class Link
{

public:
    Edge(Node *sourceNode, Node *destNode);
    ~Edge();

private:
    Node *source, *dest;

};

#endif // LINK_H
