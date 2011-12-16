//=================================================================
// 
// renderer.h
// 
// Written by Luke Olsen, Jan. 2004
// Based on example code distributed with Qt under GPL license.
//
// Provides a bare-bones implementation of a GL widget under Qt.
// Custom function declarations and class variables can be 
// added here.
// 
//=================================================================

#ifndef RENDERER_H
#define RENDERER_H

#include <QGLWidget>
#include <QMouseEvent>
#include "Point.h"
#include "Pup.h"
#include "fileio.h"
#include "controlpoint.h"

class Renderer : public QGLWidget
{
    Q_OBJECT

public:

    Renderer(QWidget *parent);
    ~Renderer();
    
signals:

    //don't call this directly, call updateOtherPanes() instead
    void updateNext(int origin_pane);

public slots:
	
    void updateMe(int origin_pane);
    void undo();
    void redo();
    void saveSlot();
    void loadSlot();
    void saveCollectionSlot();
    void loadCollectionSlot();
    void addToCollectionSlot();
    void fadeSlot(bool checked);
    void makeDefaultSlot();
    void applyToAllSlot();
    void clearSlot();
    void nextBasisSlot();
    void previousBasisSlot();
    void applySlot();

protected:

    // These three functions are REQUIRED for GL widget to operate
    // under Qt. Add custom rendering code to renderer.cpp.
        void initializeGL();
        void paintGL();
        void resizeGL( int w, int h );
    
    //for mouse input
        void mousePressEvent( QMouseEvent *e);
        void mouseReleaseEvent( QMouseEvent *e);
        void mouseMoveEvent( QMouseEvent *e);
        void wheelEvent( QWheelEvent *e );

private:

    //Stuff shared between all render panes or only needed for one.
        //all render panes will need this
        static Pup pup_curve;

        //to keep track of the pane that currently has mouse focus
        static int currently_focused_pane;
        //to ensure all the panes get updated when the signal "updateAll" is emitting
        static int panes_updated;
        //to determine if all the panes have loaded
        static int panes_loaded;

        //only the projection pane needs this
        static double frustum_data[6];

        //only one pane needs mouse info at any one time
        //what if the user clicks and drags out of the pane?
        static bool mouseDown;
        static Point lastMousePress;
        static Point lastMousePosition;

        //change of states
        static vector<Pup> states;
        static int stateIndex;
        void updateStates();
        static bool drawFadeSelected;

        //Basis Collections
        static vector<Nurbs> BasisCollection;
        static int indexOfBasisCollection;

    //Stuff each render pane needs.
        int this_pane_type;

    //Pane specific mouse handlers
        void mousePressPupPane( QMouseEvent *e );
        void mousePressParameterPane( QMouseEvent *e );
        void mousePressProjectionPane( QMouseEvent *e );

        void mouseReleasePupPane();
        void mouseReleaseParameterPane();
        void mouseReleaseProjectionPane();

        void mouseMovePupPane();
        void mouseMoveParameterPane();
        void mouseMoveProjectionPane();

    //Pane specific drawing functions
        void drawPupPane();
        void drawParameterPane();
        void drawProjectionPane();
        void drawFade();
        void drawCollectionPane();

    // additional drawing functions
        void drawMousePos();
        void draw2DGrid(float,Point);
        void draw2DBorder(float,Point);

    //constructor helpers
        void setupFrustum();

    //signal wrappers
        void updateOtherPanes();

   //dimension modification Helpers
        Point mapPupCoord(Point in);
        Point mapParameterCoord(Point in);

};

#endif 
