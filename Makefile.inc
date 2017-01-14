# ====================================================================
#   					"BGLgeom library"
#      Course on Advanced Programming for Scientific Computing
#                     Politecnico di Milano
#                         A.Y. 2015-2016
#=====================================================================


# Directory where the project resides.
## USER MUST EDIT! ##
PROJECT_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia
# libBGLgeom is stored here:
export BGLGEOM_DIR = $(PROJECT_DIR)/libBGLgeom
# example on network diffusion is stored here
export NETW_DIR = $(PROJECT_DIR)/network_diffusion
# example on fracture intersection is sotred here
export FRAC_DIR = $(PROJECT_DIR)/fracture_intersection

# Installation path
## USER MUST EDIT! ##
INSTALL_PATH = /D/BGLgeom

# Creating variable for path to BGL
## USER MUST EDIT ##
export mkBGLInc = /D/boost_1_61_0/boost/graph

# Creating variable for path to vtk includes
## USER MUST EDIT THESE ONLY IF DO NOT USE MODULE SYSTEM OF MOX CLUSTER ##
export mkVtkInc   = /u/geo2/sw/Toolchains/gcc+system/4.9/Packages/libs/vtk/5/include/vtk-5.10
export mkVtkLib   = /u/geo2/sw/Toolchains/gcc+system/4.9/Packages/libs/vtk/5/lib/vtk-5.10
export mkEigenInc = /u/geo2/sw/Packages/libs/eigen/3/include/eigen3

# Directories for needed library
EIGEN_INC_DIR = $(mkEigenInc)
VTK_INC_DIR   = $(mkVtkInc)
VTK_LIB_DIR   = $(mkVtkLib)
BGL_INC_DIR   = $(mkBGLInc)

# Parsing vtk libraries. In the modules are present also libraries for Python. We filter them out
VTK_LIBS1 = $(wildcard $(VTK_LIB_DIR)/libvtk*.so.5.10.1) #collecting all libraries
VTK_LIBS2 = $(notdir $(VTK_LIBS1))
VTK_LIBS3 = $(basename $(basename $(basename $(basename $(VTK_LIBS2))))) #Discarding .so.5.10.1
VTK_LIBS4 = $(foreach lib, $(VTK_LIBS3), $(if $(findstring Python, $(lib)),,$(lib)))
VTK_LIBS  = $(VTK_LIBS4:lib%=%)
# Libraries needed to the linker
LDLIBS = -L$(VTK_LIB_DIR) $(foreach lib, $(VTK_LIBS), -l$(lib))

# Compiler and standard for the compiler
CXX = g++
STDFLAG = -std=c++11

# DEFINES: Possible -DXX statements
# TOL: Tolerance on being zero. Used to check distances for points and segments, mainly. The user can change it.
DEFINES += -DTOL=1e-10

# OPT: Level of optimization
# REALESE: Allows to choose the level of optimization. If set, full optimization, otherwise debug mode
ifdef RELEASE
OPT = -O3
DEFINES += -DNDEBUG
else
OPT = -g
endif

# Warning level. The option -Wno-deprecated deactivates annoying warning from BGL
WARNS = -Wall -Wno-deprecated

# Collect all includes
ALL_INC_DIR= $(EIGEN_INC_DIR) $(VTK_INC_DIR) $(BGL_INC_DIR)
includes := $(foreach inc,$(ALL_INC_DIR),-I$(inc) )
INCLUDES += -I. -I./include $(includes)

export CPPFLAGS = $(INCLUDES) $(DEFINES)
export CXXFLAGS = $(STDFLAG) $(WARNS) $(OPT)

