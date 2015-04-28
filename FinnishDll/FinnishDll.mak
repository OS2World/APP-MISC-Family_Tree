# FinnishDll.mak
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .cpp .obj .rc .res

.all: \
    Finnish.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ge+ /Sp1 /Gm /Gd- /C %s
#           /Ge+ -> no _dllentry but _exeentry if main()

Finnish.dll: \
    Finnish.def   \
    Finnish.obj \
    Finnish.res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe /Ge- /FeFinnish.dll /B"/EXEPACK:1 /NODEFAULTLIBRARYSEARCH" Finnish.obj Finnish.def
    rc.exe Finnish.res Finnish.dll

Finnish.res: \
    Finnish.rc \
    {..\;$(INCLUDE)}ftree.h

Finnish.obj: \
    Finnish.cpp
