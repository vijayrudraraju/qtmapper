#ifndef FORM_H
#define FORM_H

#include <list>

#include <QTimer>
#include <QStandardItemModel>

#include <mapper/mapper.h>

#include "ui_form.h"
#include "edge.h"
#include "node.h"
#include "mappergraphicsscene.h"

class Form : public QWidget, private Ui::Form {

    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

    void itemMoved();
    void addNode();

    //int (*createTestDeviceFunction)();
    void addNewDevice( const char* name );
    void addDbCallbackFunction( device_callback_func* f );
    void setMapperDevice( mapper_device device );

public slots:
    void update();
    void updateMouseState( bool is_pressed );
    void updateSelectedNodes( bool is_selected );
    void updateSelectionMode( int index );

protected:
    //void keyPressEvent(QKeyEvent *event);
    //void timerEvent(QTimerEvent *event);
    //void wheelEvent(QWheelEvent *event);
    //void viewportEvent(QEvent* event);
    //void drawBackground(QPainter *painter, const QRectF &rect);
    //void scaleView(qreal scaleFactor);

private:
    int timerId;
    std::list <Node*> node_pointer_list;
    MapperGraphicsScene* scene;

    QTimer* timer;
    device_callback_func* db_callback_function;
    mapper_device qtmapper;

    //QStandardItemModel* neighborhood_model;
    //QStandardItemModel* real_time_source_model;
    QStandardItemModel* source_model;
    //QStandardItemModel* real_time_destination_model;
    QStandardItemModel* destination_model;

    int default_x;
    int default_y;

    //std::list <Node*> active_node_list;
    bool mouse_is_pressed;
    //int cued_add_action_index; //1, 2, 3, 4 (4 possible actions)
    //int cued_remove_action_index; //1, 2, 3, 4 (4 possible actions)

};

#endif // FORM_H
