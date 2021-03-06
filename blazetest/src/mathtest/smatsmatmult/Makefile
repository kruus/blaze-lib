#==================================================================================================
#
#  Makefile for the smatsmatmult module of the Blaze test suite
#
#  Copyright (C) 2013 Klaus Iglberger - All Rights Reserved - All Rights Reserved
#
#  This file is part of the Blaze library. You can redistribute it and/or modify it under
#  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
#  forms, with or without modification, are permitted provided that the following conditions
#  are met:
#
#  1. Redistributions of source code must retain the above copyright notice, this list of
#     conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice, this list
#     of conditions and the following disclaimer in the documentation and/or other materials
#     provided with the distribution.
#  3. Neither the names of the Blaze development group nor the names of its contributors
#     may be used to endorse or promote products derived from this software without specific
#     prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
#  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
#  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
#  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
#  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
#  DAMAGE.
#
#==================================================================================================


# Including the compiler and library settings
ifneq ($(MAKECMDGOALS),clean)
-include ../../Makeconfig
endif


# Setting the source, object and dependency files
SRC = $(wildcard ./*.cpp)
DEP = $(SRC:.cpp=.d)
OBJ = $(SRC:.cpp=.o)
BIN = $(SRC:.cpp=)


# General rules
default: MCaMCa MCaMCb MCbMCa MCbMCb \
         LCaLCa LCaLCb LCbLCa LCbLCb \
         UCaUCa UCaUCb UCbUCa UCbUCb \
         SCaSCa SCaSCb SCbSCa SCbSCb \
         AliasingTest
all: $(BIN)
essential: MCaMCa LCaLCa UCaUCa SCaSCa AliasingTest
single: MCaMCa


# Build rules
LCaLCa: LCaLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCaLCb: LCaLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCaMCa: LCaMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCaMCb: LCaMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCaSCa: LCaSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCaSCb: LCaSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCaUCa: LCaUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCaUCb: LCaUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbLCa: LCbLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbLCb: LCbLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbMCa: LCbMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbMCb: LCbMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbSCa: LCbSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbSCb: LCbSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbUCa: LCbUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LCbUCb: LCbUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

MCaLCa: MCaLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCaLCb: MCaLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCaMCa: MCaMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCaMCb: MCaMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCaSCa: MCaSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCaSCb: MCaSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCaUCa: MCaUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCaUCb: MCaUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbLCa: MCbLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbLCb: MCbLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbMCa: MCbMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbMCb: MCbMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbSCa: MCbSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbSCb: MCbSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbUCa: MCbUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MCbUCb: MCbUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

SCaLCa: SCaLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCaLCb: SCaLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCaMCa: SCaMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCaMCb: SCaMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCaSCa: SCaSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCaSCb: SCaSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCaUCa: SCaUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCaUCb: SCaUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbLCa: SCbLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbLCb: SCbLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbMCa: SCbMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbMCb: SCbMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbSCa: SCbSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbSCb: SCbSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbUCa: SCbUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SCbUCb: SCbUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

UCaLCa: UCaLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCaLCb: UCaLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCaMCa: UCaMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCaMCb: UCaMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCaSCa: UCaSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCaSCb: UCaSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCaUCa: UCaUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCaUCb: UCaUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbLCa: UCbLCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbLCb: UCbLCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbMCa: UCbMCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbMCb: UCbMCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbSCa: UCbSCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbSCb: UCbSCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbUCa: UCbUCa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UCbUCb: UCbUCb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

AliasingTest: AliasingTest.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)


# Cleanup
clean:
	@$(RM) $(DEP) $(OBJ) $(BIN)


# Makefile includes
ifneq ($(MAKECMDGOALS),clean)
-include $(DEP)
endif


# Makefile generation
%.d: %.cpp
	@$(CXX) -MM -MP -MT "$*.o $*.d" -MF $@ $(CXXFLAGS) $<


# Setting the independent commands
.PHONY: default all essential single clean
