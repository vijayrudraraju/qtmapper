#ifndef MAPPERGRAPHICSVIEW_H
#define MAPPERGRAPHICSVIEW_H

#include <QGraphicsView>

class MapperGraphicsView : public QGraphicsView {

    Q_OBJECT

public:
    explicit MapperGraphicsView( QWidget *parent = 0 );

signals:
    void mouseStateChanged( bool is_pressed );

public slots:

protected:
    void wheelEvent( QWheelEvent *event );
    void scaleView( qreal factor );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

};

#endif // MAPPERGRAPHICSVIEW_H
