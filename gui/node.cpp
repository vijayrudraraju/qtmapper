#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOption>

#include <cstdio>

#include "edge.h"
#include "node.h"

Node::Node(QGraphicsView *graphWidget)
    : graph(graphWidget) {

    is_destination = false;
    is_source = false;
    conflict_flag = 0;

    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);
    //setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    name_item.setParentItem( this );
    name_item.setPos( 23, 23 );
    name_item.setText( "test" );

}

Node::~Node(  ) {


}

void Node::setName( const char* new_name ) {

    name = new_name;
    name_item.setText( new_name );

}

void Node::addMapping( Node* destination,
                       const char* source_signal_name,
                       const char* destination_signal_name ) {

    qt_mapping qt_map =
            (qt_mapping) calloc( 1, sizeof(struct _qt_mapping) );

    qt_map->destination_node = destination;
    qt_map->source_signal_name = strdup(source_signal_name);
    qt_map->destination_signal_name = strdup(destination_signal_name);

    this->destination_list.push_back( qt_map );

    printf( "Node::addMapping %s - %s %s %s\n",
            this->name,
            this->destination_list.back()->destination_node->name,
            this->destination_list.back()->source_signal_name,
            this->destination_list.back()->destination_signal_name);

}

bool Node::advance() {

    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;

}

QRectF Node::boundingRect() const {

    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                  23 + 20 + adjust, 23 + 10 + adjust);

}

QPainterPath Node::shape() const {

    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;

}

void Node::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * ) {

    //if ( option->state & QStyle::State_Selected ) {
    if ( is_source ) {

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::yellow);
        painter->drawEllipse(-7, -7, 20, 20);

    } else if ( is_destination ) {

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::blue);
        painter->drawEllipse(-7, -7, 20, 20);

    } else {

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
        painter->drawEllipse(-7, -7, 20, 20);

    }
    /*
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
    */

}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {

    switch (change) {

        case ItemPositionHasChanged:
            //foreach (Edge *edge, edgeList)
            //    edge->adjust();
            //graph->itemMoved();
            break;
        case ItemSelectedHasChanged:
            printf ( "node says -> selection signal %s %d\n", name, value.toBool() );
            emit selectionStateChanged( value.toBool() );
            break;
        default:
            break;

    };

    return QGraphicsItem::itemChange(change, value);

}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    //printf( "node says -> mouse press %s\n", name);
    emit nodePressed( this );

    QGraphicsItem::mousePressEvent(event);

}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    //printf( "node says -> mouse release %s\n", name);
    emit nodeReleased( this );

    QGraphicsItem::mouseReleaseEvent(event);

}
