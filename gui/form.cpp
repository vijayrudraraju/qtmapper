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

    source_model = new QStandardItemModel( 0, 4 );
    source_list->setModel( source_model );
    source_signal_list->setModel( source_model );
    source_list->setMinimumSize( 600, 300 );

    destination_model = new QStandardItemModel( 0, 4 );
    destination_list->setModel( destination_model );
    destination_signal_list->setModel( destination_model );
    destination_list->setMinimumSize( 600, 300 );

    this->active_node_name = "";

    connect( selection_mode_toggle, SIGNAL(currentChanged(int)),
             this, SLOT(updateSelectionMode(int)) );
    connect( graphics_view, SIGNAL(mouseStateChanged(bool)),
             this, SLOT(updateMouseState(bool)) );

    setWindowTitle( tr("Mapper GUI") );

    //Node *node1 = new Node(graphicsView);
    //Node *node2 = new Node(graphicsView);
    //Node *node3 = new Node(graphicsView);
    //Node *node4 = new Node(graphicsView);
    //centerNode = new Node(graphicsView);
    //Node *node6 = new Node(graphicsView);
    //Node *node7 = new Node(graphicsView);
    //Node *node8 = new Node(graphicsView);
    //Node *node9 = new Node(graphicsView);
    //scene->addItem(node1);
    //scene->addItem(node2);
    //scene->addItem(node3);
    //scene->addItem(node4);
    //scene->addItem(centerNode);
    //scene->addItem(node6);
    //scene->addItem(node7);
    //scene->addItem(node8);
    //scene->addItem(node9);

    /*
    scene->addItem(new Edge(node1, node2));
    scene->addItem(new Edge(node2, node3));
    scene->addItem(new Edge(node2, centerNode));
    scene->addItem(new Edge(node3, node6));
    scene->addItem(new Edge(node4, node1));
    scene->addItem(new Edge(node4, centerNode));
    scene->addItem(new Edge(centerNode, node6));
    scene->addItem(new Edge(centerNode, node8));
    scene->addItem(new Edge(node6, node9));
    scene->addItem(new Edge(node7, node4));
    scene->addItem(new Edge(node8, node7));
    scene->addItem(new Edge(node9, node8));
    */

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

    if ( this->active_node_name != "" ) {

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

void Form::addDbCallbackFunction( device_callback_func* f ) {

    mapper_db_remove_device_callback( this->db_callback_function, (void*) 0 );
    this->db_callback_function = f;
    mapper_db_add_device_callback( this->db_callback_function, (void*) 0 );

}

void Form::setMapperDevice( mapper_device device ) {

    this->qtmapper = device;

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

/*
void Form::scaleView(qreal scaleFactor)
{
    qreal factor =
            graphicsView->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    graphicsView->scale(scaleFactor, scaleFactor);
}
*/

/*
void Form::wheelEvent(QWheelEvent *event) {

    //scaleView(pow((double)2, -event->delta() / 240.0));
    //event->accept();
}
*/

/*
void Form::viewportEvent( QEvent* event ) {

    //event->accept();

}
*/

/*
void Form::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        //centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        //centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        //centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        //centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;

    default:
        QWidget::keyPressEvent(event);
    }
}
*/
