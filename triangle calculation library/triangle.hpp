
#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace triangle {

constexpr double PI = 3.14159265358979323846;

struct Point {
    double x;
    double y;
};

class Triangle {
private:
    double a;
    double b;
    double c;

public:
    Triangle(double sideA, double sideB, double sideC);

    double getA() const;
    double getB() const;
    double getC() const;

    double perimeter() const;
    double semiperimeter() const;
    double area() const;

    double angleA() const;
    double angleB() const;
    double angleC() const;

    double angleARadians() const;
    double angleBRadians() const;
    double angleCRadians() const;

    double heightA() const;
    double heightB() const;
    double heightC() const;

    double medianA() const;
    double medianB() const;
    double medianC() const;

    double bisectorA() const;
    double bisectorB() const;
    double bisectorC() const;

    double inradius() const;
    double circumradius() const;

    double exradiusA() const;
    double exradiusB() const;
    double exradiusC() const;

    double sinA() const;
    double sinB() const;
    double sinC() const;

    double cosA() const;
    double cosB() const;
    double cosC() const;

    double tanA() const;
    double tanB() const;
    double tanC() const;

    Point vertexA() const;
    Point vertexB() const;
    Point vertexC() const;

    Point centroid() const;
    Point incenter() const;
    Point circumcenter() const;
    Point orthocenter() const;
    Point ninePointCenter() const;

    double eulerDistance() const;

    bool isEquilateral() const;
    bool isIsosceles() const;
    bool isScalene() const;
    bool isRight() const;
    bool isAcute() const;
    bool isObtuse() const;

    std::string type() const;

    void draw(int width = 70, int height = 25) const;
};

}

#endif
