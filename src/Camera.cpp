#include <GL/glu.h>
#include "Camera.h"

void Camera::use() {
    gluLookAt(position[Vector3d::X], position[Vector3d::Y], position[Vector3d::Z],
              target[Vector3d::X],   target[Vector3d::Y],   target[Vector3d::Z],
              up[Vector3d::X],       up[Vector3d::Y],       up[Vector3d::Z]);
}

void Camera::use(double aspectRatio) {
    gluLookAt(position[Vector3d::X], position[Vector3d::Y]*aspectRatio, position[Vector3d::Z],
              target[Vector3d::X],   target[Vector3d::Y],   target[Vector3d::Z],
              up[Vector3d::X],       up[Vector3d::Y],       up[Vector3d::Z]);
}

const Vector3d &Camera::getPosition() const {
    return position;
}

void Camera::setPosition(const Vector3d &position) {
    Camera::position = position;
}

void Camera::addToPosition(const Vector3d &position) {
    Camera::position += position;
}

const Vector3d &Camera::getTarget() const {
    return target;
}

void Camera::setTarget(const Vector3d &target) {
    Camera::target = target;
}

void Camera::addToTarget(const Vector3d &target) {
    Camera::target += target;
}

const Vector3d &Camera::getUp() const {
    return up;
}

void Camera::setUp(const Vector3d &up) {
    Camera::up = up;
}
