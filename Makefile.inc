###	BGLgeom Makefile.inc ###
#Vedi makefile.inc in Example della pacs_repository

# Directory where the library resides
#che è la mia, l'utente la cambia
PROJECT_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia
# libBGLgeom is stored here:
export BGLGEOM_DIR = $(PROJECT_DIR)/libBGLgeom
# example on network diffusion is stored here
export NETW_DIR = $(PROJECT_DIR)/network_diffusion
# example on fracture intersection is sotred here
export FRAC_DIR = $(PROJECT_DIR)/fracture_intersection

INSTALL_PATH ?= /usr/local


#Creating variable for path to vtk includes
export mkVtkInc = /u/geo2/sw/Toolchains/gcc+system/4.9/Packages/libs/vtk/5/include/vtk-5.10

# Directories for needed library
EIGEN_INC_DIR = $(mkEigenInc)
VTK_INC_DIR   = $(mkVtkInc)
VTK_LIB_DIR   = $(mkVtkLib)
#MUPARSER_INC_DIR=$(mkMuParserInc)
#MUPARSER_LIB_DIR=$(mkMuParserLib)
#NURBS ?

#Parsing vtk libraries. In the modules are present also libraries for Python. We filter them out
VTK_LIBS1 = $(wildcard $(VTK_LIB_DIR)/libvtk*.so.5.10.1) #collecting all libraries
VTK_LIBS2 = $(notdir $(VTK_LIBS1))
VTK_LIBS3 = $(basename $(basename $(basename $(basename $(VTK_LIBS2))))) #Discarding .so.5.10.1
VTK_LIBS4 = $(foreach lib, $(VTK_LIBS3), $(if $(findstring Python, $(lib)),,$(lib)))
VTK_LIBS  = $(VTK_LIBS4:lib%=%)

OTHER_INC_DIR=
OTHER_LIB_DIR=

CXX=g++
# Since we are only uning c++ I set CC=CXX so that the linker LINK.o is correct
# make has no separate macro for the linker!
#CC=$(CXX)

#  DEFINES: Possible -DXX statements
# Example: -DNDEBUG disactivates asserts
# DEFINES=

WARNS = -Wall -Wno-deprecated

# Level of optimization
# Mettere la parte corrispondente dell'esempio del corso se voglio essere più "standard", direi
OPT = -g

STDFLAG = -std=c++11


# collect all includes
# QUeste sono solo le directory
ALL_INC_DIR= $(OTHER_INC_DIR) $(EIGEN_INC_DIR) $(VTK_INC_DIR)
# Così credo che ci appiccichi davanti -I ad ognuna
includes := $(foreach inc,$(ALL_INC_DIR),-I$(inc) )
# E ora creo una variabile che mi permette di chiamarle tutte insieme
INCLUDES += -I. -I./include $(includes)
#Libraries needed to the linker
LDLIBS = -L$(VTK_LIB_DIR) $(foreach lib, $(VTK_LIBS), -l$(lib))

export CPPFLAGS = $(INCLUDES)
export CXXFLAGS = $(STDFLAG) $(WARNS) $(OPT)

