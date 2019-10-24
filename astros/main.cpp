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

	//Cria o sol a terra e a lua
	Sphere sol(1.0);
	Sphere terra(0.5);
	Sphere lua(0.1);

    Texture texturaSol, texturaTerra, texturaLua;
    Material matSol, matTerra, matLua;
    Transform terraTr, solTr, luaTr;
    Transform rotZterra, rotZterraSi, rotZlua;

	//Carrega e aplica as texturas
	if(texturaSol.LoadFromFile(argv[1]) && texturaTerra.LoadFromFile(argv[2])  && texturaLua.LoadFromFile(argv[3])){
		matSol.SetTexture(texturaSol);
		matTerra.SetTexture(texturaTerra);
		matLua.SetTexture(texturaLua);
	}else{
		cout << "Não foi possível ler o arquivo de textura.";
		return 0;
	}

    //Aplica os materiais
	sol.SetMaterial(matSol);
	terra.SetMaterial(matTerra);
 	lua.SetMaterial(matLua);

    solTr.AddChild(sol);
    solTr.MakeTranslation(0, 0, 0);


    rotZterraSi.AddChild(terra);
    terraTr.AddChild(rotZterraSi);
    terraTr.MakeTranslation(5, 0, 0);
    rotZterra.AddChild(terraTr);

    luaTr.AddChild(lua);
    luaTr.MakeTranslation(1, 0, 0);
    rotZlua.AddChild(luaTr);
    terraTr.AddChild(rotZlua);
    
    // terraTr.AddChild(rotZterra);
    // rotZterra.AddChild(terra);

    scene.AddObject(&solTr);
    scene.AddObject(&rotZterra);
    // scene.AddObject();

    MyIH idle;
    idle.rotZterra = &rotZterra;
    idle.rotZterraSi = &rotZterraSi;
    idle.rotZlua = &rotZlua;

    //Add a light to the scene.
    scene.AddLight( Light::BRIGHT_AMBIENT() );
    scene.AddCamera(&camera);
    scene.DrawLightsOGL();

    //Setup the viewer.
    viewer.SetTitle("Sistema Solar");
    viewer.SetScene(scene);
    viewer.SetIdleHandler(&idle);

    // viewer.SetIdleHandler(&idle);
    //Enable texture in OpenGL
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Enter the main loop
    VART::ViewerGlutOGL::MainLoop();
}
