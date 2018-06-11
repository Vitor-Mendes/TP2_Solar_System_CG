#include <iostream>
#include "CorpoCeleste.h"

void CorpoCeleste::draw(bool orbits, bool trueSize) {
    glColor3f(1.0, 1.0, 1.0);

    if(!cullFace) glDisable(GL_CULL_FACE);
    glPushMatrix();
        // Movimento de translação (Rotação em volta do pai)
        glRotated(orbitAngle, orbitVector[X], orbitVector[Y], orbitVector[Z]);
        glPushMatrix();
            //Posição do astro em relação ao sol
            if(orbits && orbitVector.getNorm() > 0) {
                glDisable(GL_LIGHTING);
                drawOrbit();
                glEnable(GL_LIGHTING);
            }
            glTranslated(position[X], position[Y], position[Z]);

            //Escala o astro para o seu tamanho relativo ao sol
            glPushMatrix();
                if(trueSize || parent == nullptr)
                    glScaled(size, size, size);
                else
                    glScaled(size*SIZE_MULTIPLIER, size*SIZE_MULTIPLIER, size*SIZE_MULTIPLIER);

                //Rotação no eixo X
                glRotated(rotation[X], 1.0, 0.0, 0.0);

                //Rotação no eixo Z
                //glRotated(rotation[Z]-23.439, 0.0, 0.0, 1.0);

                //Rotação no eixo Y (em torno de si mesmo)
                glRotated(-rotation[Y], 0.0, 1.0, 0.0);

                glGetFloatv (GL_MODELVIEW_MATRIX, lastModelViewMatrix);
                if(parent == nullptr) {
                    glDisable(GL_LIGHTING);
                    Mesh::simpleDraw();
                    glEnable(GL_LIGHTING);
                }
                else Mesh::simpleDraw();

            glPopMatrix();

            if(!childrens.empty()) {
                //glRotated(-orbitAngle, orbitVector[X], orbitVector[Y], orbitVector[Z]);
                for(CorpoCeleste* children : childrens)
                    children->draw(orbits, trueSize);
            }
        glPopMatrix();
    glPopMatrix();
    glEnable(GL_CULL_FACE);
}

int CorpoCeleste::getTextureId() const {
    return textureId;
}

void CorpoCeleste::setTextureId(int textureId) {
    CorpoCeleste::textureId = textureId;
}

double CorpoCeleste::getOrbitAngle() const {
    return orbitAngle;
}

void CorpoCeleste::setOrbitAngle(double orbitAngle) {
    CorpoCeleste::orbitAngle = orbitAngle;
}

const Vector3d &CorpoCeleste::getRotation() const {
    return rotation;
}

void CorpoCeleste::setRotation(const Vector3d &rotation) {
    CorpoCeleste::rotation = rotation;
}

const Vector3d &CorpoCeleste::getOrbitVector() const {
    return orbitVector;
}

void CorpoCeleste::setOrbitVector(const Vector3d &orbitVector) {
    CorpoCeleste::orbitVector = orbitVector;
}

double CorpoCeleste::getSize() const {
    return size;
}

void CorpoCeleste::setSize(double size) {
    CorpoCeleste::size = size;
}

void CorpoCeleste::pushChildren(CorpoCeleste* children) {
    children->setParent(this);
    this->childrens.push_back(children);
}

void CorpoCeleste::atualiza(bool slow) {
    double rotationAngle = this->rotationAngle * (slow ? 0.05 : 1);
    double orbitStep     = this->orbitStep * (slow ? 0.05 : 1);
    if((rotation[Y] += rotationAngle) >= 360.0) rotation[Y] -= 360.0;
    if((orbitAngle += orbitStep) >= 360) orbitAngle -= 360.0;

    for(auto& children : childrens)
        children->atualiza(slow);
}

void CorpoCeleste::drawOrbit() {
    glDisable(GL_CULL_FACE);

    glPushMatrix();
        glScaled(this->position[X], this->size*10, this->position[X]);
        orbit.simpleDraw();
    glPopMatrix();

    glEnable(GL_CULL_FACE);
}

double CorpoCeleste::getOrbitStep() const {
    return orbitStep;
}

void CorpoCeleste::setOrbitStep(double orbitStep) {
    CorpoCeleste::orbitStep = orbitStep;
}

const GLfloat *CorpoCeleste::getLastModelViewMatrix() const {
    return lastModelViewMatrix;
}

void CorpoCeleste::loadTexture() {
    Mesh::loadTexture(texturePath);
}

CorpoCeleste *CorpoCeleste::getParent() const {
    return parent;
}

void CorpoCeleste::setParent(CorpoCeleste *parent) {
    CorpoCeleste::parent = parent;
}
