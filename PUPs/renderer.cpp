#include "renderer.h"
#include <math.h>
#include <GL/glu.h>
#include <QString>
#include "nurbs.h"

using namespace std;

#define CURVE_PANE 0
#define PARAMETER_PANE 1
#define BASIS_PANE 2
#define PROJECTION_PANE 3

//initialised static variables
Pup Renderer::pup_curve = Pup();
int Renderer::panes_loaded = -1;
int Renderer::currently_focused_pane = -1;
bool Renderer::mouseDown = false;
Point Renderer::lastMousePress = Point();
Point Renderer::lastMousePosition = Point();
int Renderer::selectable_basis_point_index = -1;
int Renderer::selected_basis_point_index = -1;
int Renderer::selectable_pup_point_index = -1;
int Renderer::selected_pup_point_index = -1;
int Renderer::selected_nurb_basis_index = -1;
float Renderer::selection_radius = 10;
//uninitialised static variables shared across all renderers
PupBasis Renderer::default_basis;
double Renderer::frustum_data[6];

//=================================================================
// CONSTRUCTOR, DESTRUCTOR & HELPERS
//=================================================================
Renderer::Renderer( QWidget* parent )
    : QGLWidget( parent )
{
    //WE CAN ONLY GET THE TYPE OF RENDERER AFTER THE CONSTRUCTOR
     this_pane_type = -1;
    //only need to setup static variables in one of the renderers
    if (panes_loaded == -1)
    {
        setupFrustum();
        setupDefaultBasis();
        panes_loaded++;
    }
}
Renderer::~Renderer()
{
}
//=================================================================
// OPENGL FUNCTIONS & DRAWING FUNCTIONS
//=================================================================
void Renderer::paintGL()
{
    //dont start rendering until all the pane types are determined (we don't want to be comparing strings each time)
    if (panes_loaded < 4)
    {
        if (this_pane_type == -1)
        {
            if (objectName() == "glCurvePane") this_pane_type = CURVE_PANE;
            else if (objectName() == "glParameterPane") this_pane_type = PARAMETER_PANE;
            else if (objectName() == "glBasisEditorPane") this_pane_type = BASIS_PANE;
            else if (objectName() == "glProjectionPane") this_pane_type = PROJECTION_PANE;
            else return;
            //if we get to this line then the pane type was just determined
            panes_loaded++;
        }

        if (panes_loaded == 4){
            //if we get to this line then the last pane type was just determined
            updateOtherPanes();
        } else {
            return;
        }
    }

    glViewport( 0, 0, width(), height());
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );    
    glLoadIdentity();

    //call the appropriate drawing function based on the current pane type
    if (this_pane_type == CURVE_PANE) drawPupPane();
    else if (this_pane_type == PARAMETER_PANE) drawParameterPane();
    else if (this_pane_type == BASIS_PANE) drawBasisPane();
    else if (this_pane_type == PROJECTION_PANE) drawProjectionPane();

    draw2DGrid(20,20,1,Point(0.9,0.9,0.9));

    glFlush();
}
void Renderer::initializeGL()
{
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
}
void Renderer::resizeGL( int w, int h )
{
    if(width()>=height()){
            double ratio = width()/(double)height();
            frustum_data[0] = -1.0*ratio;
            frustum_data[1] = ratio;
            frustum_data[2] = -1.0;
            frustum_data[3] = 1.0;

    }
    else if(height()>width()){
            double ratio = height()/(double)width();
            frustum_data[0] = -1.0;
            frustum_data[1] = 1.0;
            frustum_data[2] = -1.0*ratio;
            frustum_data[3] = ratio;
    }
    updateOtherPanes();
}

void Renderer::drawPupPane()
{
    //setup the orthographic viewing matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0,  width(), height(), 0);
    glMatrixMode( GL_MODELVIEW );

    //for making glVertex calls more readable
    Point current_point = Point();

    //draw selected control point
    if (selected_pup_point_index != -1){
        current_point = pup_curve.control_points[selected_pup_point_index];
        glPointSize(8);
        glColor3f(0,1,0);
        glBegin(GL_POINTS);
            glVertex2d(current_point.x, current_point.y);
        glEnd();
    }

    //draw selectable control point
    if (selectable_pup_point_index != -1){
        current_point = pup_curve.control_points[selectable_pup_point_index];
        glPointSize(8);
        glColor3f(0,1,1);
        glBegin(GL_POINTS);
            glVertex2d(current_point.x, current_point.y);
        glEnd();
    }

    //draw the rest of the control points
    glPointSize(5);
    glColor3f(0,0,1);
    glBegin(GL_POINTS);
        for (unsigned int i = 0; i < pup_curve.control_points.size(); i++){
            current_point = pup_curve.control_points[i];
            glVertex2d(current_point.x, current_point.y);
        }
    glEnd();

    //draw the pup curve
    glLineWidth(2);
    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < pup_curve.curve_points.size(); i++){
            current_point = pup_curve.curve_points[i];
            glVertex2d(current_point.x, current_point.y);
        }
    glEnd();
}
void Renderer::drawParameterPane()
{
    makeCurrent();
    //for making everything more readable
    PupBasis* p_cur_basis = 0;
    Point cur_point = Point();

    double largest_y = -100000;
    double smallest_y = 100000;

    //determine viewport height for the entire parameter space
    for (int i = 0; i < pup_curve.basis_functions.size(); i++)
    {
        p_cur_basis = &pup_curve.basis_functions[i];
        if (p_cur_basis->basis_function.top_most.y > largest_y) largest_y = p_cur_basis->basis_function.top_most.y;
        if (p_cur_basis->basis_function.bottom_most.y < smallest_y) smallest_y = p_cur_basis->basis_function.bottom_most.y;
    }

    double x_mult, width_mult;

    for (int i = 0; i < pup_curve.basis_functions.size(); i++)
    {
        p_cur_basis = &pup_curve.basis_functions[i];

        x_mult = pup_curve.basis_centers[i]-p_cur_basis->actual_radius_left;
        width_mult = p_cur_basis->actual_radius_left + p_cur_basis->actual_radius_right;

        //set viewport for current basis
        glViewport(x() + width()*x_mult,y(),width()*width_mult,height()-30);

        //setup the orthographic viewing matrix for the current basis
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D(p_cur_basis->basis_function.left_most.x,  p_cur_basis->basis_function.right_most.x, smallest_y, largest_y);
        glMatrixMode( GL_MODELVIEW );

        glColor3f(0,0,0);
        if (i == selected_pup_point_index)
        {
            glColor3f(0,1,0);
        }
        glLineWidth(2);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < p_cur_basis->basis_function.curve_points.size(); j++){
            glVertex2d(p_cur_basis->basis_function.curve_points[j].x, p_cur_basis->basis_function.curve_points[j].y);
        }
        glEnd();
    }

    //draw all the basis curves
    //draw knots
    //draw centers
}
void Renderer::drawBasisPane()
{
    // if a control point is selected in the PUPs pane, draw the corresponding basis function, otherwise draw nothing
    if (selected_pup_point_index != -1)
    {
        //setup the orthographic viewing matrix (currently forces max weight of 2)
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D(-2, 2, -2, 2);
        glMatrixMode( GL_MODELVIEW );

        //for making things more readable...
        PupBasis* current_basis = &(pup_curve.basis_functions[selected_pup_point_index]);
        Point current_point = Point();

        //draw selected control point
        if (selected_basis_point_index != -1){
            current_point = current_basis->basis_function.control_points[selected_basis_point_index];
            glPointSize(8);
            glColor3f(0,1,0);
            glBegin(GL_POINTS);
                glVertex2d(current_point.x, current_point.y);
            glEnd();
        }

        //draw selectable control point
        if (selectable_basis_point_index != -1){
            current_point = current_basis->basis_function.control_points[selectable_basis_point_index];
            glPointSize(8);
            glColor3f(0,1,1);
            glBegin(GL_POINTS);
                glVertex2d(current_point.x, current_point.y);
            glEnd();
        }

        //draw control points
        glPointSize(5);
        glColor3f(0,0,1);
        glBegin(GL_POINTS);
        for (int i = 0; i < current_basis->basis_function.control_points.size(); i++){
            current_point = current_basis->basis_function.control_points[i];
            glVertex2d(current_point.x,current_point.y);
        }
        glEnd();

        //draw curve points
        glLineWidth(2);
        glColor3f(0,0,0);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < current_basis->basis_function.curve_points.size(); i++){
            current_point = current_basis->basis_function.curve_points[i];
            glVertex2d(current_point.x,current_point.y);
        }
        glEnd();
    }
}
void Renderer::drawProjectionPane()
{
    //SETS UP 3D PROJECTION MATRIX (this should be saved rather than generated each time...)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(frustum_data[0],frustum_data[1],frustum_data[2],frustum_data[3],frustum_data[4],frustum_data[5]);
    glMatrixMode( GL_MODELVIEW );
    //make curve visible
    //glTranslatef(0,0,-5);

    //draw 3d and 2d simultaneously for control points, and one after the other for the curve
}

//=================================================================
// MOUSE EVENT FUNCTIONS
//=================================================================

void Renderer::mousePressEvent( QMouseEvent *e )
{
    currently_focused_pane = this_pane_type;
    lastMousePress = lastMousePosition;
    mouseDown = true;

    //call the appropriate mouse function based on the current pane type
    if (this_pane_type == CURVE_PANE) mousePressPupPane();
    else if (this_pane_type == PARAMETER_PANE) mousePressParameterPane();
    else if (this_pane_type == BASIS_PANE) mousePressBasisPane();
    else if (this_pane_type == PROJECTION_PANE) mousePressProjectionPane();
}
void Renderer::mousePressPupPane()
{
    if (selectable_pup_point_index != -1){
        selected_pup_point_index = selectable_pup_point_index;
    } else {
        pup_curve.addControlPoint(lastMousePress, default_basis, 1);
        selected_pup_point_index = pup_curve.control_points.size()-1;
    }

    updateGL();
    updateOtherPanes();
}
void Renderer::mousePressBasisPane(){

    Point temp = lastMousePress;
    if (selectable_basis_point_index != -1){
        selected_basis_point_index = selectable_basis_point_index;
    } else {
        //Point temp = lastMousePress;
        temp = mapBasisCoord(temp);

        pup_curve.basis_functions[selected_pup_point_index].basis_function.control_points.push_back(temp);
        pup_curve.basis_functions[selected_pup_point_index].basis_function.weights.push_back(1);
        selected_basis_point_index = pup_curve.basis_functions[selected_pup_point_index].basis_function.control_points.size()-1;
        pup_curve.basis_functions[selected_pup_point_index].basis_function.updateAll();
        pup_curve.updateCurve();
       // pup_curve.basis_functions[selected_pup_point_index].basis_function.curve_points.insert(
        //        pup_curve.basis_functions[selected_pup_point_index].basis_function.curve_points.size()-2, temp);
    }

    updateGL();
    updateOtherPanes();



/*

    float current_distance = 0;
    float closest_distance = selection_radius;
    int closest_point_index = -1;
    //loop through all control points to see if any are within selection radius
    for (unsigned int i = 0; i < basisPoints.size(); i++){
        current_distance = (basisPoints[i] - lastMousePosition).magnitude();
        if (current_distance < closest_distance){
            closest_point_index = i;
            closest_distance = current_distance;
        }
    }

    if (closest_distance < selection_radius){
        selected_nurb_basis_index = closest_point_index;
    } else {
        selected_nurb_basis_index = -1;
    }


    qDebug()  << current_distance;*/

}
void Renderer::mousePressParameterPane(){}
void Renderer::mousePressProjectionPane(){}

void Renderer::mouseReleaseEvent( QMouseEvent *e )
{
    mouseDown = false;

    //call the appropriate mouse function based on the current pane type
    if (this_pane_type == CURVE_PANE) mouseReleasePupPane();
    else if (this_pane_type == PARAMETER_PANE) mouseReleaseParameterPane();
    else if (this_pane_type == BASIS_PANE) mouseReleaseBasisPane();
    else if (this_pane_type == PROJECTION_PANE) mouseReleaseProjectionPane();
}
void Renderer::mouseReleasePupPane()
{
    if (selected_pup_point_index != -1){
        selectable_pup_point_index = selected_pup_point_index;
    }
}
void Renderer::mouseReleaseBasisPane(){
    if (selected_basis_point_index != -1)
    {
        selectable_basis_point_index = selected_basis_point_index;
    }
}
void Renderer::mouseReleaseParameterPane(){}
void Renderer::mouseReleaseProjectionPane(){}

void Renderer::mouseMoveEvent( QMouseEvent *e )
{
    QPoint temp = mapFromGlobal(e->globalPos());
    lastMousePosition.x = temp.x();
    lastMousePosition.y = temp.y();

    //call the appropriate mouse function based on the current pane type
    if (this_pane_type == CURVE_PANE) mouseMovePupPane();
    else if (this_pane_type == PARAMETER_PANE) mouseMoveParameterPane();
    else if (this_pane_type == BASIS_PANE) mouseMoveBasisPane();
    else if (this_pane_type == PROJECTION_PANE) mouseMoveProjectionPane();
}
void Renderer::mouseMovePupPane()
{
    //if a control point is selected and the mouse button is down then move the control point
    if ((selected_pup_point_index != -1)&&(mouseDown)){
        pup_curve.control_points[selected_pup_point_index] = lastMousePosition;
        pup_curve.updateCurve();
    }
    //if a control point isnt selected, find the closest selectable point
    else
    {
        float current_distance = 0;
        float closest_distance = selection_radius;
        int closest_point_index = -1;

        //loop through all control points to see if any are within selection radius
        for (unsigned int i = 0; i < pup_curve.control_points.size(); i++){
            current_distance = (pup_curve.control_points[i] - lastMousePosition).magnitude();
            if (current_distance < closest_distance){
                closest_point_index = i;
                closest_distance = current_distance;
            }
        }
        //see if the closest point is close enough
        if (closest_distance < selection_radius){
            selectable_pup_point_index = closest_point_index;
        } else {
            selectable_pup_point_index = -1;
        }

    }


    updateGL();
    updateOtherPanes();
}
void Renderer::mouseMoveBasisPane(){
    if ((selected_basis_point_index != -1) && mouseDown)
    {
        pup_curve.basis_functions[selected_pup_point_index].basis_function.control_points[selected_basis_point_index] = mapBasisCoord(lastMousePosition);
        pup_curve.basis_functions[selected_pup_point_index].basis_function.updateCurve();
    }

    else if (pup_curve.basis_functions.size() > 0)
    {
        float current_distance = 0;
        float closest_distance = selection_radius/100;
        int closest_point_index = -1;

        //loop through all control points to see if any are within selection radius
        for (unsigned int i = 0; i < pup_curve.basis_functions[selected_pup_point_index].basis_function.control_points.size(); i++){
            current_distance = (pup_curve.basis_functions[selected_pup_point_index].basis_function.control_points[i]
                                - mapBasisCoord(lastMousePosition)).magnitude();
            if (current_distance < closest_distance){
                closest_point_index = i;
                closest_distance = current_distance;
            }
        }
        //see if the closest point is close enough
        if (closest_distance < selection_radius){
            selectable_basis_point_index = closest_point_index;
        } else {
            selectable_basis_point_index = -1;
        }

    }
    pup_curve.updateAll();
    updateGL();
    updateOtherPanes();

}
void Renderer::mouseMoveParameterPane(){}
void Renderer::mouseMoveProjectionPane(){}


//This function maps a point to the view used in the basis viewer
//Only call this function from code that is updaing the basis viewer.
Point Renderer::mapBasisCoord(Point in)
{
    Point ret = in;
    ret.x = ret.x - this->width()/2;
    ret.y = ret.y - this->height()/2;
    ret.x = ret.x/(this->width()/4);
    ret.y = -ret.y/(this->height()/4);

    return ret;

}



//=================================================================
// SLOT FUNCTIONS & signal wrappers
//=================================================================

void Renderer::updateMe(int origin_pane)
{
    //if the current pane with mouse focus is not the current pane, then this pane needs to be updated
    if (origin_pane != this_pane_type){
        updateGL();
        emit updateNext(origin_pane);
    }
}

void Renderer::updateOtherPanes(){
    emit updateNext(this_pane_type);
}

// When the user presses Control Z or presses the undo button,
// the program loads the last known state that the system was in.
// There must be a previous state to do so.

void Renderer::undo(){
    qDebug() << "undoed!";
    if(stateIndex - 1 >= 0){
        State currentState = states[stateIndex-1];
        ControlPoints = currentState.loadControlPoints();
    }
    updateGL();
}

// When the user presses Control Y or presses the undo button,
// the program loads the next known state that the system was in.
// There must be a known next state to do so.
void Renderer::redo(){
    qDebug() << "redoed!";
    if(stateIndex + 1 < states.size()){
        State currentState = states[stateIndex+1];
        ControlPoints = currentState.loadControlPoints();
    }
    updateGL();
}

void Renderer::saveSlot(){
    qDebug() << "saved!";
}

void Renderer::loadSlot(){
    qDebug() << "loaded!";
}

//=================================================================
// ENGINE HELPER FUNCTIONS
//=================================================================

void Renderer::setupFrustum()
{
    frustum_data[4] = 2.0;
    frustum_data[5] = 1000.0;

    if(width()>=height()){
            double ratio = width()/(double)height();
            frustum_data[0] = -1.0*ratio;
            frustum_data[1] = ratio;
            frustum_data[2] = -1.0;
            frustum_data[3] = 1.0;
    }
    else if(height()>width()){
            double ratio = height()/(double)width();
            frustum_data[0] = -1.0;
            frustum_data[1] = 1.0;
            frustum_data[2] = -1.0*ratio;
            frustum_data[3] = ratio;
    }
}

void Renderer::setupDefaultBasis()
{
    //the weights are generated by default to 1.0
    vector<Point> temp_control_points = vector<Point>();
    temp_control_points.push_back(Point(-1,0,0));
    temp_control_points.push_back(Point(-0.5,0,0));
    temp_control_points.push_back(Point(0,1,0));
    temp_control_points.push_back(Point(0.5,0,0));
    temp_control_points.push_back(Point(1,0,0));

    Nurbs default_func = Nurbs(temp_control_points,vector<double>(),3,false);
    default_basis = PupBasis(default_func,1.5,1.5,true);
}

//=================================================================
// DRAWING HELPER FUNCTIONS
//=================================================================

void Renderer::draw2DGrid(float x_gap, float y_gap, float line_width, Point rgb)
{
    glViewport(x(),y(),width(),height());
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0,  width(), height(), 0);
    glMatrixMode( GL_MODELVIEW );

    glLineWidth(line_width);
    glColor3f(rgb.x,rgb.y,rgb.z);
    glBegin(GL_LINES);
    for (int i = 0; i < width(); i+=x_gap){
        glVertex3f(i,0,0);
        glVertex3f(i,height(),0);
    }
    for (int i = 0; i < height(); i+=y_gap){
        glVertex3f(0,i,0);
        glVertex3f(width(),i,0);
    }
    glEnd();
}

void Renderer::drawMousePos()
{
    glColor3f(0,0,1);
    //show local mouse pos
    QString debug_string;
    debug_string.append("x: ");
    debug_string.append( QString::number(lastMousePosition.x) );
    debug_string.append(", y: ");
    debug_string.append(  QString::number(lastMousePosition.y) );
    const QString c_debug_string = debug_string;
    renderText(x(),y(),c_debug_string);
}

