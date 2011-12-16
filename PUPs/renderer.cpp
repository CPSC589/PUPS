#include "renderer.h"
#include <math.h>
#include <GL/glu.h>
#include <QString>
#include "nurbs.h"

using namespace std;

#define CURVE_PANE 0
#define PARAMETER_PANE 1
#define PROJECTION_PANE 2
#define COLLECTION_PANE 3

//initialised static variables
Pup Renderer::pup_curve = Pup();
int Renderer::panes_loaded = -1;
int Renderer::currently_focused_pane = -1;
bool Renderer::mouseDown = false;
Point Renderer::lastMousePress = Point();
Point Renderer::lastMousePosition = Point();
bool Renderer::show_normalized_basis = false;
int Renderer::stateIndex = -1;
bool Renderer::drawFadeSelected = false;
int Renderer::indexOfBasisCollection = -1;
vector<Nurbs> Renderer::BasisCollection = vector<Nurbs>();
vector<Pup> Renderer::states = vector<Pup>();

//uninitialised static variables shared across all renderers
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
            else if (objectName() == "glProjectionPane") this_pane_type = PROJECTION_PANE;
            else if (objectName() == "glBasisCollection") this_pane_type = COLLECTION_PANE;
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
    if (this_pane_type == CURVE_PANE){
        drawPupPane();
        draw2DGrid(1,Point(0.95,0.95,0.95));

        if (drawFadeSelected && (stateIndex > 0)){
            drawFade();
        }
    }
    else if (this_pane_type == PARAMETER_PANE) {
        drawParameterPane();
        draw2DGrid(1,Point(0.95,0.95,0.95));
    }
    else if (this_pane_type == PROJECTION_PANE) drawProjectionPane();
    else if (this_pane_type == COLLECTION_PANE) {
        if(BasisCollection.size() > 0){
            drawCollectionPane();
        }
    }

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


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

    glMatrixMode(GL_MODELVIEW);
    updateOtherPanes();
}

void Renderer::drawCollectionPane(){
    //setup the orthographic viewing matrix for parameter space
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, -1.0, 2.0);
    glMatrixMode( GL_MODELVIEW );

    double cur_u = 0;

    //non normalised curve
    glLineWidth(2);
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    vector<Point>* curvePoints = &BasisCollection[indexOfBasisCollection].curve_points;
    double leftMostX = (curvePoints->at(0)).x;
    double rightMostX = (curvePoints->at(curvePoints->size()-1).x)- leftMostX;
    for(int i = 0; i < curvePoints->size(); i++){
        glVertex2d(((curvePoints->at(i)).x-leftMostX)/rightMostX, (curvePoints->at(i)).y);
    }

    glEnd();

    //control points
    glPointSize(6);
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_POINTS);
    //loop through the influences
    vector<Point>* controlPoints = &BasisCollection[indexOfBasisCollection].control_points;
    leftMostX = (controlPoints->at(0)).x;
    rightMostX = (controlPoints->at(controlPoints->size()-1).x) - leftMostX;
    //rightMostX = (controlPoints->at())
    for (int j = 0; j < controlPoints->size(); j++)
    {
        glVertex2d(((controlPoints->at(j)).x - leftMostX)/rightMostX, (controlPoints->at(j)).y - leftMostX);
    }
    glEnd();
}

void Renderer::drawPupPane()
{
    //setup the orthographic viewing matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0,  1, 1, 0);
    glMatrixMode( GL_MODELVIEW );

    //for making glVertex calls more readable
    Point current_point = Point();

    //draw selected control point
    if (pup_curve.selected_point_index != -1){
        current_point = pup_curve.control_points[pup_curve.selected_point_index];
        glPointSize(pup_curve.weights[pup_curve.selected_point_index]*6);
        glColor3f(0,1,0);
        glBegin(GL_POINTS);
            glVertex2d(current_point.x, current_point.y);
        glEnd();
    }

    //draw selectable control point
    if (pup_curve.selectable_point_index != -1){
        current_point = pup_curve.control_points[pup_curve.selectable_point_index];
        glPointSize(pup_curve.weights[pup_curve.selectable_point_index]*6);
        glColor3f(0,1,1);
        glBegin(GL_POINTS);
            glVertex2d(current_point.x, current_point.y);
        glEnd();
    }

    //draw the rest of the control points

    glColor3f(0,0,1);
    //glPointSize(6);

        for (unsigned int i = 0; i < pup_curve.control_points.size(); i++){
    glPointSize(pup_curve.weights[i]*6);
            glBegin(GL_POINTS);
            current_point = pup_curve.control_points[i];

            glVertex2d(current_point.x, current_point.y);
            glEnd();
        }


    //draw the pup curve
    glLineWidth(1);
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
    //setup the orthographic viewing matrix for parameter space
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, -1.0, 2.0);
    glMatrixMode( GL_MODELVIEW );

    double cur_u = 0;
    if (pup_curve.selected_point_index != -1)
    {
        //only show control points if the basis functions are not normalized
        if (!show_normalized_basis)
        {
            //selected control point
            if (pup_curve.selected_basis_point_index != -1){
                glPointSize(8);
                glColor3f(0,1,0);
                glBegin(GL_POINTS);
                glVertex2d( pup_curve.basis_functions[pup_curve.selected_point_index].control_points[pup_curve.selected_basis_point_index].x, pup_curve.basis_functions[pup_curve.selected_point_index].control_points[pup_curve.selected_basis_point_index].y);
                glEnd();
            }

            //selectable control point
            if (pup_curve.selectable_basis_point_index != -1){
                glPointSize(8);
                glColor3f(0,1,1);
                glBegin(GL_POINTS);
                glVertex2d(pup_curve.basis_functions[pup_curve.selected_point_index].control_points[pup_curve.selectable_basis_point_index].x, pup_curve.basis_functions[pup_curve.selected_point_index].control_points[pup_curve.selectable_basis_point_index].y);
                glEnd();
            }

            //control points
            glPointSize(6);
            glColor3f(0,0,1);
            glBegin(GL_POINTS);
            for (int j = 0; j < pup_curve.basis_functions[pup_curve.selected_point_index].control_points.size(); j++)
            {
                glVertex2d( pup_curve.basis_functions[pup_curve.selected_point_index].control_points[j].x, pup_curve.basis_functions[pup_curve.selected_point_index].control_points[j].y);
            }
            glEnd();

            //selectable line left
            if (pup_curve.selectable_basis_line_index == 0){
                glLineWidth(2);
                glColor3f(0,0,1);
                glBegin(GL_LINES);
                glVertex2f(pup_curve.basis_functions[pup_curve.selected_point_index].control_points[0].x, -1);
                glVertex2f(pup_curve.basis_functions[pup_curve.selected_point_index].control_points[0].x, 2);
                glEnd();
            }

            //selectable line right
            else if (pup_curve.selectable_basis_line_index == 1){
                glLineWidth(2);
                glColor3f(0,0,1);
                glBegin(GL_LINES);
                glVertex2f(pup_curve.basis_functions[pup_curve.selected_point_index].control_points[pup_curve.basis_functions[pup_curve.selected_point_index].control_points.size()-1].x, -1);
                glVertex2f(pup_curve.basis_functions[pup_curve.selected_point_index].control_points[pup_curve.basis_functions[pup_curve.selected_point_index].control_points.size()-1].x, 2);
                glEnd();
            }
        }

        //non normalised curve
        glLineWidth(2);
        glColor3f(0,1,0);
        glBegin(GL_LINE_STRIP);
        //loop through the influences
        bool zero_influence = false;
        for (int j = 0; j < pup_curve.basis_influences.size(); j++)
        {
            if (pup_curve.basis_influences[j][pup_curve.selected_point_index].basis_scalar == 0)
            {
                if (zero_influence == false){
                    glEnd();
                    zero_influence = true;
                }
            } else {
                if (zero_influence == true){
                    glBegin(GL_LINE_STRIP);
                    zero_influence = false;
                }
                //show normalized basis influence if necessary
                if (show_normalized_basis){
                    glVertex2d(cur_u, pup_curve.basis_influences[j][pup_curve.selected_point_index].basis_scalar * pup_curve.normalising_coefficients[j]);
                }
                else {
                    glVertex2d(cur_u, pup_curve.basis_influences[j][pup_curve.selected_point_index].basis_scalar);
                }
            }
            cur_u+=pup_curve.u_increment;
        }
        if (zero_influence == false){
            glEnd();
        }


    }


    //look at the basis influences for each basis function
    for (int i = 0; i < pup_curve.basis_functions.size(); i++)
    {
        if (i == pup_curve.selected_point_index){
            continue;
        }

        //reset the current u value
        cur_u = 0.0;
        //non normalised
        glColor3f(0,0,0);
        glLineWidth(1);
        glBegin(GL_LINE_STRIP);
        //loop through the influences
        bool zero_influence = false;
        for (int j = 0; j < pup_curve.basis_influences.size(); j++)
        {
            if (pup_curve.basis_influences[j][i].basis_scalar == 0){
                if (zero_influence == false){
                    glEnd();
                    zero_influence = true;
                }
            } else {
                if (zero_influence == true){
                    glBegin(GL_LINE_STRIP);
                    zero_influence = false;
                }
                if (show_normalized_basis){
                    glVertex2d(cur_u, pup_curve.basis_influences[j][i].basis_scalar * pup_curve.normalising_coefficients[j]);
                }
                else {
                    glVertex2d(cur_u, pup_curve.basis_influences[j][i].basis_scalar);
                }
            }
            cur_u+=pup_curve.u_increment;
        }
        if (zero_influence == false){
            glEnd();
        }
    }

    //draw y = 0 line
    glLineWidth(1);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(1,0);
    glEnd();
}

void Renderer::drawProjectionPane()
{
    //SETS UP 3D PROJECTION MATRIX (this should be saved rather than generated each time...)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(frustum_data[0],frustum_data[1],frustum_data[2],frustum_data[3],frustum_data[4],frustum_data[5]);

    glMatrixMode( GL_MODELVIEW );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*gluLookAt(0, 0, -0.3, // Eye/camera position
              0, 0, -1.0,     // Look-at position
              1.0, 0.0, 0.0);    // "Up" vector*/

    //make curve visible
    //glTranslatef(0,0,-5);

    //draw 3d and 2d simultaneously for control points, and one after the other for the curve

    Point current_point = Point();
    //draw the pup curve
    glLineWidth(2);
    glColor3f(0,0,1);
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i < pup_curve.curve_points.size(); i++){
            current_point = pup_curve.curve_points[i];
            glVertex3d(current_point.x, current_point.y, -1);
        //glVertex3d(frustum_data[0], frustum_data[2], -1);
        //glVertex3d(frustum_data[1], frustum_data[3], -1);
    }
    glEnd();
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
    if (this_pane_type == CURVE_PANE) mousePressPupPane(e);
    else if (this_pane_type == PARAMETER_PANE) mousePressParameterPane(e);
    else if (this_pane_type == PROJECTION_PANE) mousePressProjectionPane(e);
}

void Renderer::mousePressPupPane( QMouseEvent *e )
{
    if (e->button() == Qt::LeftButton){
        if (pup_curve.selectable_point_index != -1){
            pup_curve.selectControlPoint(pup_curve.selectable_point_index);
        } else {
            pup_curve.addControlPoint(mapPupCoord(lastMousePress));
        }
        updateGL();
        updateOtherPanes();
    } else if (e->button() == Qt::RightButton){
        if (pup_curve.selectable_point_index != -1){
            pup_curve.removeControlPoint(pup_curve.selectable_point_index);
            updateGL();
            updateOtherPanes();
        }
    }
}

void Renderer::wheelEvent( QWheelEvent *e ){
    int changeValue = e->delta();
    int indexOfPoint = pup_curve.selectable_point_index;
    int indexOfBasisPoint = pup_curve.selectable_basis_point_index;
    if(pup_curve.control_points.size() > 0 && indexOfPoint != -1){
        double oldWeight = pup_curve.weights[indexOfPoint];
        double modifiedWeight = oldWeight + 20*(1/(double)changeValue);
        pup_curve.modifyControlPointWeight(modifiedWeight, indexOfPoint);
        updateGL();
        updateOtherPanes();
    }
    if(pup_curve.control_points.size() > 0 && indexOfBasisPoint != -1){
        Nurbs nurb = pup_curve.basis_functions[pup_curve.selected_point_index];
        double oldWeight = nurb.weights[indexOfBasisPoint];
        double modifiedWeight = oldWeight + 20*(1/(double)changeValue);
        pup_curve.modifyBasisControlPointWeight(modifiedWeight, indexOfBasisPoint);
        updateGL();
        updateOtherPanes();
    }
}

void Renderer::mousePressParameterPane( QMouseEvent *e )
{
    Point temp = mapParameterCoord(lastMousePress);

    if (pup_curve.selected_point_index != -1)
    {
        if (e->button() == Qt::LeftButton){
            if (pup_curve.selectable_basis_point_index != -1){
                pup_curve.selected_basis_point_index = pup_curve.selectable_basis_point_index;
            } else if (pup_curve.selectable_basis_line_index == -1){
                pup_curve.addBasisControlPoint(temp);
            }
            updateGL();
            updateOtherPanes();
        } else if (e->button() == Qt::RightButton){
            if (pup_curve.selectable_basis_point_index != -1){
                pup_curve.removeBasisControlPoint(pup_curve.selectable_basis_point_index);
                updateGL();
                updateOtherPanes();
            }
        }
    }
}
void Renderer::mousePressProjectionPane( QMouseEvent *e ){}

void Renderer::mouseReleaseEvent( QMouseEvent *e )
{
    mouseDown = false;

    //call the appropriate mouse function based on the current pane type
    if (this_pane_type == CURVE_PANE) mouseReleasePupPane();
    else if (this_pane_type == PARAMETER_PANE) mouseReleaseParameterPane();
    else if (this_pane_type == PROJECTION_PANE) mouseReleaseProjectionPane();
    updateStates();
}

void Renderer::mouseReleasePupPane()
{
    if (pup_curve.selected_point_index != -1){
        pup_curve.selectable_point_index = pup_curve.selected_point_index;
    }
}

void Renderer::mouseReleaseParameterPane(){
    if (pup_curve.selected_basis_point_index != -1){
        pup_curve.selectable_basis_point_index = pup_curve.selected_basis_point_index;
    }
}
void Renderer::mouseReleaseProjectionPane(){}

void Renderer::mouseMoveEvent( QMouseEvent *e )
{
    QPoint temp = mapFromGlobal(e->globalPos());
    lastMousePosition.x = temp.x();
    lastMousePosition.y = temp.y();

    //stops mouse input at borders (border is pixels in width)
    if ((this_pane_type == CURVE_PANE)||(this_pane_type == PARAMETER_PANE)){
        if (lastMousePosition.x > width()-2) lastMousePosition.x = width()-2;
        else if (lastMousePosition.x < 2) lastMousePosition.x = 2;
        if (lastMousePosition.y > height()-2) lastMousePosition.y = height()-2;
        else if (lastMousePosition.y < 2) lastMousePosition.y = 2;
    }

    //call the appropriate mouse function based on the current pane type
    if (this_pane_type == CURVE_PANE) mouseMovePupPane();
    else if (this_pane_type == PARAMETER_PANE) mouseMoveParameterPane();
    else if (this_pane_type == PROJECTION_PANE) mouseMoveProjectionPane();
}

void Renderer::mouseMovePupPane()
{
    //if a control point is selected and the mouse button is down then move the control point
    if ((pup_curve.selected_point_index != -1)&&(mouseDown)){
        pup_curve.modifyCurrentControlPointPosition(mapPupCoord(lastMousePosition));
    }
    //if a control point isnt selected, find the closest selectable point
    else
    {
        pup_curve.updateSelectableControlPointIndex(0.1,mapPupCoord(lastMousePosition));
    }

    updateGL();
    updateOtherPanes();
}

void Renderer::mouseMoveParameterPane()
{
    Point temp = mapParameterCoord(lastMousePosition);
    if ((mouseDown)&&(pup_curve.selected_point_index != -1)&&(pup_curve.selectable_basis_point_index != -1))
    {
        pup_curve.modifyBasisControlPointPosition(temp,pup_curve.selected_basis_point_index);
    } else if ((mouseDown)&&(pup_curve.selectable_basis_line_index == 0)){
        pup_curve.stretchBasisLeft(temp.x);
    } else if ((mouseDown)&&(pup_curve.selectable_basis_line_index == 1)){
        pup_curve.stretchBasisRight(temp.x);
    }
    else {
        pup_curve.updateSelectableBasisControlPointIndex(0.1,temp);
    }
    updateOtherPanes();
    updateGL();
}
void Renderer::mouseMoveProjectionPane(){}

Point Renderer::mapPupCoord(Point in)
{
    in.x = in.x/width();
    in.y = in.y/height();
    return in;
}

Point Renderer::mapParameterCoord(Point in)
{
    in.x /= width();
    in.y = (height()-in.y) * 3.0/height() - 1.0;
    return in;
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


void Renderer::fadeSlot(bool checked){
    if(checked){
        drawFadeSelected = true;
    }
    else{
        drawFadeSelected = false;
    }
    updateGL();
    updateOtherPanes();
}

void Renderer::nextBasisSlot(){
    if(indexOfBasisCollection < BasisCollection.size()-1){
        indexOfBasisCollection += 1;
        updateGL();
        updateOtherPanes();
    }

}

void Renderer::previousBasisSlot(){
    if(indexOfBasisCollection > 0){
        indexOfBasisCollection -= 1;
        updateGL();
        updateOtherPanes();
    }
}

void Renderer::makeDefaultSlot(){
    if(indexOfBasisCollection >= 0){
        pup_curve.default_basis = BasisCollection[indexOfBasisCollection];
        updateGL();
        updateOtherPanes();
    }
}

void Renderer::applySlot(){
    if(indexOfBasisCollection >= 0 && pup_curve.selected_point_index != -1){
        Nurbs original = pup_curve.basis_functions[pup_curve.selected_point_index];
        Nurbs basis = BasisCollection[indexOfBasisCollection];
        //pup_curve.basis_functions[pup_curve.selected_point_index] =
    }
    updateGL();
    updateOtherPanes();
}

void Renderer::applyToAllSlot(){
    if(indexOfBasisCollection >= 0){
        for(int i = 0; i < pup_curve.control_points.size(); i++){
            pup_curve.basis_functions[i] = BasisCollection[indexOfBasisCollection];
        }
        updateGL();
        updateOtherPanes();
    }
}


// When the user presses Control Z or presses the undo button,
// the program loads the last known state that the system was in.
// There must be a previous state to do so.

void Renderer::undo(){
    if(stateIndex - 1 >= 0){
        Pup currentState = states[stateIndex-1];
        pup_curve = currentState;
        pup_curve.selected_point_index = currentState.selected_point_index;
        pup_curve.selected_basis_point_index = currentState.selected_basis_point_index;
        stateIndex -= 1;
    }
    updateGL();
    updateOtherPanes();
}

// When the user presses Control Y or presses the undo button,
// the program loads the next known state that the system was in.
// There must be a known next state to do so.
void Renderer::redo(){
    if(stateIndex + 1 < states.size()){
        Pup currentState = states[stateIndex+1];
        pup_curve = currentState;
        pup_curve.selected_point_index = currentState.selected_point_index;
        pup_curve.selected_basis_point_index = currentState.selected_basis_point_index;
        stateIndex += 1;
    }
    updateGL();
    updateOtherPanes();
}

void Renderer::saveSlot(){
    FileIO outputIO = FileIO();
    QWidget* widget = new QWidget();
    QDir* dir = new QDir();
    QString currentPath = dir->currentPath();
    QFileDialog* saveDialog = new QFileDialog(widget,"Save",currentPath,".pups");
    QString fileName = saveDialog->getSaveFileName();
    if(fileName.compare(QString("")) != 0){ // Prevents crash if user closes dialog without input
        vector<ControlPoint> CPs = vector<ControlPoint>();
        vector<Point> inputControlPoints = pup_curve.control_points;
        vector<double> weights = pup_curve.weights;

        for(int i = 0; i < inputControlPoints.size(); i++){
            ControlPoint newCP = ControlPoint(inputControlPoints[i]);
            newCP.changeWeight(weights[i]);
            CPs.push_back(newCP);
        }

        Nurbs defaultBasis = pup_curve.default_basis;
        double defaultWeight = pup_curve.default_weight;
        double uInc = pup_curve.u_increment;
        vector<Nurbs> basisFunctions = pup_curve.basis_functions;

        outputIO.saveData(defaultBasis, defaultWeight, uInc, CPs, basisFunctions, fileName.toStdString());
    }
    //updateGL(); Don't really need this update.
}

void Renderer::loadSlot(){
    FileIO inputIO = FileIO();
    QWidget* widget = new QWidget();
    QDir* dir = new QDir();
    QString currentPath = dir->currentPath();
    QFileDialog* loadDialog = new QFileDialog(widget,"Load",currentPath,".pups");
    QString fileName = loadDialog->getOpenFileName();
    if(fileName.compare(QString("")) != 0){ // Prevents crash if user closes dialog without input
        Pup pupCurve = inputIO.loadData(fileName.toStdString());
        pup_curve = pupCurve;
        updateGL();
    }
}

void Renderer::saveCollectionSlot(){
    FileIO outputIO = FileIO();
    QWidget* widget = new QWidget();
    QDir* dir = new QDir();
    QString currentPath = dir->currentPath();
    QFileDialog* saveDialog = new QFileDialog(widget,"Save",currentPath,".col");
    QString fileName = saveDialog->getSaveFileName();
    if(fileName.compare(QString("")) != 0){ // Prevents crash if user closes dialog without input
        outputIO.saveCollection(BasisCollection, fileName.toStdString());
    }
}

void Renderer::loadCollectionSlot(){
    FileIO inputIO = FileIO();
    QWidget* widget = new QWidget();
    QDir* dir = new QDir();
    QString currentPath = dir->currentPath();
    QFileDialog* loadDialog = new QFileDialog(widget,"Load",currentPath,".col");
    QString fileName = loadDialog->getOpenFileName();
    if(fileName.compare(QString("")) != 0){ // Prevents crash if user closes dialog without input
        indexOfBasisCollection = 0;
        BasisCollection = inputIO.loadCollection(fileName.toStdString());
    }
    updateGL();
    updateOtherPanes();
}

void Renderer::addToCollectionSlot(){
    if(pup_curve.basis_functions.size() > 0 && pup_curve.selected_point_index != -1){
        BasisCollection.push_back(pup_curve.basis_functions[pup_curve.selected_point_index]);
        indexOfBasisCollection += 1;
        updateGL();
        updateOtherPanes();
    }
}

void Renderer::clearSlot(){
    Pup newPupCurve = Pup();
    pup_curve = newPupCurve;
    updateGL();
    updateOtherPanes();
}

void Renderer::normalizedSlot(bool b){
    show_normalized_basis = b;
    updateGL();
    //updateOtherPanes();
}

//=================================================================
// ENGINE HELPER FUNCTIONS
//=================================================================

void Renderer::setupFrustum()
{
    frustum_data[4] = 0.5;
    frustum_data[5] = 10.0;

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

void Renderer::updateStates(){


    if(states.size() == 20){
        states.erase(states.begin());
    }
    else{
        stateIndex += 1;
    }


    int timesToPop = states.size() - stateIndex;
    for(int i = 0; i < timesToPop; i++){
        states.pop_back();
    }

    states.push_back(pup_curve);
    updateGL();
    updateOtherPanes();

}

//=================================================================
// DRAWING HELPER FUNCTIONS
//=================================================================

void Renderer::draw2DGrid(float line_width, Point rgb)
{
    double x_gap = pup_curve.u_increment*10000.0;
    double y_gap = pup_curve.u_increment*10000.0;
    //glViewport(0,0,width(),height());
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0,  width(), height(), 0);
    glMatrixMode( GL_MODELVIEW );

    glLineWidth(line_width);
    glColor3f(rgb.x,rgb.y,rgb.z);
    glBegin(GL_LINES);
    for (int i = 0; i <= width(); i+=x_gap){
        glVertex3f(i,0,0);
        glVertex3f(i,height(),0);
    }
    for (int i = 0; i <= height(); i+=y_gap){
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

void Renderer::drawFade()
{
    //setup the orthographic viewing matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0,  1, 1, 0);
    glMatrixMode( GL_MODELVIEW );

    //for making glVertex calls more readable
    Point current_point = Point();
    Pup pupCurve = states[stateIndex-1];

    //draw the pup curve
    glLineWidth(1);
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < pupCurve.curve_points.size(); i++){
            current_point = pupCurve.curve_points[i];
            glVertex2d(current_point.x, current_point.y);
        }
    glEnd();
}

