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
    sides = 2;
    concavity = 1.0;
    this->r = 155;
    this->g = 155;
    this->b = 155;

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

    for ( QList<QStandardItem*>::iterator it = this->dest_model_list.begin();
            it != this->dest_model_list.end();
            it++ ) {

        delete (*it);

    }
    this->dest_model_list.clear();

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
                       const char* destination_signal_name,
                       qt_mapping mapping ) {

    /*
    qt_mapping qt_map =
            (qt_mapping) calloc( 1, sizeof(struct _qt_mapping) );

    qt_map->destination_node = destination;
    qt_map->src_name = strdup(source_signal_name);
    qt_map->dest_name = strdup(destination_signal_name);
    */

    mapping->destination_node = destination;
    this->destination_list.push_back( mapping );

    printf( "Node::addMapping %s - %s %s %s\n",
            this->name,
            this->destination_list.back()->destination_node->name,
            this->destination_list.back()->src_name,
            this->destination_list.back()->dest_name);

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

    qreal adjust = 120;
    return QRectF( -(diameter/2) - adjust, -(diameter/2) - adjust,
                  diameter + 20 + adjust, diameter + 10 + adjust );

}

void Node::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget ) {

    name_item.setPos( (diameter/2)+5, -25 );
    output_num_item.setText( "outputs " + QString::number(outputs) );
    output_num_item.setPos( (diameter/2)+5, -10 );
    input_num_item.setText( "inputs " + QString::number(inputs) );
    input_num_item.setPos( (diameter/2)+5, 5 );

    painter->setOpacity( 0.7 );
    painter->setPen(Qt::NoPen);

    if ( is_source ) {

        painter->setBrush(Qt::yellow);

    } else if ( is_destination ) {

        painter->setBrush(Qt::blue);

    } else {

        //painter->setBrush(Qt::darkGray);
        painter->setBrush( QBrush( QColor( this->r, this->g, this->b ) ) );

    }

    this->diameter = ( this->outputs + this->inputs + 1 ) * 20;
    double half = diameter/2;
    double quarter = diameter/4;

    double quad_x;
    double quad_y;
    double concavity = 0.0;

    QPainterPath path;
    qreal adjust = 120;
    /*
    path.moveTo( -(diameter/2) - adjust, -(diameter/2) - adjust );
    path.lineTo( diameter + 20 + adjust, -(diameter/2) - adjust );
    path.lineTo( diameter + 20 + adjust, diameter + 10 + adjust );
    path.lineTo( -(diameter/2) - adjust, diameter + 10 + adjust );
    path.lineTo( -(diameter/2) - adjust, -(diameter/2) - adjust );
    painter->drawPath( path );
    */

    if ( this->outputs > this->inputs ) {

        concavity = 60.0;

    } else if ( this->inputs > this->outputs ) {

        concavity = -20.0;

    } else if ( this->inputs == this->outputs ) {

        concavity = 0.0;

    }

    sides = this->outputs + this->inputs;

    if ( sides == 0 ) {

        painter->drawEllipse( -half, -half,
                            diameter, diameter);

    } else if ( sides == 1) {

        path.arcTo( -half, -half, diameter, diameter, 135.0, 270.0 );

        Utility::findQuadPoint( half / sqrt(2) , -half / sqrt(2),
                                -half / sqrt(2) , -half / sqrt(2),
                                &quad_x, &quad_y, -concavity );
        path.quadTo( quad_x, quad_y,
                     -half / sqrt(2) , -half / sqrt(2) );

        painter->drawPath( path );

    } else if ( sides == 2 ) {

        path.arcTo( -half, -half, diameter, diameter, 135.0, 90.0 );

        Utility::findQuadPoint( -half / sqrt(2) , half / sqrt(2),
                                half / sqrt(2) , half / sqrt(2),
                                &quad_x, &quad_y, -concavity );
        path.quadTo( quad_x, quad_y,
                     half / sqrt(2) , half / sqrt(2) );

        path.arcTo( -half, -half, diameter, diameter, 315.0, 90.0 );

        Utility::findQuadPoint( half / sqrt(2) , -half / sqrt(2),
                                -half / sqrt(2) , -half / sqrt(2),
                                &quad_x, &quad_y, -concavity );
        path.quadTo( quad_x, quad_y,
                     -half / sqrt(2) , -half / sqrt(2) );

        painter->drawPath( path );

    } else if ( sides == 3 ) {

        path.moveTo( 0, -half );

        Utility::findQuadPoint( 0, -half, quarter*sqrt(3), quarter,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     quarter * sqrt(3), quarter );

        Utility::findQuadPoint( quarter*sqrt(3), quarter,
                                -quarter*sqrt(3), quarter,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y, -quarter * sqrt(3), quarter );

        Utility::findQuadPoint( -quarter*sqrt(3), quarter,
                                0, -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y, 0, -half );

        painter->drawPath( path );

    } else if ( sides == 4 ) {

        path.moveTo( 0, -half );

        Utility::findQuadPoint( 0, -half, half, 0,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     half, 0 );

        Utility::findQuadPoint( half, 0,
                                0, half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, half );

        Utility::findQuadPoint( 0, half,
                                -half, 0,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     -half, 0 );

        Utility::findQuadPoint( -half, 0,
                                0, -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, -half );

        painter->drawPath( path );

    } else if ( sides == 5 ) {

        path.moveTo( 0, -half );

        Utility::findQuadPoint( 0, -half,
                                0.95 * half, 0.31 * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.95 * half, 0.31 * -half );

        Utility::findQuadPoint( 0.95 * half, 0.31 * -half,
                                0.59 * half, 0.81 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.59 * half, 0.81 * half );

        Utility::findQuadPoint( 0.59 * half, 0.81 * half,
                                0.59 * -half, 0.81 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.59 * -half, 0.81 * half );

        Utility::findQuadPoint( 0.59 * -half, 0.81 * half,
                                0.95 * -half, 0.31 * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.95 * -half, 0.31 * -half );

        Utility::findQuadPoint( 0.95 * -half, 0.31 * -half,
                                0, -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, -half );

        painter->drawPath( path );

    } else if ( sides == 6 ) {

        path.moveTo( 0, -half );

        Utility::findQuadPoint( 0, -half,
                                sqrt(3) * quarter, 0.5 * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     sqrt(3) * quarter, 0.5 * -half );

        Utility::findQuadPoint( sqrt(3) * quarter, 0.5 * -half,
                                sqrt(3) * quarter, 0.5 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     sqrt(3) * quarter, 0.5 * half );

        Utility::findQuadPoint( sqrt(3) * quarter, 0.5 * half,
                                0, half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, half );

        Utility::findQuadPoint( 0, half,
                                sqrt(3) * -quarter, 0.5 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     sqrt(3) * -quarter, 0.5 * half );

        Utility::findQuadPoint( sqrt(3) * -quarter, 0.5 * half,
                                sqrt(3) * -quarter, 0.5 * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     sqrt(3) * -quarter, 0.5 * -half );

        Utility::findQuadPoint( sqrt(3) * -quarter, 0.5 * -half,
                                0, -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, -half );

        painter->drawPath( path );

    } else if ( sides == 7 ) {

        path.moveTo( 0, -half );

        Utility::findQuadPoint( 0, -half,
                                0.78 * half, 0.62 * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.78 * half, 0.62 * -half );

        Utility::findQuadPoint( 0.78 * half, 0.62 * -half,
                                0.97 * half, 0.22 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.97 * half, 0.22 * half );

        Utility::findQuadPoint( 0.97 * half, 0.22 * half,
                                0.43 * half, 0.9 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.43 * half, 0.9 * half );

        Utility::findQuadPoint( 0.43 * half, 0.9 * half,
                                0.43 * -half, 0.9 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.43 * -half, 0.9 * half );

        Utility::findQuadPoint( 0.43 * -half, 0.9 * half,
                                0.97 * -half, 0.22 * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.97 * -half, 0.22 * half );

        Utility::findQuadPoint( 0.97 * -half, 0.22 * half,
                                0.78 * -half, 0.62 * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0.78 * -half, 0.62 * -half );

        Utility::findQuadPoint( 0.78 * -half, 0.62 * -half,
                                0, -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, -half );

        painter->drawPath( path );

    } else if ( sides == 8 ) {

        path.moveTo( 0, -half );

        Utility::findQuadPoint( 0, -half,
                                1/sqrt(2) * half, 1/sqrt(2) * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                    1/sqrt(2) * half, 1/sqrt(2) * -half );

        Utility::findQuadPoint( 1/sqrt(2) * half, 1/sqrt(2) * -half,
                                half, 0,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     half, 0 );

        Utility::findQuadPoint( half, 0,
                                1/sqrt(2) * half, 1/sqrt(2) * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     1/sqrt(2) * half, 1/sqrt(2) * half );

        Utility::findQuadPoint( 1/sqrt(2) * half, 1/sqrt(2) * half,
                                0, half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, half );

        Utility::findQuadPoint( 0, half,
                                1/sqrt(2) * -half, 1/sqrt(2) * half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     1/sqrt(2) * -half, 1/sqrt(2) * half );

        Utility::findQuadPoint( 1/sqrt(2) * -half, 1/sqrt(2) * half,
                                -half, 0,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     -half, 0 );

        Utility::findQuadPoint( -half, 0,
                                1/sqrt(2) * -half, 1/sqrt(2) * -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     1/sqrt(2) * -half, 1/sqrt(2) * -half );

        Utility::findQuadPoint( 1/sqrt(2) * -half, 1/sqrt(2) * -half,
                                0, -half,
                                &quad_x, &quad_y, concavity );
        path.quadTo( quad_x, quad_y,
                     0, -half );


        painter->drawPath( path );

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
