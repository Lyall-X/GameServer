CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
INCLUDES = -I ../LuaJIT-2.0.1/src -I ../boost-1.47.0 -I .

OBJS = src/class.o\
src/class_info.o\
src/class_registry.o\
src/class_rep.o\
src/create_class.o\
src/error.o\
src/exception_handler.o\
src/function.o\
src/inheritance.o\
src/link_compatibility.o\
src/object_rep.o\
src/open.o\
src/pcall.o\
src/scope.o\
src/stack_content_by_name.o\
src/weak_ref.o\
src/wrapper_base.o

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	luabind.a

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

luabind.a: $(OBJS)
	ar $(ARFLAGS) $@ $^

clean:
	-rm ./src/*.o
	-rm ./src/*.a

%.o : %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $<	