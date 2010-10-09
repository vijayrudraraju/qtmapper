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

    this->setAcceptHoverEvents(true);
    this->hover_flag = false;
    this->is_deletable = false;

}

Link::~Link() {

    scene->removeItem( this );

}

QRectF Link::boundingRect() const {

        return QRectF(-pen_width, -pen_width,
                  line_width+(2*pen_width), line_height+(2*pen_width));

}

void Link::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget ) {

    QPen pen;

    if ( !this->hover_flag ) {

        pen.setColor(Qt::black);

    } else {

        pen.setColor(Qt::red);

    }

    pen.setWidth(pen_width);
    painter->setPen(pen);
    //painter->setBrush(Qt::black);

    if ( this->is_inverted ) {

        //printf( "2 line_height: %d width %d\n", line_height, line_width);
        painter->drawLine(line_width, 0, 0, line_height);

    } else {

        painter->drawLine(0, 0, line_width, line_height);

    }

    QPen invisible_pen;
    invisible_pen.setColor(Qt::transparent);
    invisible_pen.setWidth(pen_width);
    painter->setPen(invisible_pen);

    if ( this->is_inverted ) {

        //printf( "2 line_height: %d width %d\n", line_height, line_width);
        painter->drawLine(line_width, pen_width, 0, line_height+pen_width);
        //painter->drawLine(line_width, pen_width*2, 0, line_height+(pen_width*2));
        painter->drawLine(line_width, -pen_width, 0, line_height-pen_width);
        //painter->drawLine(line_width, -(pen_width*2), 0, line_height-(pen_width*2));

    } else {

        painter->drawLine(0, pen_width, line_width, line_height+pen_width);
        painter->drawLine(0, -pen_width, line_width, line_height-pen_width);
        //painter->drawLine(0, pen_width*2, line_width, line_height+(pen_width*2));
        //painter->drawLine(0, -(pen_width*2), line_width, line_height-(pen_width*2));

    }

}

void Link::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if ( this->is_deletable ) {

        printf( "Link::mouse press\n" );
       emit linkPressed( this );

    }

    QGraphicsItem::mousePressEvent( event );

}

void Link::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    if ( this->is_deletable ) {

        printf( "hover enter\n");
        this->hover_flag = true;
        this->update();

    }

    QGraphicsItem::hoverEnterEvent( event );

}


void Link::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {

    if ( this->is_deletable ) {

        printf( "hover leave\n");
        this->hover_flag = false;
        this->update();

    }

    QGraphicsItem::hoverLeaveEvent( event );

}
