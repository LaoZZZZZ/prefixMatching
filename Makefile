CC	= g++
CFLAGS	= -std=c++11 -fPIC -w -O3
LDFLAGS	= -pthread
PROG	= run


# command

RM	=rm
CP	=cp
CD	=cd
MV	=mv

	
SOURCES	= ./src/snowball.cpp ./src/filesplittor.cpp ./src/suffix.cpp ./src/sequenceblock.cpp ./src/fastapattern.cpp ./src/rawpattern.cpp ./src/suffixtree.cpp ./src/linksuffixtree.cpp ./src/alphabet.cpp ./src/random_util.cpp ./src/reorderdrive.cpp ./src/linearmatching.cpp ./src/treematchingmultipleway.cpp ./src/treematching.cpp ./src/fastqpattern.cpp ./src/qual.cpp ./src/sequenceOutput.cpp ./src/main.cpp ./src/suffixtrie.cpp ./src/random_source.cpp ./src/suffixparam.cpp ./src/preprocess.cpp ./src/ds.cpp ./src/suffixreadsloader.cpp ./src/sequence.cpp ./src/matchingalgorithm.cpp ./src/dnastring.cpp ./src/node.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run
all: $(SOURCES) $(EXECUTABLE)
clean:
	$(RM) $(OBJECTS)
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
.cpp.o:
	$(CC)  -c $(CFLAGS) $< -o $@

