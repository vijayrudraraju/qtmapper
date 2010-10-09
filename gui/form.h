#ifndef FORM_H
#define FORM_H

#include <list>

#include <QTimer>
#include <QStandardItemModel>

#include <mapper/mapper.h>>

#include "ui_form.h"
#include "edge.h"
#include "node.h"
#include "link.h"
#include "mappergraphicsscene.h"

class Form : public QWidget, private Ui::Form {

    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);

    void addNode();
    void sendNewMappingRequest( const char* source_signal_path,
                                const char* dest_signal_path );

    //int (*createTestDeviceFunction)();
    void addNewDevice( const char* name,
                       const char* host,
                       int port,
                       int can_alias );
    void addNewSignal( mapper_db_signal record );
    void addNewMapping( mapper_db_mapping record );

    void addDbDeviceCallbackFunction( device_callback_func* f );
    void addDbSignalCallbackFunction( signal_callback_func* f );
    void addDbLinkCallbackFunction( link_callback_func* f );
    void addDbMappingCallbackFunction( mapping_callback_func* f );
    void setMapperDevice( mapper_device device );

    void updateIsDeletable( bool checked );

    static bool IsNameMatch( Node* i );

    static const char* device_search_term;
    //static const char* source_search_term;
    //static const char* destination_search_term;

public slots:
    void update();

    void updateMappingView( );
    void updateVisualizationMode( int mode_index );

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

protected:
    //void keyPressEvent(QKeyEvent *event);
    //void timerEvent(QTimerEvent *event);
    //void wheelEvent(QWheelEvent *event);
    //void viewportEvent(QEvent* event);
    //void drawBackground(QPainter *painter, const QRectF &rect);
    //void scaleView(qreal scaleFactor);
    bool eventFilter( QObject* obj, QEvent* event );

private:
    void changeVisualizationMode( int current_mode );

    void addNodeToDestinationView( Node* the_node );
    void removeNodeFromDestinationView( Node* the_node );
    void addNodeToSourceView( Node* the_node );
    void removeNodeFromSourceView( Node* the_node );

    void clearMappingView( );

    int timerId;
    std::list <Node*> node_pointer_list;
    std::list <Link*> displayed_mapping_list;
    MapperGraphicsScene* scene;
    QGraphicsScene mapping_scene;

    QTimer* timer;

    device_callback_func* db_device_callback_function;
    signal_callback_func* db_signal_callback_function;
    link_callback_func* db_link_callback_function;
    mapping_callback_func* db_mapping_callback_function;
    mapper_device qtmapper;

    QStandardItemModel* source_model;
    QStandardItemModel* destination_model;

    QGraphicsItem* selected_source_circle;
    QGraphicsItem* selected_destination_circle;

    QPersistentModelIndex selected_signal;

    int default_x;
    int default_y;

    const char* active_node_name;
    bool mouse_is_pressed;
    //int cued_add_action_index; //1, 2, 3, 4 (4 possible actions)
    //int cued_remove_action_index; //1, 2, 3, 4 (4 possible actions)

};

#endif // FORM_H
