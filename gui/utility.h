#ifndef UTILITY_H
#define UTILITY_H

#include "node.h"

bool nodeSortInputsFunction( Node* first, Node* second ) {

    return ( (int)(first->inputs) < (int)(second->inputs) );

}

bool nodeSortOutputsFunction( Node* first, Node* second ) {

    return ( (int)(first->outputs) < (int)(second->outputs) );

}

#endif // UTILITY_H
