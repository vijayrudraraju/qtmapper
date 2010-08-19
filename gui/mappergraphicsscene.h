#ifndef MAPPERGRAPHICSSCENE_H
#define MAPPERGRAPHICSSCENE_H

#include <QGraphicsScene>

class MapperGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MapperGraphicsScene(QObject *parent = 0);
    void setSelectionArea( const QPainterPath &path,
                           const QTransform &deviceTransform );
    void setSelectionArea( const QPainterPath &path,
                           Qt::ItemSelectionMode mode,
                           const QTransform &deviceTransform);

signals:

public slots:

};

#endif // MAPPERGRAPHICSSCENE_H
