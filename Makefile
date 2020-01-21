GCCFLAGS = -g -Wall -Wextra -O3 #-ansi -pedantic -std=c11
EXEC = main 

all: $(EXEC)

#EXEC = % with this, so %.o works
$(EXEC) : % : %.o
	g++ $(GCCFLAGS) -o $@ $^

%.o: %.cpp
	g++ $(GCCFLAGS) -c $^

stat: $(EXEC)
	valgrind --tool=callgrind ./$(EXEC)

#kcachegrind callgrind...


clean:
	-rm $(EXEC)
	-rm *.o
