# HungarianDll.mak
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .cpp .obj .rc .res

.all: \
    .\Hungar.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ge+ /Sp1 /Gm /Gd- /C %s
#           /Ge+ -> no _dllentry but _exeentry if main()

.\Hungar.dll: \
    Hungarian.def \
    Hungarian.obj \
    Hungarian.res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe /Ge- /FeHungar.dll /B"/EXEPACK:1 /NODEFAULTLIBRARYSEARCH" Hungarian.obj Hungarian.def
    rc.exe .\Hungarian.res Hungar.dll

Hungarian.res: \
    Hungarian.rc \
    {..\;$(INCLUDE)}ftree.h

Hungarian.obj: \
    Hungarian.cpp
