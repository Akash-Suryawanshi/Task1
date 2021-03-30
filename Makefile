run:
	g++ Task1.cpp header.h transform_crop.cpp method3_final.cpp -o output -lpthread -std=c++11 `pkg-config --cflags --libs opencv`;