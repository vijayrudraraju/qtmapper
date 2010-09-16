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
    scene->setSceneRect( 0, 0, 1000, 1000 );
    scene->setStickyFocus( true );

    graphics_view->
            setScene( scene );
    graphics_view->
            setCacheMode( QGraphicsView::CacheBackground );
    graphics_view->
            setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
    graphics_view->
            setRenderHint( QPainter::Antialiasing );
    //graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    graphics_view->scale( qreal(0.7), qreal(0.7) );
    graphics_view->setMinimumSize( 600, 300 );

    QStringList header_labels;
    header_labels << "name" << "host" << "port" << "can alias?";

    source_model = new QStandardItemModel( 0, 4 );
    source_model->setHorizontalHeaderLabels( header_labels );
    source_list->setModel( source_model );
    source_list->setMinimumSize( 600, 300 );

    destination_model = new QStandardItemModel( 0, 4 );
        destination_model->setHorizontalHeaderLabels( header_labels );
        destination_list->setModel( destination_model );
        destination_list->setMinimumSize( 600, 300 );

    source_signal_list->setModel( source_model );
        source_signal_list->setColumnHidden( 1, true );
        source_signal_list->setColumnHidden( 2, true );
        source_signal_list->setColumnHidden( 3, true );
    destination_signal_list->setModel( destination_model );
        destination_signal_list->setColumnHidden( 1, true );
        destination_signal_list->setColumnHidden( 2, true );
        destination_signal_list->setColumnHidden( 3, true );

    this->active_node_name = "";

    connect( selection_mode_toggle, SIGNAL(currentChanged(int)),
             this, SLOT(updateSelectionMode(int)) );
    connect( graphics_view, SIGNAL(mouseStateChanged(bool)),
             this, SLOT(updateMouseState(bool)) );

    setWindowTitle( tr("Mapper GUI") );

}

Form::~Form(  ) {


}

void Form::update(  ) {

    //printf( "Form::update(  )\n" );

    mdev_poll( this->qtmapper, 0 );

}

void Form::updateMouseState( bool is_pressed ) {

    printf( "form says -> mouse state %d\n", is_pressed );
    mouse_is_pressed = is_pressed;

    int index;

    if ( !mouse_is_pressed ) {

        for ( std::list<Node*>::iterator it = node_pointer_list.begin();
                it != node_pointer_list.end();
                it++ ) {

            if ( (*it)->conflict_flag == -1 ) {

                index =
                        destination_model->
                        indexFromItem( (*it)->
                                       destination_model_list.first() ).row();
                destination_model->takeRow( index );
                (*it)->is_destination = false;
                (*it)->is_source = true;
                (*it)->conflict_flag = 0;

            } else if ( (*it)->conflict_flag == 1 ) {

                index =
                        source_model->
                        indexFromItem( (*it)->
                                       source_model_list.first() ).row();
                source_model->takeRow( index );
                (*it)->is_destination = true;
                (*it)->is_source = false;
                (*it)->conflict_flag = 0;

            }

            (*it)->update();

        }

    }

}

void Form::updatePressedNode( Node* reference ) {

        printf( "form says -> pressed node %s\n", reference->name );
        this->active_node_name = reference->name;

}

void Form::updateReleasedNode( Node* reference ) {

    int i;

    if ( this->active_node_name == reference->name ) {

        printf( "form says -> released node %s\n", reference->name );

        if ( selection_mode_toggle->currentIndex() == 0 &&
             reference->is_source ) {

            i = source_model->
                    indexFromItem( reference->
                                   source_model_list.first() ).row();
            source_model->takeRow( i );
            //reference->setSelected( false );
            reference->is_source = false;
            reference->conflict_flag = 0;

        } else if ( selection_mode_toggle->currentIndex() == 1 &&
                    reference->is_destination ) {

            i = destination_model->
                    indexFromItem( reference->
                                   destination_model_list.first() ).row();
            destination_model->takeRow( i );
            //makereference->setSelected( false );
            reference->is_destination = false;
            reference->conflict_flag = 0;

        } else if ( selection_mode_toggle->currentIndex() == 0 &&
                    !reference->is_source ) {

            source_model->
                appendRow( reference->source_model_list );
            reference->is_source = true;
            reference->conflict_flag = -1;

        } else if ( selection_mode_toggle->currentIndex() == 1 &&
                    !reference->is_destination ) {

            destination_model->
                appendRow( reference->destination_model_list );
            reference->is_destination = true;
            reference->conflict_flag = 1;

        }

    }

    this->active_node_name = "";

}

void Form::updateSelectedNodes( bool is_selected ) {

    Node* sender = (Node*)(this->sender());

    if ( strcmp( this->active_node_name, "" ) ) {

        return;

    }

    printf( "form says -> node selected\n" );

    if ( selection_mode_toggle->currentIndex() == 0 &&
         is_selected &&
         !sender->is_source ) {

            source_model->
                appendRow( sender->source_model_list );
            sender->is_source = true;
            sender->conflict_flag = -1;

    } else if ( selection_mode_toggle->currentIndex() == 1 &&
                is_selected &&
                !sender->is_destination ) {

            destination_model->
                appendRow( sender->destination_model_list );
            sender->is_destination = true;
            sender->conflict_flag = 1;

    }

    if ( !sender->is_source || !sender->is_destination ) {

        sender->conflict_flag = 0;

    }

}

void Form::updateSelectionMode( int index ) {

    printf( "selection mode changed %d\n", index );

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

void Form::setMapperDevice( mapper_device device ) {

    this->qtmapper = device;

}

bool Form::IsNameMatch( Node* i ) {

    return !strcmp( i->name, device_search_term );

}

void Form::addNewSignal( mapper_db_signal record ) {

    std::list<Node*>::iterator it;

    printf( "vijay searching for %s\n", record->device_name );
    Form::device_search_term = record->device_name;

    it = std::find_if( this->node_pointer_list.begin(),
                        this->node_pointer_list.end(),
                        IsNameMatch );

    printf( "vijay found %s\n", (*it)->name );

    QStandardItem* item_1_child = new QStandardItem;
    item_1_child->setText( record->name );
    (*it)->source_model_list[0]->setChild( 0, 0, item_1_child );

}

void Form::addNewDevice( const char* name,
                         const char* host,
                         int port,
                         int can_alias ) {

    std::string temp_string;
    std::stringstream out;

    printf( "addNewDevice(  ) %d %d \n", default_x, default_y );

    this->node_pointer_list.push_back( new Node(graphics_view) );
    this->node_pointer_list.back()->setName( name );
    scene->addItem(this->node_pointer_list.back());
    this->node_pointer_list.back()->setPos( default_x, default_y );

    QStandardItem* item_1 = new QStandardItem;
    QStandardItem* item_2 = new QStandardItem;
    QStandardItem* item_3 = new QStandardItem;
    QStandardItem* item_4 = new QStandardItem;
    item_1->setText( name );
    item_2->setText( host );
    out << port;
    item_3->setText( out.str().c_str() );
    out.str( "" );
    out << can_alias;
    item_4->setText( out.str().c_str() );
    this->node_pointer_list.back()->source_model_list
            << item_1 << item_2 << item_3 << item_4;

    QStandardItem* destination_item_1 = new QStandardItem;
    QStandardItem* destination_item_2 = new QStandardItem;
    QStandardItem* destination_item_3 = new QStandardItem;
    QStandardItem* destination_item_4 = new QStandardItem;
    destination_item_1->setText( name );
    destination_item_2->setText( host );
    out.str( "" );
    out << port;
    destination_item_3->setText( out.str().c_str() );
    out.str( "" );
    out << can_alias;
    destination_item_4->setText( out.str().c_str() );
    this->node_pointer_list.back()->destination_model_list
            << destination_item_1
            << destination_item_2
            << destination_item_3
            << destination_item_4;

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

void Form::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}
