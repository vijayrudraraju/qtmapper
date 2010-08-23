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
#include "ui_form.h"

mapper_device qtmapper = 0;
int default_port = 9000;

int setup_qtmapper(  ) {

    qtmapper = mdev_new( "qtmapper", default_port );

    if ( !qtmapper ) {

        return 1;

    }

    printf( "qtmapper created\n" );

    return 0;

}
void cleanup_qtmapper(  ) {

    if ( qtmapper ) {

        printf( "freeing qtmapper..." );
        mdev_free( qtmapper );
        printf( "ok\n" );

    }

}

Form* form;

void dbCallbackFunction( mapper_db_device record,
                                mapper_db_action_t action,
                                void* user ) {

    printf( "Form::db_callback_function( ... )\n" );
    printf( "record->name %s action %d user %p \n",
            record->name, action, user );

    if ( action == MDB_NEW ) {

        form->addNewDevice( record->name,
                            record->host,
                            record->port,
                            record->canAlias );

    }

}

int main( int argc, char *argv[] ) {

    if ( setup_qtmapper(  ) ) {

        printf( "error initializing qtmapper\n" );
        cleanup_qtmapper(  );
        return 1;

    }

    QApplication app(argc, argv);

    form = new Form(  );
    form->setMapperDevice( qtmapper );
    form->addDbCallbackFunction( dbCallbackFunction );
    form->show();

    app.exec();

    return 0;

}
