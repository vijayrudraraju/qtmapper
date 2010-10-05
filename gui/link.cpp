#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOption>

#include "link.h"

Link::Link( QGraphicsView *viewWidget, QGraphicsScene* new_scene  )
    : view(viewWidget), scene(new_scene) {

    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

}

Link::~Link() {

    scene->removeItem( this );

}

QRectF Link::boundingRect() const {

        return QRectF(-4, -4,
                  line_width+8, line_height+8);

}

void Link::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget ) {

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter->setPen(pen);
    painter->setBrush(Qt::black);

    if ( this->is_inverted ) {

        //printf( "2 line_height: %d width %d\n", line_height, line_width);
        painter->drawLine(line_width, 0, 0, line_height);

    } else {

        painter->drawLine(0, 0, line_width, line_height);

    }

}

void Link::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    printf( "link says -> mouse press %s %s\n",
            this->source_signal_name,
            this->dest_signal_name );
    //emit nodePressed( this );
    //printf( "node says -> mouse press %s\n", name);
    emit linkPressed( this );


    QGraphicsItem::mousePressEvent(event);

}
