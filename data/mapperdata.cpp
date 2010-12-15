
#include <algorithm>
#include "mapperdata.h"

MapperData::MapperData() {

}

void MapperData::modifyMappingData( mapper_db_mapping record ) {

    std::list<mapper_db_mapping>::iterator dbit;
    Utility::mapping_search_struct = record;
    std::replace_if( this->mapping_struct_list.begin(),
                        this->mapping_struct_list.end(),
                        Utility::findDbMapping,
                        record );

}



void MapperData::addDeviceData( mapper_db_device record ) {

    this->removeDeviceData( record );

    std::list<mapper_db_device>::iterator dbit;
    Utility::device_search_struct = record;
    dbit = std::find_if( this->device_struct_list.begin(),
                    this->device_struct_list.end(),
                    Utility::findDbDevice );

    if ( dbit == this->device_struct_list.end() ) {

        this->device_struct_list.push_back( record );

    } else {

        std::replace_if( this->device_struct_list.begin(),
                        this->device_struct_list.end(),
                        Utility::findDbDevice,
                        record );

    }

}
void MapperData::addSignalData( mapper_db_signal record ) {

    this->removeSignalData( record );

    std::list<mapper_db_device>::iterator it;
    Utility::device_search_struct_name = record->device_name;
    it = std::find_if( this->device_struct_list.begin(),
                        this->device_struct_list.end(),
                        Utility::findDbDeviceByName );

    this->signal_struct_map.insert(
            std::pair<mapper_db_device, mapper_db_signal>((*it), record) );

}
void MapperData::addMappingData( mapper_db_mapping record ) {

    this->removeMappingData( record );
    this->mapping_struct_list.push_back( record );

}



void MapperData::updateAllMappingPairs(  ) {

    this->mapping_struct_map.clear();

    std::list<mapper_db_mapping>::iterator it;
    for ( it = this->mapping_struct_list.begin();
            it != this->mapping_struct_list.end();
            it++ ) {

        this->updateMappingPair( *it );

    }

}
void MapperData::updateMappingPair( mapper_db_mapping record ) {

    printf( "\nMapperData::updateMappingPair src_name %s, dest_name %s\n",
            record->src_name,
            record->dest_name );

    QString str = record->src_name;
    QStringList parsed_str = str.split( "/", QString::SkipEmptyParts );

    QString str_2 = record->dest_name;
    QStringList parsed_str_2 = str_2.split( "/", QString::SkipEmptyParts );

    parsed_str[0].prepend("/");
    parsed_str_2[0].prepend("/");
    parsed_str[1].prepend("/");
    parsed_str_2[1].prepend("/");

    for ( QStringList::iterator it = parsed_str.begin() + 2;
          it != parsed_str.end();
          it++ ) {

        it->prepend("/");
        parsed_str[1].append( (*it) );

    }

    for ( QStringList::iterator it = parsed_str_2.begin() + 2;
            it != parsed_str_2.end();
            it++ ) {

        it->prepend("/");
        parsed_str_2[1].append( (*it) );

    }

    std::list<mapper_db_device>::iterator it;
    Utility::device_search_struct_name =
            strdup( parsed_str[0].toLatin1().constData() );
    it = std::find_if( this->device_struct_list.begin(),
                        this->device_struct_list.end(),
                        Utility::findDbDeviceByName );

    std::multimap<mapper_db_device, mapper_db_signal>::iterator itt;

    mapping_pair map_pair = (mapping_pair)calloc( 1, sizeof(struct _mapping_pair) );

    std::pair<std::multimap<mapper_db_device, mapper_db_signal>::iterator,
            std::multimap<mapper_db_device, mapper_db_signal>::iterator> ret;
    ret = signal_struct_map.equal_range( (*it) );
    for ( itt = ret.first; itt != ret.second; ++itt ) {

        if ( !strcmp((*itt).second->name,
                     parsed_str[1].toLatin1().constData())) {

            map_pair->source_device = (*itt).first;
            map_pair->source_signal = (*itt).second;
            break;

        }

    }

    Utility::device_search_struct_name =
            strdup( parsed_str_2[0].toLatin1().constData() );
    it = std::find_if( this->device_struct_list.begin(),
                        this->device_struct_list.end(),
                        Utility::findDbDeviceByName );
    //printf("device name %s\n", (*it)->name);

    ret = signal_struct_map.equal_range( (*it) );
    itt = ret.first;
    //printf("signal name %s\n", (*itt).second->name);
    for ( itt = ret.first; itt != ret.second; ++itt ) {

        if ( !strcmp((*itt).second->name,
                     parsed_str_2[1].toLatin1().constData())) {

            map_pair->dest_device = (*itt).first;
            map_pair->dest_signal = (*itt).second;
            break;

        }

    }

    if ( map_pair->dest_device != 0 && map_pair->source_device != 0 ) {
        this->mapping_struct_map.insert(
                std::pair<mapping_pair, mapper_db_mapping>(map_pair, record) );
    }

}



void MapperData::removeDeviceData( mapper_db_device record ) {

    std::list<mapper_db_device>::iterator dbit;
    Utility::device_search_struct = record;
    std::remove_if( this->device_struct_list.begin(),
                    this->device_struct_list.end(),
                    Utility::findDbDevice );

}
void MapperData::removeSignalData( mapper_db_signal record ) {

    std::list<mapper_db_device>::iterator it;
    Utility::device_search_struct_name = record->device_name;
    it = std::find_if( this->device_struct_list.begin(),
                        this->device_struct_list.end(),
                        Utility::findDbDeviceByName );

    //std::multimap<mapper_db_device, mapper_db_signal>::iterator multi_it;
    //multi_it = this->signal_struct_map.find( (*it) );

    std::pair<std::multimap<mapper_db_device, mapper_db_signal>::iterator,
        std::multimap<mapper_db_device, mapper_db_signal>::iterator> ret;
    std::multimap<mapper_db_device, mapper_db_signal>::iterator multi_it;
    ret = this->signal_struct_map.equal_range( (*it) );

    Utility::signal_search_struct = record;
    multi_it = std::find_if( ret.first,
                             ret.second,
                             Utility::findDbSignalPair );

    if ( multi_it != ret.second ) {

        this->signal_struct_map.erase( multi_it );

    }

}
void MapperData::removeMappingData( mapper_db_mapping record ) {

    printf( "\nMapperData::removeMappingPair src_name %s, dest_name %s\n",
            record->src_name,
            record->dest_name );

    printf( "before mapping_struct_list size %d\n", this->mapping_struct_list.size() );

    std::list<mapper_db_mapping>::iterator dbit;
    Utility::mapping_search_struct = record;
    this->mapping_struct_list.remove_if( Utility::findDbMapping );

    printf( "after mapping_struct_list size %d\n", this->mapping_struct_list.size() );

}

