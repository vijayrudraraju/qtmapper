/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created: Tue Aug 10 19:06:06 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *neighborhood_tab;
    QVBoxLayout *verticalLayout;
    QTabWidget *inner_tabWidget;
    QWidget *tab_1;
    QHBoxLayout *horizontalLayout;
    QListView *listView;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_2;
    QListView *listView_2;
    QPushButton *pushButton;
    QGraphicsView *graphicsView;
    QWidget *edit_tab;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(800, 600);
        verticalLayout_2 = new QVBoxLayout(Form);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 10, 2, 2);
        tabWidget = new QTabWidget(Form);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideLeft);
        tabWidget->setUsesScrollButtons(false);
        neighborhood_tab = new QWidget();
        neighborhood_tab->setObjectName(QString::fromUtf8("neighborhood_tab"));
        verticalLayout = new QVBoxLayout(neighborhood_tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        inner_tabWidget = new QTabWidget(neighborhood_tab);
        inner_tabWidget->setObjectName(QString::fromUtf8("inner_tabWidget"));
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        horizontalLayout = new QHBoxLayout(tab_1);
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listView = new QListView(tab_1);
        listView->setObjectName(QString::fromUtf8("listView"));

        horizontalLayout->addWidget(listView);

        inner_tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_2 = new QHBoxLayout(tab_2);
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        listView_2 = new QListView(tab_2);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));

        horizontalLayout_2->addWidget(listView_2);

        inner_tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(inner_tabWidget);

        pushButton = new QPushButton(neighborhood_tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        graphicsView = new QGraphicsView(neighborhood_tab);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

        verticalLayout->addWidget(graphicsView);

        tabWidget->addTab(neighborhood_tab, QString());
        edit_tab = new QWidget();
        edit_tab->setObjectName(QString::fromUtf8("edit_tab"));
        tabWidget->addTab(edit_tab, QString());

        verticalLayout_2->addWidget(tabWidget);


        retranslateUi(Form);

        tabWidget->setCurrentIndex(0);
        inner_tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        inner_tabWidget->setTabText(inner_tabWidget->indexOf(tab_1), QApplication::translate("Form", "selected sources", 0, QApplication::UnicodeUTF8));
        inner_tabWidget->setTabText(inner_tabWidget->indexOf(tab_2), QApplication::translate("Form", "selected destinations", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Form", "add test device", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(neighborhood_tab), QApplication::translate("Form", "neighborhood", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(edit_tab), QApplication::translate("Form", "edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
