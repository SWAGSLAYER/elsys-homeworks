/*
    Homework:
    Implement the following figures:
    1. Ellipse #DONE
    2. Rectangle #DONE
    3. Line #DONE
    4. Polygon #DONE
    5. Polyline #DONE
    6. Path #DONE
    Deadline: 16.02.2016 17:00
    Place: https://github.com/egotsev/elsys-homeworks/11a/{number_in_class}/01
*/

#include <iostream>
#include <list>

using namespace std;

class Point {
    int x_, y_;

public:
    Point(int x=0, int y=0) : x_(x), y_(y) {}

    int get_x() const {
        return x_;
    }

    int get_y() const {
        return y_;
    }

    void draw() const {
        cout << x_ << "," << y_ << " ";
    }

    static void draw(list<Point> points_) {
        for (list<Point>::const_iterator it = points_.begin();
                it != points_.end(); it++) {
            it->draw();
        }
    }
};

class Shape {
public:
    virtual void draw() const=0;
};

class Circle : public Shape {
    int radius_;
    Point center_;

public:
    Circle(Point center, int radius): center_(center), radius_(radius) {}

    void draw() const {
        cout << "<circle cx=\"" << center_.get_x()
            << "\" cy=\"" << center_.get_y()
            << "\" r=\"" << radius_ << "\" />"
            << endl;
    }
};

class Ellipse : public Shape {
    Point center_;
    int rx_;
    int ry_;

public:
    Ellipse(Point center, int rx, int ry)
    : center_(center), rx_(rx), ry_(ry) {}

    void draw() const {
        cout << "<ellipse cx=\"" << center_.get_x() << "\""
            << " cy=\"" << center_.get_y() << "\""
            << " rx=\"" << rx_ << "\""
            << " ry=\"" << ry_ << "\" />"
            << endl;
    }
};

class Rectangle : public Shape {
    Point p_;
    int width_;
    int height_;

public:
    Rectangle(Point p, int width, int height)
    : p_(p), width_(width), height_(height) {}

    void draw() const {
        cout << "<rectangle x=\"" << p_.get_x() << "\""
            << " y=\"" << p_.get_y() << "\""
            << " width=\"" << width_ << "\""
            << " height=\"" << height_ << "\" />"
            << endl;
    }
};

class Line : public Shape {
    Point p1_;
    Point p2_;

public:
    Line(Point p1, Point p2)
    : p1_(p1), p2_(p2) {}

    void draw() const {
        cout << "<line x1=\"" << p1_.get_x() << "\""
            << " y1=\"" << p1_.get_y() << "\""
            << " x2=\"" << p2_.get_x() << "\""
            << " y2=\"" << p2_.get_y() << "\" />"
            << endl;
    }
};

class Polygon : public Shape {
protected:
    list<Point> points_;
public:
    Polygon(list<Point> points): points_(points) {}

    void draw() const {
        cout << "<polygon points=\"";
        Point::draw(points_);
        cout << "\" />" << endl;
    }
};

class Polyline : public Polygon {
public:
    Polyline(list<Point> points_): Polygon(points_) {}
    void draw() const {
        cout << "<polyline points=\"";
        Point::draw(points_);
        cout << "\" />" << endl;
    }
};

typedef pair <char, Point> PathPair;
class Path : public Shape {
    list<PathPair> definition_;

public:
    Path(list<PathPair> definition): definition_(definition) {}

    void draw() const {
        cout << "<path d=\"";
        for (list<PathPair>::const_iterator it = definition_.begin();
                it != definition_.end(); it++) {
            cout << it->first << " ";
            it->second.draw();
        }
        cout << "\" />" << endl;
    }
};

class CompositeFigure : public Shape {
    list<Shape*> content_;

public:
    ~CompositeFigure() {
        for (list<Shape*>::iterator it = content_.begin(); it != content_.end(); it++) {
            delete *it;
        }
    }

    void add(Shape* shape) {
        content_.push_back(shape);
    }

    void draw() const {
        for (list<Shape*>::const_iterator it = content_.begin(); it != content_.end(); it++) {
            (*it)->draw();
        }
    }
};

class Canvas : public CompositeFigure {
    int width_, height_;

public:
    Canvas(int width=100, int height=100) : width_(width), height_(height) {}

    void draw() const {
        cout << "<svg xmlns=\"https://www.w3.org/2000/svg\" width=\"" << width_
            << "\" height=\"" << height_
            << "\">" << endl;
        CompositeFigure::draw();
        cout << "</svg>" << endl;
    }

};


int main() {
    Canvas c(400, 600);

    c.add(new Circle(Point(20, 20), 15));
    c.add(new Ellipse(Point(50, 100), 10, 10));
    c.add(new Rectangle(Point(50, 20), 150, 150));
    c.add(new Line(Point(0, 0), Point(200, 200)));

    list<Point> polygon_points;
    polygon_points.push_back(Point(220, 10));
    polygon_points.push_back(Point(300, 210));
    polygon_points.push_back(Point(170, 250));
    c.add(new Polygon(polygon_points));

    list<Point> polyline_points;
    polyline_points.push_back(Point(20, 20));
    polyline_points.push_back(Point(40, 25));
    polyline_points.push_back(Point(60, 40));
    c.add(new Polyline(polyline_points));

    list<PathPair> definition;
    definition.push_back(make_pair('M', Point(150, 0)));
    definition.push_back(make_pair('L', Point(75, 200)));
    definition.push_back(make_pair('L', Point(225, 200)));
    definition.push_back(make_pair('Z', Point(150, 0)));
    c.add(new Path(definition));

    c.draw();
    return 0;
}
