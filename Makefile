all:
	clang++ -O3 -std=c++11 src/Main.cpp -o Main
	./Main
	xdg-open image.ppm
