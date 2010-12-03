#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <mapper/mapper_db.h>

#include "node.h"

class Link : public QObject, public QGraphicsLineItem {

    Q_OBJECT
    Q_INTERFACES( QGraphicsItem )

public:
    Link( QGraphicsView* viewWidget, QGraphicsScene* new_scene );
    ~Link(  );

    char source_signal_name[1024];
    char dest_signal_name[1024];
    mapper_db_mapping mapping;

    bool is_deletable;
    bool is_selected;
    const static int pen_width = 2;

signals:
    void linkPressed( Link* reference );
    void linkSelected( Link* reference );
    void linkUnselected( Link* reference );

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
