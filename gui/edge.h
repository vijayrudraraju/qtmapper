#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include "node.h"

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode);
    ~Edge();

    Node *sourceNode() const;
    void setSourceNode(Node *node);

    Node *destNode() const;
    void setDestNode(Node *node);

    void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Node *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif // EDGE_H
