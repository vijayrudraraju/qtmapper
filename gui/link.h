#ifndef LINK_H
#define LINK_H

#include "node.h"

class Link
{

public:
    Link(  );
    ~Link(  );

private:
    Node *source, *dest;

};

#endif // LINK_H