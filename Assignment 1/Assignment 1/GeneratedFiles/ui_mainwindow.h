/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Sep 10 16:33:32 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "mypanelopengl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionRectangle;
    QAction *actionCircle;
    QAction *actionTriangle;
    QAction *actionCustom;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    MyPanelOpenGL *panelGL;
    QGroupBox *circleGroup;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_4;
    QSpinBox *spinBox_3;
    QGroupBox *rectangleGroup;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_2;
    QDoubleSpinBox *doubleSpinBox_3;
    QGroupBox *triangleGroup;
    QComboBox *pointBox;
    QPushButton *quitBtn;
    QMenuBar *menuBar;
    QMenu *menuDraw;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(609, 525);
        actionRectangle = new QAction(MainWindow);
        actionRectangle->setObjectName(QString::fromUtf8("actionRectangle"));
        actionCircle = new QAction(MainWindow);
        actionCircle->setObjectName(QString::fromUtf8("actionCircle"));
        actionTriangle = new QAction(MainWindow);
        actionTriangle->setObjectName(QString::fromUtf8("actionTriangle"));
        actionCustom = new QAction(MainWindow);
        actionCustom->setObjectName(QString::fromUtf8("actionCustom"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        panelGL = new MyPanelOpenGL(centralWidget);
        panelGL->setObjectName(QString::fromUtf8("panelGL"));

        gridLayout->addWidget(panelGL, 0, 0, 4, 1);

        circleGroup = new QGroupBox(centralWidget);
        circleGroup->setObjectName(QString::fromUtf8("circleGroup"));
        label_3 = new QLabel(circleGroup);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 20, 46, 13));
        doubleSpinBox = new QDoubleSpinBox(circleGroup);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setGeometry(QRect(70, 20, 41, 22));
        doubleSpinBox->setDecimals(1);
        doubleSpinBox->setMinimum(1);
        doubleSpinBox->setMaximum(50);
        doubleSpinBox->setSingleStep(0.1);
        label_4 = new QLabel(circleGroup);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 60, 46, 13));
        spinBox_3 = new QSpinBox(circleGroup);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setGeometry(QRect(70, 60, 42, 22));
        spinBox_3->setMinimum(5);
        spinBox_3->setMaximum(60);

        gridLayout->addWidget(circleGroup, 0, 1, 1, 1);

        rectangleGroup = new QGroupBox(centralWidget);
        rectangleGroup->setObjectName(QString::fromUtf8("rectangleGroup"));
        label = new QLabel(rectangleGroup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 46, 13));
        label_2 = new QLabel(rectangleGroup);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 46, 13));
        doubleSpinBox_2 = new QDoubleSpinBox(rectangleGroup);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setGeometry(QRect(60, 20, 41, 22));
        doubleSpinBox_2->setDecimals(1);
        doubleSpinBox_2->setMinimum(1);
        doubleSpinBox_2->setMaximum(20);
        doubleSpinBox_3 = new QDoubleSpinBox(rectangleGroup);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setGeometry(QRect(60, 50, 41, 22));
        doubleSpinBox_3->setDecimals(1);
        doubleSpinBox_3->setMinimum(1);
        doubleSpinBox_3->setMaximum(20);

        gridLayout->addWidget(rectangleGroup, 1, 1, 1, 1);

        triangleGroup = new QGroupBox(centralWidget);
        triangleGroup->setObjectName(QString::fromUtf8("triangleGroup"));
        pointBox = new QComboBox(triangleGroup);
        pointBox->setObjectName(QString::fromUtf8("pointBox"));
        pointBox->setGeometry(QRect(20, 20, 69, 22));
        pointBox->setMaxVisibleItems(3);

        gridLayout->addWidget(triangleGroup, 2, 1, 1, 1);

        quitBtn = new QPushButton(centralWidget);
        quitBtn->setObjectName(QString::fromUtf8("quitBtn"));
        quitBtn->setMaximumSize(QSize(121, 23));

        gridLayout->addWidget(quitBtn, 3, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 609, 21));
        menuDraw = new QMenu(menuBar);
        menuDraw->setObjectName(QString::fromUtf8("menuDraw"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuDraw->menuAction());
        menuDraw->addAction(actionRectangle);
        menuDraw->addAction(actionCircle);
        menuDraw->addAction(actionTriangle);
        menuDraw->addAction(actionCustom);

        retranslateUi(MainWindow);
        QObject::connect(quitBtn, SIGNAL(clicked()), MainWindow, SLOT(close()));
        QObject::connect(MainWindow, SIGNAL(actionTriggered(QString)), panelGL, SLOT(setDrawMode(QString)));
        QObject::connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), panelGL, SLOT(changeRectangleWidth(double)));
        QObject::connect(doubleSpinBox_3, SIGNAL(valueChanged(double)), panelGL, SLOT(changeRectangleHeight(double)));
        QObject::connect(spinBox_3, SIGNAL(valueChanged(int)), panelGL, SLOT(changeCircleVerticies(int)));
        QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), panelGL, SLOT(changeCircleRadius(double)));
        QObject::connect(MainWindow, SIGNAL(actionTriggered(QString)), MainWindow, SLOT(disableWidgets(QString)));
        QObject::connect(pointBox, SIGNAL(currentIndexChanged(int)), panelGL, SLOT(setCursorTriangle(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionRectangle->setText(QApplication::translate("MainWindow", "Rectangle", 0, QApplication::UnicodeUTF8));
        actionCircle->setText(QApplication::translate("MainWindow", "Circle", 0, QApplication::UnicodeUTF8));
        actionTriangle->setText(QApplication::translate("MainWindow", "Triangle", 0, QApplication::UnicodeUTF8));
        actionCustom->setText(QApplication::translate("MainWindow", "Custom", 0, QApplication::UnicodeUTF8));
        circleGroup->setTitle(QApplication::translate("MainWindow", "Circle", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Radius:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Verticies:", 0, QApplication::UnicodeUTF8));
        rectangleGroup->setTitle(QApplication::translate("MainWindow", "Rectangle", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Width:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Height:", 0, QApplication::UnicodeUTF8));
        triangleGroup->setTitle(QApplication::translate("MainWindow", "Triangle", 0, QApplication::UnicodeUTF8));
        pointBox->clear();
        pointBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Point 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Point 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Point 3", 0, QApplication::UnicodeUTF8)
        );
        quitBtn->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        menuDraw->setTitle(QApplication::translate("MainWindow", "Draw", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
