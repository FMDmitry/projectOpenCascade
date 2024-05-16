#include "GlfwOcctView.h"


#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <TopAbs_ShapeEnum.hxx>


#include <GLFW/glfw3.h>
#include "imgui/imgui.h"


namespace
{
    //! Convert GLFW mouse button into Aspect_VKeyMouse.
    static Aspect_VKeyMouse mouseButtonFromGlfw(int theButton)
    {
        switch (theButton)
        {
        case GLFW_MOUSE_BUTTON_LEFT:   return Aspect_VKeyMouse_LeftButton;
        case GLFW_MOUSE_BUTTON_RIGHT:  return Aspect_VKeyMouse_RightButton;
        case GLFW_MOUSE_BUTTON_MIDDLE: return Aspect_VKeyMouse_MiddleButton;
        }
        return Aspect_VKeyMouse_NONE;
    }

    //! Convert GLFW key modifiers into Aspect_VKeyFlags.
    static Aspect_VKeyFlags keyFlagsFromGlfw(int theFlags)
    {
        Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
        if ((theFlags & GLFW_MOD_SHIFT) != 0)
        {
            aFlags |= Aspect_VKeyFlags_SHIFT;
        }
        if ((theFlags & GLFW_MOD_CONTROL) != 0)
        {
            aFlags |= Aspect_VKeyFlags_CTRL;
        }
        if ((theFlags & GLFW_MOD_ALT) != 0)
        {
            aFlags |= Aspect_VKeyFlags_ALT;
        }
        if ((theFlags & GLFW_MOD_SUPER) != 0)
        {
            aFlags |= Aspect_VKeyFlags_META;
        }
        return aFlags;
    }
}

// ================================================================
// Function : GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::GlfwOcctView()
{
    myInterface = std::make_unique<InterfaceCAD>(this);
}

// ================================================================
// Function : ~GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::~GlfwOcctView()
{

}

// ================================================================
// Function : toView
// Purpose  :
// ================================================================
GlfwOcctView* GlfwOcctView::toView(GLFWwindow* theWin)
{
    return static_cast<GlfwOcctView*>(glfwGetWindowUserPointer(theWin));
}

// ================================================================
// Function : errorCallback
// Purpose  :
// ================================================================
void GlfwOcctView::errorCallback(int theError, const char* theDescription)
{
    Message::DefaultMessenger()->Send(TCollection_AsciiString("Error") + theError + ": " + theDescription, Message_Fail);
}

// ================================================================
// Function : run
// Purpose  :
// ================================================================
void GlfwOcctView::run()
{
    initWindow(1366, 768, "Демонстрация возможностей: GLFW+OpenCascade7.8+ImGUI Фоменко Д.М.");
    initViewer();
    myInterface->initGui(myOcctWindow);

    initDemoScene();



    if (myView.IsNull())
    {
        return;
    }
    myView->MustBeResized();
    myOcctWindow->Map();
    mainloop();
    cleanup();
}

// ================================================================
// Function : initWindow
// Purpose  :
// ================================================================
void GlfwOcctView::initWindow(int theWidth, int theHeight, const char* theTitle)
{
    glfwSetErrorCallback(GlfwOcctView::errorCallback);
    glfwInit();
    const bool toAskCoreProfile = true;
    if (toAskCoreProfile)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if defined (__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    myOcctWindow = new GlfwOcctWindow(theWidth, theHeight, theTitle);
    glfwSetWindowUserPointer(myOcctWindow->getGlfwWindow(), this);

    // window callback
    glfwSetWindowSizeCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onResizeCallback);
    glfwSetFramebufferSizeCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onFBResizeCallback);

    // mouse callback
    glfwSetScrollCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseScrollCallback);
    glfwSetMouseButtonCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseButtonCallback);
    glfwSetCursorPosCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseMoveCallback);
}

// ================================================================
// Function : initViewer
// Purpose  :
// ================================================================
void GlfwOcctView::initViewer()
{
    if (myOcctWindow.IsNull()
        || myOcctWindow->getGlfwWindow() == nullptr)
    {
        return;
    }

    Handle(OpenGl_GraphicDriver) aGraphicDriver
        = new OpenGl_GraphicDriver(myOcctWindow->GetDisplay(), Standard_False);
    aGraphicDriver->SetBuffersNoSwap(Standard_True);

    Handle(V3d_Viewer) aViewer = new V3d_Viewer(aGraphicDriver);
    aViewer->SetDefaultLights();
    aViewer->SetLightOn();
    aViewer->SetDefaultTypeOfView(V3d_PERSPECTIVE);
    aViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
    myView = aViewer->CreateView();
    myView->SetImmediateUpdate(Standard_False);
    myView->SetWindow(myOcctWindow, myOcctWindow->NativeGlContext());
    //myView->ChangeRenderingParams().ToShowStats = Standard_True;
    myContext = new AIS_InteractiveContext(aViewer);

}

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_XYZ.hxx>
#include <gp_Trsf.hxx>

void GlfwOcctView::lab2()
{
    if (myContext.IsNull())
    {
        return;
    }

    gp_Pnt pointOne(100,0,100);
    gp_Pnt pointTwo(0,100,100);

    Standard_Real disBetPOne = pointOne.Distance(pointTwo);

    Standard_Real disBetPTwo = 0;
    auto deltaX = pointOne.X() - pointTwo.X()  ;
    auto deltaY = pointOne.Y() - pointTwo.Y();
    auto deltaZ = pointOne.Z() - pointTwo.Z();

    disBetPTwo = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

    //Rotation

    gp_Trsf rotationPoint;
    rotationPoint.SetRotation(gp::OX(), 1.6);
    gp_Pnt pointThree = pointOne.Transformed(rotationPoint);
    TopoDS_Vertex vertexThree = BRepBuilderAPI_MakeVertex(pointThree);
    Handle(AIS_Shape) aisPointThree = new AIS_Shape(vertexThree);
    aisPointThree->SetColor(Quantity_NOC_ORANGE1);
    myContext->Display(aisPointThree, AIS_Shaded, 0, Standard_False);

    TopoDS_Vertex vertexOne = BRepBuilderAPI_MakeVertex(pointOne);
    TopoDS_Vertex vertexTwo = BRepBuilderAPI_MakeVertex(pointTwo);

    Handle(AIS_Shape) aisPointOne = new AIS_Shape(vertexOne);
    Handle(AIS_Shape) aisPointTwo = new AIS_Shape(vertexTwo);

    aisPointOne->SetColor(Quantity_NOC_YELLOW);
    aisPointTwo->SetColor(Quantity_NOC_WHITE);

    myContext->Display(aisPointOne, AIS_Shaded, 0, Standard_False);
    myContext->Display(aisPointTwo, AIS_Shaded, 0, Standard_True);

}

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Circ.hxx>

void GlfwOcctView::lab3()
{
    int R = 100;
    int n = 5;
    gp_Pnt point(0,0,0);
    gp_Dir dir(0,0,1);
    gp_Circ circle(gp_Ax2(point, dir),R);
    BRepBuilderAPI_MakeEdge makeEdge(circle);
    Handle(AIS_Shape) aCircle = new AIS_Shape(TopoDS_Edge());
    aCircle -> SetColor(Quantity_NOC_YELLOW);
    aCircle ->Set(makeEdge.Edge());

    myContext->Display(aCircle,AIS_Shaded, 0, false);
    gp_Pnt pnt2(0,0,0);
    for (int i = 0; i < n; i++) {
        int k = 360/n + i * 360/n;
        Standard_Real x1 = R*cos(k * M_PI /180);
        Standard_Real y1 = R*sin(k * M_PI /180);
        gp_Pnt pnt1(x1,y1,0);
        Standard_Real x2 = R*cos((k+360/n) * M_PI /180);
        Standard_Real y2 = R*sin((k+360/n) * M_PI /180);
        gp_Pnt pnt2(x2,y2,0);
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pnt1,pnt2);
        Handle(AIS_Shape) theLine = new  AIS_Shape(edge);
        theLine -> SetColor(Quantity_NOC_ORANGE);
        myContext->Display(theLine,AIS_Shaded, 0, false);
    }
}

#include <GeomAPI_PointsToBSpline.hxx>
#include <gp_Trsf.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_BSplineCurve.hxx>

void GlfwOcctView::lab4()
{
    const double a = 45.0;
    const double b = 0.005;
    const int N = 1000;
    const double angleIncrement = 8 * M_PI / N;

    TColgp_Array1OfPnt array(1, N);

    for (int i = 0; i < N; ++i) {
        double angle = i * angleIncrement;
        double radius = a - (a * i / N);
        double x = radius * cos(angle);
        double y = radius * sin(angle);
        double z = b * i;
        array(i + 1) = gp_Pnt(x, y, z);
    }

    Handle(Geom_BSplineCurve) curve_bspline = GeomAPI_PointsToBSpline(array).Curve();

    TopoDS_Edge myEdge1 = BRepBuilderAPI_MakeEdge(curve_bspline);

    Handle(AIS_Shape) aisEdge1 = new AIS_Shape(myEdge1);

    aisEdge1->SetColor(Quantity_NOC_ORANGE);

    myContext->Display(aisEdge1, true);

}
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BezierSurface.hxx>
void GlfwOcctView::lab6()
{
    int arraySize=10;
    TColgp_Array2OfPnt myStucture(0,arraySize,0,arraySize);
    srand(time(NULL));
    for(int i=0; i<=arraySize; i++)
    {
        for(int j=0; j<=arraySize; j++)
        {
            int z=rand()%(100-1);
            myStucture.SetValue(j,i,gp_Pnt(i+20*i^2,j+20*j^2,z));
        }
    }
    Handle (Geom_Surface) myGSurface = new Geom_BezierSurface(myStucture);
    TopoDS_Face myFace = BRepBuilderAPI_MakeFace(myGSurface,0.01);
    Handle(AIS_Shape) aisFace=new AIS_Shape(myFace);
    aisFace->SetColor(Quantity_NOC_HOTPINK);
    aisFace->SetDisplayMode(AIS_Shaded);
    myContext->Display(aisFace, true );
}

#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

void GlfwOcctView::lab7()
{
    /*TopoDS_Shape cube = BRepPrimAPI_MakeBox(gp_Pnt(0.0, 0.0, 0.0), gp_Pnt(100, 100, 150)).Shape();
    Handle(AIS_Shape) aCube=new AIS_Shape(cube);
    aCube->SetMaterial( Graphic3d_NOM_GOLD );
    aCube->SetDisplayMode(AIS_Shaded);
    aCube->SetColor(Quantity_NOC_WHITE);
    myContext->Display(aCube, true );
*/
    gp_Ax2 anAxis2;
    anAxis2.SetLocation (gp_Pnt (50, 50, 0.0));
    Handle(AIS_Shape) aCylinder = new AIS_Shape (BRepPrimAPI_MakeCylinder(anAxis2, 60, 150, 4 * M_PI/2).Shape());
    aCylinder->SetDisplayMode(AIS_Shaded);
    aCylinder->SetColor(Quantity_NOC_WHITE);
    myContext->Display (aCylinder, AIS_Shaded, 0, false);

    gp_Ax2 anAxis(gp_Pnt(50,50,150),gp_Dir(0,0,1));
    TopoDS_Shape torus = BRepPrimAPI_MakeTorus(anAxis,60, 80, (0 * M_PI /180), (90 * M_PI /180));
    Handle(AIS_Shape) aTorus=new AIS_Shape(torus);
    aTorus->SetMaterial( Graphic3d_NOM_GOLD );
    aTorus->SetDisplayMode(AIS_Shaded);
    aTorus->SetColor(Quantity_NOC_ORANGERED);
    myContext->Display(aTorus, true );

}
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
void GlfwOcctView::lab8()
{


    TopoDS_Shape boxPlatform1 = BRepPrimAPI_MakeBox(gp_Pnt(-25, -25, 0.0), gp_Pnt(25, 25, 10)).Shape();
    TopoDS_Shape cylinderPlatform1 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(0, 0, 10), gp_Dir(0, 0, 1)), 12.5, 40).Shape();
    TopoDS_Shape cylinderPlatform2 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(0, 0, 10), gp_Dir(0, 0, 1)), 7.5, 40).Shape();
    TopoDS_Shape ShapeCut_1 = BRepAlgoAPI_Cut(cylinderPlatform1, cylinderPlatform2);
    TopoDS_Shape ShapeFuse_1 = BRepAlgoAPI_Fuse(boxPlatform1, ShapeCut_1);
    Handle(AIS_Shape) platform = new AIS_Shape(ShapeFuse_1);
    platform->SetMaterial( Graphic3d_NOM_CHROME);
    platform->SetDisplayMode(AIS_Shaded);
    platform->SetColor(Quantity_NOC_BISQUE);

    TopoDS_Shape cylinderRack1 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(100, 0, 0), gp_Dir(0, 0, 1)), 15, 50).Shape();
    TopoDS_Shape boxRack1 = BRepPrimAPI_MakeBox(gp_Pnt(75, -25, 50), gp_Pnt(125, 25, 100)).Shape();
    TopoDS_Shape boxRack2 = BRepPrimAPI_MakeBox(gp_Pnt(75.5, -45/2, 55), gp_Pnt(122.5, 45/2, 100)).Shape();
    TopoDS_Shape ShapeCut_2 = BRepAlgoAPI_Cut(boxRack1, boxRack2);
    TopoDS_Shape ShapeFuse_2 = BRepAlgoAPI_Fuse(cylinderRack1, ShapeCut_2);
    Handle(AIS_Shape) rack = new AIS_Shape(ShapeFuse_2);
    rack->SetMaterial( Graphic3d_NOM_CHROME);
    rack->SetDisplayMode(AIS_Shaded);
    rack->SetColor(Quantity_NOC_CHOCOLATE);

    myContext->Display(rack, true );
    myContext->Display(platform, true );


}

#include <TopoDS.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

void GlfwOcctView::lab9()
{
    TopoDS_Shape boxСorner1 = BRepPrimAPI_MakeBox(gp_Pnt(0, 0, 0), gp_Pnt(100, 100, 20)).Shape();
    TopoDS_Shape boxСorner2 = BRepPrimAPI_MakeBox(gp_Pnt(0, 0, 20), gp_Pnt(20, 100, 100)).Shape();
    TopoDS_Shape boxСorner3 = BRepPrimAPI_MakeBox(gp_Pnt(20, 0, 20), gp_Pnt(100, 20, 100)).Shape();
    TopoDS_Shape boxСorner4 = BRepPrimAPI_MakeBox(gp_Pnt(35, 0, 60), gp_Pnt(65, 20, 100)).Shape();


    TopTools_IndexedMapOfShape edge1; // массив ребер
    TopExp::MapShapes(boxСorner1, TopAbs_EDGE, edge1);
    BRepFilletAPI_MakeFillet aFillet1(boxСorner1);
    aFillet1.Add(35, TopoDS::Edge(edge1.FindKey(7)));

    TopTools_IndexedMapOfShape edge2; // массив ребер
    TopExp::MapShapes(boxСorner4, TopAbs_EDGE, edge2);
    BRepFilletAPI_MakeFillet aFillet2(boxСorner4);
    aFillet2.Add(10, TopoDS::Edge(edge2.FindKey(4)));
    aFillet2.Add(10, TopoDS::Edge(edge2.FindKey(8)));

    TopTools_IndexedMapOfShape edge3; // массив ребер
    TopExp::MapShapes(boxСorner2, TopAbs_EDGE, edge3);
    BRepFilletAPI_MakeChamfer aChamfer1(boxСorner2);
    aChamfer1.Add(20,TopoDS::Edge(edge3.FindKey(12)));

    TopTools_IndexedMapOfShape edge4; // массив ребер
    TopExp::MapShapes(boxСorner3, TopAbs_EDGE, edge4);
    BRepFilletAPI_MakeChamfer aChamfer2(boxСorner3);
    aChamfer2.Add(20,TopoDS::Edge(edge4.FindKey(6)));

    TopoDS_Shape ShapeFuse1 = BRepAlgoAPI_Fuse(aFillet1, aChamfer1);
    TopoDS_Shape ShapeFuse2 = BRepAlgoAPI_Fuse(ShapeFuse1, aChamfer2);
    TopoDS_Shape ShapeCut1 = BRepAlgoAPI_Cut(ShapeFuse2, aFillet2);

    TopoDS_Shape cylinderСorner1 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(65, 65, 10), gp_Dir(0, 0, 1)), 35, 25).Shape();
    TopoDS_Shape cylinderСorner2 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(65, 65, 0), gp_Dir(0, 0, 1)), 20, 50).Shape();

    TopoDS_Shape ShapeCut2 = BRepAlgoAPI_Cut(cylinderСorner1, cylinderСorner2);

    TopTools_IndexedMapOfShape edge5; // массив ребер
    TopExp::MapShapes(ShapeCut2, TopAbs_EDGE, edge5);
    BRepFilletAPI_MakeChamfer aChamfer3(ShapeCut2);
    aChamfer3.Add(2,TopoDS::Edge(edge5.FindKey(1)));


    TopoDS_Shape ShapeFuse3 = BRepAlgoAPI_Fuse(ShapeCut1, aChamfer3);
    TopoDS_Shape ShapeCut3 = BRepAlgoAPI_Cut(ShapeFuse3, cylinderСorner2);

    Handle (AIS_Shape) aAISShape = new AIS_Shape(ShapeCut3);
    aAISShape->SetMaterial(Graphic3d_NOM_CHROME);
    aAISShape->SetDisplayMode(AIS_Shaded);
    myContext->Display(aAISShape, true );

}

#include <BRepPrimAPI_MakeSphere.hxx>

// ================================================================
// Function : initDemoScene
// Purpose  :
// ================================================================
void GlfwOcctView::initDemoScene()
{
    if (myContext.IsNull())
    {
        return;
    }
    myView->TriedronDisplay (Aspect_TOTP_LEFT_LOWER, Quantity_NOC_TOMATO3, 0.08, V3d_WIREFRAME);

    gp_Ax2 anAxis;
    anAxis.SetLocation (gp_Pnt (0.0, 0.0, 0.0));
    Handle(AIS_Shape) aBox = new AIS_Shape (BRepPrimAPI_MakeBox (anAxis, 50, 50, 50).Shape());
    myContext->Display (aBox, AIS_Shaded, 0, false);
    anAxis.SetLocation (gp_Pnt (25.0, 125.0, 0.0));

    // Global private object
    aCone = new AIS_Shape (BRepPrimAPI_MakeCone (anAxis, 25, 0, 10).Shape());
    myContext->Display (aCone, AIS_Shaded, 0, false);

    anAxis.SetLocation (gp_Pnt (100, 100.0, 0.0));
    Handle(AIS_Shape) aCylinder = new AIS_Shape (BRepPrimAPI_MakeCylinder(anAxis, 25, 50, 3 * M_PI/2).Shape());
    myContext->Display (aCylinder, AIS_Shaded, 0, false);

    anAxis.SetLocation (gp_Pnt (150, 50.0, 0.0));
    Handle(AIS_Shape) aTorus = new AIS_Shape (BRepPrimAPI_MakeTorus(anAxis, 30, 15, 3 * M_PI/2).Shape());
    myContext->Display (aTorus, AIS_Shaded, 0, false);


    anAxis.SetLocation (gp_Pnt (150, 150.0, 0.0));
    Handle(AIS_Shape) aSphere= new AIS_Shape (BRepPrimAPI_MakeSphere(anAxis, 30, 3 * M_PI/2).Shape());
    myContext->Display (aSphere, AIS_Shaded, 0, false);

}

// ================================================================
// Function : mainloop
// Purpose  :
// ================================================================
void GlfwOcctView::mainloop()
{
    while (!glfwWindowShouldClose(myOcctWindow->getGlfwWindow()))
    {
        glfwWaitEvents();
        if (!myView.IsNull())
        {
            FlushViewEvents(myContext, myView, Standard_True);
            myInterface->renderGui(myOcctWindow);
        }      
    }
}

// ================================================================
// Function : cleanup
// Purpose  :
// ================================================================
void GlfwOcctView::cleanup()
{

    myInterface->clearImGUI();

    if (!myView.IsNull())
    {
        myView->Remove();
    }
    if (!myOcctWindow.IsNull())
    {
        myOcctWindow->Close();
    }
    glfwTerminate();
}

// ================================================================
// Function : onResize
// Purpose  :
// ================================================================
void GlfwOcctView::onResize(int theWidth, int theHeight)
{

    if (theWidth != 0
        && theHeight != 0
        && !myView.IsNull())
    {
        myView->Window()->DoResize();
        myView->MustBeResized();
        myView->Invalidate();
        myView->Redraw();
    }
}

// ================================================================
// Function : onMouseScroll
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseScroll(double theOffsetX, double theOffsetY)
{
    ImGuiIO& aIO = ImGui::GetIO();
    if (!myView.IsNull() && !aIO.WantCaptureMouse)
    {
        UpdateZoom(Aspect_ScrollDelta(myOcctWindow->CursorPosition(), int(theOffsetY * 8.0)));
    }
}

// ================================================================
// Function : onMouseButton
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseButton(int theButton, int theAction, int theMods)
{
    ImGuiIO& aIO = ImGui::GetIO();
    if (myView.IsNull() || aIO.WantCaptureMouse)
    {
        return;
    }

    const Graphic3d_Vec2i aPos = myOcctWindow->CursorPosition();
    if (theAction == GLFW_PRESS)
    {
        PressMouseButton(aPos, mouseButtonFromGlfw(theButton), keyFlagsFromGlfw(theMods), false);
    }
    else
    {
        ReleaseMouseButton(aPos, mouseButtonFromGlfw(theButton), keyFlagsFromGlfw(theMods), false);
    }
}

// ================================================================
// Function : onMouseMove
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseMove(int thePosX, int thePosY)
{
    if (myView.IsNull())
    {
        return;
    }

    ImGuiIO& aIO = ImGui::GetIO();
    if (aIO.WantCaptureMouse)
    {
        myView->Redraw();
    }
    else
    {
        const Graphic3d_Vec2i aNewPos(thePosX, thePosY);
        UpdateMousePosition(aNewPos, PressedMouseButtons(), LastMouseFlags(), Standard_False);
    }
}
