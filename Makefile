
all: test test2

test: test.cpp flow_text.cpp
	g++ -o test test.cpp flow_text.cpp -lncurses

test2: test2.cpp flow_text.cpp
	g++ -o test2 test2.cpp flow_text.cpp

