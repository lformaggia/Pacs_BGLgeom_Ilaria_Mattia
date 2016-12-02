###	BGLgeom Makefile.inc ###
#Vedi makefile.inc in Example della pacs_repository

# Directory where the library resides
PROJECT_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia#che è la mia, l'utente la cambia
# libBGLgeom is stored here:
export BGLGEOM_DIR = $(PROJECT_DIR)/libBGlgeom
# example on network diffusion is stored here
export NETW_DIR = $(PROJECT_DIR)/network_diffusion
# example on fracture intersection is sotred here
export FRAC_DIR = $(PROJECT_DIR)/fracture_intersection

# Directories for needed library
EIGEN_INC_DIR=$(mkEigenInc)
VTK_INC_DIR=
VTK_LIB_DIR=
#MUPARSER_INC_DIR=$(mkMuParserInc)
#MUPARSER_LIB_DIR=$(mkMuParserLib)
#NURBS ?

OTHER_INC_DIR=
OTHER_LIB_DIR=

CXX=g++
# Since we are only uning c++ I set CC=CXX so that the linker LINK.o is correct
# make has no separate macro for the linker!
#CC=$(CXX)

#  DEFINES: Possible -DXX statements
# Example: -DNDEBUG disactivates asserts
# DEFINES=

WARNS = -Wall

# Level of optimization
# Mettere la parte corrispondente dell'esempio del corso se voglio essere più "standard", direi
OPT = -g

STDFLAG = -std=c++11


# collect all includes
# QUeste sono solo le directory
ALL_INC_DIR= $(OTHER_INC_DIR) $(EIGEN_INC_DIR) #$(MUPARSER_INC_DIR)
# Così credo che ci appiccichi davanti -I ad ognuna
includes := $(foreach inc,$(ALL_INC_DIR),-I$(inc) )
# E ora creo una variabile che mi permette di chiamarle tutte insieme
INCLUDES += -I. -I./include $(includes)

export CPPFLAGS = $(INCLUDES)
export CXXFLAGS = $(STDFLAG) $(WARNS) $(OPT)

