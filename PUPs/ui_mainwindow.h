/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Dec 2 12:24:33 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "renderer.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionClose;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionSave;
    QAction *actionLoad;
    QAction *actionTest;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter_3;
    QSplitter *splitter_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    Renderer *glCurvePane;
    QFrame *frame1;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    Renderer *glParameterPane;
    QSplitter *splitter;
    QFrame *frame2;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    Renderer *glProjectionPane;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    Renderer *glBasisEditorPane;
    QMenuBar *menuBar;
    QMenu *menuApplication;
    QMenu *menuEdit;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(931, 560);
        MainWindow->setMinimumSize(QSize(400, 300));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionTest = new QAction(MainWindow);
        actionTest->setObjectName(QString::fromUtf8("actionTest"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        splitter_2 = new QSplitter(splitter_3);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        frame = new QFrame(splitter_2);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(3);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 2, 0, 0);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMargin(0);
        label->setIndent(2);

        verticalLayout_2->addWidget(label);

        glCurvePane = new Renderer(frame);
        glCurvePane->setObjectName(QString::fromUtf8("glCurvePane"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(glCurvePane->sizePolicy().hasHeightForWidth());
        glCurvePane->setSizePolicy(sizePolicy);
        glCurvePane->setMinimumSize(QSize(200, 200));
        glCurvePane->setCursor(QCursor(Qt::ArrowCursor));
        glCurvePane->setMouseTracking(true);
        glCurvePane->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_2->addWidget(glCurvePane);

        splitter_2->addWidget(frame);
        frame1 = new QFrame(splitter_2);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setFrameShape(QFrame::Panel);
        frame1->setFrameShadow(QFrame::Sunken);
        frame1->setLineWidth(3);
        verticalLayout_3 = new QVBoxLayout(frame1);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 2, 0, 0);
        label_3 = new QLabel(frame1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setFrameShape(QFrame::NoFrame);
        label_3->setFrameShadow(QFrame::Sunken);
        label_3->setLineWidth(4);
        label_3->setIndent(2);

        verticalLayout_3->addWidget(label_3);

        glParameterPane = new Renderer(frame1);
        glParameterPane->setObjectName(QString::fromUtf8("glParameterPane"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(glParameterPane->sizePolicy().hasHeightForWidth());
        glParameterPane->setSizePolicy(sizePolicy2);
        glParameterPane->setMinimumSize(QSize(200, 100));
        glParameterPane->setCursor(QCursor(Qt::ArrowCursor));
        glParameterPane->setMouseTracking(true);
        glParameterPane->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_3->addWidget(glParameterPane);

        splitter_2->addWidget(frame1);
        splitter_3->addWidget(splitter_2);
        splitter = new QSplitter(splitter_3);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        frame2 = new QFrame(splitter);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setFrameShape(QFrame::Panel);
        frame2->setFrameShadow(QFrame::Sunken);
        frame2->setLineWidth(3);
        verticalLayout = new QVBoxLayout(frame2);
        verticalLayout->setSpacing(2);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 2, 0, 0);
        label_2 = new QLabel(frame2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setIndent(2);

        verticalLayout->addWidget(label_2);

        glProjectionPane = new Renderer(frame2);
        glProjectionPane->setObjectName(QString::fromUtf8("glProjectionPane"));
        sizePolicy.setHeightForWidth(glProjectionPane->sizePolicy().hasHeightForWidth());
        glProjectionPane->setSizePolicy(sizePolicy);
        glProjectionPane->setMinimumSize(QSize(200, 200));
        glProjectionPane->setCursor(QCursor(Qt::ArrowCursor));
        glProjectionPane->setMouseTracking(true);
        glProjectionPane->setFocusPolicy(Qt::StrongFocus);

        verticalLayout->addWidget(glProjectionPane);

        splitter->addWidget(frame2);
        frame_2 = new QFrame(splitter);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::Panel);
        frame_2->setFrameShadow(QFrame::Sunken);
        frame_2->setLineWidth(3);
        verticalLayout_4 = new QVBoxLayout(frame_2);
        verticalLayout_4->setSpacing(2);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 2, 0, 0);
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFrameShape(QFrame::NoFrame);
        label_4->setFrameShadow(QFrame::Sunken);
        label_4->setLineWidth(4);
        label_4->setIndent(2);

        verticalLayout_4->addWidget(label_4);

        glBasisEditorPane = new Renderer(frame_2);
        glBasisEditorPane->setObjectName(QString::fromUtf8("glBasisEditorPane"));
        sizePolicy.setHeightForWidth(glBasisEditorPane->sizePolicy().hasHeightForWidth());
        glBasisEditorPane->setSizePolicy(sizePolicy);
        glBasisEditorPane->setMinimumSize(QSize(200, 200));
        glBasisEditorPane->setCursor(QCursor(Qt::ArrowCursor));
        glBasisEditorPane->setMouseTracking(true);
        glBasisEditorPane->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_4->addWidget(glBasisEditorPane);

        splitter->addWidget(frame_2);
        splitter_3->addWidget(splitter);

        horizontalLayout->addWidget(splitter_3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 931, 21));
        menuApplication = new QMenu(menuBar);
        menuApplication->setObjectName(QString::fromUtf8("menuApplication"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuApplication->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuApplication->addAction(actionSave);
        menuApplication->addAction(actionLoad);
        menuApplication->addAction(actionClose);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);

        retranslateUi(MainWindow);
        QObject::connect(actionClose, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(glCurvePane, SIGNAL(updateNext(int)), glProjectionPane, SLOT(updateMe(int)));
        QObject::connect(glProjectionPane, SIGNAL(updateNext(int)), glBasisEditorPane, SLOT(updateMe(int)));
        QObject::connect(glBasisEditorPane, SIGNAL(updateNext(int)), glParameterPane, SLOT(updateMe(int)));
        QObject::connect(glParameterPane, SIGNAL(updateNext(int)), glCurvePane, SLOT(updateMe(int)));
        QObject::connect(actionUndo, SIGNAL(triggered()), glCurvePane, SLOT(undo()));
        QObject::connect(actionRedo, SIGNAL(triggered()), glCurvePane, SLOT(redo()));
        QObject::connect(actionLoad, SIGNAL(triggered()), glCurvePane, SLOT(loadSlot()));
        QObject::connect(actionSave, SIGNAL(triggered()), glCurvePane, SLOT(saveSlot()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        actionLoad->setText(QApplication::translate("MainWindow", "Load", 0, QApplication::UnicodeUTF8));
        actionTest->setText(QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "PUPs Curve Editor:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        glCurvePane->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        label_3->setText(QApplication::translate("MainWindow", "Parameter Space:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        glParameterPane->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        label_2->setText(QApplication::translate("MainWindow", "PUPs Projection Display:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        glProjectionPane->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        label_4->setText(QApplication::translate("MainWindow", "Basis Editor:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        glBasisEditorPane->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        menuApplication->setTitle(QApplication::translate("MainWindow", "Application", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
