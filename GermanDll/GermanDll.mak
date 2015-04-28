# GermanDll.mak
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .cpp .obj .rc .res

.all: \
    German.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ge+ /Sp1 /Gm /Gd- /C %s
#           /Ge+ -> no _dllentry but _exeentry if main()

German.dll: \
    German.def   \
    German.obj \
    German.res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe /Ge- /FeGerman.dll /B"/EXEPACK:1 /NODEFAULTLIBRARYSEARCH" German.obj German.def
    rc.exe German.res German.dll

German.res: \
    German.rc \
    {..\;$(INCLUDE)}ftree.h

German.obj: \
    German.cpp
