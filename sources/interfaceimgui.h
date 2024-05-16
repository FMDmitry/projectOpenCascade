#pragma once
#include "GlfwOcctWindow.h"
#include <AIS_Shape.hxx>
#include <memory>

class GlfwOcctView;
class ModelingCascade;
class InterfaceCAD
{
public:
    InterfaceCAD(GlfwOcctView *obj);
    virtual ~InterfaceCAD();

    //! Init ImGui.
    void initGui(Handle(GlfwOcctWindow) &objGLFW);

    //! Render ImGUI.
    void renderGui(Handle(GlfwOcctWindow) &objGLFW);

    //! Cleanup ImGUI
    void clearImGUI();

    //!Menu
    void menuToolBar(Handle(GlfwOcctWindow) &objGLFW);


private:
   GlfwOcctView *objView;
   std::unique_ptr<ModelingCascade> objModelingCascade;
   Handle(AIS_Shape) aTestCone;
   int k = 0;
};
