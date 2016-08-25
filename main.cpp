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
#include "GeometricObject.h"
#include "Window.h"
#include "Polygon.h"
#include "Line.h"
#include "Point.h"

/* +++++++++++++++++++++++++++++++ CONSTANTS ++++++++++++++++++++++++++++++++ */
static const double WINDOW_WIDTH = 600;
static const double WINDOW_HEIGHT = 500;

static const double VIEW_PORT_WIDTH = 500;
static const double VIEW_PORT_HEIGHT = 400;

static const double OBJECT_VIEWER_WIDTH = 100;
static const double OBJECT_VIEWER_HEIGHT = 100;

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
static void actionTranslateObject(GtkWidget *widget, gpointer user_data);
static void actionMoveStep(GtkWidget *widget, gpointer user_data);
static void actionMoveUp(GtkWidget *widget, gpointer user_data);
static void actionMoveIn(GtkWidget *widget, gpointer user_data);
static void actionMoveLeft(GtkWidget *widget, gpointer user_data);
static void actionMoveRight(GtkWidget *widget, gpointer user_data);
static void actionMoveDown(GtkWidget *widget, gpointer user_data);
static void actionMoveOut(GtkWidget *widget, gpointer user_data);
static GtkTreeModel * create_and_fill_model(void);
static GtkWidget * create_view_and_model(void);
static gboolean drawDisplayFiles(GtkWidget *widget, cairo_t *cr, gpointer data);
static void tree_selection_changed_cb(GtkTreeSelection *selection, gpointer data);
double ViewPortTransformationX(double xw);
double ViewPortTransformationY(double yw);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

enum {
    COL_NAME = 0,
    COL_NUM_POINTS,
    NUM_COLS
};

typedef struct createObjectArgs {
    GtkWidget* name;
    GtkWidget* numberOfPoints;
    std::vector<GtkWidget *> *pointXEntryVector;
    std::vector<GtkWidget *> *pointYEntryVector;

    // This is C++ in all of its glory.

    createObjectArgs(GtkWidget* entryName, GtkWidget* entryNumPoints) : name(entryName), numberOfPoints(entryNumPoints) {
        pointXEntryVector = new std::vector<GtkWidget*>();
        pointYEntryVector = new std::vector<GtkWidget*>();
    }

    // This is the destructor.  Will delete the array of vertices, if present.

    ~createObjectArgs() {
        if (name) delete[] name;
        if (numberOfPoints) delete[] numberOfPoints;
        if (pointXEntryVector) delete[] pointXEntryVector;
        if (pointYEntryVector) delete[] pointYEntryVector;
    }
} createObjectArgs;

typedef struct oprationParametres {
    GtkWidget* x;
    GtkWidget* y;
    GtkWidget* angle;

    // This is C++ in all of its glory.

    oprationParametres(GtkWidget* xEnter, GtkWidget* yEnter, GtkWidget* angleEnter) : x(xEnter), y(yEnter), angle(angleEnter) {
    }

    oprationParametres(GtkWidget* xEnter, GtkWidget* yEnter) : x(xEnter), y(yEnter) {
    }

    // This is the destructor.  Will delete the array of vertices, if present.

    ~oprationParametres() {
    }
} oprationParametres;

int main(int argc, char **argv) {

    Line line1("line1", 0.0, 0.0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    Polygon polygon1("Polygon1",{Point(0.0, 0.0), Point(0.0, WINDOW_HEIGHT / 2), Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Point(WINDOW_WIDTH / 2, 0.0)});
    displayFile.addObject(line1);
    displayFile.addObject(polygon1);

    GtkWidget *mainWindow;
    GtkWidget *grid;
    GtkWidget *buttonAddObject;
    GtkWidget *objectsViewer;
    GtkWidget *buttonRefreshViewer; /* necessário ??? */


    //GtkWidget *; /* colocar um label com o nome "Objeto selecionado"*/
    GtkWidget *entrySelectedObject; // setar para não poder ser editado (como ??)
    GtkWidget *buttonTranslateObject;
    GtkWidget *entryTranslateValueX;
    GtkWidget *entryTranslateValueY;
    GtkWidget *buttonEscalonateObject;
    GtkWidget *entryEscalonateValueX;
    GtkWidget *entryEscalonateValueY;
    GtkWidget *entryRotationAngle;

    GtkWidget *buttonRotateRelateToPoint;
    GtkWidget *entryRotateToPointValueX;
    GtkWidget *entryRotateToPointValueY;
    GtkWidget *buttonRotateRelateToOrigen;
    GtkWidget *buttonRotateRelateToObject;

    GtkWidget *buttonWindowStep;
    GtkWidget *entryWindowStep;
    GtkWidget *buttonWindowUp;
    GtkWidget *buttonWindowLeft;
    GtkWidget *buttonWindowRight;
    GtkWidget *buttonWindowDown;
    GtkWidget *buttonWindowIn;
    GtkWidget *buttonWindowOut;

    GtkWidget *viewPort;
    GtkWidget *entryLogArea; // setar para não poder ser editado (como ??)
    GtkTreeSelection *treeViewerSelected;

    gtk_init(&argc, &argv);

    /* DEFINE GRAPHIC ELEMENTS */
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    grid = gtk_grid_new();
    buttonAddObject = gtk_button_new_with_label("Adiciona Objeto");

    buttonTranslateObject = gtk_button_new_with_label("Transladar");
    buttonWindowStep = gtk_button_new_with_label("Passo");
    buttonWindowUp = gtk_button_new_with_label("Up");
    buttonWindowLeft = gtk_button_new_with_label("Left");
    buttonWindowRight = gtk_button_new_with_label("Right");
    buttonWindowDown = gtk_button_new_with_label("Down");
    buttonWindowIn = gtk_button_new_with_label("In");
    buttonWindowOut = gtk_button_new_with_label("Out");
    entryTranslateValueX = gtk_entry_new();
    entryTranslateValueY = gtk_entry_new();
    entryRotationAngle = gtk_entry_new();
    entryWindowStep = gtk_entry_new();
    entryLogArea = gtk_entry_new();
    objectsViewer = create_view_and_model();
    viewPort = gtk_drawing_area_new();

    //    objectsListTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(objectsListViewer));
    //    stepInputTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(StepInputArea));
    //    logTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(logTextArea));

    /* SET MAIN WINDOW TITLE, SIZE, SIGNAL AND BORDER */
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Trabalho 1 de CG");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), WINDOW_WIDTH, WINDOW_HEIGHT);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(close_window), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 5);
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);

    /* SET OBJECT LIST VIEWER */
    treeViewerSelected = gtk_tree_view_get_selection(GTK_TREE_VIEW(objectsViewer));
    gtk_tree_selection_set_mode(treeViewerSelected, GTK_SELECTION_SINGLE);
    g_signal_connect(G_OBJECT(treeViewerSelected), "changed", G_CALLBACK(tree_selection_changed_cb), NULL);
    //gtk_tree_model_foreach(GTK_TREE_MODEL(objectsListViewer), foreach_func, NULL);

    /* SET STEP INPUT AREA SIZE */
    gtk_entry_set_max_length(GTK_ENTRY(entryWindowStep), 10);
    //g_signal_connect (StepInputArea, "activate", G_CALLBACK (enter_callback), StepInputArea);
    gtk_entry_set_text(GTK_ENTRY(entryWindowStep), "10");
    //gtk_widget_set_size_request(StepInputArea, STEP_INPUT_AREA_WIDTH, STEP_INPUT_AREA_HEIGHT);

    /* SET VIEW PORT SIZE */
    gtk_widget_set_size_request(viewPort, VIEW_PORT_WIDTH, VIEW_PORT_HEIGHT);

    /* SET LOG TEXT AREA SIZE */
    gtk_widget_set_size_request(entryLogArea, LOG_TEXT_AREA_WIDTH, LOG_TEXT_AREA_HEIGHT);

    /* ADD THE GRID TO THE MAIN WINDOW */
    gtk_container_add(GTK_CONTAINER(mainWindow), grid);

    /* ADD GRAPHIC ELEMENTS TO THE GRID */
    // coluna, linha, largura (número de colunas que ocupa), altura (número de linhas que ocupa)
    gtk_grid_attach(GTK_GRID(grid), buttonAddObject, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), objectsViewer, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonWindowStep, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryWindowStep, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonWindowUp, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonWindowLeft, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonWindowRight, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonWindowDown, 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonWindowIn, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonWindowOut, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), viewPort, 3, 0, 15, 15);
    gtk_grid_attach(GTK_GRID(grid), entryLogArea, 3, 15, 15, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonTranslateObject, 0, 7, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), entryTranslateValueX, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryTranslateValueY, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryRotationAngle, 1, 9, 1, 1);

    oprationParametres* objParameters = new oprationParametres(entryTranslateValueX, entryTranslateValueY, entryRotationAngle);

    /* DEFINE BUTTONS SIGNALS */
    g_signal_connect(buttonAddObject, "clicked", G_CALLBACK(actionAddObject), (gpointer) viewPort);
    g_signal_connect(buttonWindowStep, "clicked", G_CALLBACK(actionMoveStep), (gpointer) entryWindowStep);
    g_signal_connect(buttonWindowUp, "clicked", G_CALLBACK(actionMoveUp), (gpointer) entryWindowStep);
    g_signal_connect(buttonWindowIn, "clicked", G_CALLBACK(actionMoveIn), (gpointer) entryWindowStep);
    g_signal_connect(buttonWindowLeft, "clicked", G_CALLBACK(actionMoveLeft), (gpointer) entryWindowStep);
    g_signal_connect(buttonWindowRight, "clicked", G_CALLBACK(actionMoveRight), (gpointer) entryWindowStep);
    g_signal_connect(buttonWindowDown, "clicked", G_CALLBACK(actionMoveDown), (gpointer) entryWindowStep);
    g_signal_connect(buttonWindowOut, "clicked", G_CALLBACK(actionMoveOut), (gpointer) entryWindowStep);
    g_signal_connect(buttonTranslateObject, "clicked", G_CALLBACK(actionTranslateObject), (gpointer) entryWindowStep);
    //    /* SET OBJECT LIST TEXT (SET IT'S BUFFER) */
    //    gtk_text_buffer_set_text(objectsListTextBuffer, "Hello, this is \nsome text\nto objects list\narea", -1);
    //
    //    /* SET STEP INPUT TEXT (SET IT'S BUFFER) */
    //    gtk_text_buffer_set_text(stepInputTextBuffer, "10", -1);
    //
    //    /* SET LOG AREA TEXT (SET IT'S BUFFER) */
    //    gtk_text_buffer_set_text(logTextBuffer, "Hello, this is \nsome text\nto log text area", -1);

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

static void createObjectWindowDestroy(GtkWidget* widget, gpointer data) {
    createObjectArgs* args = (createObjectArgs*) data;
    free(args);
}

static void confirmObjectInsertion(GtkWidget *widget, gpointer data) {
    createObjectArgs* objArgs = (createObjectArgs*) data;

    std::string objName = gtk_entry_get_text(GTK_ENTRY(objArgs->name));
    int objNumPoints = std::atoi(gtk_entry_get_text(GTK_ENTRY(objArgs->numberOfPoints)));

    std::vector<GtkWidget *> *pointXVector = objArgs->pointXEntryVector;
    std::vector<GtkWidget *> *pointYVector = objArgs->pointYEntryVector;
    std::vector<Point> points;

    std::cout << "nome: " << objName << std::endl;
    std::cout << "número de pontos: " << objNumPoints << std::endl;

    for (int i = 0; i < objNumPoints; i++) {
        GtkWidget * widgetPontoX = pointXVector->at(i);
        GtkWidget * widgetPontoY = pointYVector->at(i);

        double px = std::atof(gtk_entry_get_text(GTK_ENTRY(widgetPontoX)));
        double py = std::atof(gtk_entry_get_text(GTK_ENTRY(widgetPontoY)));

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

    int objNumPoints = std::atoi(gtk_entry_get_text(GTK_ENTRY(objArgs->numberOfPoints)));

    std::vector<GtkWidget *> *pointXEntryVector = new std::vector<GtkWidget *>();
    std::vector<GtkWidget *> *pointYEntryVector = new std::vector<GtkWidget *>();
    std::vector<GtkWidget *> pointLabelVector;

    int gridCoord1 = 0;
    int gridCoord2 = 0;

    for (int i = 0; i < objNumPoints; i++) {
        std::string labelName("Entre cordenadas X e Y do ponto " + std::to_string(i));
        GtkWidget* label = gtk_label_new(labelName.c_str());
        GtkWidget* coordX = gtk_entry_new();
        GtkWidget* coordY = gtk_entry_new();

        pointXEntryVector->push_back(coordX);
        pointYEntryVector->push_back(coordY);
        pointLabelVector.push_back(label);

        gtk_grid_attach(GTK_GRID(grid), label, gridCoord1++, gridCoord2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), coordX, gridCoord1++, gridCoord2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), coordY, gridCoord1, gridCoord2++, 1, 1);
        gridCoord1 = 0;
    }

    gtk_grid_attach(GTK_GRID(grid), cancelButton, ++gridCoord1, gridCoord2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), confirmButton, ++gridCoord1, gridCoord2, 1, 1);

    objArgs->pointXEntryVector = pointXEntryVector;
    objArgs->pointYEntryVector = pointYEntryVector;

    gtk_widget_show_all(window);

    g_signal_connect(confirmButton, "clicked", G_CALLBACK(confirmObjectInsertion), (gpointer) objArgs);
    g_signal_connect_swapped(cancelButton, "clicked", G_CALLBACK(gtk_widget_destroy), window);
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
    gtk_grid_attach(GTK_GRID(grid), labelObjNumPoints, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryObjNumPoints, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), cancelButton, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), confirmButton, 1, 2, 1, 1);

    createObjectArgs* objEntryArgs = new createObjectArgs(entryObjName, entryObjNumPoints);

    gtk_widget_show_all(window);

    g_signal_connect(confirmButton, "clicked", G_CALLBACK(readObjectPoints), (gpointer) objEntryArgs);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), (gpointer) window);
    g_signal_connect_swapped(cancelButton, "clicked", G_CALLBACK(gtk_widget_destroy), window);
}

static void actionTranslateObject(GtkWidget *widget, gpointer user_data) {
    oprationParametres* args = (oprationParametres*) user_data;
    
    GtkWidget* dx  = args->x;
    GtkWidget* dy  = args->y;    
    
    double valueX = std::atof(gtk_entry_get_text(GTK_ENTRY(dx)));
    double valueY = std::atof(gtk_entry_get_text(GTK_ENTRY(dy)));
    
    //translateObject(double dx, double dy);
    
    g_print("O botao \"Tranladar\" foi clicado\n");
}

static void actionMoveStep(GtkWidget *widget, gpointer user_data) {
    GtkEntry* e = (GtkEntry*) user_data;
    const gchar* entry = gtk_entry_get_text(e);
    double r = atof(entry);
    g_print("O botao \"Passo\" foi clicado\n");
    g_print("O valor do passo é de %.2f\n", r);
}

static void actionMoveUp(GtkWidget *widget, gpointer user_data) {
    GtkEntry* e = (GtkEntry*) user_data;
    const gchar* entry = gtk_entry_get_text(e);
    double value = atof(entry);

    g_print("O botao \"Up\" foi clicado\n");
    window.moveUp(value);
}

static void actionMoveDown(GtkWidget *widget, gpointer user_data) {
    GtkEntry* e = (GtkEntry*) user_data;
    const gchar* entry = gtk_entry_get_text(e);
    double value = atof(entry);

    g_print("O botao \"Down\" foi clicado\n");
    window.moveDown(value);
}

static void actionMoveLeft(GtkWidget *widget, gpointer user_data) {
    GtkEntry* e = (GtkEntry*) user_data;
    const gchar* entry = gtk_entry_get_text(e);
    double value = atof(entry);

    g_print("O botao \"Left\" foi clicado\n");
    window.moveLeft(value);
}

static void actionMoveRight(GtkWidget *widget, gpointer user_data) {
    GtkEntry* e = (GtkEntry*) user_data;
    const gchar* entry = gtk_entry_get_text(e);
    double value = atof(entry);

    g_print("O botao \"Right\" foi clicado\n");
    window.moveRight(value);
}

static void actionMoveIn(GtkWidget *widget, gpointer user_data) {
    GtkEntry* e = (GtkEntry*) user_data;
    const gchar* entry = gtk_entry_get_text(e);
    double value = atof(entry);

    g_print("O botao \"In\" foi clicado\n");
    window.zoomIn(value);
}

static void actionMoveOut(GtkWidget *widget, gpointer user_data) {
    GtkEntry* e = (GtkEntry*) user_data;
    const gchar* entry = gtk_entry_get_text(e);
    double value = atof(entry);

    g_print("O botao \"Out\" foi clicado\n");
    window.zoomOut(value);
}

static void tree_selection_changed_cb(GtkTreeSelection *selection, gpointer data) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *name;
    gint numPoints;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, COL_NAME, &name, COL_NUM_POINTS, &numPoints, -1);

        g_print("You selected an object called %s with %d points\n", name, numPoints);

        g_free(name);
    }
}

static GtkTreeModel * create_and_fill_model(void) {
    GtkListStore *store;
    GtkTreeIter iter;

    store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);

    /* vetor de objetos a serem listados*/
    std::vector<GeometricObject> graficObjects = displayFile.getObjects();

    /* itera sobre todos os objetos que serão listados*/
    for (GeometricObject obj : graficObjects) {

        /* pega o nome do objeto*/
        std::string objName = obj.getName();
        /* pega o número de pontos do objeto*/
        int objNumPoints = obj.getNumPoints();

        /* Append a row and fill with the name and number of points data */
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                COL_NAME, objName.c_str(),
                COL_NUM_POINTS, objNumPoints,
                -1);
    }

    return GTK_TREE_MODEL(store);
}

static GtkWidget * create_view_and_model(void) {
    GtkCellRenderer *renderer;
    GtkTreeModel *model;
    GtkWidget *view;

    view = gtk_tree_view_new();

    /* --- Column #1 --- */

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            -1,
            "Nome",
            renderer,
            "text", COL_NAME,
            NULL);

    /* --- Column #2 --- */

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            -1,
            "Número de pontos",
            renderer,
            "text", COL_NUM_POINTS,
            NULL);

    model = create_and_fill_model();

    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    /* The tree view has acquired its own reference to the
     *  model, so we can drop ours. That way the model will
     *  be freed automatically when the tree view is destroyed */

    g_object_unref(model);

    return view;
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


//#include <gtk/gtk.h>
//#include <stdlib.h>
//
//struct Window
//{
//    GtkWidget *numerator;
//    GtkWidget *denominator;
//    GtkWidget *button;
//    GtkWidget *label;
//};
//
//
//void button_clicked(GtkWidget *widget, gpointer data)
//{
//    Window* w = (Window*)data;
//    char buf[10];
//
//    char buffer[200];
//
//    GtkEntry* e = (GtkEntry*)w->numerator;
//    const gchar* entry1 = gtk_entry_get_text(e);
//
//    char* test = (char*)entry1;
//    int r = atoi(test);
//
//
//    sprintf(buf,"%d",r);
//
//    GtkWidget *label = w->label;
//    gtk_label_set_text(GTK_LABEL(label), buf);
//}
//
//
//int main(int argc, char*argv[])
//{
//    GtkWidget *window;
//    GtkWidget *table;
//    Window w;
//
//
//    //Set up my window
//    gtk_init(&argc,&argv);
//    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    gtk_window_set_title(GTK_WINDOW(window), "Division");
//    gtk_window_set_default_size(GTK_WINDOW(window),500,500);
//    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
//
//    //Create my table and add it to the window
//    table = gtk_table_new(4,2,FALSE);
//    gtk_container_add(GTK_CONTAINER(window),table);
//
//    //Create instances of all my widgets
//    w.numerator = gtk_entry_new();
//    w.denominator = gtk_entry_new();
//    w.button = gtk_button_new_with_label("Click");
//    w.label = gtk_label_new("result");
//
//    //Attack the widgets to the table
//    gtk_table_attach(GTK_TABLE(table), w.numerator,0,1,0,1,GTK_FILL,GTK_FILL,5,5);
//    gtk_table_attach(GTK_TABLE(table), w.denominator,0,1,1,2,GTK_FILL,GTK_FILL,5,5);
//    gtk_table_attach(GTK_TABLE(table), w.button,0,1,2,3,GTK_FILL,GTK_FILL,5,5);
//    gtk_table_attach(GTK_TABLE(table), w.label,0,1,3,4,GTK_FILL,GTK_FILL,5,5);
//
//    //attach the click action to with the button to invoke the button_clicked function
//    g_signal_connect(G_OBJECT(w.button),"clicked",G_CALLBACK(button_clicked),&w);   
//      g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
//
//    gtk_widget_show_all(window);
//
//
//    gtk_main();
//
//    return 0;
//}


//#include <gtk/gtk.h>
//
//enum {
//    COL_NAME = 0,
//    COL_AGE,
//    NUM_COLS
//};
//
//static GtkTreeModel * create_and_fill_model(void) {
//    GtkListStore *store;
//    GtkTreeIter iter;
//
//    store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
//
//    /* Append a row and fill in some data */
//    gtk_list_store_append(store, &iter);
//    gtk_list_store_set(store, &iter,
//            COL_NAME, "Heinz El-Mann",
//            COL_AGE, 51,
//            -1);
//
//    /* append another row and fill in some data */
//    gtk_list_store_append(store, &iter);
//    gtk_list_store_set(store, &iter,
//            COL_NAME, "Jane Doe",
//            COL_AGE, 23,
//            -1);
//
//    /* ... and a third row */
//    gtk_list_store_append(store, &iter);
//    gtk_list_store_set(store, &iter,
//            COL_NAME, "Joe Bungop",
//            COL_AGE, 91,
//            -1);
//
//    return GTK_TREE_MODEL(store);
//}
//
//static GtkWidget * create_view_and_model(void) {
//    GtkCellRenderer *renderer;
//    GtkTreeModel *model;
//    GtkWidget *view;
//
//    view = gtk_tree_view_new();
//
//    /* --- Column #1 --- */
//
//    renderer = gtk_cell_renderer_text_new();
//    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
//            -1,
//            "Name",
//            renderer,
//            "text", COL_NAME,
//            NULL);
//
//    /* --- Column #2 --- */
//
//    renderer = gtk_cell_renderer_text_new();
//    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
//            -1,
//            "Age",
//            renderer,
//            "text", COL_AGE,
//            NULL);
//
//    model = create_and_fill_model();
//
//    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
//
//    /* The tree view has acquired its own reference to the
//     *  model, so we can drop ours. That way the model will
//     *  be freed automatically when the tree view is destroyed */
//
//    g_object_unref(model);
//
//    return view;
//}
//
//int main(int argc, char **argv) {
//    GtkWidget *window;
//    GtkWidget *view;
//
//    gtk_init(&argc, &argv);
//
//    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */
//
//    view = create_view_and_model();
//
//    gtk_container_add(GTK_CONTAINER(window), view);
//
//    gtk_widget_show_all(window);
//
//    gtk_main();
//
//    return 0;
//}