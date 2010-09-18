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

void Node::addMapping(Node* destination) {

    this->destination_list.push_back( destination );

}

/*
QList<Edge *> Node::edges() const {

    return edgeList;

}
*/

/*
void Node::calculateForces() {

    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;

        QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
        qreal dx = line.dx();
        qreal dy = line.dy();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList) {

        QPointF pos;
        if (edge->sourceNode() == this)
            pos = mapFromItem(edge->destNode(), 0, 0);
        else
            pos = mapFromItem(edge->sourceNode(), 0, 0);
        xvel += pos.x() / weight;
        yvel += pos.y() / weight;

    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));

}
*/

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

    printf( "node says -> mouse press %s\n", name);
    emit nodePressed( this );

    QGraphicsItem::mousePressEvent(event);

}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    printf( "node says -> mouse release %s\n", name);
    emit nodeReleased( this );

    QGraphicsItem::mouseReleaseEvent(event);

}
