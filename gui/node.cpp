#include "utility.h"

Node::Node(QGraphicsView *graphWidget)
    : graph(graphWidget) {

    is_destination = false;
    is_source = false;
    conflict_flag = 0;

    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);
    //setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    name_item.setParentItem( this );
    name_item.setPos( 23, -22 );
    name_item.setText( "test" );

    inputs = 0;
    outputs = 0;
    diameter = 20;

    input_num_item.setParentItem( this );
    input_num_item.setPos( 23, 6 );
    input_num_item.setText( "inputs " + QString::number(inputs) );

    output_num_item.setParentItem( this );
    output_num_item.setPos( 23, -8 );
    output_num_item.setText( "outputs " + QString::number(outputs) );

}

Node::~Node(  ) {

    for ( QList<QStandardItem*>::iterator it = this->source_model_list.begin();
            it != this->source_model_list.end();
            it++ ) {

        delete (*it);

    }
    this->source_model_list.clear();

    for ( QList<QStandardItem*>::iterator it = this->destination_model_list.begin();
            it != this->destination_model_list.end();
            it++ ) {

        delete (*it);

    }
    this->destination_model_list.clear();

    for ( std::list<qt_mapping>::iterator it = this->destination_list.begin();
            it != this->destination_list.end();
            it++ ) {

        delete (*it);

    }
    this->destination_list.clear();

}

void Node::setName( const char* new_name ) {

    name = new_name;
    name_item.setText( new_name );

}

void Node::addMapping( Node* destination,
                       const char* source_signal_name,
                       const char* destination_signal_name ) {

    qt_mapping qt_map =
            (qt_mapping) calloc( 1, sizeof(struct _qt_mapping) );

    qt_map->destination_node = destination;
    qt_map->source_signal_name = strdup(source_signal_name);
    qt_map->destination_signal_name = strdup(destination_signal_name);

    this->destination_list.push_back( qt_map );

    printf( "Node::addMapping %s - %s %s %s\n",
            this->name,
            this->destination_list.back()->destination_node->name,
            this->destination_list.back()->source_signal_name,
            this->destination_list.back()->destination_signal_name);

}

void Node::removeMapping( Node* destination,
                       const char* source_signal_name,
                       const char* destination_signal_name ) {

    Utility::mapping_device_term = destination->name;
    Utility::mapping_source_signal_term = source_signal_name;
    Utility::mapping_dest_signal_term = destination_signal_name;
    this->destination_list.remove_if( Utility::findMapping );

}

QRectF Node::boundingRect() const {

    qreal adjust = 0;
    return QRectF( -(diameter/2) - adjust, -(diameter/2) - adjust,
                  diameter + 20 + adjust, diameter + 10 + adjust );

}

void Node::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget ) {

    name_item.setPos( diameter+3, -23 );
    input_num_item.setText( "inputs " + QString::number(inputs) );
    input_num_item.setPos( diameter+3, 8 );
    output_num_item.setText( "outputs " + QString::number(outputs) );
    output_num_item.setPos( diameter+3, -7 );

    painter->setOpacity( 0.7 );

    if ( is_source ) {

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::yellow);
        painter->drawEllipse( -(diameter/2), -(diameter/2),
                             diameter, diameter);

    } else if ( is_destination ) {

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::blue);
        painter->drawEllipse( -(diameter/2), -(diameter/2),
                             diameter, diameter);

    } else {

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
        painter->drawEllipse( -(diameter/2), -(diameter/2),
                             diameter, diameter);

    }

}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {

    switch (change) {

        case ItemPositionHasChanged:
            //foreach (Edge *edge, edgeList)
            //    edge->adjust();
            //graph->itemMoved();
            break;
        case ItemSelectedHasChanged:
            printf ( "node says -> selection signal %s %d\n", name, value.toBool() );
            emit selectionStateChanged( value.toBool() );
            break;
        default:
            break;

    };

    return QGraphicsItem::itemChange(change, value);

}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    //printf( "node says -> mouse press %s\n", name);
    emit nodePressed( this );

    QGraphicsItem::mousePressEvent(event);

}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    //printf( "node says -> mouse release %s\n", name);
    emit nodeReleased( this );

    QGraphicsItem::mouseReleaseEvent(event);

}
