# FrenchDll.mak
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .cpp .obj .rc .res

.all: \
    French.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ge+ /Sp1 /Gm /Gd- /C %s
#           /Ge+ -> no _dllentry but _exeentry if main()

French.dll: \
    French.def   \
    French.obj \
    French.res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe /Ge- /FeFrench.dll /B"/EXEPACK:1 /NODEFAULTLIBRARYSEARCH" French.obj French.def
    rc.exe French.res French.dll

French.res: \
    French.rc \
    {..\;$(INCLUDE)}ftree.h

French.obj: \
    French.cpp
