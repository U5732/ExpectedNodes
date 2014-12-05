#!/bin/bash

CXX=g++
CXXFLAGS= -ansi -O9 -Wall
DIRSRC= ./src/
EXEC=evaluation
ALL_QUAL= $(DIRSRC)Quality/ExpectedNodes.o $(DIRSRC)Quality/density.o $(DIRSRC)Quality/Evans.o

OBJ1= $(DIRSRC)log.o $(DIRSRC)linkStream.o $(DIRSRC)community.o $(DIRSRC)Quality/quality.o $(ALL_QUAL)

all: $(EXEC)

evaluation:$(OBJ1)  $(DIRSRC)main_Evaluation.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

main_Evaluation.o:$(DIRSRC)linkStream.hpp  $(DIRSRC)community.hpp $(DIRSRC)qualityFactory.hpp


linkStream.o: $(DIRSRC)log.hpp

community.o: $(DIRSRC)linkStream.hpp $(DIRSRC)log.hpp

quality.o: $(DIRSRC)linkStream.hpp $(DIRSRC)community.hpp

$(ALL_QUAL): $(DIRSRC)Quality/quality.hpp $(DIRSRC)linkStream.hpp $(DIRSRC)community.hpp

##########################################
# Generic rules
##########################################

%.o: %.cpp %.h
	$(CXX) -o  $@ -c $< $(CXXFLAGS)

%.o: %.cpp %.hpp
	$(CXX) -o  $@ -c $< $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(DIRSRC)*.o $(DIRSRC)Quality/*.o

mrproper: clean
	rm -f *~ $(EXEC)
