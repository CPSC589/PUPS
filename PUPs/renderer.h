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
#include "PupBasis.h"
#include "controlpoint.h"
#include "state.h"

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

private:

    //Stuff shared between all render panes or only needed for one.
        //all render panes will need this
        static Pup pup_curve;
        //needed by Pup Curve pane only; used when adding new control points.
        static PupBasis default_basis;

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

        //all the panes need to know which pup control point is currently selected
        static int selected_pup_point_index;
        static int selectable_pup_point_index;
        //only the basis editor pane needs to know which basis control points are selected
        static int selected_basis_point_index;
        static int selectable_basis_point_index;
        //to be used by pup curve and basis editor panes
        static float selection_radius;
        //index to be used when editing a specific basis function
        static int selected_nurb_basis_index;

        //change of states
        vector<State> states;
        int stateIndex;

        //control point information
        vector<ControlPoint> ControlPoints;

    //Stuff each render pane needs.
        int this_pane_type;

    //Pane specific mouse handlers
        void mousePressPupPane();
        void mousePressParameterPane();
        void mousePressBasisPane();
        void mousePressProjectionPane();

        void mouseReleasePupPane();
        void mouseReleaseParameterPane();
        void mouseReleaseBasisPane();
        void mouseReleaseProjectionPane();

        void mouseMovePupPane();
        void mouseMoveParameterPane();
        void mouseMoveBasisPane();
        void mouseMoveProjectionPane();

    //Pane specific drawing functions
        void drawPupPane();
        void drawParameterPane();
        void drawProjectionPane();
        void drawBasisPane();

    // additional drawing functions
        void drawMousePos();
        void draw2DGrid(float,float,float,Point);
        void draw2DBorder(float,Point);

    //constructor helpers
        void setupDefaultBasis();
        void setupFrustum();

    //signal wrappers
        void updateOtherPanes();

   //Basis modification Helpers
        Point mapBasisCoord(Point in);
};

#endif 
