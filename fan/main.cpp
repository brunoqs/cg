// main.cpp - transform animation example

// V-ART example: Moving objects with an idle handler

// This application shows how to use ViewerGlutOGL's idle handler to animate objects.

// Changelog
// Jun 17, 2008 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/meshobject.h>

#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

using namespace std;
using namespace VART;

class MyIH : public ViewerGlutOGL::IdleHandler {
    private:
        // angulo de rotacao da bolas
        double rotationHelice = 0.1;
        double rotationBase = 0;
        bool indo = false;
    public:
        // transformacoes que ocorrerao durante a execucao do programa
        Transform* rotZhelice;
        Transform* rotZbase;
        MyIH() {
        }
        virtual ~MyIH(){
        }

        virtual void OnIdle() {
            // faz a rotacao em X da bola
            rotZhelice->MakeRotation(Point4D(0, 1.228443, 0, 1), Point4D::X(), rotationHelice);
            rotZbase->MakeRotation(Point4D(0, 0, 0, 1), Point4D::Y(), rotationBase);
            // faz a rotacao em Y da bola
            // rotationYBola->MakeRotation(Point4D::Y(), rotationBolaParameter);
            // varia o angulo de rotacao
            rotationHelice += 0.5;

            if (!indo) {
                rotationBase -= 0.01;
            } else { 
                rotationBase += 0.01;
            }

            if (rotationBase >= M_PI) {
                indo = false;
            }
            else if(rotationBase <= -M_PI / 2)  {
                indo = true;
            }



            viewerPtr->PostRedisplay();
        }
};

// The application itself:
int main(int argc, char* argv[])
{
    ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static Scene scene; // create a scene
    static ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    Camera camera(Point4D(0,0,6),Point4D(0,0,0),Point4D(0,1,0,0));
    // create some objects
    list<MeshObject*> objects;

    // Initialize scene objects
    MeshObject::ReadFromOBJ("fan.obj", &objects);
    Transform Giro;
    Transform HeliceRotation;

    list<VART::MeshObject*>::iterator iter = objects.begin();

    MeshObject* helice;
    MeshObject* base;
    MeshObject* none;
    MeshObject* grade;

    for (; iter != objects.end(); ++iter) {
        if((*iter)->GetDescription() == "helice"){
			helice = *iter;
		}
		else if((*iter)->GetDescription() == "base"){
			base = *iter;
		}
        else if ((*iter)->GetDescription() == "grade") {
            grade = *iter;
        }
		else if((*iter)->GetDescription() != "base"){
			none = *iter;
		}
    }

    // HeliceRotation.AddChild(helice);
    // HeliceRotation.MakeTranslation(10, 0, 0);

    HeliceRotation.AddChild(*helice);
    Giro.AddChild(*none);
    Giro.AddChild(*grade);
    Giro.AddChild(HeliceRotation);

    scene.AddObject(&Giro);
    scene.AddObject(base);

    MyIH idle;
    idle.rotZhelice = &HeliceRotation;
    idle.rotZbase = &Giro;
	

    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idle); // attach the idle handler to the viewer

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
