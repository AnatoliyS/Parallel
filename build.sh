#!/bin/bash
echo "Compiling generator..."
make generator
echo "Compiling main..."
make main
echo "Compiling main_par..."
make main_par
echo "Running generator..."
./generator graph.txt 12 100
echo "Running program..."
./main graph.txt
echo "---------------------------------------"
echo "Running parallel program..."
mpirun -np 2 ./main_par graph.txt