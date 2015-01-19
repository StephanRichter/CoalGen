#include <iostream>
#include <boost/program_options.hpp>
namespace programOptions = boost::program_options;

using namespace std;

namespace programOptions = boost::program_options;

struct Point {
	int x = 0, y = 0, z = 0;
	string get() {
		return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
	}

	Point(int x, int y, int z){
		this->x=x;
		this->y=y;
		this->z=z;
	}

	Point(){
		x=0;
		y=0;
		z=0;
	}

};

class Vector {
protected:
	int x = 0, y = 0, z = 0;
public:
	Vector(int x,int y,int z){
		this->x=x;
		this->y=y;
		this->z=z;
	}

	Vector operator*(Vector b)	{
		return Vector((this->y*b.z)-(this->z*b.y),(this->z*b.x)-(this->x*b.z),(this->x*b.y)-(this->y*b.x));
	}

	void printTo(ostream &stream){
		stream << "normal "<< x << " " << y << " " << z;
	}
};

struct Triangle {
	Point p1, p2, p3;
	Triangle *next = 0;

	Triangle(Point pt1,Point pt2,Point pt3){
		p1=pt1;
		p2=pt2;
		p3=pt3;
	}

	Vector normal(){
		Vector a(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z);
		Vector b(p1.x-p3.x,p1.y-p3.y,p1.z-p3.z);
		return a*b;
	}
	void printTo(ostream &stream) {
		stream << " facet ";
		normal().printTo(stream);
		stream << endl;
		stream << "  outer loop" << endl;
		stream << "   vertex " << p1.x << " " << p1.y << " " << p1.z << endl;
		stream << "   vertex " << p2.x << " " << p2.y << " " << p2.z << endl;
		stream << "   vertex " << p3.x << " " << p3.y << " " << p3.z << endl;
		stream << "  endloop" << endl;
		stream << " endfacet" << endl;
	}
};

int main(int argc, char **argv) {
	programOptions::options_description cmdOpts("Usage");
	cmdOpts.add_options()("h", programOptions::value<int>(), "Height in mm")("l", programOptions::value<int>(), "Length in mm")("w", programOptions::value<int>(), "Width in mm");
	programOptions::variables_map variablesMap;
	try {
		programOptions::store(programOptions::parse_command_line(argc, argv, cmdOpts), variablesMap);
	} catch (...) {
		cerr << cmdOpts << endl;
		exit(EXIT_FAILURE);
	}
	programOptions::notify(variablesMap);

	auto getOption = [&](string name) {
		if (!variablesMap.count(name)) {
			cerr << "--" << name << " missing\n\n" << cmdOpts << endl;
			exit(EXIT_FAILURE);
		}
		return variablesMap[name];
	};

	int length = getOption("l").as<int>();
	int width = getOption("w").as<int>();
	int height = getOption("h").as<int>();

	Point bottomRF(0,0,0), bottomLF(0,width,0), bottomRB(length,0,0), bottomLB(length,width,0);

	Triangle startTriangle(bottomRF,bottomLF,bottomLB);

	Triangle triangle(bottomRF,bottomLB,bottomRB);

	Triangle *triangle_p = &triangle;

	startTriangle.next = triangle_p;

	int wHalf = width / 2;
	int lHalf = length / 2;
	Point current, last;
//	cout << "width = " << width << endl;
//	cout << "lenght= " << length << endl;
	int count = 0;
	for (int l = 0; l < length; l++) {
		current.x = l;
		int heights[width];
		for (int w = 0; w < width; w++) {
			current.y = w;
			current.z = height -1 + rand() % 3;
			if (l == 0) { // first row, connect to bottom
				if (w == 0) { // first column

				} else if (w < wHalf) { // connect to bottomLF
					Triangle *t = new Triangle(bottomRF,last,current);
					triangle_p->next = t;
					triangle_p = t;
				} else { // connect to bottomLF
					Triangle *t = new Triangle(bottomLF,last,current);
					triangle_p->next = t;
					triangle_p = t;
				}
			}else if (l == length-1){

			} else if (length<lHalf){
				if (w == 0) { // first column

				} else {
					Point p(w-1,l-1,heights[w-1]);
					Triangle *t = new Triangle(p,last,current);
					triangle_p->next = t;
					triangle_p = t;
				}

			} else {

			}
			last = current;
			heights[w] = current.z;
//			cout << endl;
		}
	}

	triangle_p = &startTriangle;
	cout << "solid test" << endl;

	count = 0;
	while (triangle_p) {
		triangle_p->printTo(cout);
		cout << endl;
		triangle_p = triangle_p->next;
	}

	cout << "end solid" << endl;
}
