# Feel free to edit this as needed
#
program = main

CXXFLAGS = -std=c++17 -Wall

all: $(program)

objects = main.o Record.o Store.o Variant.o

$(program): $(objects)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(program) $(objects)

main.o: Record.h SharedKeys.h Store.h Variant.h
Record.o: SharedKeys.h Variant.h
Store.o: Store.h Record.h SharedKeys.h Variant.h
Variant.o: Variant.h

run: $(program)
	./$(program)

clean:
	$(RM) -f $(program) $(objects)
