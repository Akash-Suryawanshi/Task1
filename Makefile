run:
	g++ Task1.cpp header.h transform_crop.cpp videoRead.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv`;
	# method3.cpp;	