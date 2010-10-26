#include "utility.h"

const char* Utility::device_search_term;
const char* Utility::mapping_device_term;
const char* Utility::mapping_dest_signal_term;
const char* Utility::mapping_source_signal_term;

bool Utility::nodeSortInputsFunction( Node* first, Node* second ) {

    return ( (int)(first->inputs) < (int)(second->inputs) );

}

bool Utility::nodeSortOutputsFunction( Node* first, Node* second ) {

    return ( (int)(first->outputs) < (int)(second->outputs) );

}

bool Utility::isNameMatch( Node* i ) {

    //printf( "IsNameMatch %s %s\n", i->name, device_search_term);
    return !strcmp( i->name, device_search_term );

}

bool Utility::findMapping( qt_mapping i ) {

    if ( !strcmp( i->destination_node->name, mapping_device_term ) &&
         !strcmp( i->source_signal_name, mapping_source_signal_term ) &&
         !strcmp( i->destination_signal_name, mapping_dest_signal_term ) ) {

        return true;

    } else {

        return false;

    }

}

void Utility::findQuadPoint( double p_1_x, double p_1_y,
                            double p_2_x, double p_2_y,
                            double *quad_x, double *quad_y,
                            double concavity ) {

    double mid_x = ( p_1_x + p_2_x ) / 2;
    double mid_y = ( p_1_y + p_2_y ) / 2;
    double slope = ( p_2_y - p_1_y ) / ( p_2_x - p_1_x );
    double y_inter = mid_y - ( mid_x * slope );

    double x_change = abs( sqrt(pow(concavity, 2) / (1 + pow(slope, 2))) );
    double y_change = abs( slope * x_change );

    int direction;

    if ( p_2_x - p_1_x > 0 ) {

        if ( p_2_y - p_1_y >= 0 ) {

            direction = 1;

        } else {

            direction = 2;

        }

    } else if ( p_2_x - p_1_x < 0 ) {

        if ( p_2_y - p_1_y >= 0 ) {

            direction = 4;

        } else {

            direction = 3;

        }

    }

    // y = mx + b
    // 1, 2, 3, 4 -> quadrants in diagonal direction

    if ( direction == 1 ) {

        if ( concavity > 0 ) {

            *quad_x = mid_x + x_change;
            *quad_y = mid_y - y_change;

        } else {

            *quad_x = mid_x - x_change;
            *quad_y = mid_y + y_change;

        }

    } else if ( direction == 2 ) {

        if ( concavity > 0 ) {

            *quad_x = mid_x - x_change;
            *quad_y = mid_y - y_change;

        } else {

            *quad_x = mid_x + x_change;
            *quad_y = mid_y + y_change;

        }

    } else if ( direction == 3 ) {

        if ( concavity > 0 ) {

            *quad_x = mid_x - x_change;
            *quad_y = mid_y + y_change;

        } else {

            *quad_x = mid_x + x_change;
            *quad_y = mid_y - y_change;

        }

    } else if ( direction == 4 ) {

        if ( concavity > 0 ) {

            *quad_x = mid_x + x_change;
            *quad_y = mid_y + y_change;

        } else {

            *quad_x = mid_x - x_change;
            *quad_y = mid_y - y_change;

        }

    }

}
