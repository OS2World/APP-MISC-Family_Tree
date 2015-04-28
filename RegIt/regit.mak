# regit.mak

.SUFFIXES: .cpp .obj .rc .res

.all: \
    .\regit.exe

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tn- /Tdp /Oc+ /Gd- /Gm+ /C /Q+ %s

.\regit.exe:  \
    .\main.obj
    @echo " Link::Linker "
    icc.exe @<<
     /Q+ /B" /exepack:2 /OPTFUNC /NODEBUG /DEFAULTLIBRARYSEARCH"
     /FeRegit.exe
     .\main.obj
<<

.\main.obj: \
    ".\main.cpp"
