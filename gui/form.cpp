#include "form.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <math.h>

Form::Form( QWidget *parent ) :
    QWidget( parent ) {

    setupUi( this );

    default_x = 0;
    default_y = 100;

    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
    timer->start( 500 );

    scene = new QGraphicsScene( graphicsView );
    scene->setItemIndexMethod( QGraphicsScene::NoIndex );
    scene->setSceneRect( 0, 0, 1000, 1000 );

    graphicsView->
            setScene( scene );
    graphicsView->
            setCacheMode( QGraphicsView::CacheBackground );
    graphicsView->
            setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
    graphicsView->
            setRenderHint( QPainter::Antialiasing );
    //graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->scale( qreal(0.7), qreal(0.7) );
    graphicsView->setMinimumSize( 600, 300 );

    source_model = new QStandardItemModel( 0, 4 );
    source_list->setModel( source_model );
    source_list->setMinimumSize( 600, 300 );

    destination_model = new QStandardItemModel( 0, 4 );
    destination_list->setModel( destination_model );
    destination_list->setMinimumSize( 600, 300 );

    connect( selection_mode_toggle, SIGNAL(currentChanged(int)),
             this, SLOT(updateSelectionMode(int)) );

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

    printf( "Form::update(  )\n" );

    mdev_poll( this->qtmapper, 0 );

}

void Form::updateSelectedNodes( bool new_value ) {

    Node* sender = (Node*)(this->sender());
    int index;

    printf ( "slot selection %d\n", new_value );
    if ( new_value ) {

        if ( selection_mode_toggle->currentIndex() == 0 ) {

            if ( destination_model->
                 indexFromItem( sender->model_list.first() ).isValid() ) {

                index =
                        destination_model->
                        indexFromItem( sender->model_list.first() ).row();
                printf ( "slot remove from destination table %d\n", index );
                destination_model->takeRow( index );

            }

            sender->is_source = true;
            printf ( "slot add to source table %d\n",
                     source_model->rowCount() );
            source_model->
                appendRow( sender->model_list );

        } else {

            if ( source_model->
                 indexFromItem( sender->model_list.first() ).isValid() ) {

                index =
                        source_model->
                        indexFromItem( sender->model_list.first() ).row();
                printf ( "slot remove from source table %d\n", index );
                source_model->takeRow( index );

            }

            sender->is_source = false;
            printf ( "slot add to destination table %d\n",
                     destination_model->rowCount() );
            destination_model->
                appendRow( sender->model_list );

        }

    } else {

        if ( sender->is_source &&
             selection_mode_toggle->currentIndex() == 0 ) {

            index =
                    source_model->
                    indexFromItem( sender->model_list.first() ).row();
            printf ( "slot remove from source table %d\n", index );
            source_model->takeRow( index );

        } else if ( selection_mode_toggle->currentIndex() == 1 ){

            index =
                    destination_model->
                    indexFromItem( sender->model_list.first() ).row();
            printf ( "slot remove from destination table %d\n", index );
            destination_model->takeRow( index );

        }

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

void Form::addNewDevice( const char* name ) {

    printf( "addNewDevice(  ) %d %d \n", default_x, default_y );

    this->node_pointer_list.push_back( new Node(graphicsView) );
    this->node_pointer_list.back()->setName( name );
    scene->addItem(this->node_pointer_list.back());
    this->node_pointer_list.back()->setPos( default_x, default_y );

    QStandardItem* item_1 = new QStandardItem;
    QStandardItem* item_2 = new QStandardItem;
    QStandardItem* item_3 = new QStandardItem;
    QStandardItem* item_4 = new QStandardItem;
    item_1->setText( name );
    item_2->setText( "0" );
    item_3->setText( "10" );
    item_4->setText( "121" );
    this->node_pointer_list.back()->model_list
            << item_1 << item_2 << item_3 << item_4;

    printf( "appended row to model with %d %d columns\n",
            this->node_pointer_list.back()->model_list.size(),
            source_model->columnCount() );

    QObject::connect( this->node_pointer_list.back(),
                         SIGNAL(selectionStateChanged(bool)),
                         this,
                         SLOT(updateSelectedNodes(bool)) );

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
