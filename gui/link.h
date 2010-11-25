#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>

class Link : public QObject, public QGraphicsLineItem {

    Q_OBJECT
    Q_INTERFACES( QGraphicsItem )

public:
    Link( QGraphicsView* viewWidget, QGraphicsScene* new_scene );
    ~Link(  );

    char source_signal_name[1024];
    char dest_signal_name[1024];
    const char* expr;

    bool is_deletable;
    bool is_selected;
    const static int pen_width = 2;

signals:
    void linkPressed( Link* reference );

private:
    QGraphicsView* view;
    QGraphicsScene* scene;

protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

};

#endif // LINK_H
