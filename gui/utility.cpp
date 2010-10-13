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
