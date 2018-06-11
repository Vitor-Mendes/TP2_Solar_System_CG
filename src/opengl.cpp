#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <random>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

#include "../include/RigidBody.h"
#include "../include/Mesh.h"
#include "CorpoCeleste.h"
#include "Camera.h"

using namespace std;

////////// ILUMINAÇÃO ////////////
static bool light0Ligada = true; // Luz branca ligada?
static float d = 1.0;            // Intensidade da cor difusa da luz branca
static float e = 1.0;            // Intensidade da cor especular da luz branca
static float m = 0.2;            // Intensidade da luz ambiente global
static float p = 1.0;            // A luz branca é posicional?
static float s = 50.0;           // Expoente especular do material (shininess)
float matShine[] = { s };                       // expoente especular (shininess)
static bool isLightingOn = false;               // O sistema de iluminação está ligado?
static bool localViewer = false;

//////////////////////////////////

extern const double DISTANCE_DIVIDER = 25;
extern const double SIZE_MULTIPLIER  = 15;

#define ORTHO_WIDTH  (orthoHalfWidth*2)
#define ORTHO_HEIGHT (orthoHalfHeight*2)

#define degreesToRadians(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define radiansToDegrees(angleRadians) ((angleRadians) * 180.0 / M_PI)

#define KEY_ESC 27

bool* keyStates = new bool[127];
bool* specialKeyStates = new bool[127];
bool drawOrbits = false,
     drawPlane  = false,
     trueSize   = true,
     slow       = false;

int windowWidth,
    windowHeight;

double aspectRatio = 1.0;

int cameraIndex = 0;

double angleX = 0.0,
       angleY = 0.0;

double x = 0.0, y = 0.0, z = 0.0;

double orthoHalfWidth, orthoHalfHeight;

Vector3d position(0.0, 0.0, 0.0);

Camera cameras[3];

CorpoCeleste sun(PlanetEnum::SUN()),
             mercury(PlanetEnum::MERCURY()),
             venus(PlanetEnum::VENUS()),
             earth(PlanetEnum::EARTH()),
                moon(PlanetEnum::MOON()),
                    rocket(PlanetEnum::ROCKET()),
                atmosphere(PlanetEnum::EARTH_ATMOSPHERE()),
             mars(PlanetEnum::MARS()),
             jupiter(PlanetEnum::JUPITER()),
             saturn(PlanetEnum::SATURN()),
                saturnRing(PlanetEnum::SATURN_RING()),
             uranus(PlanetEnum::URANUS()),
             neptune(PlanetEnum::NEPTUNE()),
             //pluto(PlanetEnum::PLUTO()),
             sky(PlanetEnum::STARS());

Mesh plane;

void tocar_musica(char const nome[40], int loop){
    Mix_Chunk *som = NULL;
    int canal;
    int canal_audio=2;
    int taxa_audio = 22050;
    Uint16 formato_audio = AUDIO_S16SYS;
    int audio_buffers = 4096;
    if(Mix_OpenAudio(taxa_audio, formato_audio, canal_audio, audio_buffers) != 0) {
        printf("Não pode inicializar audio: %s\n", Mix_GetError());
    }
    som = Mix_LoadWAV(nome);
    if(som == NULL) {
        printf("Não pode inicializar audio: %s\n", Mix_GetError());
    }
    Mix_HaltChannel(-1);
    canal = Mix_PlayChannel( -1, som, loop);
    if(canal == -1) {
        printf("Não pode inicializar audio: %s\n", Mix_GetError());
    }
}

void parar_musica(){
    Mix_HaltChannel(-1);
}

Vector3d getRealCoordinates(CorpoCeleste* body) {
    Vector3d vec, pos = body->getPosition(), parentPos(0.0, 0.0, 0.0);
    double angle = 0.0;
    if(body->getParent() != nullptr) {
        angle     += body->getParent()->getOrbitAngle();
        parentPos += getRealCoordinates(body->getParent());
    }

    int alpha, beta, gamma;
    if(body->getOrbitVector() == Vector3d(-1.0, 0.0, 0.0)) {
        alpha = Vector3d::Z;
        beta  = Vector3d::Y;
        gamma = Vector3d::X;
    }
    else if(body->getOrbitVector() == Vector3d(0.0, -1.0, 0.0)) {
        alpha = Vector3d::Y;
        beta  = Vector3d::X;
        gamma = Vector3d::Z;
    }
    else {
        alpha = Vector3d::X;
        beta  = Vector3d::Z;
        gamma = Vector3d::Y;
    }

    vec[alpha] = pos[alpha];
    vec[beta]  = (cos(degreesToRadians(body->getOrbitAngle() + angle))*(pos[beta])) - (sin(degreesToRadians(body->getOrbitAngle() + angle))*(pos[gamma]));
    vec[gamma] = (sin(degreesToRadians(body->getOrbitAngle() + angle))*(pos[beta])) + (cos(degreesToRadians(body->getOrbitAngle() + angle))*(pos[gamma]));

    return parentPos + vec;
}

void drawScene(void) {
    // Propriedades das fontes de luz
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDif0[] = { d, d, d, 1.0 };
    float lightSpec0[] = { e, e, e, 1.0 };
    float lightPos0[] = { 0.0, 0.0, 0.0, p };
    float globAmb[] = { m, m, m, 1.0 };

    // Propriedades da fonte de luz LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);// Enable local viewpoint

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    if (light0Ligada) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    //glMultMatrixf(rocket.getLastModelViewMatrix());
    cameras[cameraIndex].use();
    glRotated(angleX, 1.0, 0.0, 0.0);
    glRotated(angleY, 0.0, 1.0, 0.0);
    glCullFace(GL_FRONT);
    sky.draw(false, false);
    glCullFace(GL_BACK);
    glRotated(-angleX, 1.0, 0.0, 0.0);
    glRotated(-angleY, 0.0, 1.0, 0.0);

    glTranslated(x, y, z);
    glRotated(angleX, 1.0, 0.0, 0.0);
    glRotated(angleY, 0.0, 1.0, 0.0);
    glEnable(GL_LIGHTING);
    sun.draw(drawOrbits, trueSize);
    glDisable(GL_LIGHTING);

    if(drawPlane) {
        glDisable(GL_CULL_FACE);
        plane.draw();
        glEnable(GL_CULL_FACE);
    }

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Inicia algumas vari�veis de estado
void inicializa(void) {

    // cor para limpar a tela
    glClearColor(0, 0, 0.0, 0);
    glEnable(GL_DEPTH_TEST);

    //anti-aliasing
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    // Propriedades do material da esfera
    float matAmbAndDif[] = { 1.0, 1.0, 1.0, 1.0 };    // cor ambiente e difusa: branca
    float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };       // cor especular: branca
    float matEmi[] = { 0.0, 0.0 , 0.0, 0.0 };

    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmi);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Não mostrar faces do lado de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Esconder o ponteiro do mouse quando dentro da janela
    glutSetCursor(GLUT_CURSOR_NONE);

    sun.loadFromFile("../src/objects/Sphere.obj");
    mercury.loadFromFile("../src/objects/Sphere.obj");
    venus.loadFromFile("../src/objects/Sphere.obj");
    earth.loadFromFile("../src/objects/Sphere.obj");
        moon.loadFromFile("../src/objects/Sphere.obj");
        atmosphere.loadFromFile("../src/objects/Sphere.obj");
    mars.loadFromFile("../src/objects/Sphere.obj");
    sky.loadFromFile("../src/objects/Sphere.obj");
    jupiter.loadFromFile("../src/objects/Sphere.obj");
    saturn.loadFromFile("../src/objects/Sphere.obj");
        saturnRing.loadFromFile("../src/objects/Ring.obj");
    uranus.loadFromFile("../src/objects/Sphere.obj");
    neptune.loadFromFile("../src/objects/Sphere.obj");
    rocket.loadFromFile("../src/objects/Rocket.obj");
    //pluto.loadFromFile("../src/objects/Earth3.obj");

    plane.loadFromFile("../src/objects/OrbitalPlane.obj");
    plane.loadTexture("../src/images/OrbitalPlane.jpg");
    plane.scale(PlanetEnum::NEPTUNE().getDistance());

    matEmi[0]=matEmi[1]=matEmi[2]=matEmi[3]=20.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmi);
    sun.loadTexture();
    matEmi[0]=matEmi[1]=matEmi[2]=matEmi[3]=0.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmi);

    sun.loadTexture();

    mercury.setOrbitAngle(0.0);
    mercury.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    mercury.loadTexture();

    venus.setOrbitAngle(0.0);
    venus.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    venus.loadTexture();

    earth.setOrbitAngle(0.0);
    earth.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    moon.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    earth.loadTexture();
    moon.loadTexture();
    rocket.loadTexture();
    rocket.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    moon.pushChildren(&rocket);
    atmosphere.loadTexture();
    earth.pushChildren(&moon);
    earth.pushChildren(&atmosphere);

    mars.setOrbitAngle(0.0);
    mars.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    mars.loadTexture();

    jupiter.loadTexture();
    jupiter.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    saturn.loadTexture();
    saturn.setOrbitVector(Vector3d(0.0, -1.0, 0.0));
    saturnRing.loadTexture();
    saturn.pushChildren(&saturnRing);

    uranus.loadTexture();
    uranus.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    neptune.loadTexture();
    neptune.setOrbitVector(Vector3d(0.0, -1.0, 0.0));

    sky.translate(Vector3d(0.0, 0.0, -100));
    sky.loadTexture();


    cameras[0] = Camera(0.0, PlanetEnum::NEPTUNE().getDistance(), 0.0,
                        0.0, 0.0, 0.0,
                        0.0, 0.0, -1.0
    );

    cameras[1] = Camera(0.0, PlanetEnum::MARS().getDistance(), PlanetEnum::MARS().getDistance(),
                        0.0, 0.0, 0.0,
                        0.0, 1.0, -1.0
    );

    cameras[2] = Camera(0.0, 0.0, 0.0, 10.0, 10.0, 10.0, 0.0, 1.0, 0.0);
    cameras[2].setPosition(getRealCoordinates(&rocket));
    cameras[2].setTarget(getRealCoordinates(&moon));

    sun.pushChildren(&mercury);
    sun.pushChildren(&venus);
    sun.pushChildren(&earth);
    sun.pushChildren(&mars);
    sun.pushChildren(&jupiter);
    sun.pushChildren(&saturn);
    sun.pushChildren(&uranus);
    sun.pushChildren(&neptune);

    glPointSize(4);
    glLineWidth(5);
    glPolygonMode(GL_FRONT, GL_FILL);

    tocar_musica("Ambient-Space-Music-Shooting-Stars.ogg", 1);
}

// Callback de redimensionamento
void resizeScreen(int w, int h) {
    aspectRatio = ((double)w)/h;
    double worldAspectRatio = ((ORTHO_WIDTH)/(ORTHO_HEIGHT));
    if (aspectRatio < worldAspectRatio) {
        double viewportHeight = w / worldAspectRatio;
        double viewportY = (h - viewportHeight)/2;
        glViewport(0, viewportY, w, viewportHeight);
    }
    else if (aspectRatio > worldAspectRatio) {
        double viewportWidth = (h) * worldAspectRatio;
        double viewportX = (w - viewportWidth)/2;
        glViewport(viewportX, 0, viewportWidth, h);
    } else {
        glViewport(0, 0, w, h);
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (float)w/(float)h, 1.0, 20000.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboardHandle() {
    Vector3d cameraValues(0.0, 0.0, 0.0);
    if (cameraIndex != 2) {
        if (keyStates['d']) {
            x += 1.0;
        }
        if (keyStates['a']) {
            x -= 1.0;
        }
        if (keyStates['w']) {
            z += 1.0;
        }
        if (keyStates['s']) {
            z -= 1.0;
        }
        if (keyStates['[']) {
            y += 1.0;
        }
        if (keyStates[']']) {
            y -= 1.0;
        }
        if (keyStates['+']) {
            angleX += 1.0;
        }
        if (keyStates['-']) {
            angleX -= 1.0;
        }
        if (keyStates['q']) {
            angleY -= 1.0;
        }
        if (keyStates['e']) {
            angleY += 1.0;
        }
    }
    if(specialKeyStates[GLUT_KEY_UP]) {
        cameraValues[Y] += 1.0;
    }
    if(specialKeyStates[GLUT_KEY_DOWN]) {
        cameraValues[Y] -= 1.0;
    }
    if(specialKeyStates[GLUT_KEY_LEFT]) {
        cameraValues[X] -= 1.0;
    }
    if(specialKeyStates[GLUT_KEY_RIGHT]) {
        cameraValues[X] += 1.0;
    }

    if(cameraIndex > 0) cameras[cameraIndex].addToTarget(cameraValues);

    glutPostRedisplay();
}

void atualiza(int time) {
    glutTimerFunc(time, atualiza, time);
    keyboardHandle();

    sun.atualiza(slow);

    cameras[2].setPosition(getRealCoordinates(&rocket));
    cameras[2].setTarget(getRealCoordinates(&moon));
}


void keyDown(unsigned char key, int xpos, int ypos) {
    keyStates[key] = true;

    if(key == KEY_ESC) {
        exit(0);
    }
    if(keyStates['o'])
        drawOrbits = !drawOrbits;
    if(keyStates['p']) drawPlane  = !drawPlane;
    if(keyStates['c']) {
        if(++cameraIndex > 2) cameraIndex = 0;
        angleX = angleY = x = y = z = 0.0;
    }

    if(keyStates['f']) trueSize = !trueSize;
    if(keyStates['r']) slow     = !slow;
}

void specialKeyDown(int key, int x, int y) {
    specialKeyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void specialKeyUp(int key, int x, int y) {
    specialKeyStates[key] = false;
}

// Rotina principal
int main(int argc, char **argv) {
    // Acordando o GLUT
    glutInit(&argc, argv);

    // Definindo a vers�o do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    windowWidth  = glutGet(GLUT_SCREEN_WIDTH);
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Configura��o inicial da janela do GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);

    // Abre a janela
    glutCreateWindow("GameBoilerplate");
    glutFullScreen();

    // Registra callbacks para alguns eventos
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resizeScreen);

    // Keyboard up and down callbacks
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    // Special keyboard up and down callbacks
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);

    inicializa();
    glutTimerFunc(0, atualiza, 17);

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
