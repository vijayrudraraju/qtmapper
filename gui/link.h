#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>

class Link : public QObject, public QGraphicsItem {

    Q_OBJECT
    Q_INTERFACES( QGraphicsItem )

public:
    Link( QGraphicsView* viewWidget, QGraphicsScene* new_scene  );
    ~Link(  );

    QRectF boundingRect() const;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);

    char source_signal_name[1024];
    char dest_signal_name[1024];
    int line_width;
    int line_height;
    bool is_inverted;

signals:
    void linkPressed( Link* reference );

private:
    QGraphicsView* view;
    QGraphicsScene* scene;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);


};

#endif // LINK_H
