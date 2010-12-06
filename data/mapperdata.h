#ifndef MAPPERDATA_H
#define MAPPERDATA_H

#include <list>
#include <map>

#include <mapper/mapper_db.h>

#include "utility.h"

typedef struct _mapping_pair {

    mapper_db_device source_device;
    mapper_db_signal source_signal;

    mapper_db_device dest_device;
    mapper_db_signal dest_signal;

} *mapping_pair;

class MapperData {

public:
    MapperData();

    std::list<mapper_db_device> device_struct_list;
    std::list<mapper_db_mapping> mapping_struct_list;

    std::multimap<mapping_pair, mapper_db_mapping> mapping_struct_map;
    std::multimap<mapper_db_device, mapper_db_signal> signal_struct_map;

    void addDeviceData( mapper_db_device record );
    void addMappingData( mapper_db_mapping record );
    void updateAllMappings(  );
    void updateMappingPair( mapper_db_mapping record );
    void addSignalData( mapper_db_signal record );

    void removeDeviceData( mapper_db_device record );
    void removeMappingData( mapper_db_mapping record );
    void removeSignalData( mapper_db_signal record );

};

#endif // MAPPERDATA_H
