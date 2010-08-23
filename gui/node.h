#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QLabel>
#include <QStandardItem>

//#include <mapper/mapper.h>

class Edge;
class Form;

class Node : public QObject, public QGraphicsItem
{

    Q_OBJECT
    Q_INTERFACES( QGraphicsItem )

public:
    Node(QGraphicsView* graphWidget);
    ~Node();

    void addEdge(Edge* edge);
    QList<Edge*> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    //void calculateForces();
    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);

    void setName( const char* new_name );

    QList<QStandardItem*> source_model_list;
    QList<QStandardItem*> destination_model_list;
    bool is_source;
    bool is_destination;
    int conflict_flag; //-1 => source, 0 => no confict, 1 => destination
    const char* name;

signals:
    void selectionStateChanged( bool is_selected );

    void nodePressed( Node* reference );
    void nodeReleased( Node* reference );

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    QGraphicsView *graph;

    QGraphicsSimpleTextItem name_item;

};

#endif // NODE_H
