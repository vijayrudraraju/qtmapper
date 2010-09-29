#include "form.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <cmath>
#include <sstream>

Form::Form( QWidget *parent ) :
    QWidget( parent ) {

    setupUi( this );

    default_x = 0;
    default_y = 100;

    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
    timer->start( 500 );

    scene = new MapperGraphicsScene( graphics_view );
    scene->setItemIndexMethod( QGraphicsScene::NoIndex );
    scene->setStickyFocus( true );
    graphics_view->
            setScene( scene );
    graphics_view->
            setCacheMode( QGraphicsView::CacheBackground );
    graphics_view->
            setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    graphics_view->
            setRenderHint( QPainter::Antialiasing );
    graphics_view->scale( qreal(0.7), qreal(0.7) );
    graphics_view->setMinimumSize( 600, 300 );
    graphics_view->setAlignment( Qt::AlignLeft|Qt::AlignTop );
    graphics_view->setSceneRect( -100, 0, 1000, 1000 );

    graphics_view_2->setScene( &mapping_scene );
    graphics_view_2->installEventFilter( this );
    graphics_view_2->setAlignment( Qt::AlignLeft|Qt::AlignTop );

    printf( "graphics_view_2->width %d, graphics_view_2->height %d\n",
            graphics_view_2->width(), graphics_view_2->height() );

    QStringList header_labels;
    header_labels << "name" << "host" << "port" << "can alias?";
    QStringList header_edit_labels;
    header_edit_labels <<
            "name" << "type" << "length" << "other" << "pointer";

    source_model = new QStandardItemModel( 0, 5 );
    source_model->setHorizontalHeaderLabels( header_labels );
    source_list->setModel( source_model );
    source_list->setMinimumSize( 600, 300 );

    destination_model = new QStandardItemModel( 0, 5 );
    destination_model->setHorizontalHeaderLabels( header_labels );
    destination_list->setModel( destination_model );
    destination_list->setMinimumSize( 600, 300 );

    source_signal_list->setModel( source_model );
    destination_signal_list->setModel( destination_model );
    /*
    this->source_signal_list->
            setStyleSheet( "selection-color: rgba(0,0,0,0%)");
            */

    this->source_list->setColumnHidden( 4, true );
    this->destination_list->setColumnHidden( 4, true );
    this->source_signal_list->setColumnHidden( 3, true );
    this->destination_signal_list->setColumnHidden( 3, true );
    this->source_signal_list->setColumnHidden( 4, true );
    this->destination_signal_list->setColumnHidden( 4, true );

    this->active_node_name = "";

    connect( selection_mode_toggle, SIGNAL(currentChanged(int)),
             this, SLOT(updateSelectionMode(int)) );
    connect( edit_mode_toggle, SIGNAL(currentChanged(int)),
             this, SLOT(updateEditSelectionMode(int)) );
    connect( graphics_view, SIGNAL(mouseStateChanged(bool)),
            this, SLOT(updateMouseState(bool)) );
    /*
    connect( graphics_view, SIGNAL(mouseDoubleClick( )),
            this, SLOT(newDoubleClick( )) );
    */
    /*
    connect( this->source_signal_list, SIGNAL(clicked(QModelIndex)),
             this, SLOT(beginToDrawMapping(QModelIndex)) );
    connect( this->source_signal_list, SIGNAL(clicked(QModelIndex)),
             this, SLOT(beginToDrawMapping(QModelIndex)) );
             */

    connect( this->destination_signal_list, SIGNAL(collapsed(QModelIndex)),
             this, SLOT(updateMappingView()) );
    connect( this->source_signal_list, SIGNAL(collapsed(QModelIndex)),
             this, SLOT(updateMappingView()) );
    connect( this->source_signal_list, SIGNAL(expanded(QModelIndex)),
             this, SLOT(updateMappingView()) );
    connect( this->destination_signal_list, SIGNAL(expanded(QModelIndex)),
             this, SLOT(updateMappingView()) );


    setWindowTitle( tr("Mapper GUI") );
}

void Form::beginToDrawMapping( QModelIndex index ) {

    QRect source_signal_rect;
    source_signal_rect =
            source_signal_list->
            visualRect( index  );
    printf( "source signal y pos: %d\n",
            source_signal_rect.topLeft().y() );

}

void Form::update(  ) {

    mdev_poll( this->qtmapper, 0 );
    QWidget::update();
    //updateMappingView();

    //this->destination_signal_list->resizeColumnToContents( 0 );
    this->destination_signal_list->setColumnWidth( 0, 180 );
    this->destination_signal_list->resizeColumnToContents( 1 );
    this->destination_signal_list->resizeColumnToContents( 2 );
    this->destination_signal_list->resizeColumnToContents( 3 );
    //this->source_signal_list->resizeColumnToContents( 0 );
    this->source_signal_list->setColumnWidth( 0, 180 );
    this->source_signal_list->resizeColumnToContents( 1 );
    this->source_signal_list->resizeColumnToContents( 2 );
    this->source_signal_list->resizeColumnToContents( 3 );

}

void Form::updateMouseState( bool is_pressed ) {

    printf( "form says -> mouse state %d\n", is_pressed );
    mouse_is_pressed = is_pressed;

    if ( !mouse_is_pressed ) {

        for ( std::list<Node*>::iterator it = node_pointer_list.begin();
                it != node_pointer_list.end();
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

}

void Form::newDoubleClick( ) {

    for ( std::list<Node*>::iterator it = node_pointer_list.begin();
            it != node_pointer_list.end();
            it++ ) {

        if ( selection_mode_toggle->currentIndex() == 1 &&
                    (*it)->is_destination ) {

            this->removeNodeFromDestinationView( (*it) );

        } else if ( selection_mode_toggle->currentIndex() == 0 &&
                    (*it)->is_source ) {

            this->removeNodeFromSourceView( (*it) );

        }

    }

}

void Form::clearMappingView( ) {
/*
    if ( mapping_scene->items().count() == 0 ) {

        return;

    }
*/

    mapping_scene.clear();

}

void Form::updateMappingView( ) {

    QStandardItem* source_item_pointer;
    QStandardItem* destination_item_pointer;
    QStandardItem* source_signal_item_pointer;
    QStandardItem* destination_signal_item_pointer;
    Node* source_pointer;
    Node* destination_pointer;
    QModelIndex source_signal_index;
    QModelIndex dest_signal_index;
    QRect source_signal_rect;
    QRect destination_signal_rect;

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
    clearMappingView( );

    for ( int i = 0;
            i < source_model->rowCount( );
            i++ ) {

        source_item_pointer = source_model->item( i, 4 );
        source_pointer = (Node*)(source_item_pointer->text().toInt());
        source_item_pointer = source_model->item( i, 0 );

        for ( std::list<qt_mapping>::iterator it =
                source_pointer->destination_list.begin();
              it != source_pointer->destination_list.end();
              it++ ) {

            for ( int j = 0;
                    j < destination_model->rowCount( );
                    j++ ) {

                destination_item_pointer = destination_model->item( j, 4 );
                destination_pointer =
                        (Node*)(destination_item_pointer->text().toInt());
                destination_item_pointer =
                        destination_model->item( j, 0 );

                if ( !strcmp((*it)->destination_node->name,
                     destination_pointer->name)
                     ) {

                    for ( int k = 0;
                          k < source_item_pointer->rowCount();
                          k++ ) {

                        if ( !strcmp(source_item_pointer->child( k, 0 )->
                             text().toLatin1().constData(),
                             (*it)->source_signal_name) ) {

                            source_signal_item_pointer =
                                    source_item_pointer->child( k, 0 );
                            break;

                        }

                    }

                    for ( int k = 0;
                          k < destination_item_pointer->rowCount();
                          k++ ) {

                        if ( !strcmp(destination_item_pointer->child( k, 0 )->
                             text().toLatin1().constData(),
                             (*it)->destination_signal_name) ) {

                            destination_signal_item_pointer =
                                    destination_item_pointer->child( k, 0 );
                            break;

                        }

                    }

                    source_signal_index =
                            source_model->
                            indexFromItem( source_signal_item_pointer );
                    dest_signal_index =
                            destination_model->
                            indexFromItem( destination_signal_item_pointer );
                    source_signal_rect =
                            source_signal_list->
                            visualRect( source_signal_index  );
                    destination_signal_rect =
                            destination_signal_list->
                            visualRect( dest_signal_index  );

                    if ( source_signal_rect.topLeft().y() == 0 ) {

                        source_signal_rect =
                                source_signal_list->
                                visualRect( source_signal_index.parent() );

                    }

                    if ( destination_signal_rect.topLeft().y() == 0 ) {

                        destination_signal_rect =
                                destination_signal_list->
                                visualRect( dest_signal_index.parent() );

                    }

                    source_vertical_offset =
                            source_signal_list->header()->height() +
                            ( source_signal_rect.height() / 2 ) + 1;
                    dest_vertical_offset =
                            destination_signal_list->header()->height() +
                            ( destination_signal_rect.height() / 2 ) + 1;

                    printf( "source signal y %d + offset %d\n",
                            source_signal_rect.topLeft().y(),
                            source_vertical_offset );
                    printf( "destination signal y %d + offset %d\n",
                            destination_signal_rect.topLeft().y(),
                            dest_vertical_offset );

                    this->mapping_scene.addLine( 0,
                                     source_vertical_offset +
                                     source_signal_rect.topLeft().y(),
                                     graphics_view_2->width(),
                                     dest_vertical_offset +
                                     destination_signal_rect.topLeft().y(),
                                     pen_width );
                    /*
                    this->graphics_view_2->
                            viewport()->setFixedSize( graphics_view_2->width(),
                                                      graphics_view_2->height() );
                    this->graphics_view_2->setSceneRect( 0, 0,
                                                      graphics_view_2->width(),
                                                      graphics_view_2->height() );
                                                      */

                }

            }

        }

    }

    printf( "updateMappingView: graphics_view_2->width %d\t, graphics_view_2->height %d\n",
            graphics_view_2->width(),
            graphics_view_2->height() );
    printf( "updateMappingView: graphics_view_2->sceneRect->width %f\t, graphics_view_2->sceneRect->height %f\n",
            graphics_view_2->sceneRect().width(),
            graphics_view_2->sceneRect().height() );
    printf( "updateMappingView: graphics_view_2->viewport->width %d\t, graphics_view_2->viewport->height %d\n",
            graphics_view_2->viewport()->width(),
            graphics_view_2->viewport()->height() );
    printf( "updateMappingView: mapping_scene->maximumViewportSize->width %d\t, mapping_scene->maximumViewportSize->height %d\n",
            graphics_view_2->maximumViewportSize().width(),
            graphics_view_2->maximumViewportSize().height() );
    printf( "updateMappingView: mapping_scene->sceneRect->width %f\t, mapping_scene->sceneRect->height %f\n\n",
            mapping_scene.sceneRect().width(),
            mapping_scene.sceneRect().height() );
}

void Form::addNodeToDestinationView( Node* the_node ) {

    QModelIndex dummy_index;
    destination_model->
        appendRow( the_node->destination_model_list );
    destination_signal_list->
        setFirstColumnSpanned( destination_model->rowCount()-1,
                               dummy_index, true );
    the_node->is_destination = true;
    the_node->is_source = false;
    the_node->update();
/*
    destination_signal_list->expand(
            destination_model->
            indexFromItem( the_node->destination_model_list.first() )
            );
*/
    //updateMappingView();

}

void Form::removeNodeFromDestinationView( Node* the_node ) {

    int i = destination_model->
            indexFromItem( the_node->
                           destination_model_list.first() ).row();
    destination_model->takeRow( i );
    the_node->is_destination = false;
    the_node->update();

}

void Form::addNodeToSourceView( Node* the_node ) {

    QModelIndex dummy_index;
    source_model->
        appendRow( the_node->source_model_list );
    source_signal_list->
        setFirstColumnSpanned( source_model->rowCount()-1,
                               dummy_index, true );
    the_node->is_source = true;
    the_node->is_destination = false;
    the_node->update();

    /*
    source_signal_list->expand(
            source_model->
            indexFromItem( the_node->source_model_list.first() )
            );
*/
    //updateMappingView();

}

void Form::removeNodeFromSourceView( Node* the_node ) {

    int i = source_model->
            indexFromItem( the_node->
                           source_model_list.first() ).row();
    source_model->takeRow( i );
    the_node->is_source = false;
    the_node->update();

}

void Form::updatePressedNode( Node* reference ) {

        printf( "form says -> pressed node %s\n", reference->name );
        this->active_node_name = reference->name;

}

void Form::updateReleasedNode( Node* reference ) {

    QModelIndex dummy_index;

    if ( this->active_node_name == reference->name ) {

        printf( "form says -> released node %s\n", reference->name );

        if ( selection_mode_toggle->currentIndex() == 0 &&
             reference->is_source ) {

            this->removeNodeFromSourceView( reference );
            reference->conflict_flag = 0;

        } else if ( selection_mode_toggle->currentIndex() == 1 &&
                    reference->is_destination ) {

            this->removeNodeFromDestinationView( reference );
            reference->conflict_flag = 0;

        } else if ( selection_mode_toggle->currentIndex() == 0 &&
                    !reference->is_source ) {

            this->addNodeToSourceView( reference );
            reference->conflict_flag = -1;

        } else if ( selection_mode_toggle->currentIndex() == 1 &&
                    !reference->is_destination ) {

            this->addNodeToDestinationView( reference );
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

            this->addNodeToSourceView( sender );
            sender->conflict_flag = -1;

    } else if ( selection_mode_toggle->currentIndex() == 1 &&
                is_selected &&
                !sender->is_destination ) {

            this->addNodeToDestinationView( sender );
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
        updateMappingView();
        return true;

    } else if ( event->type() == QEvent::Show) {
/*
        mapping_scene->setSceneRect( 0, 0,
                                     graphics_view_2->width(),
                                     graphics_view_2->height() );
                                     */
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

    } else if ( index == 1 ) {

        printf( "selected edit tab %d\n", index );
        updateMappingView();

    }

}

void Form::addDbDeviceCallbackFunction( device_callback_func* f ) {

    mapper_db_remove_device_callback(
            this->db_device_callback_function, (void*) 0 );
    this->db_device_callback_function = f;
    mapper_db_add_device_callback(
            this->db_device_callback_function, (void*) 0 );

}

void Form::addDbSignalCallbackFunction( signal_callback_func* f ) {

    printf( "add signal callback function\n" );
    mapper_db_remove_signal_callback(
            this->db_signal_callback_function, (void*) 0 );
    this->db_signal_callback_function = f;
    mapper_db_add_signal_callback(
            this->db_signal_callback_function, (void*) 0 );

}

void Form::addDbLinkCallbackFunction( link_callback_func* f ) {

    printf( "add db_link callback function\n" );
    mapper_db_remove_link_callback(
            this->db_link_callback_function, (void*) 0 );
    this->db_link_callback_function = f;
    mapper_db_add_link_callback(
            this->db_link_callback_function, (void*) 0 );

}

void Form::addDbMappingCallbackFunction( mapping_callback_func* f ) {

    printf( "add db_mapping callback function\n" );
    mapper_db_remove_mapping_callback(
            this->db_mapping_callback_function, (void*) 0 );
    this->db_mapping_callback_function = f;
    mapper_db_add_mapping_callback(
            this->db_mapping_callback_function, (void*) 0 );

}

void Form::setMapperDevice( mapper_device device ) {

    this->qtmapper = device;

}

bool Form::IsNameMatch( Node* i ) {

    //printf( "IsNameMatch %s %s\n", i->name, device_search_term);
    return !strcmp( i->name, device_search_term );

}

void Form::addNewMapping( mapper_db_mapping record ) {

    std::list<Node*>::iterator source_it;
    std::list<Node*>::iterator destination_it;

    QString str = record->src_name;
    QStringList parsed_str = str.split( "/", QString::SkipEmptyParts );

    QString str_2 = record->dest_name;
    QStringList parsed_str_2 = str_2.split( "/", QString::SkipEmptyParts );

    printf( "vijay searching for %s with %s to add mapping to...\n",
            parsed_str[0].toLatin1().constData(),
            parsed_str[1].toLatin1().constData() );
    printf( "%s with %s\n",
            parsed_str_2[0].toLatin1().constData(),
            parsed_str_2[1].toLatin1().constData() );

    parsed_str[0].prepend("/");
    parsed_str_2[0].prepend("/");
    parsed_str[1].prepend("/");
    parsed_str_2[1].prepend("/");

    Form::device_search_term =
            parsed_str[0].toLatin1().constData();
    source_it = std::find_if( this->node_pointer_list.begin(),
                        this->node_pointer_list.end(),
                        IsNameMatch );
    Form::device_search_term =
            parsed_str_2[0].toLatin1().constData();
    destination_it = std::find_if( this->node_pointer_list.begin(),
                        this->node_pointer_list.end(),
                        IsNameMatch );

    (*source_it)->addMapping( (*destination_it),
                              parsed_str[1].toLatin1().constData(),
                              parsed_str_2[1].toLatin1().constData() );

}

void Form::addNewSignal( mapper_db_signal record ) {

    std::list<Node*>::iterator it;

    printf( "vijay searching for %s to add signal\n",
            record->device_name );
    Form::device_search_term = record->device_name;

    it = std::find_if( this->node_pointer_list.begin(),
                        this->node_pointer_list.end(),
                        IsNameMatch );

    printf( "vijay found %s with %d rows\n",
            (*it)->name,
            (*it)->source_model_list[0]->rowCount() );

    std::stringstream out;
    //(*it)->source_model_list[0]->setColumnCount( 3 );

    int row_count_before_add = (*it)->source_model_list[0]->rowCount();

    QStandardItem* source_item_1_child = new QStandardItem;
    source_item_1_child->setText( record->name );
    (*it)->source_model_list[0]->setChild( row_count_before_add,
                                           0,
                                           source_item_1_child );
    QStandardItem* destination_item_1_child = new QStandardItem;
    destination_item_1_child->setText( record->name );
    (*it)->destination_model_list[0]->setChild( row_count_before_add,
                                                0,
                                                destination_item_1_child );

    QStandardItem* source_item_2_child = new QStandardItem;
    out << record->type;
    source_item_2_child->setText( out.str().c_str() );
    (*it)->source_model_list[0]->setChild( row_count_before_add,
                                           1,
                                           source_item_2_child );
    QStandardItem* destination_item_2_child = new QStandardItem;
    destination_item_2_child->setText( out.str().c_str() );
    (*it)->destination_model_list[0]->setChild( row_count_before_add,
                                                1,
                                                destination_item_2_child );

    QStandardItem* source_item_3_child = new QStandardItem;
    out.str( "" );
    out << record->length;
    source_item_3_child->setText( out.str().c_str() );
    (*it)->source_model_list[0]->setChild( row_count_before_add,
                                           2,
                                           source_item_3_child );
    QStandardItem* destination_item_3_child = new QStandardItem;
    destination_item_3_child->setText( out.str().c_str() );
    (*it)->destination_model_list[0]->setChild( row_count_before_add,
                                                2,
                                                destination_item_3_child );

}

void Form::addNewDevice( const char* name,
                         const char* host,
                         int port,
                         int can_alias ) {

    std::string temp_string;
    std::stringstream out;
    Node* new_device = new Node(graphics_view);

    printf( "addNewDevice(  ) %d %d \n", default_x, default_y );

    this->node_pointer_list.push_back( new_device );
    this->node_pointer_list.back()->setName( name );
    scene->addItem(this->node_pointer_list.back());
    this->node_pointer_list.back()->setPos( default_x, default_y );

    QStandardItem* source_item_1 = new QStandardItem;
    QStandardItem* source_item_2 = new QStandardItem;
    QStandardItem* source_item_3 = new QStandardItem;
    QStandardItem* source_item_4 = new QStandardItem;
    QStandardItem* source_pointer_item = new QStandardItem;
    source_item_1->setText( name );
    source_item_2->setText( host );
    out << port;
    source_item_3->setText( out.str().c_str() );
    out.str( "" );
    out << can_alias;
    source_item_4->setText( out.str().c_str() );
    out.str( "" );
    out << (int)new_device;
    source_pointer_item->setText( out.str().c_str() );
    this->node_pointer_list.back()->source_model_list
            << source_item_1
            << source_item_2
            << source_item_3
            << source_item_4
            << source_pointer_item;
    //printf( "pointer int %d\n", (int)new_device );

    QStandardItem* destination_item_1 = new QStandardItem;
    QStandardItem* destination_item_2 = new QStandardItem;
    QStandardItem* destination_item_3 = new QStandardItem;
    QStandardItem* destination_item_4 = new QStandardItem;
    QStandardItem* destination_pointer_item = new QStandardItem;
    destination_item_1->setText( name );
    destination_item_2->setText( host );
    out.str( "" );
    out << port;
    destination_item_3->setText( out.str().c_str() );
    out.str( "" );
    out << can_alias;
    destination_item_4->setText( out.str().c_str() );
    out.str( "" );
    out << (int)new_device;
    destination_pointer_item->setText( out.str().c_str() );
    this->node_pointer_list.back()->destination_model_list
            << destination_item_1
            << destination_item_2
            << destination_item_3
            << destination_item_4
            << destination_pointer_item;

    //printf( "appended row to model with %d %d columns\n",
    //        this->node_pointer_list.back()->model_list.size(),
    //        source_model->columnCount() );

    QObject::connect( this->node_pointer_list.back(),
                         SIGNAL(selectionStateChanged(bool)),
                         this,
                         SLOT(updateSelectedNodes(bool)) );

    QObject::connect( this->node_pointer_list.back(),
                      SIGNAL(nodePressed(Node*)),
                      this,
                      SLOT(updatePressedNode(Node*)));
    QObject::connect( this->node_pointer_list.back(),
                      SIGNAL(nodeReleased(Node*)),
                      this,
                      SLOT(updateReleasedNode(Node*)));

    default_y += 70;
    if ( default_y >= 400 ) {

        default_y = 0;
        default_x += 100;

    }

}
