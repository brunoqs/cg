#include <vart/scene.h>
#include <vart/light.h>
#include <vart/transform.h>
#include <vart/meshobject.h>
#include <vart/contrib/viewerglutogl.h>
#include <iostream>

using namespace std;
using namespace VART;

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,1.8,-9.5),VART::Point4D::ORIGIN(),VART::Point4D::Y());
    
    // Lendo os objetos
    list<MeshObject*> ruas;
    MeshObject::ReadFromOBJ("street.obj", &ruas);
    vector<MeshObject*> rua{begin(ruas), end(ruas)};
    MeshObject* street = rua[0];
    street->autoDelete = false;
    
    //Vetor com as Transformacoes da rua
    vector<Transform> transformacaoRua(3);
    
    // Rua1
    transformacaoRua[0].MakeIdentity();
    transformacaoRua[0].AddChild(*street);
        
    // Rua2
    transformacaoRua[1].MakeTranslation(0,0,16);
    transformacaoRua[1].AddChild(*street);
    
    // Rua3
    transformacaoRua[2].MakeTranslation(0,0,32);
    transformacaoRua[2].AddChild(*street);
    
    
    list<MeshObject*> postes;
    MeshObject::ReadFromOBJ("light-pole.obj", &postes);
    vector<MeshObject*> poste{begin(postes), end(postes)};
    cout << poste[0]->GetDescription() << endl;
    MeshObject* light_pole = poste[0];
    light_pole->autoDelete = false;
    
    // Vetor com as transformações dos postes
    vector<Transform> translacaoPoste(4);
    
    //Poste com as transformações base
    VART::Transform transformPoste;
    VART::Transform scalaPoste;
    scalaPoste.MakeScale(0.7,0.7,0.7);
    scalaPoste.AddChild(*light_pole);
    //O poste de iluminação foi modelado de forma que a altura está no eixo Z positivo e a origem está no centro da base do poste
    transformPoste.MakeXRotation(M_PI/2);
    transformPoste.AddChild(scalaPoste);
    
    //Esquerdo
    //poste1 (O inicio da rua é z = -8)
    translacaoPoste[0].MakeTranslation(-3.49, 0, 1);
    translacaoPoste[0].AddChild(transformPoste);
    
    //poste2 (poste1 ta posicionado 9m de distancia do inicio da rua, logo poste dois esta a 30m do poste1 e o poste2 esta a 9m do fim da rua
    translacaoPoste[1].MakeTranslation(-3.49, 0, 31);
    translacaoPoste[1].AddChild(transformPoste);
    
    //Transformação para girar o poste
    Transform rotacaoPoste;
    rotacaoPoste.MakeYRotation(M_PI);
    rotacaoPoste.AddChild(transformPoste);
    
    //Direito
    //poste3
    translacaoPoste[2].MakeTranslation(3.49,0,1);
    translacaoPoste[2].AddChild(rotacaoPoste);
    
    //poste4
    translacaoPoste[3].MakeTranslation(3.49,0,31);
    translacaoPoste[3].AddChild(rotacaoPoste);
    
    
    // Build up the scene
    scene.AddObject(&transformacaoRua[0]); //adicionada no ponto (0,0,0)
    scene.AddObject(&transformacaoRua[1]);
    scene.AddObject(&transformacaoRua[2]);
    scene.AddObject(&translacaoPoste[0]);
    scene.AddObject(&translacaoPoste[1]);
    scene.AddObject(&translacaoPoste[2]);
    scene.AddObject(&translacaoPoste[3]);

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    // Set up the viewer
    viewer.SetTitle("Rua");
    viewer.SetScene(scene); // attach the scene

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
