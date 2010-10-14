#define DEBUG

#include <stdio.h>
#include <math.h>
#include <lo/lo.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <vector>
#include <pthread.h>

#include <QtGui/QApplication>

#include <mapper/mapper.h>
#include <mapper/mapper_types.h>

#include "form.h"

mapper_monitor qtmapper = 0;
int default_port = 9000;

int setup_qtmapper(  ) {

    qtmapper = mapper_monitor_new();

    if ( !qtmapper ) {

        return 1;

    }

    printf( "qtmapper created\n" );

    return 0;

}
void cleanup_qtmapper(  ) {

    if ( qtmapper ) {

        printf( "freeing qtmapper..." );
        mapper_monitor_free( qtmapper );
        printf( "ok\n" );

    }

}

Form* form;

void dbDeviceCallbackFunction( mapper_db_device record,
                                mapper_db_action_t action,
                                void* user ) {

    printf( "Form::db_device_callback_function( ... )\n" );
    printf( "record->name %s action %d user %p \n",
            record->name, action, user );

    if ( action == MDB_NEW ) {

        form->addNewDevice( record->name,
                            record->host,
                            record->port,
                            record->canAlias );
        mapper_monitor_request_signals_by_name( qtmapper, record->name );
        mapper_monitor_request_links_by_name( qtmapper, record->name );
        mapper_monitor_request_mappings_by_name( qtmapper, record->name );

    } else if ( action == MDB_REMOVE ) {

        form->removeDevice( record->name );

    }

}

void dbSignalCallbackFunction( mapper_db_signal record,
                                mapper_db_action_t action,
                                void* user ) {

    printf( "Form::db_signal_callback_function( ... )\n" );
    printf( "record->name %s action %d user %p \n",
            record->name, action, user );
    if ( action == MDB_NEW ) {

        form->addNewSignal( record );

    } else if ( action == MDB_REMOVE ) {

        form->removeSignal( record );

    }

}

void dbLinkCallbackFunction( mapper_db_link record,
                                mapper_db_action_t action,
                                void* user ) {

    printf( "Form::db_link_callback_function( ... )\n" );
    printf( "record->src %s record->dest %s action %d user %p \n",
            record->src_name, record->dest_name, action, user );

}

void dbMappingCallbackFunction( mapper_db_mapping record,
                                mapper_db_action_t action,
                                void* user ) {

    printf( "Form::db_mapping_callback_function( ... )\n" );
    printf( "record->src %s record->dest %s action %d user %p \n",
            record->src_name, record->dest_name, action, user );
    if ( action == MDB_NEW ) {

        form->addNewMapping( record );

    } else if ( action == MDB_REMOVE ) {

        form->removeMapping( record );

    }

}

//const char* Form::device_search_term = "";

int main( int argc, char *argv[] ) {

    if ( setup_qtmapper(  ) ) {

        printf( "error initializing qtmapper\n" );
        cleanup_qtmapper(  );
        return 1;

    }

    QApplication app(argc, argv);

    form = new Form(  );
    form->setMapperMonitor( qtmapper );

    form->addDbDeviceCallbackFunction( dbDeviceCallbackFunction );
    form->addDbSignalCallbackFunction( dbSignalCallbackFunction );
    form->addDbLinkCallbackFunction( dbLinkCallbackFunction );
    form->addDbMappingCallbackFunction( dbMappingCallbackFunction );

    form->show();

    //add_new_mapping( "/qtmapper.1/outsig", "/qttestrecv.1/insig" );

    app.exec();

    return 0;

}
