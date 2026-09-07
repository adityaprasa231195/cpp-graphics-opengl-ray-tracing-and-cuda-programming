
#include <iostream>
#include <iomanip>
#include <exception>
#include "triangle.hpp"

using namespace std;
using namespace triangle;

Triangle::Triangle(double sideA, double sideB, double sideC)
    : a(sideA), b(sideB), c(sideC) {
    if (a <= 0 || b <= 0 || c <= 0)
        throw invalid_argument("All sides must be greater than zero.");

    if (a + b <= c || a + c <= b || b + c <= a)
        throw invalid_argument("The three sides cannot form a triangle.");
}

double Triangle::getA() const {
    return a;
}

double Triangle::getB() const {
    return b;
}

double Triangle::getC() const {
    return c;
}

double Triangle::perimeter() const {
    return a + b + c;
}

double Triangle::semiperimeter() const {
    return perimeter() / 2.0;
}

double Triangle::area() const {
    double s = semiperimeter();
    return sqrt(max(0.0, s * (s - a) * (s - b) * (s - c)));
}

double Triangle::angleARadians() const {
    double x = (b * b + c * c - a * a) / (2.0 * b * c);
    x = max(-1.0, min(1.0, x));
    return acos(x);
}

double Triangle::angleBRadians() const {
    double x = (a * a + c * c - b * b) / (2.0 * a * c);
    x = max(-1.0, min(1.0, x));
    return acos(x);
}

double Triangle::angleCRadians() const {
    double x = (a * a + b * b - c * c) / (2.0 * a * b);
    x = max(-1.0, min(1.0, x));
    return acos(x);
}

double Triangle::angleA() const {
    return angleARadians() * 180.0 / PI;
}

double Triangle::angleB() const {
    return angleBRadians() * 180.0 / PI;
}

double Triangle::angleC() const {
    return angleCRadians() * 180.0 / PI;
}

double Triangle::heightA() const {
    return 2.0 * area() / a;
}

double Triangle::heightB() const {
    return 2.0 * area() / b;
}

double Triangle::heightC() const {
    return 2.0 * area() / c;
}

double Triangle::medianA() const {
    return 0.5 * sqrt(2.0 * b * b + 2.0 * c * c - a * a);
}

double Triangle::medianB() const {
    return 0.5 * sqrt(2.0 * a * a + 2.0 * c * c - b * b);
}

double Triangle::medianC() const {
    return 0.5 * sqrt(2.0 * a * a + 2.0 * b * b - c * c);
}

double Triangle::bisectorA() const {
    return sqrt(
        max(0.0,
            b * c *
            (1.0 - (a * a) / ((b + c) * (b + c)))
        )
    );
}

double Triangle::bisectorB() const {
    return sqrt(
        max(0.0,
            a * c *
            (1.0 - (b * b) / ((a + c) * (a + c)))
        )
    );
}

double Triangle::bisectorC() const {
    return sqrt(
        max(0.0,
            a * b *
            (1.0 - (c * c) / ((a + b) * (a + b)))
        )
    );
}

double Triangle::inradius() const {
    return area() / semiperimeter();
}

double Triangle::circumradius() const {
    return (a * b * c) / (4.0 * area());
}

double Triangle::exradiusA() const {
    return area() / (semiperimeter() - a);
}

double Triangle::exradiusB() const {
    return area() / (semiperimeter() - b);
}

double Triangle::exradiusC() const {
    return area() / (semiperimeter() - c);
}

double Triangle::sinA() const {
    return sin(angleARadians());
}

double Triangle::sinB() const {
    return sin(angleBRadians());
}

double Triangle::sinC() const {
    return sin(angleCRadians());
}

double Triangle::cosA() const {
    return cos(angleARadians());
}

double Triangle::cosB() const {
    return cos(angleBRadians());
}

double Triangle::cosC() const {
    return cos(angleCRadians());
}

double Triangle::tanA() const {
    return tan(angleARadians());
}

double Triangle::tanB() const {
    return tan(angleBRadians());
}

double Triangle::tanC() const {
    return tan(angleCRadians());
}

Point Triangle::vertexA() const {
    return {0.0, 0.0};
}

Point Triangle::vertexB() const {
    return {c, 0.0};
}

Point Triangle::vertexC() const {
    double x = (b * b + c * c - a * a) / (2.0 * c);
    double y = sqrt(max(0.0, b * b - x * x));
    return {x, y};
}

Point Triangle::centroid() const {
    Point A = vertexA();
    Point B = vertexB();
    Point C = vertexC();

    return {
        (A.x + B.x + C.x) / 3.0,
        (A.y + B.y + C.y) / 3.0
    };
}

Point Triangle::incenter() const {
    Point A = vertexA();
    Point B = vertexB();
    Point C = vertexC();

    double p = perimeter();

    return {
        (a * A.x + b * B.x + c * C.x) / p,
        (a * A.y + b * B.y + c * C.y) / p
    };
}

Point Triangle::circumcenter() const {
    Point A = vertexA();
    Point B = vertexB();
    Point C = vertexC();

    double d =
        2.0 *
        (A.x * (B.y - C.y) +
         B.x * (C.y - A.y) +
         C.x * (A.y - B.y));

    if (fabs(d) < 1e-12)
        throw runtime_error("Cannot calculate circumcenter.");

    double ux =
        ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
         (B.x * B.x + B.y * B.y) * (C.y - A.y) +
         (C.x * C.x + C.y * C.y) * (A.y - B.y)) / d;

    double uy =
        ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
         (B.x * B.x + B.y * B.y) * (A.x - C.x) +
         (C.x * C.x + C.y * C.y) * (B.x - A.x)) / d;

    return {ux, uy};
}

Point Triangle::orthocenter() const {
    Point G = centroid();
    Point O = circumcenter();

    return {
        3.0 * G.x - 2.0 * O.x,
        3.0 * G.y - 2.0 * O.y
    };
}

Point Triangle::ninePointCenter() const {
    Point O = circumcenter();
    Point H = orthocenter();

    return {
        (O.x + H.x) / 2.0,
        (O.y + H.y) / 2.0
    };
}

double Triangle::eulerDistance() const {
    Point O = circumcenter();
    Point H = orthocenter();

    double dx = H.x - O.x;
    double dy = H.y - O.y;

    return sqrt(dx * dx + dy * dy);
}

bool Triangle::isEquilateral() const {
    const double e = 1e-9;

    return fabs(a - b) < e &&
           fabs(b - c) < e;
}

bool Triangle::isIsosceles() const {
    const double e = 1e-9;

    return fabs(a - b) < e ||
           fabs(b - c) < e ||
           fabs(a - c) < e;
}

bool Triangle::isScalene() const {
    return !isIsosceles();
}

bool Triangle::isRight() const {
    double x = a;
    double y = b;
    double z = c;

    if (x > z) swap(x, z);
    if (y > z) swap(y, z);

    return fabs(x * x + y * y - z * z) < 1e-9;
}

bool Triangle::isAcute() const {
    double x = a;
    double y = b;
    double z = c;

    if (x > z) swap(x, z);
    if (y > z) swap(y, z);

    return x * x + y * y > z * z;
}

bool Triangle::isObtuse() const {
    double x = a;
    double y = b;
    double z = c;

    if (x > z) swap(x, z);
    if (y > z) swap(y, z);

    return x * x + y * y < z * z;
}

string Triangle::type() const {
    if (isEquilateral())
        return "Equilateral";

    if (isRight() && isIsosceles())
        return "Right Isosceles";

    if (isRight())
        return "Right Scalene";

    if (isAcute() && isIsosceles())
        return "Acute Isosceles";

    if (isAcute())
        return "Acute Scalene";

    if (isObtuse() && isIsosceles())
        return "Obtuse Isosceles";

    return "Obtuse Scalene";
}

void Triangle::draw(int width, int height) const {
    Point A = vertexA();
    Point B = vertexB();
    Point C = vertexC();

    double minX = min({A.x, B.x, C.x});
    double maxX = max({A.x, B.x, C.x});
    double minY = min({A.y, B.y, C.y});
    double maxY = max({A.y, B.y, C.y});

    double rangeX = maxX - minX;
    double rangeY = maxY - minY;

    if (rangeX <= 0.0)
        rangeX = 1.0;

    if (rangeY <= 0.0)
        rangeY = 1.0;

    vector<string> screen(height, string(width, ' '));

    auto convertX = [&](double x) {
        return static_cast<int>(
            round((x - minX) / rangeX * (width - 1))
        );
    };

    auto convertY = [&](double y) {
        return height - 1 -
               static_cast<int>(
                   round((y - minY) / rangeY * (height - 1))
               );
    };

    auto drawLine = [&](Point P, Point Q) {
        int x1 = convertX(P.x);
        int y1 = convertY(P.y);
        int x2 = convertX(Q.x);
        int y2 = convertY(Q.y);

        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int steps = max(dx, dy);

        for (int i = 0; i <= steps; ++i) {
            double t = steps == 0 ? 0.0 :
                       static_cast<double>(i) / steps;

            int x = static_cast<int>(round(
                x1 + (x2 - x1) * t
            ));

            int y = static_cast<int>(round(
                y1 + (y2 - y1) * t
            ));

            if (x >= 0 && x < width &&
                y >= 0 && y < height) {
                screen[y][x] = '*';
            }
        }
    };

    drawLine(A, B);
    drawLine(B, C);
    drawLine(C, A);

    int ax = convertX(A.x);
    int ay = convertY(A.y);

    int bx = convertX(B.x);
    int by = convertY(B.y);

    int cx = convertX(C.x);
    int cy = convertY(C.y);

    if (ax >= 0 && ax < width && ay >= 0 && ay < height)
        screen[ay][ax] = 'A';

    if (bx >= 0 && bx < width && by >= 0 && by < height)
        screen[by][bx] = 'B';

    if (cx >= 0 && cx < width && cy >= 0 && cy < height)
        screen[cy][cx] = 'C';

    cout << "\n";
    cout << "============================================\n";
    cout << "               TRIANGLE DRAWING\n";
    cout << "============================================\n\n";

    for (const string& line : screen)
        cout << line << '\n';

    cout << "\nA = " << a << "\n";
    cout << "B = " << b << "\n";
    cout << "C = " << c << "\n";
}

int main() {
    double a;
    double b;
    double c;

    cout << "============================================\n";
    cout << "        TRIANGLE MATHEMATICS LIBRARY\n";
    cout << "============================================\n\n";

    cout << "Enter side A: ";
    cin >> a;

    cout << "Enter side B: ";
    cin >> b;

    cout << "Enter side C: ";
    cin >> c;

    if (cin.fail()) {
        cout << "\nInvalid input.\n";
        return 1;
    }

    try {
        Triangle t(a, b, c);

        cout << fixed << setprecision(6);

        cout << "\n============================================\n";
        cout << "               RESULTS\n";
        cout << "============================================\n";

        cout << "\nSides\n";
        cout << "A = " << t.getA() << '\n';
        cout << "B = " << t.getB() << '\n';
        cout << "C = " << t.getC() << '\n';

        cout << "\nBasic Properties\n";
        cout << "Perimeter = " << t.perimeter() << '\n';
        cout << "Semiperimeter = " << t.semiperimeter() << '\n';
        cout << "Area = " << t.area() << '\n';

        cout << "\nAngles\n";
        cout << "Angle A = " << t.angleA() << " degrees\n";
        cout << "Angle B = " << t.angleB() << " degrees\n";
        cout << "Angle C = " << t.angleC() << " degrees\n";

        cout << "\nHeights\n";
        cout << "Height A = " << t.heightA() << '\n';
        cout << "Height B = " << t.heightB() << '\n';
        cout << "Height C = " << t.heightC() << '\n';

        cout << "\nMedians\n";
        cout << "Median A = " << t.medianA() << '\n';
        cout << "Median B = " << t.medianB() << '\n';
        cout << "Median C = " << t.medianC() << '\n';

        cout << "\nAngle Bisectors\n";
        cout << "Bisector A = " << t.bisectorA() << '\n';
        cout << "Bisector B = " << t.bisectorB() << '\n';
        cout << "Bisector C = " << t.bisectorC() << '\n';

        cout << "\nRadii\n";
        cout << "Inradius = " << t.inradius() << '\n';
        cout << "Circumradius = " << t.circumradius() << '\n';
        cout << "Exradius A = " << t.exradiusA() << '\n';
        cout << "Exradius B = " << t.exradiusB() << '\n';
        cout << "Exradius C = " << t.exradiusC() << '\n';

        Point G = t.centroid();
        Point I = t.incenter();
        Point O = t.circumcenter();
        Point H = t.orthocenter();
        Point N = t.ninePointCenter();

        cout << "\nTriangle Centers\n";
        cout << "Centroid = (" << G.x << ", " << G.y << ")\n";
        cout << "Incenter = (" << I.x << ", " << I.y << ")\n";
        cout << "Circumcenter = (" << O.x << ", " << O.y << ")\n";
        cout << "Orthocenter = (" << H.x << ", " << H.y << ")\n";
        cout << "Nine-point Center = (" << N.x << ", " << N.y << ")\n";

        cout << "\nEuler Relation\n";
        cout << "Distance O-H = " << t.eulerDistance() << '\n';

        cout << "\nTrigonometric Values\n";
        cout << "sin(A) = " << t.sinA() << '\n';
        cout << "sin(B) = " << t.sinB() << '\n';
        cout << "sin(C) = " << t.sinC() << '\n';

        cout << "cos(A) = " << t.cosA() << '\n';
        cout << "cos(B) = " << t.cosB() << '\n';
        cout << "cos(C) = " << t.cosC() << '\n';

        cout << "tan(A) = " << t.tanA() << '\n';
        cout << "tan(B) = " << t.tanB() << '\n';
        cout << "tan(C) = " << t.tanC() << '\n';

        cout << "\nClassification\n";
        cout << "Triangle Type = " << t.type() << '\n';

        t.draw();
    }
    catch (const exception& e) {
        cout << "\nError: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
