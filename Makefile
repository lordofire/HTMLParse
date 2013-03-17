CC = g++
CCFLAGS = -pedantic -Wall -Werror -g

all: htmlParsingP
.PHONY: all

htmlParsingP: htmlParsingP.o
	$(CC) $(CCFLAGS) htmlParsingP.o -o htmlParsingP -lcurl

htmlParsingP.o: htmlParsingP.cpp
	$(CC) $(CCFLAGS) -c htmlParsingP.cpp -lcurl

.PHONY clean:
	rm -rf *.o *~ htmlParsingP
