main: main.cpp
	g++ -O3 -o main main.cpp
main_par: main_par.cpp
	mpiCC -O3 -o main_par main_par.cpp
generator: generator.cpp
	g++ -O3 -o generator generator.cpp
