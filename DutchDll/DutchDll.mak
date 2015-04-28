# DutchDll.mak
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .cpp .obj .rc .res

.all: \
    Dutch.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ge+ /Sp1 /Gm /Gd- /C %s
#           /Ge+ -> no _dllentry but _exeentry if main()

Dutch.dll:    \
    Dutch.def   \
    Dutch.obj   \
    Dutch.res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe /Ge- /FeDutch.dll /B"/EXEPACK:1 /NODEFAULTLIBRARYSEARCH" Dutch.obj Dutch.def
    rc.exe Dutch.res Dutch.dll

Dutch.res: \
    Dutch.rc \
    {..\;$(INCLUDE)}ftree.h

Dutch.obj: \
    Dutch.cpp
