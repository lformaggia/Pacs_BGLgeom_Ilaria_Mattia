###	BGLgeom Makefile.inc ###
#Vedi makefile.inc in Example della pacs_repository

# Directory where the library resides
BGLGEOM_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom	#che è la mia, l'utente la cambia
# Directory where the two example reside
EXAMPLE_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/	#Attenzione! Poi c'è un'altra sottodirectory per ognuno

# Directories for needed library
EIGEN_INC_DIR=$(mkEigenInc)
MUPARSER_INC_DIR=$(mkMuParserInc)
MUPARSER_LIB_DIR=$(mkMuParserLib)
#NURBS ?

OTHER_INC_DIR=
OTHER_LIB_DIR=

CXX=g++
# Since we are only uning c++ I set CC=CXX so that the linker LINK.o is correct
# make has no separate macro for the linker!
CC=$(CXX)

#Cose in più tipo includi clang o il compiler di intel


