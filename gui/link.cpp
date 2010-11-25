#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOption>

#include <cstdio>

#include "link.h"

Link::Link( QGraphicsView *viewWidget, QGraphicsScene* new_scene  )
    : view(viewWidget), scene(new_scene) {

    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);
    setFlag(ItemSendsGeometryChanges);

    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    this->setAcceptHoverEvents(true);
    this->is_deletable = false;
    this->is_selected = false;

    this->setPen( QPen(QBrush(QColor( 0, 0, 0 )), 2) );

}

Link::~Link() {

    scene->removeItem( this );

}

QVariant Link::itemChange(GraphicsItemChange change, const QVariant &value) {

    switch (change) {

        case ItemSelectedHasChanged:
            if (value.toBool()) {
                this->setPen( QPen(QBrush(QColor( 0, 255, 0 )), 2) );
                this->update();
            } else {
                this->setPen( QPen(QBrush(QColor( 0, 255, 0 )), 2) );
                this->update();
            }
            break;
        default:
            break;

    };

    return QGraphicsItem::itemChange(change, value);

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
        this->setPen( QPen(QBrush(QColor( 255, 0, 0 )), 2) );
        this->update();

    }

    QGraphicsItem::hoverEnterEvent( event );

}


void Link::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {

    if ( this->is_deletable ) {

        printf( "hover leave\n");
        this->setPen( QPen(QBrush(QColor( 0, 0, 0 )), 2) );
        this->update();

    }

    QGraphicsItem::hoverLeaveEvent( event );

}
