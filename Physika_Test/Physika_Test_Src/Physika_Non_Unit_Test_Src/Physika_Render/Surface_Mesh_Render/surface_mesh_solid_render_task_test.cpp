/*
* @file surface_mesh_solid_render_task_test.cpp
* @brief Test SurfaceMeshSolidRenderTask of Physika.
* @author WeiChen
*
* This file is part of Physika, a versatile physics simulation library.
* Copyright (C) 2013- Physika Group.
*
* This Source Code Form is subject to the terms of the GNU General Public License v2.0.
* If a copy of the GPL was not distributed with this file, you can obtain one at:
* http://www.gnu.org/licenses/gpl-2.0.html
*
*/
#include <iostream>
#include <memory>
#include <GL/freeglut.h>

#include "Physika_Core/Utilities/physika_assert.h"
#include "Physika_IO/Surface_Mesh_IO/obj_mesh_io.h"
#include "Physika_Geometry/Boundary_Meshes/surface_mesh.h"
#include "Physika_GUI/Glut_Window/glut_window.h"

#include "Physika_Render/ColorBar/ColorMap/color_map.h"

#include "Physika_Render/Lights/directional_light.h"
#include "Physika_Render/Lights/point_light.h"
#include "Physika_Render/Lights/spot_light.h"
#include "Physika_Render/Lights/flash_light.h"


#include "Physika_Render/Render_Scene_Config/render_scene_config.h"
#include "Physika_Render/Surface_Mesh_Render/surface_mesh_render_util.h"
#include "Physika_Render/Surface_Mesh_Render/surface_mesh_solid_render_task.h"
#include "Physika_Render/Surface_Mesh_Render/surface_mesh_normal_render_task.h"
#include "Physika_Render/Surface_Mesh_Render/surface_mesh_point_render_task.h"
#include "Physika_Render/Surface_Mesh_Render/surface_mesh_wireframe_render_task.h"
#include "Physika_Render/Surface_Mesh_Render/surface_mesh_point_vector_render_task.h"

#include "Physika_Render/OpenGL_Primitives/opengl_primitives.h"

using namespace std;
using namespace Physika;

SurfaceMesh<double> mesh;

void initFunction()
{
    cout << "loading mesh ......" << endl;
    //ObjMeshIO<double>::load("Physika_Test_Src/Physika_Non_Unit_Test_Src/Physika_Render/Obj_Mesh/scene_dense_mesh_refine_texture.obj", &mesh);
    ObjMeshIO<double>::load("Physika_Test_Src/Physika_Non_Unit_Test_Src/Physika_Render/Obj_Mesh/teapot.obj", &mesh);
    //ObjMeshIO<double>::load("Physika_Test_Src/Physika_Non_Unit_Test_Src/Physika_Render/Obj_Mesh/bottom_plane.obj", &mesh);
    //ObjMeshIO<double>::load("Physika_Test_Src/Physika_Non_Unit_Test_Src/Physika_Render/Obj_Mesh/ball_high.obj", &mesh);
    //ObjMeshIO<double>::load("Physika_Test_Src/Physika_Non_Unit_Test_Src/Physika_Render/Obj_Mesh/SunFlower.obj", &mesh);
    cout << "load mesh successfully" << endl;

    mesh.computeAllVertexNormals(SurfaceMesh<double>::FACE_NORMAL);

    //-----------------------------------------------------------------------------------------------------------------------------------------
    RenderSceneConfig & render_scene_config = RenderSceneConfig::getSingleton();
    auto render_util = make_shared<SurfaceMeshRenderUtil<double>>(&mesh);
    ColorMap<float> color_map;

    //-----------------------------------------------------------------------------------------------------------------------------------------
    auto solid_render_task = make_shared<SurfaceMeshSolidRenderTask<double>>(render_util);

    //solid_render_task->disableUseMaterial();
    //solid_render_task->disableUseTexture();
    //solid_render_task->disableUseLight();

    vector<Color4f> face_col_vec;
    for (int i = 0; i < mesh.numFaces(); ++i)
        face_col_vec.push_back(color_map.colorViaRatio(float(i) / mesh.numFaces()));

    vector<Color4f> vert_col_vec;
    for (int i = 0; i < mesh.numVertices(); ++i)
        vert_col_vec.push_back(color_map.colorViaRatio(float(i) / mesh.numVertices()));

    //solid_render_task->setCustomVertexColors(vert_col_vec);
    //solid_render_task->enableUseCustomColor();

    render_scene_config.pushBackRenderTask(solid_render_task);

    //-----------------------------------------------------------------------------------------------------------------------------------------
    auto normal_render_task = make_shared<SurfaceMeshNormalRenderTask<double>>(render_util);
    //normal_render_task->enableMapToColorSpace();
    //render_scene_config.pushBackRenderTask(normal_render_task);

    //-----------------------------------------------------------------------------------------------------------------------------------------
    auto wireframe_render_task = make_shared<SurfaceMeshWireframeRenderTask<double>>(render_util);
    wireframe_render_task->setUniformColor(Color4f::Black());
    render_scene_config.pushBackRenderTask(wireframe_render_task);

    //-----------------------------------------------------------------------------------------------------------------------------------------
    auto point_render_task = make_shared<SurfaceMeshPointRenderTask<double>>(render_util);
    point_render_task->setPointSize(3);
    point_render_task->setUniformColor(Color4f::Green());
    //render_scene_config.pushBackRenderTask(point_render_task);

    //-----------------------------------------------------------------------------------------------------------------------------------------
    auto point_vector_render_task = make_shared<SurfaceMeshPointVectorRenderTask<double>>(render_util);
    point_vector_render_task->setPointVectorsAsNormalVector();
    point_vector_render_task->enableUsePointVectorColor();
    point_vector_render_task->setScaleFactor(0.2);
    //render_scene_config.pushBackRenderTask(point_vector_render_task);

    //---------------------------------------------------------------------------------------------------------------------
    //light config

    //auto directional_light = make_shared<DirectionalLight>();
    //directional_light->setDirection({ 0, -1, 0 });
    //render_scene_config.pushBackLight(directional_light);


    //auto point_light = make_shared<PointLight>();
    ////point_light->setPos({ 0, 10, 0 });
    //point_light->setPos({ 0, 0, 100 });
    ////point_light->setDiffuse(Color4f::Red());
    ////point_light->setSpecular(Color4f::Black());
    //render_scene_config.pushBackLight(point_light);

    //auto spot_light = make_shared<SpotLight>();
    //spot_light->setPos({ 0, 100, 0 });
    //spot_light->setSpotDirection({ 0, -1, 0 });
    //spot_light->setSpotCutoff(0.0);
    //spot_light->setSpotOuterCutoff(20);
    ////spot_light->setSpotExponent(16);
    //spot_light->setAmbient(Color4f::Gray());
    //render_scene_config.pushBackLight(spot_light);

    auto flash_light = make_shared<FlashLight>();
    //flash_light->setAmbient(Color4f::White());
    render_scene_config.pushBackLight(flash_light);

    /*
    auto flash_light_2 = make_shared<FlashLight>();
    flash_light_2->setAmbient(Color4f::White());
    render_scene_config.pushBackLight(flash_light_2);
    */

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glClearDepth(1.0);
    glClearColor(0.49, 0.49, 0.49, 1.0);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}

void displayFunction()
{
    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    
    GlutWindow *cur_window = (GlutWindow*)glutGetWindowData();

    RenderSceneConfig & render_scene_config = RenderSceneConfig::getSingleton();
    render_scene_config.renderAllTasks();
    
    cur_window->displayFrameRate();
    glutPostRedisplay();
    glutSwapBuffers();
    */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);

    RenderSceneConfig & render_scene_config = RenderSceneConfig::getSingleton();
    ScreenBasedRenderManager & render_manager = render_scene_config.screenBasedRenderManager();
    render_manager.disableUseGammaCorrection();

    render_manager.render();

    GlutWindow *cur_window = (GlutWindow*)glutGetWindowData();
    cur_window->displayFrameRate();
    glutPostRedisplay();
    glutSwapBuffers();
}

void idleFunction()
{
    cout << "Custom idle function\n";
}

void keyboardFunction(unsigned char key, int x, int y)
{
    GlutWindow::bindDefaultKeys(key, x, y);
    switch (key)
    {
    case 't':
        cout << "test\n";
        break;
    default:
        break;
    }
}

int main()
{
    GlutWindow glut_window;
    cout << "Window name: " << glut_window.name() << "\n";
    cout << "Window size: " << glut_window.width() << "x" << glut_window.height() << "\n";

    RenderSceneConfig & render_scene_config = RenderSceneConfig::getSingleton();
    //render_scene_config.setCameraPosition(Vector<double, 3>(0, 0, 200));
    //render_scene_config.setCameraFocusPosition(Vector<double, 3>(0, 0, 0));
    render_scene_config.setCameraNearClip(0.1);
    render_scene_config.setCameraFarClip(1.0e3);

    glut_window.setDisplayFunction(displayFunction);
    //glut_window.setIdleFunction(idleFunction);
    glut_window.setInitFunction(initFunction);

    cout << "Test GlutWindow with custom display function:\n";
    glut_window.createWindow();
    cout << "Window size: " << glut_window.width() << "x" << glut_window.height() << "\n";
    cout << "Test window with GLUI controls:\n";

    return 0;
}