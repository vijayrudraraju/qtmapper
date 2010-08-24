#include <cstdio>
#include "mappergraphicsscene.h"

MapperGraphicsScene::MapperGraphicsScene(QObject *parent) :
    QGraphicsScene(parent) {

    printf( "MapperGraphicsScene constructed\n" );

}

void setSelectionArea( const QPainterPath &path,
                       const QTransform &deviceTransform ) {

    printf( "setSelectionArea\n" );

}

void setSelectionArea( const QPainterPath &path,
                       Qt::ItemSelectionMode mode,
                       const QTransform &deviceTransform ) {

    printf( "setSelectionArea\n" );

}
