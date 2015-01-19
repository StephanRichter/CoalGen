#include <iostream>
#include <boost/program_options.hpp>
namespace programOptions = boost::program_options;

using namespace std;

namespace programOptions = boost::program_options;

struct Point{
	int x=0,y=0,z=0;

	string get(){
		return "("+to_string(x)+", "+to_string(y)+", "+to_string(z)+")";
	}
};

struct Triangle{
	Point p1,p2,p3;
	Triangle *next=0;

	void print(){
		cout << p1.get() << ", " << p2.get() << ", " << p3.get() << endl;
	}
};

int main(int argc, char **argv) {
	programOptions::options_description cmdOpts("Usage");
	cmdOpts.add_options()
		("h", programOptions::value<int>(), "Height in mm")
		("l", programOptions::value<int>(), "Length in mm")
		("w", programOptions::value<int>(), "Width in mm");
	programOptions::variables_map variablesMap;
	try {
		programOptions::store(programOptions::parse_command_line(argc, argv, cmdOpts), variablesMap);
	} catch (...) {
		cerr << cmdOpts << endl;
		exit(EXIT_FAILURE);
	}
	programOptions::notify(variablesMap);

	auto getOption =[&](string name) {
		if (!variablesMap.count(name)) {
			cerr << "--" << name << " missing\n\n" << cmdOpts << endl;
			exit(EXIT_FAILURE);
		}
		return variablesMap[name];
	};

	int length=getOption("l").as<int>();
	int width=getOption("w").as<int>();
	int height=getOption("h").as<int>();

	Point bottomRF,bottomLF,bottomRB,bottomLB;
	bottomLF.y=width;
	bottomLB.y=width;
	bottomLB.x=length;
	bottomRB.x=length;

	Triangle startTriangle;
	startTriangle.p1=bottomRF;
	startTriangle.p2=bottomLF;
	startTriangle.p3=bottomLB;

	Triangle triangle;

	triangle.p1=bottomRF;
	triangle.p2=bottomLB;
	triangle.p3=bottomRB;
	Triangle *triangle_p=&triangle;

	startTriangle.next=triangle_p;

	int wHalf=width/2;
	int lHalf=length/2;
	Point current,last;
	cout << "width = " << width << endl;
	cout << "lenght= " << length<< endl;
	int count=0;
	for (int l=0; l<length; l++){
		//current.x=l;
		//int heights[width];
		for (int w=0; w<width; w++){
/*			current.y=w;
			current.z=3+height+rand() % 6;
			Point last;
			if (l==0){ // first row, connect to bottom
				if (w==0){ // first column

				} else if (w<wHalf){ // connect to bottomLF
					Triangle t;
					t.p1=bottomLF;
					t.p2=last;
					t.p3=current;
					triangle_p->next=&t;
					triangle_p=&t;
				} else {

				}
			} else {
			}
			last=current;
			heights[w]=current.z;*/
			cout << count++ << endl;
		}
	}

	triangle_p=&startTriangle;
	cout << "Triangles: " << endl;

	while (triangle_p){
		triangle_p->print();
		triangle_p=triangle_p->next;
	}
}
