#include <iostream>
#include <boost/program_options.hpp>
namespace programOptions = boost::program_options;

using namespace std;

namespace programOptions = boost::program_options;

struct Point {
	float x = 0, y = 0, z = 0;
	string get() {
		return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
	}

	Point(float x, float y, float z){
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
	float x = 0, y = 0, z = 0;
public:
	Vector(float x,float y,float z){
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

const float pi=3.1415926535;

int main(int argc, char **argv) {
	programOptions::options_description cmdOpts("Usage");
	cmdOpts.add_options()
			("h", programOptions::value<int>(), "Height in mm")
			("l", programOptions::value<int>(), "Length in mm")
			("w", programOptions::value<int>(), "Width in mm")
			("s", programOptions::value<float>()->default_value(2),"Slope");
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
	float slope = getOption("s").as<float>();

	Point bottomRF(0,0,0), bottomLF(0,width-1,0), bottomRB(length-1,0,0), bottomLB(length-1,width-1,0);

	Triangle startTriangle(bottomRF,bottomLF,bottomLB);

	Triangle triangle(bottomRF,bottomLB,bottomRB);

	Triangle *triangle_p = &triangle;

	startTriangle.next = triangle_p;

	int wHalf = (width+1) / 2;
	int lHalf = (length+1) / 2;
	Point current, last;
	float lastHeights[width];
	for (int l = 0; l < length; l++) {
		float heights[width];
		current.x = l;
		for (int w = 0; w < width; w++) {
			current.y = w;
			current.z = height +slope*sin(l*pi/length)+slope*sin(w*pi/width)+ static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1);
			if (l == 0) { // first row, connect to bottom
				if (w != 0) {
					Triangle *t = new Triangle(w < wHalf?bottomRF:bottomLF,last,current);
					triangle_p->next = t;
					triangle_p = t;

					if (w==wHalf){
						t = new Triangle(bottomRF,last,bottomLF);
						triangle_p->next = t;
						triangle_p = t;
					}
				}
			} else {
				if (w == 0) { // first column
					Point p(l-1,w,lastHeights[w]);
					Triangle *t = new Triangle(p,l<lHalf?bottomRF:bottomRB,current);
					triangle_p->next = t;
					triangle_p = t;

					if (l==lHalf){
						t = new Triangle(bottomLB,bottomLF,last);
						triangle_p->next = t;
						triangle_p = t;

						Point p(l-1,w,lastHeights[w]);
						t = new Triangle(bottomRB,p,bottomRF);
						triangle_p->next = t;
						triangle_p = t;
					}

				} else {
					Point p1(l-1,w-1,lastHeights[w-1]);
					Triangle *t = new Triangle(p1,last,current);
					triangle_p->next = t;
					triangle_p = t;

					Point p2(l-1,w,lastHeights[w]);
					t = new Triangle(p2,p1,current);
					triangle_p->next = t;
					triangle_p = t;
					if (l==length-1){ // last row
						Triangle *t = new Triangle(last,w<wHalf?bottomRB:bottomLB,current);
						triangle_p->next = t;
						triangle_p = t;

						if (w==wHalf){
							t = new Triangle(bottomRB,bottomLB,last);
							triangle_p->next = t;
							triangle_p = t;
						}
					}
					if (w==width-1){ // last column
						Point p(l-1,w,lastHeights[w]);
						Triangle *t = new Triangle(l<lHalf?bottomLF:bottomLB,p,current);
						triangle_p->next = t;
						triangle_p = t;
					}
				}
			}
			heights[w] = current.z;
			last = current;
//			cout << endl;
		}
		copy(heights,heights+width,lastHeights);
	}

	triangle_p = &startTriangle;
	cout << "solid test" << endl;
	while (triangle_p) {
		triangle_p->printTo(cout);
		cout << endl;
		triangle_p = triangle_p->next;
	}
	cout << "end solid" << endl;
}
