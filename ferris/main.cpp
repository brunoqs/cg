#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/scene.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/texture.h>
#include <vart/transform.h>


using namespace std;
using namespace VART;

class MyIH : public ViewerGlutOGL::IdleHandler {
    private:
        // angulo de rotacao da bolas
        double rotationTerraParameter = 0.1;
        double rotationTerraSi = 0.1;
        double rotationluaParameter = 0.1;
        
    public:
        // transformacoes que ocorrerao durante a execucao do programa
        Transform* rotZterra;
        Transform* rotZterraSi;
        Transform* rotZlua;
        MyIH() {
        }
        virtual ~MyIH(){
        }

        virtual void OnIdle() {
            // faz a rotacao em X da bola
            rotZterra->MakeRotation(Point4D::Z(), rotationTerraParameter);
            rotZterraSi->MakeRotation(Point4D::Z(), rotationTerraSi);
            rotZlua->MakeRotation(Point4D::Z(), rotationluaParameter);
            // faz a rotacao em Y da bola
            // rotationYBola->MakeRotation(Point4D::Y(), rotationBolaParameter);
            // varia o angulo de rotacao
            rotationTerraParameter += 0.01;
            rotationTerraSi += 0.03;
            rotationluaParameter += 0.1;

            viewerPtr->PostRedisplay();
        }
};



int main(int argc, char* argv[]) {

    ViewerGlutOGL::Init(&argc, argv);
    static ViewerGlutOGL viewer;
    static Scene scene;

	Camera camera(Point4D(0, 0, 10), Point4D(0, 0, 0), Point4D(0, 1, 0));

    // leitura dos objetos
    list<MeshObject*> listObjs;
    MeshObject::ReadFromOBJ("ferris-wheel.obj", &listObjs);

    vector<MeshObject*> objects = {begin(listObjs), end(listObjs)};

    MeshObject* chair;
    MeshObject* support;
    MeshObject* wheel;

    // reconhecimento dos obejtos
    for(unsigned int i = 0; i < objects.size(); i++) {
        if(objects[i]->GetDescription() == "chair") {
            chair = objects[i];
        }
        if(objects[i]->GetDescription() == "support") {
            support = objects[i];
        }
        if(objects[i]->GetDescription() == "wheel") {
            wheel = objects[i];
        }
    }

    Transform Tchair;

    Tchair.AddChild(chair);

    scene.AddObject(Tchair);
    // scene.AddObject(&rotZterra);
    // scene.AddObject();

    MyIH idle;

    //Add a light to the scene.
    scene.AddLight( Light::BRIGHT_AMBIENT() );
    scene.AddCamera(&camera);
    scene.DrawLightsOGL();

    //Setup the viewer.
    viewer.SetTitle("Roda Gigante");
    viewer.SetScene(scene);
    viewer.SetIdleHandler(&idle);

    // viewer.SetIdleHandler(&idle);
    //Enable texture in OpenGL
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Enter the main loop
    VART::ViewerGlutOGL::MainLoop();
}
