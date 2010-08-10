#ifndef GUITHREAD_H
#define GUITHREAD_H

#include <QThread>

class GuiThread : public QThread
{
    Q_OBJECT
public:
    explicit GuiThread(QObject *parent = 0);

    void update( mapper_db_device device_db_cache );

signals:
    void updatedGui( const char* name );

public slots:


private:
    bool restart;
    bool abort;

};

#endif // GUITHREAD_H
