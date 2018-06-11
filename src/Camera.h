#ifndef TP2_CAMERA_H
#define TP2_CAMERA_H


#include "../include/Vector3.h"

class Camera {
private:
    Vector3d position, target, up;
public:
    Camera() : position(0.0, 0.0, 0.0), target(0.0, 0.0, 0.0), up(0.0, 0.0, 0.0) {}
    Camera(double positionX, double positionY, double positionZ,
           double targetX,   double targetY,   double targetZ,
           double upX,       double upY,       double upZ) : position(positionX, positionY, positionZ), target(targetX, targetY, targetZ), up(upX, upY, upZ) {}

    void use();

    void use(double aspectRatio);

    const Vector3d &getPosition() const;

    void setPosition(const Vector3d &position);
    void addToPosition(const Vector3d &position);

    const Vector3d &getTarget() const;

    void setTarget(const Vector3d &target);
    void addToTarget(const Vector3d &target);

    const Vector3d &getUp() const;

    void setUp(const Vector3d &up);
};


#endif //TP2_CAMERA_H
