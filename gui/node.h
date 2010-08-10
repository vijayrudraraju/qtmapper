#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QLabel>

//#include <mapper/mapper.h>

class Edge;
class Form;

class Node : public QGraphicsItem
{
public:
    Node(QGraphicsView *graphWidget);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setName( const char* new_name );

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    QGraphicsView *graph;

    QLabel* name_label;
    const char* name;

};

#endif // NODE_H
