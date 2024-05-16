#include "modelingcascade.h"
#include "GlfwOcctView.h"
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>



ModelingCascade::ModelingCascade(GlfwOcctView *obj):objView(obj)
{

}

bool ModelingCascade::CreateSome3D(GlfwOcctView *obj) noexcept
{
    obj->getContext()->RemoveAll(true);
    obj->getContext()->UpdateCurrentViewer();
    gp_Ax2 anAxis;
    anAxis.SetLocation (gp_Pnt (0.0, 0.0, 0.0));
    Handle(AIS_Shape) aBox = new AIS_Shape (BRepPrimAPI_MakeBox (anAxis, 50, 50, 50).Shape());
    obj->getContext()->Display (aBox, AIS_Shaded, 0, false);
    anAxis.SetLocation (gp_Pnt (25.0, 125.0, 0.0));

    anAxis.SetLocation (gp_Pnt (100, 100.0, 0.0));
    Handle(AIS_Shape) aCylinder = new AIS_Shape (BRepPrimAPI_MakeCylinder(anAxis, 25, 50, 3 * M_PI/2).Shape());
    obj->getContext()->Display (aCylinder, AIS_Shaded, 0, false);

    anAxis.SetLocation (gp_Pnt (150, 50.0, 0.0));
    Handle(AIS_Shape) aTorus = new AIS_Shape (BRepPrimAPI_MakeTorus(anAxis, 30, 15, 3 * M_PI/2).Shape());
    obj->getContext()->Display (aTorus, AIS_Shaded, 0, false);

    anAxis.SetLocation (gp_Pnt (150, 150.0, 0.0));
    Handle(AIS_Shape) aSphere= new AIS_Shape (BRepPrimAPI_MakeSphere(anAxis, 30, 3 * M_PI/2).Shape());
    obj->getContext()->Display (aSphere, AIS_Shaded, 0, false);
    return true;
}


#include <Geom_Line.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
bool ModelingCascade::CreateStraightLine(GlfwOcctView *obj) noexcept
{
    // Define the start and end points of the line
    gp_Pnt startPoint(0., 0., 0.);
    gp_Pnt endPoint(1., 1., 0.);

    // Create a direction vector for the line
    gp_Dir direction(gp_Vec(startPoint, endPoint));

    // Create the line using the start point and direction
    Handle(Geom_Line) line = new Geom_Line(startPoint, direction);

    //!Get points the coordinates of some points on the line
    //gp_Pnt point1 = line->Value(0); // Start point
    //gp_Pnt point2 = line->Value(0.5); // Midpoint
    //gp_Pnt point3 = line->Value(1); // End point

    // Create an AIS_Shape object and initialize it with the Geom_Line
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(line);
    Handle(AIS_Shape) shape = new AIS_Shape(edge);
    obj->getContext()->Display (shape, AIS_Shaded, 0, true);

    return true;
}

bool ModelingCascade::CreateConus(GlfwOcctView *obj, int &koef , Handle(AIS_Shape) &aCone)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation (gp_Pnt (25.0, 125.0, 0.0));
    aCone = new AIS_Shape (BRepPrimAPI_MakeCone (anAxis, 25, 0, 10*koef).Shape());
    return true;
}

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <math.h>
bool ModelingCascade::lab2(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[])
{
    gp_Pnt pnt1( 0.0,0.0,0.0 );
    TopoDS_Vertex V1 = BRepBuilderAPI_MakeVertex( pnt1 );
    Handle(AIS_Shape) aV1 = new  AIS_Shape(V1);
    aV1->SetColor(Quantity_NOC_ORANGE);

    gp_Pnt pnt2( 0.0,100.0,0.0 );
    TopoDS_Vertex V2 = BRepBuilderAPI_MakeVertex( pnt2 );
    Handle(AIS_Shape) aV2 = new  AIS_Shape(V2);

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pnt1,pnt2);
    Handle(AIS_Shape) theLine = new  AIS_Shape(edge);

    const Standard_Real r=45 ;
    gp_Trsf myTrsf;
    myTrsf.SetRotation(gp::OX(), r * M_PI /180);
    gp_Pnt rotPoint = pnt2.Transformed(myTrsf);
    TopoDS_Vertex rotPointVert = BRepBuilderAPI_MakeVertex( rotPoint );
    Handle(AIS_Shape) aV3 = new  AIS_Shape(rotPointVert);
    aV3->SetColor(Quantity_NOC_PALEGREEN3);

    TopoDS_Edge edgeRot = BRepBuilderAPI_MakeEdge(pnt1,rotPoint);
    Handle(AIS_Shape) theLineRot = new  AIS_Shape(edgeRot);
    theLineRot->SetColor(Quantity_NOC_GRAY22);

    ais_shape[0] = aV1;
    ais_shape[1] = aV2;
    ais_shape[2] = theLine;
    ais_shape[3] = aV3;
    ais_shape[4] = theLineRot;
    return true;
}

bool ModelingCascade::lab3(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[])
{
    return 1;
}
#include <GeomAPI_PointsToBSpline.hxx>
#include <Geom_BSplineCurve.hxx>
bool ModelingCascade::lab4(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[])
{
    const double a = 40.0;
    const double b = 0.0;
    const int N = 1000;
    const double angleIncrement = 8 * M_PI / N;

    std::vector<gp_Pnt> points;

    for (int i = 0; i < N; ++i) {
        double angle = i * angleIncrement;
        double radius = a - (a * i / N);
        double x = radius * cos(angle);
        double y = radius * sin(angle);
        double z = b * i;
        points.push_back(gp_Pnt(x, y, z));
    }

    TColgp_Array1OfPnt array(1, N);
    for (int i = 0; i < N; ++i) {
        array(i + 1) = points[i];
    }

    const Standard_Integer degree = 3;

    Handle(Geom_BSplineCurve) curve_bspline = GeomAPI_PointsToBSpline(array).Curve();
    TopoDS_Edge myEdge1 = BRepBuilderAPI_MakeEdge(curve_bspline);

    Handle(AIS_Shape) aisEdge1 = new AIS_Shape(myEdge1);
    aisEdge1->SetWidth(3.0);
    aisEdge1->SetColor(Quantity_NOC_AQUAMARINE1);
    ais_shape[0] = aisEdge1;
    return 1;
}
#include <GeomConvert.hxx>
#include <Geom_BezierCurve.hxx>
bool ModelingCascade::lab5(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[])
{
    int x1 = 20, y1 = 30;
    int x2 = 80, y2 = 300;
    int x3 = 140, y3 = 30;
    TColgp_Array1OfPnt dots (1,100);
    for(Standard_Real t=0, i = 1; t<=1; t+=0.01, i++)
    {
        Standard_Real x = pow((1-t),2) * x1 + 2*(1-t) * t * x2 + pow(t,2) * x3;
        Standard_Real y = pow((1-t),2) * y1 + 2*(1-t) * t * y2 + pow(t,2) * y3;
        gp_Pnt pnt(x,y,0);
        dots.SetValue(i,pnt);
    }
    Handle(Geom_BSplineCurve) curve_bspline = GeomAPI_PointsToBSpline(dots).Curve();
    TopoDS_Edge myEdge = BRepBuilderAPI_MakeEdge(curve_bspline);
    Handle(AIS_Shape) aCurve = new AIS_Shape(myEdge);

    TColgp_Array1OfPnt array(1,3);
    gp_Pnt myPoint1(20,30,40);
    gp_Pnt myPoint2(80,300,40);
    gp_Pnt myPoint3(140,30,40);
    array.SetValue(1,myPoint1);
    array.SetValue(2,myPoint2);
    array.SetValue(3,myPoint3);
    Handle(Geom_BezierCurve) curve_bz = new Geom_BezierCurve(array);
    Handle(Geom_BSplineCurve) e1_geom = GeomConvert::CurveToBSplineCurve(curve_bz);
    TopoDS_Edge myEdge1 = BRepBuilderAPI_MakeEdge(e1_geom);
    Handle(AIS_Shape) aisEdge1=new AIS_Shape(myEdge1);
    aisEdge1->SetColor(Quantity_NOC_WHITESMOKE);
    ais_shape[0] = aCurve;
    ais_shape[1] = aisEdge1;
    return 1;
}

#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BSplineSurface.hxx>

bool ModelingCascade::lab6(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[])
{
    int arraySize=3;
    TColgp_Array2OfPnt myStucture(0,arraySize,0,arraySize);
    srand(time(NULL));
    for(int i=0; i<=arraySize; i++)
    {
        for(int j=0; j<=arraySize; j++)
        {
            int z=10+rand()%(100-1);
            myStucture.SetValue(j,i,gp_Pnt(i+10*pow(i,2),j+20*pow(j,2),z));
        }
    }
    Handle (Geom_Surface) myGSurface = new Geom_BezierSurface(myStucture);
    TopoDS_Face myFace = BRepBuilderAPI_MakeFace(myGSurface,0.01);
    Handle(AIS_Shape) aisFace=new AIS_Shape(myFace);
    aisFace->SetColor(Quantity_NOC_PINK1);
    aisFace->SetDisplayMode(AIS_Shaded);
    ais_shape[0] = aisFace;
    return 1;
}

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
bool ModelingCascade::lab7(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[])
{
    TopoDS_Shape cube = BRepPrimAPI_MakeBox(gp_Pnt(0.0, 0.0, 0.0), gp_Pnt(150, 100, 150)).Shape();
    Handle(AIS_Shape) aCube=new AIS_Shape(cube);
    aCube->SetMaterial( Graphic3d_NOM_CHROME);
    aCube->SetDisplayMode(AIS_Shaded);
    aCube->SetColor(Quantity_NOC_ALICEBLUE);

    gp_Ax2 anAxis(gp_Pnt(0,150,0),gp_Dir(0,0,1));
    TopoDS_Shape torus = BRepPrimAPI_MakeTorus(anAxis,150, 5, (180 * M_PI /180));
    Handle(AIS_Shape) aTorus=new AIS_Shape(torus);
    aTorus->SetMaterial( Graphic3d_NOM_CHROME);
    aTorus->SetDisplayMode(AIS_Shaded);
    aTorus->SetColor(Quantity_NOC_ALICEBLUE);

    ais_shape[0] = aCube;
    ais_shape[1] = aTorus;
    return 1;
}
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
bool ModelingCascade::lab8(GlfwOcctView *obj, opencascade::handle<AIS_Shape> ais_shape[], float radius, int Plheignt)
{
    TopoDS_Shape cube = BRepPrimAPI_MakeBox(gp_Pnt(-25, -25, 0.0), gp_Pnt(25, 25, Plheignt)).Shape();

    TopoDS_Shape cyl = BRepPrimAPI_MakeCylinder
                       (gp_Ax2(gp_Pnt(0, 0, Plheignt), gp_Dir(0, 0, 1)), 12.5, 40).Shape();
    TopoDS_Shape cyl_2 = BRepPrimAPI_MakeCylinder
                         (gp_Ax2(gp_Pnt(0, 0, Plheignt), gp_Dir(0, 0, 1)), radius/2, 40).Shape();
    TopoDS_Shape ShapeCut_1 = BRepAlgoAPI_Cut(cyl, cyl_2);
    TopoDS_Shape ShapeFuse_1 = BRepAlgoAPI_Fuse(cube, ShapeCut_1);
    Handle(AIS_Shape) platform = new AIS_Shape(ShapeFuse_1);
    platform->SetMaterial( Graphic3d_NOM_CHROME);
    platform->SetDisplayMode(AIS_Shaded);
    platform->SetColor(Quantity_NOC_ALICEBLUE);

    TopoDS_Shape cyl_3 = BRepPrimAPI_MakeCylinder
                         (gp_Ax2(gp_Pnt(100, 100, 0), gp_Dir(0, 0, 1)), 15, 50).Shape();
    TopoDS_Shape cube_2 = BRepPrimAPI_MakeBox(gp_Pnt(75, 75, 50), gp_Pnt(125, 125, 100)).Shape();
    TopoDS_Shape cube_3 = BRepPrimAPI_MakeBox(gp_Pnt(77.5, 77.5, 55), gp_Pnt(122.5, 122.5, 100)).Shape();
    TopoDS_Shape ShapeCut_2 = BRepAlgoAPI_Cut(cube_2, cube_3);
    TopoDS_Shape ShapeFuse_2 = BRepAlgoAPI_Fuse(cyl_3, ShapeCut_2);
    Handle(AIS_Shape) stoika = new AIS_Shape(ShapeFuse_2);
    stoika->SetMaterial( Graphic3d_NOM_CHROME);
    stoika->SetDisplayMode(AIS_Shaded);
    stoika->SetColor(Quantity_NOC_ALICEBLUE);
    ais_shape[0] = platform;
    ais_shape[1] = stoika;
}

#include <TopoDS.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

bool ModelingCascade::lab9(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[], float radius)
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
    TopoDS_Shape cylinderСorner2 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(65, 65, 0), gp_Dir(0, 0, 1)), radius, 50).Shape();

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
    ais_shape[0] = aAISShape;
    return 1;
}

#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>



bool ModelingCascade::lab10(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[], float radius)
{
    gp_Pnt pnt1_1(0, 0, 0);
    gp_Pnt pnt2_1(100, 0, 0);
    gp_Pnt pnt3_1(100, 100, 0);
    gp_Pnt pnt4_1(0, 100, 0);

    TopoDS_Edge aEdge1_1 = BRepBuilderAPI_MakeEdge(pnt1_1, pnt2_1);
    TopoDS_Edge aEdge2_1 = BRepBuilderAPI_MakeEdge(pnt2_1, pnt3_1);
    TopoDS_Edge aEdge3_1 = BRepBuilderAPI_MakeEdge(pnt3_1, pnt4_1);
    TopoDS_Edge aEdge4_1 = BRepBuilderAPI_MakeEdge(pnt4_1, pnt1_1);

    BRepBuilderAPI_MakeWire makeW;
    makeW.Add(aEdge1_1);
    makeW.Add(aEdge2_1);
    makeW.Add(aEdge3_1);
    makeW.Add(aEdge4_1);
    TopoDS_Wire Wc = makeW.Wire();
    TopoDS_Face F = BRepBuilderAPI_MakeFace(Wc);
    gp_Vec move(gp_Dir(0, 0, 1));
    move *= 10;

    TopoDS_Shape box1 = BRepPrimAPI_MakePrism(F, move);
    TopTools_IndexedMapOfShape edge5_1; // массив ребер
    TopExp::MapShapes(box1, TopAbs_EDGE, edge5_1);
    BRepFilletAPI_MakeFillet aFillet1(box1);
    aFillet1.Add(20,TopoDS::Edge(edge5_1.FindKey(1)));
    aFillet1.Add(20,TopoDS::Edge(edge5_1.FindKey(2)));

        ////////////

        gp_Pnt pnt1_2(42.5, 30, 10);
        gp_Pnt pnt2_2(57.5, 30, 10);
        gp_Pnt pnt3_2(57.5, 40, 10);
        gp_Pnt pnt4_2(42.5, 40, 10);

        TopoDS_Edge aEdge1_2 = BRepBuilderAPI_MakeEdge(pnt1_2, pnt2_2);
        TopoDS_Edge aEdge2_2 = BRepBuilderAPI_MakeEdge(pnt2_2, pnt3_2);
        TopoDS_Edge aEdge3_2 = BRepBuilderAPI_MakeEdge(pnt3_2, pnt4_2);
        TopoDS_Edge aEdge4_2 = BRepBuilderAPI_MakeEdge(pnt4_2, pnt1_2);

        BRepBuilderAPI_MakeWire makeW2;
        makeW2.Add(aEdge1_2);
        makeW2.Add(aEdge2_2);
        makeW2.Add(aEdge3_2);
        makeW2.Add(aEdge4_2);
        TopoDS_Wire Wc2 = makeW2.Wire();
        TopoDS_Face F2 = BRepBuilderAPI_MakeFace(Wc2);
        gp_Vec move2(gp_Dir(0, 0.5, 1));
        move2 *= 80;
        TopoDS_Shape box2 = BRepPrimAPI_MakePrism(F2, move2);

        ////////////

        gp_Pnt pnt12(45, 40, 10);
        gp_Pnt pnt22(45, 60, 50);
        gp_Pnt pnt32(45, 100, 50);
        gp_Pnt pnt42(45, 100, 10);

        TopoDS_Edge aEdge12 = BRepBuilderAPI_MakeEdge(pnt12, pnt22);
        TopoDS_Edge aEdge22 = BRepBuilderAPI_MakeEdge(pnt22, pnt32);
        TopoDS_Edge aEdge32 = BRepBuilderAPI_MakeEdge(pnt32, pnt42);
        TopoDS_Edge aEdge42 = BRepBuilderAPI_MakeEdge(pnt42, pnt12);

        BRepBuilderAPI_MakeWire makeW22;
        makeW22.Add(aEdge12);
        makeW22.Add(aEdge22);
        makeW22.Add(aEdge32);
        makeW22.Add(aEdge42);
        TopoDS_Wire Wc22 = makeW22.Wire();
        TopoDS_Face F22 = BRepBuilderAPI_MakeFace(Wc22);
        gp_Vec move22(gp_Dir(1, 0, 0));
        move22 *= 10;
        TopoDS_Shape box3 = BRepPrimAPI_MakePrism(F22, move22);


        /////

        TopoDS_Shape cyl1 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(50, 100, 60), gp_Dir(0, -1, 0)), 25, 60).Shape();
        TopoDS_Shape cyl2 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(50, 100, 60), gp_Dir(0, -1, 0)), radius, 60).Shape();

        ////////////

        gp_Pnt pnt1_3(0, 100, 10);
        gp_Pnt pnt2_3(25, 100, 60);
        gp_Pnt pnt3_3(75, 100, 60);
        gp_Pnt pnt4_3(100, 100, 10);

        TopoDS_Edge aEdge1_3 = BRepBuilderAPI_MakeEdge(pnt1_3, pnt2_3);
        TopoDS_Edge aEdge2_3 = BRepBuilderAPI_MakeEdge(pnt2_3, pnt3_3);
        TopoDS_Edge aEdge3_3 = BRepBuilderAPI_MakeEdge(pnt3_3, pnt4_3);
        TopoDS_Edge aEdge4_3 = BRepBuilderAPI_MakeEdge(pnt4_3, pnt1_3);

        BRepBuilderAPI_MakeWire makeW3;
        makeW3.Add(aEdge1_3);
        makeW3.Add(aEdge2_3);
        makeW3.Add(aEdge3_3);
        makeW3.Add(aEdge4_3);
        TopoDS_Wire Wc3 = makeW3.Wire();
        TopoDS_Face F3 = BRepBuilderAPI_MakeFace(Wc3);
        gp_Vec move3(gp_Dir(0, -1, 0));
        move3 *= 10;
        TopoDS_Shape box4 = BRepPrimAPI_MakePrism(F3, move3);

        TopoDS_Shape cyl3 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(15, 15, 0), gp_Dir(0, 0, 1)), 7.5, 10).Shape();
        TopoDS_Shape cyl4 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(85, 15, 0), gp_Dir(0, 0, 1)), 7.5, 10).Shape();

        TopoDS_Shape cyl5 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(50, 65, 60), gp_Dir(0, 0, 1)), 5, 30).Shape();



        TopoDS_Shape ShapeFuse1 = BRepAlgoAPI_Fuse(aFillet1, box2);
        TopoDS_Shape ShapeFuse2 = BRepAlgoAPI_Fuse(ShapeFuse1, box3);
        TopoDS_Shape ShapeFuse3 = BRepAlgoAPI_Fuse(ShapeFuse2, box4);

        TopoDS_Shape ShapeCut1 = BRepAlgoAPI_Cut(ShapeFuse3, cyl1);
        TopoDS_Shape ShapeCut2 = BRepAlgoAPI_Cut(ShapeCut1, cyl3);
        TopoDS_Shape ShapeCut3 = BRepAlgoAPI_Cut(ShapeCut2, cyl4);


        TopoDS_Shape ShapeCutCyl1 = BRepAlgoAPI_Cut(cyl1, cyl2);
        TopoDS_Shape ShapeCutCyl2 = BRepAlgoAPI_Cut(ShapeCutCyl1, cyl5);

        TopoDS_Shape ShapeFuse4 = BRepAlgoAPI_Fuse(ShapeCut3, ShapeCutCyl2);

        Handle (AIS_Shape) aAISShape = new AIS_Shape(ShapeFuse4);
        aAISShape->SetColor(Quantity_NOC_GOLD);
        aAISShape->SetMaterial(Graphic3d_NOM_NEON_PHC);
        aAISShape->SetDisplayMode(AIS_Shaded);
        ais_shape[0] = aAISShape;







        return 1;
}

#include <TopExp_Explorer.hxx>


bool ModelingCascade::lab12(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[])
{
    int index = 0;


    for (obj->getContext()->InitSelected(); obj->getContext()->MoreSelected(); obj->getContext()->NextSelected())
    {
        Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(obj->getContext()->SelectedInteractive());
        if (anIS.IsNull())
        {
            return false;
        }
        TopoDS_Shape S = anIS->Shape();
        TopExp_Explorer explorer(S, TopAbs_VERTEX);
        explorer.Init(S, TopAbs_FACE);
        for (; explorer.More(); explorer.Next())
        {
            TopoDS_Shape myTopoDS = explorer.Current();
            BRepAdaptor_Surface surface(TopoDS::Face(myTopoDS));
            if (surface.GetType() == GeomAbs_Plane) // Проверяем, является ли грань плоской
            {
                Handle(AIS_Shape) myShape = new AIS_Shape(myTopoDS);
                myShape->SetColor(Quantity_NOC_RED);
                myShape->SetDisplayMode(AIS_Shaded);
                ais_shape[index++] = myShape;
            }
        }
    }



    // Отображение AIS_Shape для плоских граней
    for (int i = 0; i < index; i++)
    {
        obj->getContext()->Display(ais_shape[i], true);
    }

    return true;
}

bool ModelingCascade::topoCounter(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[], int& totalVertices, int& totalEdges, int& totalFaces)
{
    int index = 0;
    int pointNumber = 1;

    for (obj->getContext()->InitSelected(); obj->getContext()->MoreSelected(); obj->getContext()->NextSelected())
    {
        Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(obj->getContext()->SelectedInteractive());
        if (anIS.IsNull())
        {
            return false;
        }
        TopoDS_Shape S = anIS->Shape();

        TopTools_IndexedMapOfShape v;
        TopExp::MapShapes(S , TopAbs_VERTEX, v);
        totalVertices=v.Size();

        TopTools_IndexedMapOfShape m;
        TopExp::MapShapes(S , TopAbs_EDGE, m);
        totalEdges=m.Size();

        TopTools_IndexedMapOfShape f;
        TopExp::MapShapes(S , TopAbs_FACE, f);
        totalFaces=f.Size();
    }

    return true;
}

#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>
bool ModelingCascade::ExportStep (const TCollection_AsciiString& theFileName, GlfwOcctView *obj)
{
    STEPControl_StepModelType aType = STEPControl_AsIs;
    STEPControl_Writer        aWriter;
    for (obj->getContext()->InitSelected(); obj->getContext()->MoreSelected(); obj->getContext()->NextSelected())
    {
        Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast (obj->getContext()->SelectedInteractive());
        if (anIS.IsNull())
        {
            return false;
        }
        TopoDS_Shape aShape = anIS->Shape();
        if (aWriter.Transfer (aShape, aType) != IFSelect_RetDone)
        {
            return false;
        }
    }
    return aWriter.Write (theFileName.ToCString()) == IFSelect_RetDone;
}
#include <STEPControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <BRepTools.hxx>
bool ModelingCascade::ImportStep(const Standard_CString &theFileName, GlfwOcctView *obj)
{
    STEPControl_Reader aReader;
    IFSelect_ReturnStatus status = aReader.ReadFile(theFileName);
    if (status != IFSelect_RetDone) {
        return false;

    }
    else {
        aReader.NbRootsForTransfer();
        aReader.TransferRoots();
        TopoDS_Shape shape = aReader.OneShape();
        Handle(AIS_Shape) result = new  AIS_Shape(shape);
        result->SetMaterial( Graphic3d_NOM_CHROME);
        obj->getContext()->Display(result, AIS_Shaded, 0, false);
        return 1;
    }

}


bool ModelingCascade::CreateHelix(GlfwOcctView *obj, int k ,  Handle(AIS_Shape) ais_shape[])
{
    const double R = 50.0;
    const double H = 500;
    const int N = 100;
    TColgp_Array1OfPnt points (0,N*k);
    for(int j = 1; j<=k; j++){
        for (int i = 0; i <= N; i++) {
            double t = 2 * M_PI * i / N;
            double x = R * cos(t);
            double y = R * sin(t);
            double z = H * t / N + (H * 2 * M_PI / N * (j-1));
            gp_Pnt pnt(x, y, z);
            points.SetValue(i+(j-1)*N, pnt);
        }
    }
    // Standard_Real myDistance = points[0].Distance(points[200]);
    Handle(Geom_BSplineCurve) e1_geom = GeomAPI_PointsToBSpline(points).Curve();
    TopoDS_Edge myEdge1 = BRepBuilderAPI_MakeEdge(e1_geom);
    Handle(AIS_Shape) aisEdge1 = new AIS_Shape(myEdge1);
    ais_shape[0] = aisEdge1;
    return 1;
}

