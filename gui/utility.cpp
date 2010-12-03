#include "utility.h"

const char* Utility::device_search_term;
const char* Utility::mapping_device_term;
const char* Utility::mapping_dest_signal_term;
const char* Utility::mapping_source_signal_term;

mapper_db_device Utility::device_search_struct;
const char* Utility::device_search_struct_name;
mapper_db_signal Utility::signal_search_struct;
mapper_db_mapping Utility::mapping_search_struct;

bool Utility::nodeSortInputsFunction( Node* first, Node* second ) {

    return ( (int)(first->inputs) < (int)(second->inputs) );

}

bool Utility::nodeSortOutputsFunction( Node* first, Node* second ) {

    return ( (int)(first->outputs) < (int)(second->outputs) );

}

bool Utility::findDbDevice( mapper_db_device i ) {

    return !strcmp( i->name, device_search_struct->name );

}

bool Utility::findDbDeviceByName( mapper_db_device i ) {

    return !strcmp( i->name, device_search_struct_name );

}

bool Utility::findDbSignal( mapper_db_signal i ) {

    return !strcmp( i->name, signal_search_struct->name );

}

bool Utility::findDbSignalPair( std::pair<mapper_db_device, mapper_db_signal> i ) {

    return !strcmp( i.second->name, signal_search_struct->name );

}

bool Utility::findDbMapping( mapper_db_mapping i ) {

    return !( strcmp(i->src_name, mapping_search_struct->src_name ) &&
              strcmp(i->dest_name, mapping_search_struct->dest_name ) );

}

bool Utility::isNameMatch( Node* i ) {

    //printf( "IsNameMatch %s %s\n", i->name, device_search_term);
    return !strcmp( i->name, device_search_term );

}

bool Utility::findMapping( qt_mapping i ) {

    if ( !strcmp( i->destination_node->name, mapping_device_term ) &&
         !strcmp( i->src_name, mapping_source_signal_term ) &&
         !strcmp( i->dest_name, mapping_dest_signal_term ) ) {

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
    double neg_recip = 1 / slope;
    //double y_inter = mid_y - ( mid_x * neg_recip );

    double x_change = abs( sqrt(pow(concavity, 2) / (1 + pow(neg_recip, 2))) );
    double y_change = abs( neg_recip * x_change );

    if ( p_2_y - p_1_y == 0 ) {

        x_change = 0;
        y_change = abs(concavity);

    }

    if ( p_2_x - p_1_x == 0 ) {

        x_change = abs(concavity);
        y_change = 0;

    }

    int direction;

    if ( p_2_x - p_1_x >= 0 ) {

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
