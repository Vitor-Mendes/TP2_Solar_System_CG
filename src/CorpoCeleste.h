#ifndef TP2_CORPOCELESTE_H
#define TP2_CORPOCELESTE_H


#include "../include/RigidBody.h"
#include "../include/Mesh.h"
#include "PlanetEnum.h"
#include "Camera.h"

class CorpoCeleste : public Mesh {
    public:
        void draw(bool orbits, bool trueSize);

        void drawOrbit();

        CorpoCeleste(PlanetEnum planet) : size(planet.getSize()), texturePath(planet.getTexturePath()), rotationAngle(planet.getRotationAngle()), orbitStep(planet.getTranslationAngle()), orbitVector(0.0, 0.0, 0.0), cullFace(planet.getCullFace()), lastModelViewMatrix{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0}, parent(nullptr) {
            this->translate(Vector3d(-planet.getDistance(), 0.0, 0.0));
            orbit.loadFromFile("../src/objects/Orbit4.obj");
        }

    int getTextureId() const;

    void setTextureId(int textureId);

    double getOrbitAngle() const;

    void setOrbitAngle(double orbitAngle);

    const Vector3d &getRotation() const;

    void setRotation(const Vector3d &rotation);

    const Vector3d &getOrbitVector() const;

    void setOrbitVector(const Vector3d &orbitVector);
    void loadTexture();

    double getSize() const;

    void setSize(double size);

    void pushChildren(CorpoCeleste* children);

    void atualiza(bool slow);

    double getOrbitStep() const;

    void setOrbitStep(double orbitStep);

    const GLfloat *getLastModelViewMatrix() const;

    CorpoCeleste *getParent() const;

    void setParent(CorpoCeleste *parent);

private:
        bool cullFace;
        const char* texturePath;
        double rotationAngle, orbitAngle, orbitStep, size;
        Vector3d rotation, orbitVector;
        vector<CorpoCeleste*> childrens;
        CorpoCeleste* parent;
        Mesh orbit;
        GLfloat lastModelViewMatrix[16];
};


#endif //TP2_CORPOCELESTE_H
