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
#include <vart/meshobject.h>
#include <vart/texture.h>
#include <vart/contrib/viewerglutogl.h>
#include <math.h>
#define _USE_MATH_DEFINES

#include <iostream>

using namespace std;
using namespace VART;

const unsigned int MAX_KEYS = 10; // max number of keys to keep control
enum Key { FOWARD, BACKWARD, RIGHT, LEFT };
bool keyPressed[MAX_KEYS];


class KeyboardHandler : public ViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler() {
            for (unsigned int i = 0; i < MAX_KEYS; ++i)
                keyPressed[i] = false;
        }
        virtual void OnKeyDown(int key) {
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = true;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = true;
                    break;
                case KEY_UP:
                    keyPressed[FOWARD] = true;
                    break;
                case KEY_DOWN:
                    keyPressed[BACKWARD] = true;
                    break;
                case 'j':
                    keyPressed[LEFT] = true;
                    break;
                case 'l':
                    keyPressed[RIGHT] = true;
                    break;
                case 'i':
                    keyPressed[FOWARD] = true;
                    break;
                case 'k':
                    keyPressed[BACKWARD] = true;
                    break;
            }
        }
        virtual void OnKeyUp(int key) {
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = false;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = false;
                    break;
                case KEY_UP:
                    keyPressed[FOWARD] = false;
                    break;
                case KEY_DOWN:
                    keyPressed[BACKWARD] = false;
                    break;
                case 'j':
                    keyPressed[LEFT] = false;
                    break;
                case 'l':
                    keyPressed[RIGHT] = false;
                    break;
                case 'i':
                    keyPressed[FOWARD] = false;
                    break;
                case 'k':
                    keyPressed[BACKWARD] = false;
                    break;
            }
        }
    private:
};

class MyIHClass : public ViewerGlutOGL::IdleHandler {
    private:
        double rotationAngle = 0.0;
	Point4D marinePos;

    public:
        Transform* translateMarine;
        Transform* rotateMarine;

        MyIHClass() {}

        virtual ~MyIHClass() {}

        virtual void OnIdle() {
            bool someKeyIsPressed = false;
            if (keyPressed[FOWARD]) {
                translateMarine->GetTranslation(&marinePos);
	        translateMarine->MakeTranslation(marinePos + Point4D(0, 0, 0.01, 0));
                someKeyIsPressed = true;
            }
            if (keyPressed[BACKWARD]) {
                translateMarine->GetTranslation(&marinePos);
                translateMarine->MakeTranslation(marinePos + Point4D(0, 0, -0.01, 0));
                someKeyIsPressed = true;
            }
            if (keyPressed[LEFT]) {
                //translateMarine->GetTranslation(&marinePos);
	        rotateMarine->MakeRotation(marinePos, Point4D::Y(), rotationAngle);
	        rotationAngle += 0.01;
                someKeyIsPressed = true;
            }
            if (keyPressed[RIGHT]) {
                //translateMarine->GetTranslation(&marinePos);
	        rotateMarine->MakeRotation(marinePos, Point4D::Y(), rotationAngle);
	        rotationAngle -= 0.01;
                someKeyIsPressed = true;
            }
            if (someKeyIsPressed)
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
    Camera camera(Point4D(0,0,6),Point4D::ORIGIN(),Point4D::Y());

    // create some objects
    list<MeshObject*> objects;
    MyIHClass idleHandler;

    // Initialize scene objects
    MeshObject::ReadFromOBJ(argv[1], &objects);
    MeshObject* marine;
    MeshObject* marineGun;
    Texture marineTexture, gunTexture;
    Material marineMaterial, gunMaterial;

    // Build up the scene
    list<VART::MeshObject*>::iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
	if ((*iter)->GetDescription() == "Marine") {
	    marine = *iter;
            marineTexture.LoadFromFile(argv[2]);
	    marineMaterial.SetTexture(marineTexture);
	    marine->SetMaterial(marineMaterial);
	}
	else if ((*iter)->GetDescription() == "Marine_Gun"){
	    marineGun = *iter;
	    gunTexture.LoadFromFile(argv[3]);
	    gunMaterial.SetTexture(gunTexture);
	    marineGun->SetMaterial(gunMaterial);
	}
    }


    // CHAO
    VART::MeshObject floor;
    floor.MakeBox(-3,3,-6,0,-3,3);
    floor.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());

    // Initialize scene objects

    Transform rotateInitialMarine, rotateMarine;
    Transform scaleMarine;
    Transform translateMarine;

    scaleMarine.AddChild(*marine);
    scaleMarine.AddChild(*marineGun);
    // regra de 3:
    // 2.51 <-> 100
    // 1.9  <-> X 
    scaleMarine.MakeScale(0.75697211, 0.756972111, 0.75697211);
    // Ajeita para plano correto
    rotateInitialMarine.AddChild(scaleMarine);
    rotateInitialMarine.MakeRotation(Point4D::X(), -M_PI/2);
    translateMarine.AddChild(rotateInitialMarine);
    translateMarine.MakeIdentity();
    rotateMarine.AddChild(translateMarine);
    rotateMarine.MakeIdentity();


    // Build up the scene
    scene.AddObject(&rotateMarine);
    scene.AddObject(&floor);

    // Keyboard handler
    KeyboardHandler kbh; // create a keyboard handler
    viewer.SetKbHandler(&kbh); // attach the keyboard handler

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    //scene.MakeCameraViewAll();

    // Set up the Idle Handler
    idleHandler.translateMarine = &translateMarine;
    idleHandler.rotateMarine = &rotateMarine;

    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler); // attach the idle handler to the viewer

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
