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
        bool indo = true;
    public:
        // transformacoes que ocorrerao durante a execucao do programa
        Transform* rotZhelice;
        Transform* rotYNonBase;
        MyIH() {
        }
        virtual ~MyIH(){
        }

        virtual void OnIdle() {
            // faz a rotacao no eixo X 'transladado' para o centro da helice
            rotZhelice->MakeRotation(Point4D(0, 1.228443, 0, 1), Point4D::X(), rotationHelice);
            // rotaciona todo objeto que nao eh base
            rotYNonBase->MakeRotation(Point4D(0, 0, 0, 1), Point4D::Y(), rotationBase);
            rotationHelice += 0.5;

            if (!indo) {
                rotationBase -= 0.01;
            } else { 
                rotationBase += 0.01;
            }

            if (rotationBase >= M_PI / 2) {
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
    Camera camera(Point4D(-10,0,0),Point4D(0,0,0),Point4D(0,1,0,0));
    // create some objects
    list<MeshObject*> objects;

    // Initialize scene objects
    MeshObject::ReadFromOBJ("fan.obj", &objects);
    Transform Giro;
    Transform HeliceRotation;

    list<VART::MeshObject*>::iterator iter = objects.begin();

    MeshObject* helice;
    MeshObject* base;
    MeshObject* suporteEixo;
    MeshObject* grade;
    MeshObject* eixo;

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
        else if ((*iter)->GetDescription() == "eixo") {
            eixo = *iter;
        }
		else if((*iter)->GetDescription() == "suporte-eixo"){
			suporteEixo = *iter;
		}
    }

    HeliceRotation.AddChild(*helice);
    Giro.AddChild(*eixo);
    Giro.AddChild(*suporteEixo);
    Giro.AddChild(*grade);
    Giro.AddChild(HeliceRotation);

    scene.AddObject(&Giro);
    scene.AddObject(base);

    MyIH idle;
    idle.rotZhelice = &HeliceRotation;
    idle.rotYNonBase = &Giro;
	

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
