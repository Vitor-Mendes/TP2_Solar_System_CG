#include "PlanetEnum.h"

const PlanetEnum PlanetEnum::SUN() { static PlanetEnum ans(SOLAR_RADIUS, 0, 0.0, EARTH_ROTATION()/24.47, "../src/images/Sun.jpg", true); return ans;};

const PlanetEnum PlanetEnum::MERCURY() { static PlanetEnum ans(0.003507796*SOLAR_RADIUS,   83.25*SOLAR_RADIUS, EARTH_TRANSLATION()*4.150, (EARTH_ROTATION()/58.7),   "../src/images/Mercury.jpg", true);   return ans;};
const PlanetEnum PlanetEnum::VENUS()   { static PlanetEnum ans(0.008701553*SOLAR_RADIUS,  155.57*SOLAR_RADIUS, EARTH_TRANSLATION()*1.630, EARTH_ROTATION()/243.0,  "../src/images/Venus.jpg", true);  return ans;};
const PlanetEnum PlanetEnum::EARTH()   { static PlanetEnum ans(0.009160211*SOLAR_RADIUS,   215.10*SOLAR_RADIUS, EARTH_TRANSLATION(), EARTH_ROTATION(),  "../src/images/Earth.jpg", true);  return ans;};
    const PlanetEnum PlanetEnum::MOON()             { static PlanetEnum ans(0.002498174*SOLAR_RADIUS, (0.550*SOLAR_RADIUS*SIZE_MULTIPLIER), EARTH_TRANSLATION()*130.52, EARTH_ROTATION()/27, "../src/images/Moon.jpg", true);  return ans;};
        const PlanetEnum PlanetEnum::ROCKET()       { static PlanetEnum ans(0.000198174*SOLAR_RADIUS, (0.305*SOLAR_RADIUS*SIZE_MULTIPLIER), EARTH_TRANSLATION()*130.52, EARTH_ROTATION()/27, "../src/images/Spaceship.png", false);  return ans;};
    const PlanetEnum PlanetEnum::EARTH_ATMOSPHERE() { static PlanetEnum ans(0.009160211*SOLAR_RADIUS*1.01, 0, 0, -EARTH_ROTATION()*1.5, "../src/images/EarthClouds.png", true);  return ans;};
const PlanetEnum PlanetEnum::MARS()    { static PlanetEnum ans(0.004874135*SOLAR_RADIUS,  327.67*SOLAR_RADIUS, EARTH_TRANSLATION()*0.530, EARTH_ROTATION()/1.026,  "../src/images/Mars.jpg", true);  return ans;};
const PlanetEnum PlanetEnum::JUPITER() { static PlanetEnum ans(0.100517895*SOLAR_RADIUS/2.5, 1119.04*SOLAR_RADIUS/2, EARTH_TRANSLATION()*0.084, EARTH_ROTATION()/0.41, "../src/images/Jupiter.jpg", true); return ans;};
const PlanetEnum PlanetEnum::SATURN()  { static PlanetEnum ans(0.083725852*SOLAR_RADIUS/2.5, 2051.74*SOLAR_RADIUS/3, EARTH_TRANSLATION()*0.034, EARTH_ROTATION()/0.425, "../src/images/Saturn.jpg", true); return ans;};
    const PlanetEnum PlanetEnum::SATURN_RING()    { static PlanetEnum ans(0.083725852*SOLAR_RADIUS/2.5, 0, 0, 0, "../src/images/SaturnRing.png", true); return ans;};

const PlanetEnum PlanetEnum::URANUS()  { static PlanetEnum ans(0.036465432*SOLAR_RADIUS/1.3, 4127.92*SOLAR_RADIUS/5, EARTH_TRANSLATION()*0.012, EARTH_ROTATION()/0.75, "../src/images/Uranus.jpg", true); return ans;};
const PlanetEnum PlanetEnum::NEPTUNE() { static PlanetEnum ans(0.035401462*SOLAR_RADIUS/1.3, 6465.92*SOLAR_RADIUS/5, EARTH_TRANSLATION()*0.006, EARTH_ROTATION()/0.80, "../src/images/Neptune.jpg", true); return ans;};
//const PlanetEnum PlanetEnum::PLUTO()   { static PlanetEnum ans(0.000001635*SOLAR_RADIUS, 8501.70*SOLAR_RADIUS, "../src/images/Pluto.jp, true"); return ans;};

const PlanetEnum PlanetEnum::STARS()   { static PlanetEnum ans(800, 0, 0, 0, "../src/images/Stars-highres.jpg", true); return ans;};

const double PlanetEnum::getSize() const {
    return size;
}

const double PlanetEnum::getDistance() const {
    return distance / DISTANCE_DIVIDER;
}

const char *PlanetEnum::getTexturePath() const {
    return texturePath;
}

const double PlanetEnum::getRotationAngle() const {
    return rotationAngle;
}

const double PlanetEnum::getTranslationAngle() const {
    return translationAngle;
}

const bool PlanetEnum::getCullFace() const {
    return cullFace;
}
