#ifndef UTILITY_H
#define UTILITY_H

#include "node.h"
#include <math.h>

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

    static void findQuadPoint( double p_1_x, double p_1_y,
                               double p_2_x, double p_2_y,
                               double* quad_x, double* quad_y,
                               double concavity );

};

#endif // UTILITY_H
