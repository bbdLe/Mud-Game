CXX = g++

LIBDIR = ../../Libraries
THREADLIBDIR = $(LIBDIR)/ThreadLib
BASICLIBDIR = $(LIBDIR)/BasicLib

LIBS = -lpthread -ldl -lutil

CFLAGS = -I$(LIBDIR) -fpermissive -std=c++11

target:
	$(CXX) $(CFLAGS) $(THREADLIBDIR)/*.cpp -c;
	$(CXX) $(CFLAGS) $(BASICLIBDIR)/*.cpp -c;
	$(CXX) $(CFLAGS) $(LIBS) *.cpp *.o -o main