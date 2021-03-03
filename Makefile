output:
	g++ Task1.cpp header.h transform_crop.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv`;