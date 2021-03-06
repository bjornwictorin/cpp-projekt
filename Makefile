#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

#all: libclientserver.a article message_handler
all: servermain mainprogram diskservermain
#all: diskmain 

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

#libclientserver.a: connection.o server.o
#	ar rv libclientserver.a  connection.o server.o
#	ranlib libclientserver.a 

servermain: message_handler.o server.o servermain.o database.o newsgroup.o article.o connection.o
mainprogram: mainprogram.o myclient.o connection.o
diskservermain: message_handler.o server.o diskdatabase.o newsgroup.o article.o connection.o diskservermain.o



# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o libclientserver.a

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)
