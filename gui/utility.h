#ifndef UTILITY_H
#define UTILITY_H

#include "node.h"

class Utility {
public:
    static const char* device_search_term;
    static const char* mapping_device_term;
    static const char* mapping_source_signal_term;
    static const char* mapping_dest_signal_term;

    static bool nodeSortInputsFunction( Node* first, Node* second );

    static bool nodeSortOutputsFunction( Node* first, Node* second );

    static bool isNameMatch( Node* i );

    static bool findMapping( qt_mapping i );
};

#endif // UTILITY_H
