#include "guithread.h"

GuiThread::GuiThread(QObject *parent) :
    QThread(parent)
{
}

void update( mapper_db_device device_db_cache ) {

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

                emit updatedGui( cache_iterator->name );

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
