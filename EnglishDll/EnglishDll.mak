# EnglishDll.mak
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .cpp .obj .rc .res

.all: \
    .\English.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ge+ /Sp1 /Gm /Gd- /C %s
#           /Ge+ -> no _dllentry but _exeentry if main()

.\English.dll: \
    English.def \
    English.obj \
    English.res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe /Ge- /FeEnglish.dll /B"/EXEPACK:1 /NODEFAULTLIBRARYSEARCH" English.obj English.def
    rc.exe .\English.res English.dll

English.res: \
    English.rc \
    {..\;$(INCLUDE)}ftree.h

English.obj: \
    English.cpp
