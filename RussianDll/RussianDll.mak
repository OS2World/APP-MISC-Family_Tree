# RussianDll.mak
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .cpp .obj .rc .res

.all: \
    .\Russian.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ge+ /Sp1 /Gm /Gd- /C %s
#           /Ge+ -> no _dllentry but _exeentry if main()

.\Russian.dll: \
    Russian.def \
    Russian.obj \
    Russian.res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe /Ge- /FeRussian.dll /B"/EXEPACK:1 /NODEFAULTLIBRARYSEARCH" Russian.obj Russian.def
    rc.exe .\Russian.res Russian.dll

Russian.res: \
    Russian.rc \
    {..\;$(INCLUDE)}ftree.h

Russian.obj: \
    Russian.cpp
