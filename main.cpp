/* 
 * File:   main.cpp
 * Author: renato
 *
 * Created on 12 de Agosto de 2016, 19:04
 */

#include <iostream>     //std::cout
#include <vector>       //std::vector
#include <cstdlib>      //std::atoi, std::atof
#include <string.h>     //std::strdup
#include <gtk/gtk.h>
#include "DisplayFile.h"
#include "Window.h"
#include "Polygon.h"
#include "Line.h"
#include "Point.h"

/* +++++++++++++++++++++++++++++++ CONSTANTS ++++++++++++++++++++++++++++++++ */
static const double WINDOW_WIDTH = 600;
static const double WINDOW_HEIGHT = 500;

static const double VIEW_PORT_WIDTH = 500;
static const double VIEW_PORT_HEIGHT = 400;

static const double OBJECT_VIEWER_WIDTH = 190;
static const double OBJECT_VIEWER_HEIGHT = 150;

static const double STEP_INPUT_AREA_WIDTH = 20;
static const double STEP_INPUT_AREA_HEIGHT = 10;

static const double LOG_TEXT_AREA_WIDTH = 500;
static const double LOG_TEXT_AREA_HEIGHT = 90;
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* +++++++++++++++++++++++++++ GLOBAL VARIABLES +++++++++++++++++++++++++++++ */
/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
DisplayFile displayFile;
Window window(0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT);
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* ++++++++++++++++++++++++ STATIC METHODS DECLARATION ++++++++++++++++++++++ */
static void close_window(void);
static void clear_surface(void);
static void defineDrawingParameters(cairo_t *cr,
        double backgrdR, double backgrdG, double backgrdB,
        double LineR, double LineG, double LineB, double lineWidth);
static void actionAddObject(GtkButton* button, gpointer data);
static void actionAddPointsToObject(GtkButton* button, GtkWidget* pWindow);
static void actionMoveStep(GtkWidget *widget, cairo_t *cr, gpointer data);
static void actionMoveUp(GtkWidget *widget, cairo_t *cr, gpointer data);
static void actionMoveIn(GtkWidget *widget, cairo_t *cr, gpointer data);
static void actionMoveLeft(GtkWidget *widget, cairo_t *cr, gpointer data);
static void actionMoveRight(GtkWidget *widget, cairo_t *cr, gpointer data);
static void actionMoveDown(GtkWidget *widget, cairo_t *cr, gpointer data);
static void actionMoveOut(GtkWidget *widget, cairo_t *cr, gpointer data);
static gboolean drawDisplayFiles(GtkWidget *widget, cairo_t *cr, gpointer data);
double ViewPortTransformationX(double xw);
double ViewPortTransformationY(double yw);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int main(int argc, char **argv) {

    Line line1("line1", 0.0, 0.0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    Polygon polygon1("Polygon1",{Point(0.0, 0.0), Point(0.0, WINDOW_HEIGHT / 2), Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Point(WINDOW_WIDTH / 2, 0.0)});
    displayFile.addObject(line1);
    displayFile.addObject(polygon1);

    GtkWidget *mainWindow;
    GtkWidget *grid;
    GtkWidget *buttonAddObject;
    GtkWidget *objectsListViewer;
    GtkWidget *buttonStep;
    GtkWidget *StepInputArea;
    GtkWidget *buttonUp;
    GtkWidget *buttonLeft;
    GtkWidget *buttonRight;
    GtkWidget *buttonDown;
    GtkWidget *buttonIn;
    GtkWidget *buttonOut;
    GtkWidget *viewPort;
    GtkWidget *logTextArea;
    GtkTextBuffer *objectsListTextBuffer;
    GtkTextBuffer *stepInputTextBuffer;
    GtkTextBuffer *logTextBuffer;

    gtk_init(&argc, &argv);

    /* DEFINE GRAPHIC ELEMENTS */
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    grid = gtk_grid_new();
    buttonAddObject = gtk_button_new_with_label("Adiciona Objeto");
    objectsListViewer = gtk_text_view_new();
    buttonStep = gtk_button_new_with_label("Passo");
    StepInputArea = gtk_text_view_new();
    buttonUp = gtk_button_new_with_label("Up");
    buttonLeft = gtk_button_new_with_label("Left");
    buttonRight = gtk_button_new_with_label("Right");
    buttonDown = gtk_button_new_with_label("Down");
    buttonIn = gtk_button_new_with_label("In");
    buttonOut = gtk_button_new_with_label("Out");
    viewPort = gtk_drawing_area_new();
    logTextArea = gtk_text_view_new();
    objectsListTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(objectsListViewer));
    stepInputTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(StepInputArea));
    logTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(logTextArea));

    /* SET MAIN WINDOW TITLE, SIZE, SIGNAL AND BORDER */
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Trabalho 1 de CG");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), WINDOW_WIDTH, WINDOW_HEIGHT);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(close_window), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 5);

    /* SET OBJECTS LIST VIEWER SIZE */
    gtk_widget_set_size_request(logTextArea, OBJECT_VIEWER_WIDTH, OBJECT_VIEWER_HEIGHT);

    /* SET STEP INPUT AREA SIZE */
    gtk_widget_set_size_request(StepInputArea, STEP_INPUT_AREA_WIDTH, STEP_INPUT_AREA_HEIGHT);

    /* SET VIEW PORT SIZE */
    gtk_widget_set_size_request(viewPort, VIEW_PORT_WIDTH, VIEW_PORT_HEIGHT);

    /* SET LOG TEXT AREA SIZE */
    gtk_widget_set_size_request(objectsListViewer, LOG_TEXT_AREA_WIDTH, LOG_TEXT_AREA_HEIGHT);

    /* ADD THE GRID TO THE MAIN WINDOW */
    gtk_container_add(GTK_CONTAINER(mainWindow), grid);

    /* ADD GRAPHIC ELEMENTS TO THE GRID */
    gtk_grid_attach(GTK_GRID(grid), buttonAddObject, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), objectsListViewer, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonStep, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), StepInputArea, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonUp, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonIn, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonLeft, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonRight, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonDown, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonOut, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), viewPort, 2, 0, 1, 5);
    gtk_grid_attach(GTK_GRID(grid), logTextArea, 2, 6, 1, 1);

    /* DEFINE BUTTONS SIGNALS */
    g_signal_connect(buttonAddObject, "clicked", G_CALLBACK(actionAddObject), (gpointer) viewPort);
    g_signal_connect(buttonStep, "clicked", G_CALLBACK(actionMoveStep), (gpointer) mainWindow);
    g_signal_connect(buttonUp, "clicked", G_CALLBACK(actionMoveUp), (gpointer) viewPort);
    g_signal_connect(buttonIn, "clicked", G_CALLBACK(actionMoveIn), (gpointer) viewPort);
    g_signal_connect(buttonLeft, "clicked", G_CALLBACK(actionMoveLeft), (gpointer) viewPort);
    g_signal_connect(buttonRight, "clicked", G_CALLBACK(actionMoveRight), (gpointer) viewPort);
    g_signal_connect(buttonDown, "clicked", G_CALLBACK(actionMoveDown), (gpointer) viewPort);
    g_signal_connect(buttonOut, "clicked", G_CALLBACK(actionMoveOut), (gpointer) viewPort);

    /* SET OBJECT LIST TEXT (SET IT'S BUFFER) */
    gtk_text_buffer_set_text(objectsListTextBuffer, "Hello, this is \nsome text\nto objects list\narea", -1);

    /* SET STEP INPUT TEXT (SET IT'S BUFFER) */
    gtk_text_buffer_set_text(stepInputTextBuffer, "10", -1);

    /* SET LOG AREA TEXT (SET IT'S BUFFER) */
    gtk_text_buffer_set_text(logTextBuffer, "Hello, this is \nsome text\nto log text area", -1);

    /* DEFINE VIEW PORT SIGNAL */
    g_signal_connect(viewPort, "draw", G_CALLBACK(drawDisplayFiles), NULL);

    gtk_widget_show_all(mainWindow);

    gtk_main();

    return 0;
}

static void close_window(void) {
    if (surface)
        cairo_surface_destroy(surface);

    gtk_main_quit();
}

static void clear_surface(void) {
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

static void defineDrawingParameters(cairo_t *cr,
        double backgrdR, double backgrdG, double backgrdB,
        double LineR, double LineG, double LineB, double lineWidth) {

    /* Set color for background */
    cairo_set_source_rgb(cr, backgrdR, backgrdG, backgrdB);
    /* fill in the background color*/
    cairo_paint(cr);
    /* Set line color */
    cairo_set_source_rgb(cr, LineR, LineG, LineB);
    /* Set line widht */
    cairo_set_line_width(cr, lineWidth);
}

typedef struct createObjectArgs {
    GtkWidget * window;
    GtkWidget * data;
    GtkWidget * ObjectName;
    GtkWidget * ObjectNumberOfPoints;
    std::vector<GtkWidget *>  ObjectPointsX;
    std::vector<GtkWidget *>  ObjectPointsY;
} createObjectArgs;

//typedef struct objectEntryPoint {
//    GtkWidget *objectCordX;
//    GtkWidget *objectCordY;
//} objectEntryPoint;
//
//typedef struct freeArgsToDestroyAddObjectWindow {
//    GtkWidget* window;
//    GeometricObject* object;
//    GtkWidget* name;
//    GtkWidget* numPoints;
//} freeArgsToDestroyAddObjectWindow;

static void createObjectWindowDestroy(GtkWidget* widget, gpointer data) {
    //freeArgsToDestroyAddObjectWindow* args = (freeArgsToDestroyAddObjectWindow*) data;
    //free(args);
}

static void callback(GtkWidget *widget, gpointer data) {
    createObjectArgs* oed = (createObjectArgs*) data;
    const gchar *entry_text1;
    const gchar *entry_text2;

    entry_text1 = gtk_entry_get_text(GTK_ENTRY(oed->ObjectName));
    entry_text2 = gtk_entry_get_text(GTK_ENTRY(oed->ObjectNumberOfPoints));
    g_print("Contents of entries:\n%s\n%s\n", entry_text1, entry_text2);
}

static void confirmObjectInsertion(GtkWidget *widget, gpointer data) {
    createObjectArgs* objArgs = (createObjectArgs*) data;
    
    std::string objName(gtk_entry_get_text(GTK_ENTRY(objArgs->ObjectName)));
    int objNumPoints = std::atoi(gtk_entry_get_text(GTK_ENTRY(objArgs->ObjectNumberOfPoints)));
    std::vector<GtkWidget *> pointXVector = objArgs->ObjectPointsX;
    std::vector<GtkWidget *> pointYVector = objArgs->ObjectPointsY;
    std::vector<Point> points;

    std::cout << "nome: " << objName << std::endl;
    std::cout << "número de pontos: " << objNumPoints << std::endl;
    
    for (int i = 0; i < objNumPoints; i++) {
        GtkWidget * widgetPontoX = pointXVector[i];
         if (i == 0){
            std::cout << "Endereço do primeiro GtkWidgetX na função chamada: " << widgetPontoX << std::endl;
        }
        std::cout << "Endereço de pointXEntryVector na função chamada: " << pointXVector << std::endl;
        std::cout << "vai dar merda... "  << std::endl;
        double px = std::atof(gtk_entry_get_text(GTK_ENTRY(widgetPontoX)));
        std::cout << "kaaabummmmm" << objArgs << std::endl;
        double py = std::atof(gtk_entry_get_text(GTK_ENTRY((*pointYVector)[i])));
        Point p(px, py);
        points.push_back(p);
    }

    GeometricObject myObj(objName, points);

    displayFile.addObject(myObj);
}

static void readObjectPoints(GtkWidget *widget, gpointer data) {
    createObjectArgs* objArgs = (createObjectArgs*) data;

    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget * grid = gtk_grid_new();
    GtkWidget * confirmButton = gtk_button_new_with_label("Confirma");
    GtkWidget * cancelButton = gtk_button_new_with_label("Cancela");

    gtk_window_set_title(GTK_WINDOW(window), "Adiciona Pontos");
    gtk_window_set_default_size(GTK_WINDOW(window), 120, 50);

    gtk_container_add(GTK_CONTAINER(window), grid);

    int objNumPoints = std::atoi(gtk_entry_get_text(GTK_ENTRY(objArgs->ObjectNumberOfPoints)));

    //createObjectArgs* objEntryArgs = (createObjectArgs*) g_malloc(sizeof (createObjectArgs));
    
    std::vector<GtkWidget *> pointXEntryVector;
    std::vector<GtkWidget *> pointYEntryVector;
    std::vector<GtkWidget *> pointLabelVector;
    
    int gridCoord1 = 0;
    int gridCoord2 = 0;

    for (int i = 0; i < objNumPoints; i++) {
        std::string labelName("Entre cordenadas X e Y do ponto" + std::to_string(i));
        GtkWidget* label = gtk_label_new(labelName.c_str());
        GtkWidget* coordX = gtk_entry_new();
        GtkWidget* coordY = gtk_entry_new();

        if (i == 0){
            std::cout << "Endereço do primeiro GtkWidgetX quando criado: " << coordX << std::endl;
        }
        pointXEntryVector.push_back(coordX);
        pointYEntryVector.push_back(coordY);
        pointLabelVector.push_back(label);
        
        gtk_grid_attach(GTK_GRID(grid), label, gridCoord1++, gridCoord2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), coordX, gridCoord1++, gridCoord2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), coordY, gridCoord1, gridCoord2++, 1, 1);
        gridCoord1 = 0;
    }

    gtk_grid_attach(GTK_GRID(grid), cancelButton, ++gridCoord1, gridCoord2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), confirmButton, ++gridCoord1, gridCoord2, 1, 1);

    std::cout << "Endereço de pointXEntryVector quando criado: " << &pointXEntryVector << std::endl;
    objArgs->ObjectPointsX = pointXEntryVector;
    objArgs->ObjectPointsY = pointYEntryVector;
    
    gtk_widget_show_all(window);

    g_signal_connect(confirmButton, "clicked", G_CALLBACK(confirmObjectInsertion), (gpointer) objArgs);
}

static void actionAddObject(GtkButton* button, gpointer data) {
    g_print("O botao \"Adiciona objeto\" foi clicado\n");

    GtkWidget* window;
    GtkWidget* confirmButton;
    GtkWidget* cancelButton;
    GtkWidget* grid;
    GtkWidget* labelObjName;
    GtkWidget* labelObjNumPoints;
    GtkWidget* entryObjName;
    GtkWidget* entryObjNumPoints;

    labelObjName = gtk_label_new("Nome do objeto:");
    labelObjNumPoints = gtk_label_new("Número de pontos do objeto:");

    createObjectArgs* objEntryArgs = (createObjectArgs*) g_malloc(sizeof (createObjectArgs));

    entryObjName = gtk_entry_new();
    entryObjNumPoints = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(entryObjName), "Objeto nome");
    gtk_entry_set_text(GTK_ENTRY(entryObjNumPoints), "0");

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Criar polígono");
    gtk_window_set_default_size(GTK_WINDOW(window), 120, 50);

    grid = gtk_grid_new();
    confirmButton = gtk_button_new_with_label("Confirma");
    cancelButton = gtk_button_new_with_label("Cancela");

    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_attach(GTK_GRID(grid), labelObjName, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryObjName, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryObjName, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryObjNumPoints, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), cancelButton, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), confirmButton, 1, 2, 1, 1);

    objEntryArgs->window = window;
    objEntryArgs->data = (GtkWidget*) data;
    objEntryArgs->ObjectName = entryObjName;
    objEntryArgs->ObjectNumberOfPoints = entryObjNumPoints;

    gtk_widget_show_all(window);

    g_signal_connect(confirmButton, "clicked", G_CALLBACK(readObjectPoints), (gpointer) objEntryArgs);
    //g_signal_connect_swapped(confirmButton, "clicked", G_CALLBACK(actionAddPointsToObject), window);
    g_signal_connect(cancelButton, "clicked", G_CALLBACK(createObjectWindowDestroy), (gpointer) objEntryArgs);
    g_signal_connect(window, "destroy", G_CALLBACK(createObjectWindowDestroy), (gpointer) objEntryArgs);
}

static void actionAddPointsToObject(GtkButton* button, GtkWidget* pWindow) {
    g_print("O botao \"Adiciona objeto\" foi clicado\n");

    GtkWidget* window;
    GtkWidget* confirmButton;
    GtkWidget* cancelButton;
    GtkWidget* grid;
    GtkWidget* labelObjName;
    GtkWidget* entryObjName;
    GtkWidget* labelObjNumPoints;
    GtkWidget* entryObjNumPoints;

    labelObjName = gtk_label_new("Nome do objeto:");
    labelObjNumPoints = gtk_label_new("Número de pontos do objeto:");

    entryObjName = gtk_entry_new();
    entryObjNumPoints = gtk_entry_new();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Criar polígono");
    gtk_window_set_default_size(GTK_WINDOW(window), 120, 50);

    grid = gtk_grid_new();
    confirmButton = gtk_button_new_with_label("Confirma");
    cancelButton = gtk_button_new_with_label("Cancela");

    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_attach(GTK_GRID(grid), labelObjName, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryObjName, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labelObjNumPoints, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryObjNumPoints, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), cancelButton, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), confirmButton, 1, 2, 1, 1);

    gtk_widget_show_all(window);

    g_signal_connect_swapped(confirmButton, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    g_signal_connect_swapped(cancelButton, "clicked", G_CALLBACK(gtk_widget_destroy), window);
}

static void actionMoveStep(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_print("O botao \"Passo\" foi clicado\n");
}

static void actionMoveUp(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_print("O botao \"Up\" foi clicado\n");
    window.moveUp(50.0);
}

static void actionMoveDown(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_print("O botao \"Down\" foi clicado\n");
    window.moveDown(50.0);
}

static void actionMoveLeft(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_print("O botao \"Left\" foi clicado\n");
    window.moveLeft(50.0);
}

static void actionMoveRight(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_print("O botao \"Right\" foi clicado\n");
    window.moveRight(50.0);
}

static void actionMoveIn(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_print("O botao \"In\" foi clicado\n");
    window.zoomIn(50.0);
}

static void actionMoveOut(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_print("O botao \"Out\" foi clicado\n");
    window.zoomOut(50.0);
}

static gboolean drawDisplayFiles(GtkWidget *widget, cairo_t *cr, gpointer data) {

    /* vetor de objetos a serem desenhados*/
    std::vector<GeometricObject> graficObjects = displayFile.getObjects();

    defineDrawingParameters(cr, 1, 1, 1, 0, 0, 0, 1);

    /* itera sobre todos os objetos que serão desenhados*/
    for (GeometricObject obj : graficObjects) {

        /* pega todos os pontos do objeto*/
        std::vector<Point> objPoints = obj.getPointsVector();
        /* pega o número de pontos do objeto*/
        int objNumPoints = objPoints.size();

        /* se tiver menos de dois pontos, quer dizer que não é possível desenhar
         nem uma reta, no caso, o objeto seria apenas um ponto */
        if (objNumPoints < 2) {
            break;
        }

        Point p1, p2;
        double xp1, yp1;
        double xp2, yp2;

        /* define o ponto inicial (primeiro ponto do objeto) */
        p1 = objPoints[0];
        xp1 = ViewPortTransformationX(p1.getX());
        yp1 = ViewPortTransformationY(p1.getY());

        /* move para o ponto inicial */
        cairo_move_to(cr, xp1, yp1);

        /* itera sobre todos os pontos do objeto */
        for (int i = 1; i < objNumPoints; i++) {
            p2 = objPoints[i];
            xp2 = ViewPortTransformationX(p2.getX());
            yp2 = ViewPortTransformationY(p2.getY());
            cairo_line_to(cr, xp2, yp2);
        }
        /* fecha o caminho (liga o último ponto ao primeiro) */
        cairo_close_path(cr);
        /* desenha efetivamente no caminho formado*/
        cairo_stroke(cr);

        gtk_widget_queue_draw(widget);
    }
    return FALSE;
}

double ViewPortTransformationX(double xw) {
    float xvp = ((xw - window.getXmin()) / (window.getXmax() - window.getXmin())) * (VIEW_PORT_WIDTH);
    return xvp;
}

double ViewPortTransformationY(double yw) {
    float yvp = (1 - ((yw - window.getYmin()) / (window.getYmax() - window.getYmin()))) * (VIEW_PORT_HEIGHT);
    return yvp;
}

//static gboolean reDrawObjects(GtkWidget *widget, cairo_t *cr, gpointer data) {
//
//    std::vector<GeometricObject> graficObjects = displayFile.getObjects();
//defineDrawingParameters(cr, 1, 1, 1, 0, 0, 0, 1);
//    /* itera sobre todos os objetos que serão desenhados*/
//    for (GeometricObject obj : graficObjects) {
//
//        /* pega todos os pontos do objeto*/
//        std::vector<Point> objPoints = obj.getPointsVector();
//        /* pega o número de pontos do objeto*/
//        int objNumPoints = objPoints.size();
//
//        /* se tiver menos de dois pontos, quer dizer que não é possível desenhar
//         nem uma reta, no caso, o objeto seria apenas um ponto */
//        if (objNumPoints < 2) {
//            break;
//        }
//
//        int index = 0;
//        Point p1 = objPoints[index];
//        double xp1;
//        double yp1;
//        Point p2;
//        double xp2;
//        double yp2;
//
//        /* itera sobre todos os pontos do objeto */
//        while (index < objNumPoints - 1) {
//            p2 = objPoints[++index];
//            xp1 = p1.getX();
//            yp1 = p1.getY();
//            xp2 = p2.getX();
//            yp2 = p2.getY();
//
//            xp1 = ViewPortTransformationX(xp1);
//            yp1 = ViewPortTransformationY(yp1);
//            xp2 = ViewPortTransformationX(xp2);
//            yp2 = ViewPortTransformationY(yp2);
//
//            cairo_move_to(cr, xp1, yp1);
//            cairo_line_to(cr, xp2, yp2);
//            /* stroke the path with the chosen color so it's actually visible */
//            cairo_stroke(cr);
//
//            p1 = p2;
//        }
//
//        /* se o objeto não for uma linha (é um polígono, no caso), ligue o último ponto, no primeiro */
//        if (objNumPoints > 2) {
//            p1 = objPoints[0];
//            p2 = objPoints[objNumPoints - 1];
//
//            xp1 = p1.getX();
//            yp1 = p1.getY();
//            xp2 = p2.getX();
//            yp2 = p2.getY();
//
//            xp1 = ViewPortTransformationX(xp1);
//            yp1 = ViewPortTransformationY(yp1);
//            xp2 = ViewPortTransformationX(xp2);
//            yp2 = ViewPortTransformationY(yp2);
//
//            cairo_move_to(cr, xp1, yp1);
//            cairo_line_to(cr, xp2, yp2);
//            cairo_stroke(cr);
//        }
//    }
//
//    gtk_widget_queue_draw(widget);
//
//    return FALSE;
//}


//    /* MYSTEREOUS SIGNALS */
//    /* Signals used to handle the backing surface */
//    //g_signal_connect(viewPort, "draw", G_CALLBACK(draw_cb), NULL);
//    g_signal_connect(viewPort, "configure-event", G_CALLBACK(configure_event_cb), NULL);
//    /* Event signals */
//    g_signal_connect(viewPort, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
//    //g_signal_connect(viewPort, "button-press-event", G_CALLBACK(button_press_event_cb), NULL);
//    /* Ask to receive events the drawing area doesn't normally subscribe to. In particular, we need to ask for the
//     * button press and motion notify events that want to handle. */
//    gtk_widget_set_events(viewPort, gtk_widget_get_events(viewPort) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

/* Redraw the screen from the surface. Note that the ::draw signal receives a ready-to-be-used 
 * cairo_t that is already clipped to only draw the exposed areas of the widget */
//static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data);
///* Handle motion events by continuing to draw if button 1 is still held down.The ::motion-notify 
// * signal handler receives a GdkEventMotion struct which contains this information. */
//static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, gpointer data);
//static void clear_surface(void);
///* Create a new surface of the appropriate size to store our scribbles */
//static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
///* Handle button press events by either drawing a rectangle or clearing the surface, depending on 
// * which button was pressed. The ::button-press signal handler receives a GdkEventButton
// * struct which contains this information. */
//static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event, gpointer data);

//static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data) {
//    cairo_set_source_surface(cr, surface, 0, 0);
//    cairo_paint(cr);
//
//    return FALSE;
//}
//
//static void draw_brush(GtkWidget *widget, gdouble x, gdouble y) {
//    cairo_t *cr;
//
//    /* Paint to the surface, where we store our state */
//    cr = cairo_create(surface);
//
//    cairo_rectangle(cr, x - 3, y - 3, 6, 6);
//    cairo_fill(cr);
//
//    cairo_destroy(cr);
//
//    /* Now invalidate the affected region of the drawing area. */
//    gtk_widget_queue_draw_area(widget, x - 3, y - 3, 6, 6);
//}
//
//static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
//    /* paranoia check, in case we haven't gotten a configure event */
//    if (surface == NULL)
//        return FALSE;
//
//    if (event->state & GDK_BUTTON1_MASK)
//        draw_brush(widget, event->x, event->y);
//
//    /* We've handled it, stop processing */
//    return TRUE;
//}
//
//static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
//    if (surface)
//        cairo_surface_destroy(surface);
//
//    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
//            CAIRO_CONTENT_COLOR,
//            gtk_widget_get_allocated_width(widget),
//            gtk_widget_get_allocated_height(widget));
//
//    /* Initialize the surface to white */
//    clear_surface();
//
//    /* We've handled the configure event, no need for further processing. */
//    return TRUE;
//}
//
//static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event, gpointer data) {
//    /* paranoia check, in case we haven't gotten a configure event */
//    if (surface == NULL)
//        return FALSE;
//
//    if (event->button == GDK_BUTTON_PRIMARY) {
//        draw_brush(widget, event->x, event->y);
//    } else if (event->button == GDK_BUTTON_SECONDARY) {
//        clear_surface();
//        gtk_widget_queue_draw(widget);
//    }
//
//    /* We've handled the event, stop processing */
//    return TRUE;
//}

//static void adiciona_ponto_poligono(GtkWidget* button, gpointer data) {
//    GtkWidget* window;
//    GtkWidget* labelX;
//    GtkWidget* labelY;
//    GtkWidget* entryX;
//    GtkWidget* entryY;
//    GtkWidget* confirmButton;
//    GtkWidget* grid;
//    Poligono* pol = (Poligono*) data;
//    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    labelX = gtk_label_new("X");
//    labelY = gtk_label_new("Y");
//    entryX = gtk_entry_new();
//    gtk_window_set_title(GTK_WINDOW(window), "Adiciona ponto polígono");
//    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
//    gtk_entry_set_text(GTK_ENTRY(entryX), "0");
//    entryY = gtk_entry_new();
//    gtk_entry_set_text(GTK_ENTRY(entryY), "0");
//    grid = gtk_grid_new();
//    confirmButton = gtk_button_new_with_label("Confirma");
//    gtk_container_add(GTK_CONTAINER(window), grid);
//    gtk_grid_attach(GTK_GRID(grid), labelX, 0, 0, 1, 1);
//    gtk_grid_attach(GTK_GRID(grid), entryX, 2, 0, 2, 1);
//    gtk_grid_attach(GTK_GRID(grid), labelY, 0, 2, 1, 1);
//    gtk_grid_attach(GTK_GRID(grid), entryY, 2, 2, 2, 1);
//    gtk_grid_attach(GTK_GRID(grid), confirmButton, 0, 4, 4, 1);
//    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_true), NULL);
//    adiciona_ponto_poligono_confirma_arg_t* args =
//            (adiciona_ponto_poligono_confirma_arg_t*) malloc(
//            sizeof (adiciona_ponto_poligono_confirma_arg_t));
//    args->entryX = entryX;
//    args->entryY = entryY;
//    args->pol = pol;
//    args->window = window;
//    gtk_widget_show_all(window);
//    g_signal_connect(confirmButton, "clicked",
//            G_CALLBACK(adiciona_ponto_poligono_confirma), (gpointer) args);
//    g_signal_connect(window, "destroy",
//            G_CALLBACK(adiciona_ponto_poligono_window_destroy),
//            (gpointer) args);
//}