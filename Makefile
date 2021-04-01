run:
	g++ Task1.cpp header.h transform_crop.cpp method4.cpp -o output -lpthread -std=c++11 `pkg-config --cflags --libs opencv`;
	# method3.cpp;	
