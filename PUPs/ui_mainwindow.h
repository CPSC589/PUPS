/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Dec 15 19:02:53 2011
**      by: Qt User Interface Compiler version 4.7.0
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
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
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
    QAction *actionFade;
    QAction *actionApply_Current_Basis_to_New;
    QAction *actionSave_Basis_Collection;
    QAction *actionLoad_Basis_Collection;
    QAction *actionAdd_Selected_Basis_to_Collection;
    QAction *actionClear_Screen;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    Renderer *glCurvePane;
    QFrame *frame2;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget_2;
    QWidget *tab_7;
    QVBoxLayout *verticalLayout_8;
    Renderer *glProjectionPane;
    QWidget *tab_8;
    QVBoxLayout *verticalLayout_9;
    Renderer *glBasisCollection;
    QHBoxLayout *ButtonLayout;
    QPushButton *PreviousButton;
    QPushButton *ApplyButton;
    QPushButton *ApplyAllButton;
    QPushButton *MakeDefaultButton;
    QPushButton *NextButton;
    QPushButton *AddToCollectionButton;
    QFrame *frame1;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    Renderer *glParameterPane;
    QMenuBar *menuBar;
    QMenu *menuApplication;
    QMenu *menuEdit;
    QMenu *menuView;
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
        actionFade = new QAction(MainWindow);
        actionFade->setObjectName(QString::fromUtf8("actionFade"));
        actionFade->setCheckable(true);
        actionFade->setChecked(false);
        actionApply_Current_Basis_to_New = new QAction(MainWindow);
        actionApply_Current_Basis_to_New->setObjectName(QString::fromUtf8("actionApply_Current_Basis_to_New"));
        actionSave_Basis_Collection = new QAction(MainWindow);
        actionSave_Basis_Collection->setObjectName(QString::fromUtf8("actionSave_Basis_Collection"));
        actionLoad_Basis_Collection = new QAction(MainWindow);
        actionLoad_Basis_Collection->setObjectName(QString::fromUtf8("actionLoad_Basis_Collection"));
        actionAdd_Selected_Basis_to_Collection = new QAction(MainWindow);
        actionAdd_Selected_Basis_to_Collection->setObjectName(QString::fromUtf8("actionAdd_Selected_Basis_to_Collection"));
        actionClear_Screen = new QAction(MainWindow);
        actionClear_Screen->setObjectName(QString::fromUtf8("actionClear_Screen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        frame = new QFrame(splitter);
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

        splitter->addWidget(frame);
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
        tabWidget_2 = new QTabWidget(frame2);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        verticalLayout_8 = new QVBoxLayout(tab_7);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        glProjectionPane = new Renderer(tab_7);
        glProjectionPane->setObjectName(QString::fromUtf8("glProjectionPane"));
        sizePolicy.setHeightForWidth(glProjectionPane->sizePolicy().hasHeightForWidth());
        glProjectionPane->setSizePolicy(sizePolicy);
        glProjectionPane->setMinimumSize(QSize(200, 200));
        glProjectionPane->setCursor(QCursor(Qt::ArrowCursor));
        glProjectionPane->setMouseTracking(true);
        glProjectionPane->setFocusPolicy(Qt::TabFocus);

        verticalLayout_8->addWidget(glProjectionPane);

        tabWidget_2->addTab(tab_7, QString());
        glProjectionPane->raise();
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        verticalLayout_9 = new QVBoxLayout(tab_8);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        glBasisCollection = new Renderer(tab_8);
        glBasisCollection->setObjectName(QString::fromUtf8("glBasisCollection"));
        sizePolicy.setHeightForWidth(glBasisCollection->sizePolicy().hasHeightForWidth());
        glBasisCollection->setSizePolicy(sizePolicy);
        glBasisCollection->setMinimumSize(QSize(200, 200));
        glBasisCollection->setCursor(QCursor(Qt::ArrowCursor));
        glBasisCollection->setMouseTracking(true);
        glBasisCollection->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_9->addWidget(glBasisCollection);

        ButtonLayout = new QHBoxLayout();
        ButtonLayout->setSpacing(6);
        ButtonLayout->setObjectName(QString::fromUtf8("ButtonLayout"));
        PreviousButton = new QPushButton(tab_8);
        PreviousButton->setObjectName(QString::fromUtf8("PreviousButton"));

        ButtonLayout->addWidget(PreviousButton);

        ApplyButton = new QPushButton(tab_8);
        ApplyButton->setObjectName(QString::fromUtf8("ApplyButton"));

        ButtonLayout->addWidget(ApplyButton);

        ApplyAllButton = new QPushButton(tab_8);
        ApplyAllButton->setObjectName(QString::fromUtf8("ApplyAllButton"));

        ButtonLayout->addWidget(ApplyAllButton);

        MakeDefaultButton = new QPushButton(tab_8);
        MakeDefaultButton->setObjectName(QString::fromUtf8("MakeDefaultButton"));

        ButtonLayout->addWidget(MakeDefaultButton);

        NextButton = new QPushButton(tab_8);
        NextButton->setObjectName(QString::fromUtf8("NextButton"));

        ButtonLayout->addWidget(NextButton);


        verticalLayout_9->addLayout(ButtonLayout);

        AddToCollectionButton = new QPushButton(tab_8);
        AddToCollectionButton->setObjectName(QString::fromUtf8("AddToCollectionButton"));

        verticalLayout_9->addWidget(AddToCollectionButton);

        tabWidget_2->addTab(tab_8, QString());

        verticalLayout->addWidget(tabWidget_2);

        splitter->addWidget(frame2);
        splitter_2->addWidget(splitter);
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

        verticalLayout_4->addWidget(splitter_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 931, 26));
        menuApplication = new QMenu(menuBar);
        menuApplication->setObjectName(QString::fromUtf8("menuApplication"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuApplication->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuApplication->addAction(actionSave);
        menuApplication->addAction(actionLoad);
        menuApplication->addAction(actionSave_Basis_Collection);
        menuApplication->addAction(actionLoad_Basis_Collection);
        menuApplication->addAction(actionClose);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addAction(actionClear_Screen);
        menuView->addAction(actionFade);

        retranslateUi(MainWindow);
        QObject::connect(actionClose, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(glParameterPane, SIGNAL(updateNext(int)), glCurvePane, SLOT(updateMe(int)));
        QObject::connect(actionLoad, SIGNAL(triggered()), glCurvePane, SLOT(loadSlot()));
        QObject::connect(actionSave, SIGNAL(triggered()), glCurvePane, SLOT(saveSlot()));
        QObject::connect(actionUndo, SIGNAL(triggered()), glCurvePane, SLOT(undo()));
        QObject::connect(actionRedo, SIGNAL(triggered()), glCurvePane, SLOT(redo()));
        QObject::connect(actionFade, SIGNAL(toggled(bool)), glCurvePane, SLOT(fadeSlot(bool)));
        QObject::connect(actionApply_Current_Basis_to_New, SIGNAL(triggered()), glCurvePane, SLOT(makeDefaultSlot()));
        QObject::connect(actionSave_Basis_Collection, SIGNAL(triggered()), glCurvePane, SLOT(saveCollectionSlot()));
        QObject::connect(actionLoad_Basis_Collection, SIGNAL(triggered()), glCurvePane, SLOT(loadCollectionSlot()));
        QObject::connect(actionClear_Screen, SIGNAL(triggered()), glCurvePane, SLOT(clearSlot()));
        QObject::connect(actionAdd_Selected_Basis_to_Collection, SIGNAL(triggered()), glCurvePane, SLOT(addToCollectionSlot()));
        QObject::connect(ApplyButton, SIGNAL(clicked()), glCurvePane, SLOT(applySlot()));
        QObject::connect(AddToCollectionButton, SIGNAL(clicked()), glCurvePane, SLOT(addToCollectionSlot()));
        QObject::connect(PreviousButton, SIGNAL(clicked()), glCurvePane, SLOT(previousBasisSlot()));
        QObject::connect(NextButton, SIGNAL(clicked()), glCurvePane, SLOT(nextBasisSlot()));
        QObject::connect(MakeDefaultButton, SIGNAL(clicked()), glCurvePane, SLOT(makeDefaultSlot()));
        QObject::connect(ApplyAllButton, SIGNAL(clicked()), glCurvePane, SLOT(applyToAllSlot()));
        QObject::connect(glBasisCollection, SIGNAL(updateNext(int)), glParameterPane, SLOT(updateMe(int)));
        QObject::connect(glCurvePane, SIGNAL(updateNext(int)), glProjectionPane, SLOT(updateMe(int)));
        QObject::connect(glProjectionPane, SIGNAL(updateNext(int)), glBasisCollection, SLOT(updateMe(int)));

        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Partition of Unity Parametrics (PUPs): The Educational Tool - Andrew Legault, Kent Duncan, Scott Chorel", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0, QApplication::UnicodeUTF8));
        actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0, QApplication::UnicodeUTF8));
        actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save Current State", 0, QApplication::UnicodeUTF8));
        actionLoad->setText(QApplication::translate("MainWindow", "Load State", 0, QApplication::UnicodeUTF8));
        actionFade->setText(QApplication::translate("MainWindow", "Fade", 0, QApplication::UnicodeUTF8));
        actionApply_Current_Basis_to_New->setText(QApplication::translate("MainWindow", "Make Selected Basis Default", 0, QApplication::UnicodeUTF8));
        actionSave_Basis_Collection->setText(QApplication::translate("MainWindow", "Save Basis Collection", 0, QApplication::UnicodeUTF8));
        actionLoad_Basis_Collection->setText(QApplication::translate("MainWindow", "Load Basis Collection", 0, QApplication::UnicodeUTF8));
        actionAdd_Selected_Basis_to_Collection->setText(QApplication::translate("MainWindow", "Add Selected Basis to Collection", 0, QApplication::UnicodeUTF8));
        actionClear_Screen->setText(QApplication::translate("MainWindow", "Clear Screen", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "PUPs Curve Editor:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        glCurvePane->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        glProjectionPane->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_7), QApplication::translate("MainWindow", "Projection Display", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        glBasisCollection->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        PreviousButton->setText(QApplication::translate("MainWindow", "Previous", 0, QApplication::UnicodeUTF8));
        ApplyButton->setText(QApplication::translate("MainWindow", "Apply", 0, QApplication::UnicodeUTF8));
        ApplyAllButton->setText(QApplication::translate("MainWindow", "Apply to All", 0, QApplication::UnicodeUTF8));
        MakeDefaultButton->setText(QApplication::translate("MainWindow", "Make Default Basis", 0, QApplication::UnicodeUTF8));
        NextButton->setText(QApplication::translate("MainWindow", "Next", 0, QApplication::UnicodeUTF8));
        AddToCollectionButton->setText(QApplication::translate("MainWindow", "Add Selected Basis Function to Collection", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_8), QApplication::translate("MainWindow", "Basis Collection", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Parameter Space:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        glParameterPane->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        menuApplication->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
