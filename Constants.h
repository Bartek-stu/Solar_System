#ifndef SOLAR_SYSTEM_CONSTANTS_H
#define SOLAR_SYSTEM_CONSTANTS_H
#include <iostream>
#include <GLFW/glfw3.h>
#include <tuple>

constexpr std::size_t n = 90; // defines quality (number of triangles of a single circle)
constexpr std::size_t numberOfPlanets = 9;
constexpr std::size_t numberOfRings = 5;
constexpr std::size_t verticesSize = (2 * numberOfPlanets - 1 + numberOfRings) * ((n + 1) * 5);
constexpr std::size_t indicesSize = numberOfPlanets * n * 3 + (numberOfPlanets - 1 + numberOfRings) * n;
constexpr GLfloat planetRadius[9] {696.34 * 1e3, 2439.7, 6051.8, 6371,
                                   3389.5, 69911, 58232, 25362, 24622};
constexpr GLfloat scaledRadius[9] {0.05,
                                   2.f * planetRadius[1] / planetRadius[0],
                                   2.f * planetRadius[2] / planetRadius[0],
                                   2.f * planetRadius[3] / planetRadius[0],
                                   2.f * planetRadius[4] / planetRadius[0],
                                   0.4f * planetRadius[5] / planetRadius[0],
                                   0.4f * planetRadius[6] / planetRadius[0],
                                   0.4f * planetRadius[7] / planetRadius[0],
                                   0.4f * planetRadius[8] / planetRadius[0]};

constexpr GLfloat positions[9] {0, 0.05, 0.09, 0.13, 0.17, 0.21, 0.25, 0.29, 0.32};
constexpr std::tuple<GLfloat, GLfloat, GLfloat> planetColor[9] {std::make_tuple(0.956f, 1.f, 0.043f), std::make_tuple(0.592f, 0.592f, 0.592f),
                                                                std::make_tuple(0.596f, 0.627f, 0.f), std::make_tuple(0.f, 0.282f, 0.772f),
                                                                std::make_tuple(0.807f, 0.035f, 0.01f), std::make_tuple(0.87f, 0.419f, 0.f),
                                                                std::make_tuple(0.76f, 0.533f, 0.317f), std::make_tuple(0.298f, 0.854f, 0.796f),
                                                                std::make_tuple(0.105f, 0.f, 1.f),
};
constexpr double planetSpeed[9] {0, 1, 0.345, 0.241, 0.128, 0.02, 0.0083, 0.0028, 0.00146};
constexpr double speed_scale = 0.3;

constexpr GLfloat a_ellipse= 3; // a parameter of the ellipse
constexpr GLfloat b_ellipse = 2; //b parameter of the ellipse

#endif //SOLAR_SYSTEM_CONSTANTS_H
