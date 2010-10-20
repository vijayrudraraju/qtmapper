#ifndef FORM_H
#define FORM_H

#include <list>

#include <QTimer>
#include <QStandardItemModel>

#include <mapper/mapper.h>

#include "ui_form.h"
#include "link.h"
#include "mappergraphicsscene.h"

class Node;

class Form : public QWidget, private Ui::Form {

    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);

    void addNode();
    void sendNewMappingRequest( const char* source_signal_path,
                                const char* dest_signal_path );

    //int (*createTestDeviceFunction)();
    void removeDevice( const char* name );
    void addNewDevice( const char* name,
                       const char* host,
                       int port,
                       int can_alias );
    void removeSignal( mapper_db_signal record );
    void addNewSignal( mapper_db_signal record );
    void removeMapping( mapper_db_mapping record );
    void addNewMapping( mapper_db_mapping record );

    void addDbDeviceCallbackFunction( device_callback_func* f );
    void addDbSignalCallbackFunction( signal_callback_func* f );
    void addDbLinkCallbackFunction( link_callback_func* f );
    void addDbMappingCallbackFunction( mapping_callback_func* f );
    void setMapperMonitor( mapper_monitor mon );

    void updateIsDeletable( bool checked );

    //static bool IsNameMatch( Node* i );

    //static const char* device_search_term;
    //static const char* source_search_term;
    //static const char* destination_search_term;

public slots:
    void update();

    void clearSources();
    void clearDests();

    void updateMappingView( );

    void updateDeleteButtonState( bool checked );
    void updatePressedLink( Link* reference );

    void updateSelectedNodes( bool is_selected );
    void updateSelectionMode( int index );
    void updateEditSelectionMode( int index );

    void updateMouseState( bool is_pressed );
    void updatePressedNode( Node* reference );
    void updateReleasedNode( Node* reference );
    void newDoubleClick( );

    void beginToDrawMapping( const QModelIndex& index );
    void finishDrawingMapping( const QModelIndex& index );

    void changeVisualizationMode( int mode_index );

protected:
    //void keyPressEvent(QKeyEvent *event);
    //void timerEvent(QTimerEvent *event);
    //void wheelEvent(QWheelEvent *event);
    //void viewportEvent(QEvent* event);
    //void drawBackground(QPainter *painter, const QRectF &rect);
    //void scaleView(qreal scaleFactor);
    bool eventFilter( QObject* obj, QEvent* event );

private:
    void updateVisualizationLinks( int current_mode );
    void updateVisualizationNodes( int current_mode );
    void updateVisualizationStuff( int current_mode );

    void addNodeToDestinationView( Node* the_node );
    void removeNodeFromDestinationView( Node* the_node );
    void addNodeToSourceView( Node* the_node );
    void removeNodeFromSourceView( Node* the_node );

    void clearMappingView( );

    int timerId;

    std::list<Node*> node_pointer_list;
    std::list<Link*> displayed_mapping_list;
    std::list<QGraphicsItem*> visualization_links;

    MapperGraphicsScene* scene;
    QGraphicsScene mapping_scene;

    QTimer* timer;

    device_callback_func* db_device_callback_function;
    signal_callback_func* db_signal_callback_function;
    link_callback_func* db_link_callback_function;
    mapping_callback_func* db_mapping_callback_function;

    mapper_db db;
    mapper_monitor mon;

    QStandardItemModel* source_model;
    QStandardItemModel* destination_model;

    QGraphicsItem* selected_source_circle;
    QGraphicsItem* selected_destination_circle;

    QPersistentModelIndex selected_signal;
    int signal_selected_flag;

    int default_x;
    int default_y;

    const char* active_node_name;
    bool mouse_is_pressed;

};

#endif // FORM_H
