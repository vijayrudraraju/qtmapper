#include "form.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <math.h>

Form::Form( QWidget *parent ) :
    QWidget( parent )
{
    setupUi( this );

    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
    timer->start(2000);

    scene = new QGraphicsScene(graphicsView);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-100, -100, 400, 200);
    graphicsView->setScene(scene);
    graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);

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

    //node1->setPos(-50, -50);
    //node2->setPos(0, -50);
    //node3->setPos(50, -50);
    //node4->setPos(-50, 0);
    //centerNode->setPos(0, 0);
    //node6->setPos(50, 0);
    //node7->setPos(-50, 50);
    //node8->setPos(0, 50);
    //node9->setPos(50, 50);

    graphicsView->scale(qreal(1.0), qreal(1.0));
    graphicsView->setMinimumSize(400, 200);
    setWindowTitle(tr("Mapper GUI"));

    /*QObject::connect(pushButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(createTestDevice()));*/
}

Form::~Form(  ) {
}

void Form::update(  ) {

    printf( "Form::update(  )\n");

    mdev_poll( this->qtmapper, 0 );

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

    printf( "addNewDevice(  )\n" );

    this->node_pointer_list.push_back( new Node(graphicsView) );
    this->node_pointer_list.back()->setName( name );
    scene->addItem(this->node_pointer_list.back());
    this->node_pointer_list.back()->setPos(-50, -50);

}

void Form::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void Form::scaleView(qreal scaleFactor)
{
    qreal factor =
            graphicsView->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    graphicsView->scale(scaleFactor, scaleFactor);
}

void Form::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, -event->delta() / 240.0));
}

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
    /*
    case Qt::Key_Space:
    case Qt::Key_Enter:
        foreach (QGraphicsItem *item, graphicsView->scene()->items()) {
            if (qgraphicsitem_cast<Node *>(item))
                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
        }
        break;
    */
    default:
        QWidget::keyPressEvent(event);
    }
}
