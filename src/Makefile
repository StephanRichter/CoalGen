all: dirs bin/coalgen
clean:
	rm -rf bin objects; ls -l
dirs:
	mkdir -p objects bin
bin/coalgen: objects/coalgen.o
	g++ -std=c++11 -Wall -g -lboost_program_options $^ -o bin/coalgen
objects/%.o: %.cpp
	g++ -std=c++11 -c -Wall -g $^ -o $@

