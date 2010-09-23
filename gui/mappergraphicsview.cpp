#include <math.h>
#include <cstdio>

#include <QWheelEvent>

#include "mappergraphicsview.h"

MapperGraphicsView::MapperGraphicsView( QWidget *parent ) :
    QGraphicsView(parent) {

    printf( "MapperGraphicsView\n" );

}

void MapperGraphicsView::mousePressEvent( QMouseEvent *event ) {

    QGraphicsView::mousePressEvent( event );
    emit mouseStateChanged( true );

}

void MapperGraphicsView::mouseReleaseEvent( QMouseEvent *event ) {

    QGraphicsView::mouseReleaseEvent( event );
    emit mouseStateChanged( false );

}

void MapperGraphicsView::mouseDoubleClickEvent( QMouseEvent *event ) {

    QGraphicsView::mouseDoubleClickEvent( event );
    emit mouseDoubleClick( );

}

void MapperGraphicsView::wheelEvent( QWheelEvent *event ) {

    scaleView(pow((double)2, event->delta() / 240.0));
    //printf( "wheelEvent \n" );
    event->accept();

}

void MapperGraphicsView::scaleView( qreal factor ) {

    // check to make sure we don't zoom out or in too far
    qreal test_factor =
        matrix().scale(factor, factor).mapRect(QRectF(0, 0, 1, 1)).width();

    if ( test_factor < 0.07 || test_factor > 50 ) {

        return;

    }

    // zoom
    scale( factor, factor );

}
