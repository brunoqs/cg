#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/scene.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/texture.h>
#include <vart/transform.h>

#include <math.h>
#define _USE_MATH_DEFINES

#define CHAIRS_COUNT 10
#define WHEEL_RADIUS 70
#define ANGLE_BETWEEN_CHAIRS (2 * M_PI) / CHAIRS_COUNT

using namespace std;
using namespace VART;

class MyIH : public ViewerGlutOGL::IdleHandler {
    private:
        double rotXWheelParameter = 0.1;
        double dy = 0.0, dz = 0.0;
        
    public:
        // transformacoes que ocorrerao durante a execucao do programa
        Transform* rotXWheel;
        vector<Transform*> trans_chairs;
        
        MyIH() {
        }
        virtual ~MyIH(){
        }

        virtual void OnIdle() {
            rotXWheel->MakeRotation(Point4D::X(), rotXWheelParameter);

            for(int i = 0; i < CHAIRS_COUNT; i++) {
                dy = WHEEL_RADIUS * sin(rotXWheelParameter + ANGLE_BETWEEN_CHAIRS*i);
                dz = WHEEL_RADIUS * -cos(rotXWheelParameter + ANGLE_BETWEEN_CHAIRS*i);
                trans_chairs[i]->MakeTranslation(0.0, dy, dz);
            }

            rotXWheelParameter += 0.01;

            viewerPtr->PostRedisplay();
        }
};


int main(int argc, char* argv[]) {

    ViewerGlutOGL::Init(&argc, argv);
    static ViewerGlutOGL viewer;
    static Scene scene;

	Camera camera(Point4D(100, 0, 0), Point4D::ORIGIN(), Point4D::Y());

    // leitura dos objetos
    list<MeshObject*> objects;
    MeshObject::ReadFromOBJ("ferris-wheel.obj", &objects);

    list<MeshObject*>::iterator iter = objects.begin();

    MeshObject* chair;
    MeshObject* support;
    MeshObject* wheel;

    // reconhecimento dos obejtos
    for (; iter != objects.end(); ++iter) {
        if((*iter)->GetDescription() == "chair") {
            chair = *iter;
        }
        if((*iter)->GetDescription() == "support") {
            support = *iter;
        }
        if((*iter)->GetDescription() == "wheel") {
            wheel = *iter;
        }
    }

    // WHEEL
    Transform rotXWheel;
    rotXWheel.AddChild(*wheel);
    scene.AddObject(&rotXWheel);

    // CHAIRS
    vector<Transform*> trans_chairs;
    // para cada cadeira, cria uma transformacao
    for (int i = 0; i < CHAIRS_COUNT; i++){
        Transform *transform = new Transform();
        transform->AddChild(*chair);
        trans_chairs.push_back(transform);
        scene.AddObject(trans_chairs[i]);
    }

    MyIH idle;
    idle.rotXWheel = &rotXWheel;
    idle.trans_chairs = trans_chairs;

    //Add a light to the scene.
    scene.AddLight( Light::BRIGHT_AMBIENT() );
    scene.AddCamera(&camera);
    scene.DrawLightsOGL();

    //Setup the viewer.
    viewer.SetTitle("Roda Gigante");
    viewer.SetScene(scene);
    viewer.SetIdleHandler(&idle);

    //Enable texture in OpenGL
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Enter the main loop
    VART::ViewerGlutOGL::MainLoop();
}
