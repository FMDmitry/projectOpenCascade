#pragma once
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>

class GlfwOcctView;
class ModelingCascade
{
public:
    ModelingCascade(GlfwOcctView *obj);
    ModelingCascade(const ModelingCascade&) = default;
    ModelingCascade(ModelingCascade&&) = default;
    ModelingCascade& operator = (const ModelingCascade&) = default;
    ModelingCascade& operator = (ModelingCascade&&) = default;
    virtual ~ModelingCascade() = default;

    bool CreateHelix(GlfwOcctView *obj, int k ,  Handle(AIS_Shape) ais_shape[]);
    bool lab2(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[]);
    bool lab3(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[]);
    bool lab4(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[]);
    bool lab5(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[]);
    bool lab6(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[]);
    bool lab7(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[]);
    bool lab8(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[], float radius = 15, int Plheignt = 10);
    bool lab9(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[], float radius = 10);
    bool lab10(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[], float radius = 10);
    bool lab12(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[]);
    bool topoCounter(GlfwOcctView *obj, Handle(AIS_Shape) ais_shape[], int& totalVertices, int& totalEdges, int& totalFaces);


    bool CreateConus(GlfwOcctView *obj, int &koef, Handle(AIS_Shape) &aCone);
    bool CreateSome3D(GlfwOcctView *obj) noexcept;
    bool CreateStraightLine(GlfwOcctView *obj) noexcept;
    bool ExportStep(const TCollection_AsciiString &theFileName, GlfwOcctView *obj);
    bool ImportStep(const Standard_CString& theFileName, GlfwOcctView *obj);

private:
         // GlfwOcctView *objGlfwOcctView;
private:
    GlfwOcctView *objView;
};
