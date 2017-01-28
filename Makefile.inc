# ====================================================================
#   					"BGLgeom library"
#      Course on Advanced Programming for Scientific Computing
#                     Politecnico di Milano
#                         A.Y. 2015-2016
#
#		Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
#=====================================================================

### USER CONFIGURATION VARIABLES ###
# Directory where the project resides.
#PROJECT_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia
PROJECT_DIR=/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia

# Installation path
#INSTALL_PATH = /D/BGLgeom
INSTALL_PATH = /home/pacs_student/Desktop/BGLgeom

# Path to Boost Graph Library
#export mkBGLInc = /D/boost_1_61_0/boost #/graph

# Path to GetFEM main folder
#export mkGetFEMHome = /D/GetFEM/5.0

# Path to MANworks main folder
#export mkMANworksHome = /D/MANworks

# Tolenrance on being zero (for comparison on points and segments)
# This variable is needed when including point.hpp and intersections2D.hpp headers, 
# and this happens for all tests in the library and in both applications. If you do 
# not want to set this value, a default one is provided.
TOL_VALUE = 1e-10

## EDIT THESE ONLY IF YOU DO NOT USE MODULE SYSTEM OF MOX CLUSTER ##
# Path to VTK includes
#export mkVtkInc   = /u/geo2/sw/Toolchains/gcc+system/4.9/Packages/libs/vtk/5/include/vtk-5.10
# Path to VTK libraries
#export mkVtkLib   = /u/geo2/sw/Toolchains/gcc+system/4.9/Packages/libs/vtk/5/lib/vtk-5.10
# Path to Eigen includes
#export mkEigenInc = /u/geo2/sw/Packages/libs/eigen/3/include/eigen3


### NOW DO NOT EDIT ANYTHING ###

# Path to GetFEM library
GETFEM_LIB_DIR = $(mkGetFEMHome)/lib
# Path to GetFEM includes
GETFEM_INC_DIR = $(mkGetFEMHome)/include
# Path to MANworks library
MANWORKS_LIB_DIR = $(mkMANworksHome)/lib
# Path to Manworks includes
MANWORKS_INC_DIR = $(mkMANworksHome)/include
# Path to Eigen includes
EIGEN_INC_DIR = $(mkEigenInc)
# Path to VTK includes
VTK_INC_DIR   = $(mkVtkInc)
# Path to VTK library
VTK_LIB_DIR   = $(mkVtkLib)
# Path to BGL includes
BGL_INC_DIR   = $(mkBGLInc)

## Project Folders
# libBGLgeom is stored here (we consider the installation path)
BGLGEOM_DIR = $(INSTALL_PATH)
# Example on network diffusion is stored here
NETW_DIR = $(PROJECT_DIR)/network_diffusion
# Example on fracture intersection is sotred here
FRAC_DIR = $(PROJECT_DIR)/fracture_intersection

# Parsing vtk libraries. In the modules are present also libraries for Python. We filter them out
VTK_LIBS1 = $(wildcard $(VTK_LIB_DIR)/libvtk*.so.5.10.1) #collecting all libraries
VTK_LIBS2 = $(notdir $(VTK_LIBS1))
VTK_LIBS3 = $(basename $(basename $(basename $(basename $(VTK_LIBS2))))) #Discarding .so.5.10.1
VTK_LIBS4 = $(foreach lib, $(VTK_LIBS3), $(if $(findstring Python, $(lib)),,$(lib)))
VTK_LIBS  = $(VTK_LIBS4:lib%=%)
## Libraries needed to the linker
# For all code
LDLIBS = -L$(VTK_LIB_DIR) $(foreach lib, $(VTK_LIBS), -l$(lib))
# Specific for network_diffusion application
LDLIBS_DIFF = -L$(GETFEM_LIB_DIR) -lgetfem -L$(MANWORKS_LIB_DIR) -lproblem3d1d

# Collect all includes
ALL_INC_DIR= $(EIGEN_INC_DIR) $(VTK_INC_DIR) $(BGL_INC_DIR)
includes := $(foreach inc,$(ALL_INC_DIR),-I$(inc))
INCLUDES += -I. -I./include $(includes)
# Includes specific for netwrok_diffusion application
INC_DIR_DIFF = $(GETFEM_INC_DIR) $(MANWORKS_INC_DIR)
includes_diff := $(foreach inc, $(INC_DIR_DIFF),-I$(inc))
INCLUDES_DIFF += -I. -I./include $(includes_diff)

# Compiler and standard for the compiler
CXX = g++
STDFLAG = -std=c++11

# DEFINES: Possible -DXX statements
# TOL: Tolerance on being zero. Used to check distances for points and segments, mainly.
DEFINES += -DTOL=$(TOL_VALUE)

# OPT: Level of optimization
# REALESE: Allows to choose the level of optimization. If set (RELEASE=yes when compiling), full optimization, otherwise debug mode
ifdef RELEASE
OPT = -O3
DEFINES += -DNDEBUG
else
OPT = -g
endif

# Warning level. The option -Wno-deprecated deactivates annoying warning from BGL
WARNS = -Wall -Wno-deprecated

export CPPFLAGS = $(DEFINES) $(INCLUDES)
export CXXFLAGS = $(STDFLAG) $(WARNS) $(OPT)

