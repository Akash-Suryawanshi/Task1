output:
	g++ Task1.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv`;