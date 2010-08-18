#ifndef MAPPERGRAPHICSVIEW_H
#define MAPPERGRAPHICSVIEW_H

#include <QGraphicsView>

class MapperGraphicsView : public QGraphicsView {

    Q_OBJECT

public:
    explicit MapperGraphicsView(QWidget *parent = 0);

signals:

public slots:

protected:
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal factor);

};

#endif // MAPPERGRAPHICSVIEW_H
