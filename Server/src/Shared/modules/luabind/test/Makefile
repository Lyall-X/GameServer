INCLUDES=-I ../../boost-1.47.0 -I ../ -I ../../LuaJIT-2.0.1/src
all:
	echo "building test"
	g++  $(INCLUDES) -c main.cpp -o main.o
	g++  $(INCLUDES) test_vector_of_object.cpp -o test_vector_of_object.exe -lm ../../bin/lua51.dll ../../bin/luabind.a main.o
