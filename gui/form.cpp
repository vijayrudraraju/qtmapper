#include "form.h"
#include "utility.h"

#include <QGraphicsScene>
#include <QWheelEvent>

#include <cmath>
#include <sstream>
#include <algorithm>

Form::Form( QWidget *parent ) :
    QWidget( parent ) {

    setupUi( this );

    this->database = new MapperData(  );

    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
    timer->start( 1000 );

    scene = new MapperGraphicsScene( graphics_view );
    graphics_view->setScene( scene );
    graphics_view->setRenderHint( QPainter::Antialiasing );
    graphics_view->scale( qreal(0.7), qreal(0.7) );
    graphics_view->setMinimumSize( 600, 300 );
    graphics_view->setAlignment( Qt::AlignLeft|Qt::AlignTop );
    graphics_view->setSceneRect( -400, -400, 4000, 4000 );
    //scene->setItemIndexMethod( QGraphicsScene::NoIndex );
    //scene->setStickyFocus( true );
    //graphics_view->
    //        setCacheMode( QGraphicsView::CacheBackground );
    //graphics_view->
     //       setViewportUpdateMode( QGraphicsView::FullViewportUpdate );

    graphics_view_2->setScene( &mapping_scene );
    graphics_view_2->installEventFilter( this );
    graphics_view_2->setAlignment( Qt::AlignLeft|Qt::AlignTop );

    QStringList header_labels;
    header_labels << "signal name" << "type" << "units" << "min" << "max";

    displayed_source_model = new QStandardItemModel( 0, 6 );
    displayed_source_model->setHorizontalHeaderLabels( header_labels );
    source_list->setModel( displayed_source_model );
    source_list->setMinimumSize( 600, 200 );

    displayed_dest_model = new QStandardItemModel( 0, 6 );
    displayed_dest_model->setHorizontalHeaderLabels( header_labels );
    destination_list->setModel( displayed_dest_model );
    destination_list->setMinimumSize( 600, 200 );

    source_signal_list->setModel( displayed_source_model );
    dest_signal_list->setModel( displayed_dest_model );

    /*
    this->source_list->setColumnHidden( 4, true );
    this->destination_list->setColumnHidden( 4, true );
    this->source_signal_list->setColumnHidden( 3, true );
    this->destination_signal_list->setColumnHidden( 3, true );
    this->source_signal_list->setColumnHidden( 4, true );
    this->destination_signal_list->setColumnHidden( 4, true );
    */

    this->active_node_name = "";
    this->selected_source_circle = NULL;
    this->selected_dest_circle = NULL;

    this->signal_selected_flag = 0;

    connect( this->source_signal_list, SIGNAL(clicked(QModelIndex)),
             this, SLOT(beginToDrawMapping(QModelIndex)) );
    connect( this->dest_signal_list, SIGNAL(clicked(QModelIndex)),
             this, SLOT(beginToDrawMapping(QModelIndex)) );

    connect( this->source_signal_list, SIGNAL(clicked(QModelIndex)),
             this, SLOT(finishDrawingMapping(QModelIndex)) );
    connect( this->dest_signal_list, SIGNAL(clicked(QModelIndex)),
             this, SLOT(finishDrawingMapping(QModelIndex)) );

    connect( this->dest_signal_list, SIGNAL(collapsed(QModelIndex)),
             this, SLOT(updateMappingView()) );
    connect( this->source_signal_list, SIGNAL(collapsed(QModelIndex)),
             this, SLOT(updateMappingView()) );
    connect( this->source_signal_list, SIGNAL(expanded(QModelIndex)),
             this, SLOT(updateMappingView()) );
    connect( this->dest_signal_list, SIGNAL(expanded(QModelIndex)),
             this, SLOT(updateMappingView()) );

    connect( this->deleteButton, SIGNAL(toggled(bool)),
             this, SLOT(updateDeleteButtonState(bool)) );

    /*
    connect( this->vis_mode_toggle, SIGNAL(currentIndexChanged(int)),
             this, SLOT(changeVisualizationMode(int)) );

    connect( selection_mode_toggle, SIGNAL(currentChanged(int)),
             this, SLOT(updateSelectionMode(int)) );

    connect( edit_mode_toggle, SIGNAL(currentChanged(int)),
             this, SLOT(updateEditSelectionMode(int)) );

    connect( graphics_view, SIGNAL(mouseStateChanged(bool)),
            this, SLOT(updateMouseState(bool)) );
    */

    /*
    connect( this->clear_sources_button, SIGNAL(clicked()),
             this, SLOT(clearSources()));
    connect( this->clear_dests_button, SIGNAL(clicked()),
             this, SLOT(clearDests()));
    */

    QList<QString> mode_str;
    mode_str.append("");
    mode_str.append("bypass");
    mode_str.append("line");
    mode_str.append("expression");
    mode_str.append("calibrate");
    this->comboBox->addItems( mode_str );

    QString picker_str[7];
    picker_str[0] = "# of signals";
    picker_str[1] = "# of inputs";
    picker_str[2] = "# of outputs";
    picker_str[3] = "# of connections";
    picker_str[4] = "position x";
    picker_str[5] = "temperature";
    picker_str[6] = "update rate";

    for ( int i=0; i<7; i++ ) {

        this->x_param_picker->addItem( picker_str[i] );
        this->y_param_picker->addItem( picker_str[i] );

        this->size_param_picker->addItem( picker_str[i] );
        this->size_param_picker_2->addItem( picker_str[i] );

        this->sides_param_picker->addItem( picker_str[i] );
        this->sides_param_picker_2->addItem( picker_str[i] );

        this->concavity_param_picker->addItem( picker_str[i] );
        this->concavity_param_picker_2->addItem( picker_str[i] );

        this->color_param_picker->addItem( picker_str[i] );
        this->color_param_picker_2->addItem( picker_str[i] );

        this->dist_param_picker->addItem( picker_str[i] );

    }

    setWindowTitle( tr("libmapper monitor") );

}



void Form::clearSources() {

    /*
    for ( std::list<Node*>::iterator it = this->master_source_node_pointer_list.begin();
            it != this->master_source_node_pointer_list.end();
            it++ ) {

        if ( (*it)->is_source ) {

            (*it)->conflict_flag = 0;
            this->removeNodeFromSourceView( (*it) );

        }

    }
    */

}
void Form::clearDests() {

    /*
    for ( std::list<Node*>::iterator it = this->master_source_node_pointer_list.begin();
            it != this->master_source_node_pointer_list.end();
            it++ ) {

        if ( (*it)->is_destination ) {

            (*it)->conflict_flag = 0;
            this->removeNodeFromDestinationView( (*it) );

        }

    }
    */

}



void Form::updateIsDeletable( bool checked ) {

    if ( !checked ) {

        for ( std::list<Link*>::iterator it =
                this->displayed_mapping_list.begin();
                it != this->displayed_mapping_list.end();
                it++ ) {

            (*it)->is_deletable = false;

        }

    } else {

        for ( std::list<Link*>::iterator it =
                this->displayed_mapping_list.begin();
                it != this->displayed_mapping_list.end();
                it++ ) {

            (*it)->is_deletable = true;

        }

    }

}
void Form::updateDeleteButtonState( bool checked ) {

    this->updateIsDeletable( checked );

}



void Form::update(  ) {

    mapper_monitor_poll( this->mon, 0 );
    QWidget::update();

    this->dest_signal_list->setColumnWidth( 0, 180 );
    this->dest_signal_list->resizeColumnToContents( 1 );
    this->dest_signal_list->resizeColumnToContents( 2 );
    this->dest_signal_list->resizeColumnToContents( 3 );
    this->dest_signal_list->resizeColumnToContents( 4 );
    this->dest_signal_list->resizeColumnToContents( 5 );

    this->source_signal_list->setColumnWidth( 0, 180 );
    this->source_signal_list->resizeColumnToContents( 1 );
    this->source_signal_list->resizeColumnToContents( 2 );
    this->source_signal_list->resizeColumnToContents( 3 );
    this->source_signal_list->resizeColumnToContents( 4 );
    this->source_signal_list->resizeColumnToContents( 5 );

}



void Form::updateMouseState( bool is_pressed ) {

    printf( "form says -> mouse state %d\n", is_pressed );
    this->mouse_is_pressed = is_pressed;

    /*
    if ( !mouse_is_pressed ) {

        for ( std::list<Node*>::iterator it = master_source_node_pointer_list.begin();
                it != master_source_node_pointer_list.end();
                it++ ) {

            if ( (*it)->conflict_flag == -1 ) {

                (*it)->conflict_flag = 0;
                this->removeNodeFromDestinationView( (*it) );
                (*it)->is_source = true;

            } else if ( (*it)->conflict_flag == 1 ) {

                (*it)->conflict_flag = 0;
                this->removeNodeFromSourceView( (*it) );
                (*it)->is_destination = true;

            }

            (*it)->update();

        }

    }
    */

}
void Form::newDoubleClick( ) {

    /*
    for ( std::list<Node*>::iterator it = master_source_node_pointer_list.begin();
            it != master_source_node_pointer_list.end();
            it++ ) {

        if ( selection_mode_toggle->currentIndex() == 1 &&
                    (*it)->is_destination ) {

            this->removeNodeFromDestinationView( (*it) );

        } else if ( selection_mode_toggle->currentIndex() == 0 &&
                    (*it)->is_source ) {

            this->removeNodeFromSourceView( (*it) );

        }

    }
    */

}



void Form::beginToDrawMapping( const QModelIndex& index ) {

    printf( "begin valid %d %d %d\n",
            index.isValid(),
            index.parent().isValid(),
            index.parent().parent().isValid() );

    if ( !this->makeButton->isChecked() ) {

        return;

    }


    if ( (QStandardItemModel*)index.model() == this->displayed_source_model &&
         (this->signal_selected_flag == 1 || this->signal_selected_flag == 0) ) {

        printf("begin source side!\n");
        QPen pen_color;
        pen_color.setColor( Qt::yellow );
        QBrush brush_color;
        brush_color.setColor( Qt::yellow );
        brush_color.setStyle( Qt::SolidPattern );

        QRect source_signal_rect;
        source_signal_rect =
                source_signal_list->
                visualRect( index  );

        int source_vertical_offset =
                source_signal_list->header()->height();

        if ( this->selected_source_circle != NULL ) {
            this->mapping_scene.removeItem( this->selected_source_circle );
        }
        if ( index.isValid() && index.parent().isValid() ) {

            printf("begin source side is valid!\n");
            this->selected_source_circle =
            this->mapping_scene.addEllipse( -1 * (source_signal_rect.height()/2 - 1),
                             source_vertical_offset +
                             source_signal_rect.topLeft().y() + 1,
                             source_signal_rect.height() - 2,
                             source_signal_rect.height() - 2,
                             pen_color,
                             brush_color );

            this->selected_signal = index;
            this->signal_selected_flag = 1;

        } else {

            this->signal_selected_flag = 0;

        }

    } else if ( (QStandardItemModel*)index.model() == this->displayed_dest_model &&
                (this->signal_selected_flag == -1 || this->signal_selected_flag == 0) ) {

        printf("begin destination side!\n");
        QPen pen_color;
        pen_color.setColor( Qt::blue );
        QBrush brush_color;
        brush_color.setColor( Qt::blue );
        brush_color.setStyle( Qt::SolidPattern );

        QRect dest_signal_rect;
        dest_signal_rect =
                dest_signal_list->
                visualRect( index  );

        int dest_vertical_offset =
                dest_signal_list->header()->height();

        if ( this->selected_dest_circle != NULL ) {
            this->mapping_scene.removeItem(
                    this->selected_dest_circle
                    );
        }
        if ( index.isValid() && index.parent().isValid() ) {

            printf("begin destination side is valid!\n");
            this->selected_dest_circle =
                    this->mapping_scene.addEllipse(
                             graphics_view_2->width() - dest_signal_rect.height() + 4,
                             dest_vertical_offset +
                             dest_signal_rect.topLeft().y() + 1,
                             dest_signal_rect.height() - 2,
                             dest_signal_rect.height() - 2,
                             pen_color,
                             brush_color );

            this->selected_signal = index;
            this->signal_selected_flag = -1;

        } else {

            this->signal_selected_flag = 0;

        }

    }

}
void Form::finishDrawingMapping( const QModelIndex& index ) {

    printf( "finish valid %d %d %d\n",
            index.isValid(),
            index.parent().isValid(),
            index.parent().parent().isValid() );

    if ( !this->makeButton->isChecked() || !signal_selected_flag ) {

        return;

    }

    if ( (QStandardItemModel*)index.model() == this->displayed_source_model &&
         this->signal_selected_flag == -1 ) {

        printf("finish source side!\n");
        QString source_str = "";
        QString dest_str = "";

        QPen pen_color;
        pen_color.setColor( Qt::yellow );
        QBrush brush_color;
        brush_color.setColor( Qt::yellow );
        brush_color.setStyle( Qt::SolidPattern );

        QRect source_signal_rect;
        source_signal_rect =
                source_signal_list->
                visualRect( index  );

        int source_vertical_offset =
                source_signal_list->header()->height();

        if ( this->selected_source_circle != NULL ) {
            this->mapping_scene.removeItem( this->selected_source_circle );
        }

        if ( index.isValid() && index.parent().isValid() ) {

            printf("finish source side is valid!\n");
            this->selected_source_circle =
            this->mapping_scene.addEllipse( -1 * (source_signal_rect.height()/2 - 1),
                                     source_vertical_offset +
                                     source_signal_rect.topLeft().y() + 1,
                                     source_signal_rect.height() - 2,
                                     source_signal_rect.height() - 2,
                                     pen_color,
                                     brush_color );

            source_str.append( this->displayed_source_model->itemFromIndex( index.parent() )->text() );
            source_str.append( this->displayed_source_model->
                              itemFromIndex( index.parent().child(index.row(),0) )->text() );

            dest_str.append( this->displayed_dest_model->itemFromIndex( selected_signal.parent() )->text() );
            dest_str.append( this->displayed_dest_model->
                               itemFromIndex( selected_signal.parent().child(selected_signal.row(),0) )->
                               text() );

            printf( "new mapping between %s %s\n",
                    source_str.toLatin1().constData(),
                    dest_str.toLatin1().constData() );

            this->sendNewMappingRequest( source_str.toLatin1().constData(),
                                         dest_str.toLatin1().constData() );

            this->signal_selected_flag = 0;

        }

    } else if ( (QStandardItemModel*)index.model() == this->displayed_dest_model &&
                this->signal_selected_flag == 1 ) {

        printf("finish destination side!\n");
        QString source_str = "";
        QString dest_str = "";

        QPen pen_color;
        pen_color.setColor( Qt::blue );
        QBrush brush_color;
        brush_color.setColor( Qt::blue );
        brush_color.setStyle( Qt::SolidPattern );

        QRect dest_signal_rect;
        dest_signal_rect =
                dest_signal_list->
                visualRect( index  );

        int dest_vertical_offset =
                dest_signal_list->header()->height();

        if ( this->selected_dest_circle != NULL ) {
            this->mapping_scene.removeItem( this->selected_dest_circle );
        }

        if ( index.isValid() && index.parent().isValid() ) {

            printf("finish dest side is valid!\n");
            this->selected_dest_circle =
            this->mapping_scene.addEllipse(
                     graphics_view_2->width() - dest_signal_rect.height() + 4,
                     dest_vertical_offset +
                     dest_signal_rect.topLeft().y() + 1,
                     dest_signal_rect.height() - 2,
                     dest_signal_rect.height() - 2,
                     pen_color,
                     brush_color );

            dest_str.append( this->displayed_dest_model->itemFromIndex( index.parent() )->text() );
            dest_str.append( this->displayed_dest_model->
                              itemFromIndex( index.parent().child(index.row(),0) )->text() );

            source_str.append( this->displayed_source_model->itemFromIndex( selected_signal.parent() )->text() );
            source_str.append( this->displayed_source_model->
                               itemFromIndex( selected_signal.parent().child(selected_signal.row(),0) )->
                               text() );

            printf( "new mapping between %s %s\n",
                    source_str.toLatin1().constData(),
                    dest_str.toLatin1().constData() );

            this->sendNewMappingRequest( source_str.toLatin1().constData(),
                                         dest_str.toLatin1().constData() );
            this->signal_selected_flag = 0;

        }

    }

}
void Form::sendNewMappingRequest( const char* source_signal_path,
                                  const char* dest_signal_path ) {

    std::string src_dev = std::string(source_signal_path)
        .substr(0,std::string(source_signal_path).find('/', 1));

    std::string dest_dev = std::string(dest_signal_path)
        .substr(0,std::string(dest_signal_path).find('/', 1));

    mapper_monitor_link( mon, src_dev.c_str(), dest_dev.c_str() );
    mapper_monitor_connect( mon, source_signal_path,
                            dest_signal_path, 0, 0 );

}



void Form::clearMappingView( ) {

    if ( this->selected_source_circle != NULL ) {
        this->mapping_scene.removeItem( this->selected_source_circle );
    }
    if ( this->selected_dest_circle != NULL ) {
        this->mapping_scene.removeItem( this->selected_dest_circle );
    }
    mapping_scene.clear();
    this->displayed_mapping_list.clear();

}
// most complicated function tasked with drawing lines between signals that
// are mapped to each other
void Form::updateMappingView( ) {

    Link* new_link_pointer;

    QStandardItem* source_item_pointer;
    QStandardItem* dest_item_pointer;

    QStandardItem* source_signal_item_pointer = 0;
    QStandardItem* dest_signal_item_pointer = 0;

    QModelIndex source_signal_index;
    QModelIndex dest_signal_index;

    QRect source_signal_rect;
    QRect dest_signal_rect;

    QPen pen_width;
    pen_width.setWidth( 1 );

    int source_vertical_offset;
    int dest_vertical_offset;

    this->graphics_view_2->
            viewport()->setFixedSize( graphics_view_2->width(),
                                      graphics_view_2->height() );
    this->graphics_view_2->setSceneRect( 0, 0,
                                      graphics_view_2->width(),
                                      graphics_view_2->height() );
    this->clearMappingView( );
    this->database->updateAllMappings();

    const char* source_device_name;
    const char* source_signal_name;
    const char* dest_device_name;
    const char* dest_signal_name;

    std::multimap<mapping_pair, mapper_db_mapping>::iterator it;
    for ( it = this->database->mapping_struct_map.begin();
            it != this->database->mapping_struct_map.end();
            it++ ) {

        source_device_name = mapping_pair( (*it).first )->source_device->name;
        dest_device_name = mapping_pair( (*it).first )->dest_device->name;
        source_signal_name = mapping_pair( (*it).first )->source_signal->name;
        dest_signal_name = mapping_pair( (*it).first )->dest_signal->name;

        // grab source display info
        for ( int i = 0; i < displayed_source_model->rowCount( ); i++ ) {

            source_item_pointer =
                    this->displayed_source_model->item( i, 0 );

            if ( !strcmp(source_item_pointer->text().toLatin1().constData(),
                 source_device_name) ) {

                for ( int j = 0;
                      j < source_item_pointer->rowCount();
                      j++ ) {

                    source_signal_item_pointer =
                            source_item_pointer->child( j, 0 );

                    if ( !strcmp(source_signal_item_pointer->text().toLatin1().constData(),
                         source_signal_name) ) {

                        break;

                    }

                    source_signal_item_pointer = 0;

                }

                break;

            }

            source_item_pointer = 0;

        }

        // grab dest display info
        for ( int i = 0; i < displayed_dest_model->rowCount( ); i++ ) {

            dest_item_pointer =
                    this->displayed_dest_model->item( i, 0 );

            if ( !strcmp(dest_item_pointer->text().toLatin1().constData(),
                 dest_device_name) ) {

                for ( int j = 0;
                      j < dest_item_pointer->rowCount();
                      j++ ) {

                    dest_signal_item_pointer =
                            dest_item_pointer->child( j, 0 );

                    if ( !strcmp(dest_signal_item_pointer->text().toLatin1().constData(),
                         dest_signal_name) ) {

                        break;

                    }

                    dest_signal_item_pointer = 0;

                }

                break;

            }

            dest_item_pointer = 0;

        }

        // creating mapping display item (link)
        if ( source_signal_item_pointer != 0 &&
             dest_signal_item_pointer != 0 ) {

            source_signal_index =
                    displayed_source_model->
                    indexFromItem( source_signal_item_pointer );
            dest_signal_index =
                    displayed_dest_model->
                    indexFromItem( dest_signal_item_pointer );

            source_signal_rect =
                    source_signal_list->
                    visualRect( source_signal_index  );
            dest_signal_rect =
                    dest_signal_list->
                    visualRect( dest_signal_index  );

            if ( source_signal_rect.topLeft().y() == 0 ) {

                source_signal_rect =
                        source_signal_list->
                        visualRect( source_signal_index.parent() );

            }

            if ( dest_signal_rect.topLeft().y() == 0 ) {

                dest_signal_rect =
                        dest_signal_list->
                        visualRect( dest_signal_index.parent() );

            }

            source_vertical_offset =
                    source_signal_list->header()->height() +
                    ( source_signal_rect.height() / 2 ) + Link::pen_width;
            dest_vertical_offset =
                    dest_signal_list->header()->height() +
                    ( dest_signal_rect.height() / 2 ) + Link::pen_width;

            /*
            printf( "source signal y %d + offset %d\n",
                    source_signal_rect.topLeft().y(),
                    source_vertical_offset );
            printf( "destination signal y %d + offset %d\n",
                    dest_signal_rect.topLeft().y(),
                    dest_vertical_offset );
                    */


            new_link_pointer = new Link( this->graphics_view_2, &mapping_scene );
            new_link_pointer->mapping = (*it).second;
            new_link_pointer->setLine( 0,
                                       source_vertical_offset +
                                       source_signal_rect.topLeft().y(),
                                       graphics_view_2->width(),
                                       dest_vertical_offset +
                                       dest_signal_rect.topLeft().y() );

            strcpy( new_link_pointer->source_signal_name,
                    new_link_pointer->mapping->src_name );
            strcpy( new_link_pointer->dest_signal_name,
                    new_link_pointer->mapping->dest_name );

            this->displayed_mapping_list.push_back( new_link_pointer );
            QObject::connect( this->displayed_mapping_list.back(),
                              SIGNAL(linkPressed(Link*)),
                              this,
                              SLOT(updatePressedLink(Link*)));
            QObject::connect( this->displayed_mapping_list.back(),
                              SIGNAL(linkSelected(Link*)),
                              this,
                              SLOT(updateSelectedLink(Link*)));
            QObject::connect( this->displayed_mapping_list.back(),
                              SIGNAL(linkUnselected(Link*)),
                              this,
                              SLOT(updateUnselectedLink(Link*)));
            mapping_scene.addItem(new_link_pointer);

            source_signal_item_pointer = 0;
            dest_signal_item_pointer = 0;

        }

    }

}


void Form::removeMapping( mapper_db_mapping record ) {

    this->database->removeMappingData( record );

}
void Form::addNewMapping( mapper_db_mapping record ) {

    this->database->addMappingData( record );

}



void Form::removeSignal( mapper_db_signal record ) {

    this->database->removeSignalData( record );

}
void Form::addNewSignal( mapper_db_signal record ) {

    this->database->addSignalData( record );

    this->clearSourceSignalListDisplay();
    this->updateSourceSignalListDisplay();

    this->clearDestSignalListDisplay();
    this->updateDestSignalListDisplay();

}



void Form::removeDevice( mapper_db_device record ) {

    this->database->removeDeviceData( record );

}
void Form::addNewDevice( mapper_db_device record ) {

    this->database->addDeviceData( record );

    this->clearSourceDeviceListDisplay();
    this->clearDestDeviceListDisplay();
    this->updateSourceDeviceListDisplay();
    this->updateDestDeviceListDisplay();

}



void Form::clearSourceDeviceListDisplay() {

    this->displayed_source_model->clear();
    QStringList header_labels;
    header_labels << "signal name" << "type" << "units" << "min" << "max";
    displayed_source_model->setHorizontalHeaderLabels( header_labels );

}
void Form::updateSourceDeviceListDisplay() {

    std::list<mapper_db_device>::iterator it;
    std::stringstream out;

    for ( it = this->database->device_struct_list.begin();
            it != this->database->device_struct_list.end();
            it++ ) {

        QList<QStandardItem*> item_list;

        QStandardItem* item_1 = new QStandardItem;
        QStandardItem* item_2 = new QStandardItem;
        QStandardItem* item_3 = new QStandardItem;
        QStandardItem* item_4 = new QStandardItem;

        item_1->setText( (*it)->name );
        item_2->setText( (*it)->host );
        out << (*it)->port;
        item_3->setText( out.str().c_str() );
        out.str( "" );
        out << false;
        item_4->setText( out.str().c_str() );
        out.str( "" );

        item_list << item_1 << item_2 << item_3 << item_4;

        QModelIndex dummy_index;
        this->displayed_source_model->
            appendRow( item_list );
        this->source_signal_list->
            setFirstColumnSpanned( displayed_source_model->rowCount()-1,
                                   dummy_index, true );
        /*
        source_signal_list->expand(
                    displayed_source_model->
                    indexFromItem( item_list.first() )
                    );*/

    }

}
void Form::clearDestDeviceListDisplay() {

    this->displayed_dest_model->clear();
    QStringList header_labels;
    header_labels << "signal name" << "type" << "units" << "min" << "max";
    displayed_dest_model->setHorizontalHeaderLabels( header_labels );

}
void Form::updateDestDeviceListDisplay() {

    std::list<mapper_db_device>::iterator it;
    std::stringstream out;

    for ( it = this->database->device_struct_list.begin();
            it != this->database->device_struct_list.end();
            it++ ) {

        QList<QStandardItem*> item_list;

        QStandardItem* item_1 = new QStandardItem;
        QStandardItem* item_2 = new QStandardItem;
        QStandardItem* item_3 = new QStandardItem;
        QStandardItem* item_4 = new QStandardItem;

        item_1->setText( (*it)->name );
        item_2->setText( (*it)->host );
        out << (*it)->port;
        item_3->setText( out.str().c_str() );
        out.str( "" );
        out << false;
        item_4->setText( out.str().c_str() );
        out.str( "" );

        item_list << item_1 << item_2 << item_3 << item_4;

        QModelIndex dummy_index;
        this->displayed_dest_model->
            appendRow( item_list );
        this->dest_signal_list->
            setFirstColumnSpanned( displayed_dest_model->rowCount()-1,
                                   dummy_index, true );
        /*
        dest_signal_list->expand(
                    displayed_dest_model->
                    indexFromItem( item_list.first() )
                    );*/

    }

}


void Form::clearSourceSignalListDisplay() {

    int row_count = 0;

    for ( int i = 0; i < this->displayed_source_model->rowCount(); i++ ) {

        row_count = this->displayed_source_model->item( i, 0 )->rowCount();
        this->displayed_source_model->item( i, 0 )->removeRows( 0, row_count );

    }

}
void Form::updateSourceSignalListDisplay() {

    std::multimap<mapper_db_device, mapper_db_signal>::iterator it;
    std::stringstream out;
    int row_count_before_add = 0;

    for ( it = this->database->signal_struct_map.begin();
            it != this->database->signal_struct_map.end();
            it++ ) {

        QStandardItem* item_1 = new QStandardItem;
        item_1->setText( (*it).second->name );

        QStandardItem* item_2 = new QStandardItem;
        out.str( "" );
        out << (*it).second->type;
        item_2->setText( out.str().c_str() );

        QStandardItem* item_3 = new QStandardItem;
        item_3->setText( (*it).second->unit );

        QStandardItem* item_4 = new QStandardItem;
        out.str( "" );
        if ( item_2->text() == "f" ) {
            out << (*it).second->minimum->f;
        }
        item_4->setText( out.str().c_str() );

        QStandardItem* item_5 = new QStandardItem;
        out.str( "" );
        if ( item_2->text() == "f" ) {
            out << (*it).second->maximum->f;
        }
        item_5->setText( out.str().c_str() );

        for ( int i = 0; i < this->displayed_source_model->rowCount(); i++ ) {

            if ( !strcmp(this->displayed_source_model->item( i, 0 )->text().toLatin1().constData(),
                         (*it).first->name) ) {

                if ( (*it).second->is_output == 1 ) {

                    row_count_before_add =
                            this->displayed_source_model->item( i, 0 )->rowCount();

                    this->displayed_source_model->item( i, 0 )->setChild( row_count_before_add,
                                                           0,
                                                           item_1 );

                    this->displayed_source_model->item( i, 0 )->setChild( row_count_before_add,
                                                           1,
                                                           item_2 );

                    this->displayed_source_model->item( i, 0 )->setChild( row_count_before_add,
                                                           2,
                                                           item_3 );

                    this->displayed_source_model->item( i, 0 )->setChild( row_count_before_add,
                                                           3,
                                                           item_4 );

                    this->displayed_source_model->item( i, 0 )->setChild( row_count_before_add,
                                                           4,
                                                           item_5 );

                }

            }

        }

    }

}
void Form::clearDestSignalListDisplay() {

    int row_count = 0;

    for ( int i = 0; i < this->displayed_dest_model->rowCount(); i++ ) {

        row_count = this->displayed_dest_model->item( i, 0 )->rowCount();
        this->displayed_dest_model->item( i, 0 )->removeRows( 0, row_count );

    }

}
void Form::updateDestSignalListDisplay() {

    std::multimap<mapper_db_device, mapper_db_signal>::iterator it;
    std::stringstream out;
    int row_count_before_add = 0;

    for ( it = this->database->signal_struct_map.begin();
            it != this->database->signal_struct_map.end();
            it++ ) {

        QStandardItem* item_1 = new QStandardItem;
        item_1->setText( (*it).second->name );

        QStandardItem* item_2 = new QStandardItem;
        out.str( "" );
        out << (*it).second->type;
        item_2->setText( out.str().c_str() );

        QStandardItem* item_3 = new QStandardItem;
        item_3->setText( (*it).second->unit );

        QStandardItem* item_4 = new QStandardItem;
        out.str( "" );
        if ( item_2->text() == "f" ) {
            out << (*it).second->minimum->f;
        }
        item_4->setText( out.str().c_str() );

        QStandardItem* item_5 = new QStandardItem;
        out.str( "" );
        if ( item_2->text() == "f" ) {
            out << (*it).second->maximum->f;
        }
        item_5->setText( out.str().c_str() );

        for ( int i = 0; i < this->displayed_dest_model->rowCount(); i++ ) {

            if ( !strcmp(this->displayed_dest_model->item( i, 0 )->text().toLatin1().constData(),
                         (*it).first->name) ) {

                if ( (*it).second->is_output == 0 ) {

                    row_count_before_add =
                            this->displayed_dest_model->item( i, 0 )->rowCount();

                    this->displayed_dest_model->item( i, 0 )->setChild( row_count_before_add,
                                                           0,
                                                           item_1 );

                    this->displayed_dest_model->item( i, 0 )->setChild( row_count_before_add,
                                                           1,
                                                           item_2 );

                    this->displayed_dest_model->item( i, 0 )->setChild( row_count_before_add,
                                                           2,
                                                           item_3 );

                    this->displayed_dest_model->item( i, 0 )->setChild( row_count_before_add,
                                                           3,
                                                           item_4 );

                    this->displayed_dest_model->item( i, 0 )->setChild( row_count_before_add,
                                                           4,
                                                           item_5 );

                }

            }

        }

    }

}


void Form::updateLinkParameterDisplay( Link *reference ) {

    this->checkBox->setChecked( reference->mapping->muted );

    this->comboBox->setCurrentIndex( reference->mapping->mode );

    this->lineEdit->setText( reference->mapping->expression );
    this->lineEdit_1->setText( QString("%1").arg(reference->mapping->range.src_min) );
    this->lineEdit_2->setText( QString("%1").arg(reference->mapping->range.src_max) );
    this->lineEdit_3->setText( QString("%1").arg(reference->mapping->range.dest_min) );
    this->lineEdit_4->setText( QString("%1").arg(reference->mapping->range.dest_max) );

}
void Form::clearLinkParameterDisplay() {

    this->checkBox->setChecked( 0 );

    this->comboBox->setCurrentIndex( 0 );

    this->lineEdit->setText( "" );
    this->lineEdit_1->setText( "" );
    this->lineEdit_2->setText( "" );
    this->lineEdit_3->setText( "" );
    this->lineEdit_4->setText( "" );

}


/*
void Form::addNodeToDestinationView( Node* the_node ) {

    QModelIndex dummy_index;
    displayed_dest_model->
        appendRow( the_node->dest_model_list );
    destination_signal_list->
        setFirstColumnSpanned( displayed_dest_model->rowCount()-1,
                               dummy_index, true );
    the_node->is_destination = true;
    the_node->is_source = false;
    the_node->update();

    //destination_signal_list->expand(
    //        destination_model->
    //        indexFromItem( the_node->destination_model_list.first() )
    //        );

    updateMappingView();

}
void Form::removeNodeFromDestinationView( Node* the_node ) {

    int i = displayed_dest_model->
            indexFromItem( the_node->
                           dest_model_list.first() ).row();
    displayed_dest_model->takeRow( i );
    the_node->is_destination = false;
    the_node->update();

}
void Form::addNodeToSourceView( Node* the_node ) {

    QModelIndex dummy_index;
    displayed_source_model->
        appendRow( the_node->source_model_list );
    source_signal_list->
        setFirstColumnSpanned( displayed_source_model->rowCount()-1,
                               dummy_index, true );

    the_node->is_source = true;
    the_node->is_destination = false;
    the_node->update();

    updateMappingView();

}
void Form::removeNodeFromSourceView( Node* the_node ) {

    int i = displayed_source_model->
            indexFromItem( the_node->
                           source_model_list.first() ).row();
    displayed_source_model->takeRow( i );
    the_node->is_source = false;
    the_node->update();

}
*/

void Form::updatePressedLink( Link *reference ) {

    /*
    printf("updatePressedLink source %s dest %s\n",
           reference->source_signal_name,
           reference->dest_signal_name );
           */
    mapper_monitor_disconnect( this->mon,
                               reference->source_signal_name,
                               reference->dest_signal_name );

}
void Form::updateSelectedLink( Link *reference ) {

    this->selected_mapping_list.push_back( reference );
    this->updateLinkParameterDisplay( reference );

}
void Form::updateUnselectedLink( Link* reference ) {

    for ( std::list<Link*>::iterator it =
            this->selected_mapping_list.begin();
            it != this->selected_mapping_list.end();
            it++ ) {

        if ( !strcmp((*it)->source_signal_name,
                     reference->source_signal_name) &&
             !strcmp((*it)->dest_signal_name,
                     reference->dest_signal_name) ) {

            this->selected_mapping_list.remove( (*it) );

        }

    }

    if ( this->selected_mapping_list.size() == 0 ) {

        this->clearLinkParameterDisplay();

    }

}



void Form::updatePressedNode( Node* reference ) {

    //printf( "form says -> pressed node %s\n", reference->name );
    this->active_node_name = reference->name;

}
void Form::updateReleasedNode( Node* reference ) {

    QModelIndex dummy_index;

    if ( this->active_node_name == reference->name ) {

        printf( "form says -> released node %s\n", reference->name );

        if ( selection_mode_toggle->currentIndex() == 0 &&
             reference->is_source ) {

            //this->removeNodeFromSourceView( reference );
            reference->conflict_flag = 0;

        } else if ( selection_mode_toggle->currentIndex() == 1 &&
                    reference->is_destination ) {

            //this->removeNodeFromDestinationView( reference );
            reference->conflict_flag = 0;

        } else if ( selection_mode_toggle->currentIndex() == 0 &&
                    !reference->is_source ) {

            //this->addNodeToSourceView( reference );
            reference->conflict_flag = -1;

        } else if ( selection_mode_toggle->currentIndex() == 1 &&
                    !reference->is_destination ) {

            //this->addNodeToDestinationView( reference );
            reference->conflict_flag = 1;

        }

    }

    this->active_node_name = "";

}
void Form::updateSelectedNodes( bool is_selected ) {

    Node* sender = (Node*)(this->sender());
    QModelIndex dummy_index;

    if ( strcmp( this->active_node_name, "" ) ) {

        return;

    }

    printf( "form says -> node selected\n" );

    if ( selection_mode_toggle->currentIndex() == 0 &&
         is_selected &&
         !sender->is_source ) {

            //this->addNodeToSourceView( sender );
            sender->conflict_flag = -1;

    } else if ( selection_mode_toggle->currentIndex() == 1 &&
                is_selected &&
                !sender->is_destination ) {

            //this->addNodeToDestinationView( sender );
            sender->conflict_flag = 1;

    }

}



bool Form::eventFilter( QObject *obj, QEvent *event ) {

    if ( event->type() == QEvent::Resize ) {
/*
        printf( "got resize event %d %d\n",
                ((QResizeEvent*)event)->size().width(),
                ((QResizeEvent*)event)->size().height() );
                */
        //updateMappingView();
        return true;

    } else if ( event->type() == QEvent::Show) {

        return true;

    } else {

        return QObject::eventFilter(obj, event);

    }

}



void Form::updateSelectionMode( int index ) {

    if ( index == 0 ) {

        printf( "selected sources tab %d\n", index );

    } else if ( index == 1 ) {

        printf( "selected destinations tab %d\n", index );

    }

}
void Form::updateEditSelectionMode( int index ) {

    if ( index == 0 ) {

        printf( "selected neighborhood tab %d\n", index );
        this->changeVisualizationMode( this->vis_mode_toggle->currentIndex() );

    } else if ( index == 1 ) {

        printf( "selected edit tab %d\n", index );
        //updateMappingView();

    }

}



void Form::addDbDeviceCallbackFunction( device_callback_func* f ) {

    mapper_db_remove_device_callback(this->db,
        this->db_device_callback_function, (void*) 0 );
    this->db_device_callback_function = f;
    mapper_db_add_device_callback(this->db,
        this->db_device_callback_function, (void*) 0 );

}
void Form::addDbSignalCallbackFunction( signal_callback_func* f ) {

    printf( "add signal callback function\n" );
    mapper_db_remove_signal_callback(this->db,
            this->db_signal_callback_function, (void*) 0 );
    this->db_signal_callback_function = f;
    mapper_db_add_signal_callback(this->db,
            this->db_signal_callback_function, (void*) 0 );

}
void Form::addDbLinkCallbackFunction( link_callback_func* f ) {

    printf( "add db_link callback function\n" );
    mapper_db_remove_link_callback(this->db,
            this->db_link_callback_function, (void*) 0 );
    this->db_link_callback_function = f;
    mapper_db_add_link_callback(this->db,
            this->db_link_callback_function, (void*) 0 );

}
void Form::addDbMappingCallbackFunction( mapping_callback_func* f ) {

    printf( "add db_mapping callback function\n" );
    mapper_db_remove_mapping_callback(this->db,
            this->db_mapping_callback_function, (void*) 0 );
    this->db_mapping_callback_function = f;
    mapper_db_add_mapping_callback(this->db,
            this->db_mapping_callback_function, (void*) 0 );

}



void Form::setMapperMonitor( mapper_monitor mon ) {

    this->mon = mon;
    this->db = mapper_monitor_get_db(mon);

}



void Form::changeVisualizationMode( int mode_index ) {

    this->updateVisualizationStuff( mode_index );

}
void Form::updateVisualizationNodes( int current_mode ) {

    if ( current_mode == 0 ) {

        int cluster_1_x = 200;
        int cluster_1_y = 0;
        int cluster_2_x = 800;
        int cluster_2_y = 0;
        int cluster_3_x = 1400;
        int cluster_3_y = 0;

        std::list<Node*> cluster_1;
        std::list<Node*> cluster_2;
        std::list<Node*> cluster_3;
/*
        for ( std::list<Node*>::iterator it =
                this->master_source_node_pointer_list.begin();
              it != this->master_source_node_pointer_list.end();
              it++ ) {

            printf( "name: %s, source rows: %d, dest rows: %d\n",
                    (*it)->name,
                    (*it)->source_model_list[0]->rowCount(),
                    (*it)->dest_model_list[0]->rowCount() );

            if ( (*it)->source_model_list[0]->rowCount() == 0 &&
                 (*it)->dest_model_list[0]->rowCount() == 0 ) {

                cluster_3.push_back((*it));
                continue;

            }

            if ( (*it)->source_model_list[0]->rowCount() >=
                 (*it)->dest_model_list[0]->rowCount() ) {

                cluster_1.push_back((*it));

            } else {

                cluster_2.push_back((*it));

            }

            (*it)->outputs = (*it)->source_model_list[0]->rowCount();
            (*it)->inputs = (*it)->dest_model_list[0]->rowCount();
            (*it)->diameter = ( (*it)->outputs + (*it)->inputs + 1 ) * 20;
            (*it)->update();

        }*/

        cluster_1.sort( Utility::nodeSortOutputsFunction );
        cluster_2.sort( Utility::nodeSortInputsFunction );

        for ( std::list<Node*>::iterator it =
                cluster_1.begin();
                it != cluster_1.end();
                it++ ) {

            (*it)->setPos( cluster_1_x,
                           cluster_1_y );
            cluster_1_y += (*it)->diameter + 160;

        } for ( std::list<Node*>::iterator it =
                cluster_2.begin();
                it != cluster_2.end();
                it++ ) {

            (*it)->setPos( cluster_2_x,
                           cluster_2_y );
            cluster_2_y += (*it)->diameter + 160;

        } for ( std::list<Node*>::iterator it =
                cluster_3.begin();
                it != cluster_3.end();
                it++ ) {

            (*it)->setPos( cluster_3_x,
                           cluster_3_y );
            cluster_3_y += (*it)->diameter + 160;

        }

    }

    else if ( current_mode == 1 ) {
/*
        for ( std::list<Node*>::iterator it =
                this->master_source_node_pointer_list.begin();
              it != this->master_source_node_pointer_list.end();
              it++ ) {

            for ( std::list<qt_mapping>::iterator itt =
                    (*it)->destination_list.begin();
                    itt != (*it)->destination_list.end();
                    itt++ ) {



            }

        }
*/
    }

}
void Form::updateVisualizationLinks( int current_mode ) {

    for ( std::list<QGraphicsItem*>::iterator it =
            this->visualization_links.begin();
            it != this->visualization_links.end();
            it++ ) {

        scene->removeItem( (*it) );

    }

    this->visualization_links.clear();

    if ( current_mode == 0 ) {
/*
        for ( std::list<Node*>::iterator it =
                this->master_source_node_pointer_list.begin();
                it != this->master_source_node_pointer_list.end();
                it++ ) {


            for ( std::list<qt_mapping>::iterator itt =
                    (*it)->destination_list.begin();
                    itt != (*it)->destination_list.end();
                    itt++ ) {

                printf( "destination coords x %f y %f./\n",
                        (*itt)->destination_node->pos().x(),
                        (*itt)->destination_node->pos().y() );
                this->visualization_links.push_back(
                    scene->addLine(
                        (*it)->pos().x(),
                        (*it)->pos().y(),
                        (*itt)->destination_node->pos().x(),
                        (*itt)->destination_node->pos().y(),
                        QPen(Qt::gray)
                        )
                    );
                this->visualization_links.back()->setZValue( -100 );

            }

        }*/
    }

}
void Form::updateVisualizationSides( int current_mode ) {



}
void Form::updateVisualizationConcavity( int current_mode ) {



}
void Form::updateVisualizationStuff( int current_mode ) {

    this->updateVisualizationNodes( current_mode );
    this->updateVisualizationLinks( current_mode );
    this->updateVisualizationSides( current_mode );
    this->updateVisualizationConcavity( current_mode );

}
