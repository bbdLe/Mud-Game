CXX = g++

LIBDIR = ../../Libraries
THREADLIBDIR = $(LIBDIR)/ThreadLib

LIBS = -lpthread -ldl -lutil

CFLAGS = -I$(LIBDIR) -fpermissive -std=c++11

target:
	$(CXX) $(CFLAGS) $(THREADLIBDIR)/*.cpp -c;
	$(CXX) $(CFLAGS) $(LIBS) *.cpp *.o -o main