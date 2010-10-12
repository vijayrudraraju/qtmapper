#ifndef UTILITY_H
#define UTILITY_H

#include "node.h"

const char* device_search_term = "";

bool nodeSortInputsFunction( Node* first, Node* second ) {

    return ( (int)(first->inputs) < (int)(second->inputs) );

}

bool nodeSortOutputsFunction( Node* first, Node* second ) {

    return ( (int)(first->outputs) < (int)(second->outputs) );

}

bool isNameMatch( Node* i ) {

    //printf( "IsNameMatch %s %s\n", i->name, device_search_term);
    return !strcmp( i->name, device_search_term );

}


#endif // UTILITY_H
