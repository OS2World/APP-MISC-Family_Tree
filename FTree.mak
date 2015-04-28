# FTree.mak
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind
#  Compile::Resource Compiler

# Be silent
#.SILENT:
# Remove default extensions and add own
.SUFFIXES:
.SUFFIXES: .cpp .obj .rc .res

GOAL=FTree.exe
OBJECTS=NOptionsDialog.obj NSearchDialog.obj NMankind.obj NUserFields.obj \
        NManTree.obj NManlist.obj NMainWindow.obj NPrint.obj Main.obj \
        NMisc.obj NListDialog.obj NEventDialog.obj NAboutDialog.obj \
        NRexx.obj NList.obj NDragDrop.obj NEditDialog.obj

RESOURCES=FTree.res
MAKEFILE=FTree.mak

#COPTIONS=/Q+ /Gm+ /C /Ti- /Gd+
#LOPTIONS=/Tdp /Q+ /Gm+ /B"/NOEXEPACK /PMTYPE:PM /NODEB /NOEXT /DEFAULT"
#LIBS=rexx.lib

COPTIONS=/Q+ /Gm+ /C /Ti- /Gd-
LOPTIONS=/Tdp /Q+ /Gm+ /B"/EXEPACK:2 /PMTYPE:PM /NODEB /NOEXT /NODEFAULT"
LIBS=rexx.lib cppom30.lib os2386.lib cppooc3.lib


.all: \
    $(GOAL)

.cpp.obj:
    @echo " Compile::C++ Compiler [$<]"
    icc.exe $(COPTIONS) $<

.rc.res:
    @echo " Compile::Resource Compiler [$<]"
    rc.exe -n -r $< %|dpfF.RES

$(GOAL): $(OBJECTS) $(RESOURCES)
    @echo " Link::Linker [$(GOAL)]"
    icc.exe /Fe$(GOAL) $(LOPTIONS) $(OBJECTS) $(LIBS)
    @echo " Bind::Resource Bind [$(GOAL)]"
    rc.exe -n $(RESOURCES) $(GOAL) >nul

Main.obj: \
    $(MAKEFILE)       \
    Main.cpp          \
    Main.hpp          \
    FTree.h           \
    NMisc.hpp       \
    NMainWindow.hpp    \
    NListDialog.hpp    \
    NEditDialog.hpp    \
    NOptionsDialog.hpp \
    NPrint.hpp         \
    NRexx.hpp

NMainWindow.obj: \
    $(MAKEFILE)       \
    NMainWindow.hpp    \
    NMankind.hpp       \
    NManTree.hpp       \
    FTree.h           \
    NMisc.hpp       \
    NListDialog.hpp    \
    NEditDialog.hpp    \
    NEventDialog.hpp   \
    NAboutDialog.hpp   \
    NOptionsDialog.hpp \
    NPrint.hpp         \
    NSearchDialog.hpp         \
    NRexx.hpp

NOptionsDialog.obj: \
    $(MAKEFILE)       \
    NOptionsDialog.cpp\
    NMankind.hpp      \
    NManTree.hpp      \
    FTree.h          \
    NMisc.hpp      \
    NUserFields.hpp      \
    NEditDialog.hpp

NListDialog.obj: \
    $(MAKEFILE)       \
    NListDialog.cpp   \
    NMankind.hpp      \
    NManList.hpp      \
    FTree.h          \
    NMisc.hpp      \
    NMainWindow.hpp   \
    NListDialog.hpp   \
    NDragDrop.hpp     \
    NEditDialog.hpp

NEditDialog.obj: \
    $(MAKEFILE)       \
    NEditDialog.cpp   \
    NMankind.hpp      \
    FTree.h          \
    NMisc.hpp      \
    NMainWindow.hpp   \
    NListDialog.hpp   \
    NDragDrop.hpp       \
    NUserFields.hpp      \
    NEditDialog.hpp

NEventDialog.obj: \
    $(MAKEFILE)       \
    NEventDialog.cpp   \
    NMankind.hpp      \
    FTree.h          \
    NMisc.hpp      \
    NDragDrop.hpp     \
    NList.hpp \
    NMainWindow.hpp

NAboutDialog.obj: \
    $(MAKEFILE)       \
    NAboutDialog.cpp \
    NAboutDialog.hpp \
    FTree.h         \
    NMainWindow.hpp

NMisc.obj: \
    $(MAKEFILE)       \
    NMisc.cpp \
    NMisc.hpp \
    NMankind.hpp \
    Main.hpp

NMankind.obj: \
    $(MAKEFILE)       \
    NMankind.cpp \
    NMankind.hpp \
    FTree.h

NManList.obj: \
    $(MAKEFILE)       \
    NManList.cpp \
    NManList.hpp \
    NMankind.hpp \
    NMisc.hpp \
    NDragDrop.hpp \
    NList.hpp \
    FTree.h

NManTree.obj: \
    $(MAKEFILE)       \
    NManTree.cpp \
    NManTree.hpp \
    NMankind.hpp \
    NMisc.hpp \
    NDragDrop.hpp \
    FTree.h

NDragDrop.obj: \
    $(MAKEFILE)       \
    NDragDrop.cpp \
    NDragDrop.hpp \
    FTree.h

NPrint.obj: \
    $(MAKEFILE)       \
    NPrint.cpp \
    NPrint.hpp \
    FTree.h

NRexx.obj: \
    $(MAKEFILE)       \
    NRexx.cpp \
    NRexx.hpp \
    NMankind.hpp \
    NMisc.hpp \
    FTree.h

FTree.res:    \
    FTree.rc  \
    BMPs\*

NList.obj: \
    $(MAKEFILE)  \
    NList.hpp

NPointersTo.obj: \
    $(MAKEFILE)  \
    NPointersTo.h \
    NPointersTo.c

NUserFields.obj: \
    $(MAKEFILE)  \
    NUserFields.hpp \
    NUserFields.cpp

NSearchDialog.obj: \
    $(MAKEFILE)  \
    NSearchDialog.hpp \
    NMankind.hpp      \
    FTree.h          \
    NMisc.hpp      \
    NMainWindow.hpp   \
    NListDialog.hpp   \
    NDragDrop.hpp       \
    NUserFields.hpp      \
    NSearchDialog.cpp
