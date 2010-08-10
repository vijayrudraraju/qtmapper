#define DEBUG

#include <QtGui/QApplication>
#include "form.h"

#include <mapper/mapper.h>
#include <mapper/mapper_types.h>
#include <stdio.h>
#include <math.h>
#include <lo/lo.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <vector>
#include <pthread.h>

int automate = 1;

//std::vector<mapper_device> device_list;


mapper_device qtmapper = 0;
//mapper_device sender = 0;
//mapper_device receiver = 0;
//mapper_db_device device_db_cache;

//mapper_signal mappersig = 0;
//mapper_signal sendsig = 0;
//mapper_signal recvsig = 0;

int default_port = 9000;

//int sent = 0;
//int received = 0;
//int mapped = 0;


/*
void* update_local_devices( void* ptr ) {

    while ( !mdev_ready(qtmapper) ) {

        printf( "\npoll qtmapper\n" );
        mdev_poll( qtmapper, 0 );
        usleep(500*1000);

    }

    mapper_db_device cache_iterator;

    int i = 0;
    while ( i >= 0 ) {

        mdev_poll( qtmapper, 0 );
        //device_db_cache =
        //        mapper_db_update_new_device_cache( device_db_cache );

        cache_iterator = device_db_cache;

        if ( cache_iterator ) {

            trace( "new registered devices:\n" );

        }

        while ( cache_iterator ) {

            trace(" cache_iterator name=%s, host=%s, port=%d\n",
                  cache_iterator->name, cache_iterator->host,
                  cache_iterator->port);
            cache_iterator =
                ((user_data_structure) cache_iterator->user_data)->
                next_new_device;

        }

        usleep(4000*1000);
        i++;

    }

}


void update_gui ( Form* form ) {

    mapper_db_device cache_iterator;
    mapper_db_device reset_device;

    int i = 0;
    while ( i >= 0 ) {

        if ( form->isVisible() ) {

            cache_iterator = device_db_cache;

            if ( cache_iterator ) {

                trace( "new gui nodes:\n" );

            }

            while ( cache_iterator ) {

                trace(" gui-cache_iterator name=%s, host=%s, port=%d\n",
                      cache_iterator->name, cache_iterator->host,
                      cache_iterator->port);

                form->addNewDevice( cache_iterator->name );

                reset_device = cache_iterator;
                cache_iterator =
                    ((user_data_structure) cache_iterator->user_data)->
                    next_new_device;
                ((user_data_structure) reset_device->user_data)->
                next_new_device = NULL;

            }

            device_db_cache = NULL;

        }

        usleep(4000*1000);
        i++;

    }

}


void* libmapper_main ( void* dummy ) {

    int result = 0;

    if ( setup_qtmapper() ) {

        printf( "error initializing mapper.\n" );
        result = 1;
        cleanup_qtmapper();
        return 0;

    }

    update_local_devices( (void*)0 );

}

void* gui_loop ( void* form ) {

    update_gui( (Form*) form );

}

void update(  ) {

    printf( "update(  )\n" );

}
*/

int setup_qtmapper(  ) {

    qtmapper = mdev_new( "qtmapper", default_port );

    if ( !qtmapper ) {

        return 1;

    }

    printf( "qtmapper created\n" );

    return 0;

}

void dbCallbackFunction(mapper_db_device record,
                                mapper_db_action_t action,
                                void* user) {

    printf( "Form::db_callback_function( ... )\n" );

}

int main(int argc, char *argv[]) {

    if ( setup_qtmapper(  ) ) {

        printf( "error initializing qtmapper\n" );
        return 1;

    }

    QApplication app(argc, argv);
    Form form;
    form.setMapperDevice( qtmapper );
    form.addDbCallbackFunction( dbCallbackFunction );
    form.show();

    app.exec();

    //pthread_t thread1, thread2;
    //int iret1, iret2;

    //iret1 = pthread_create( &thread1, NULL, libmapper_main, (void*) 0 );
    //iret2 = pthread_create( &thread2, NULL, gui_loop, &form );

    return 0;

}
