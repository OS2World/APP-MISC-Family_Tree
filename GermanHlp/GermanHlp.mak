# GermanHlp.mak
#
# The actions included in this make file are:
#  Compile::IPF Compiler

.SUFFIXES: .hlp .ipf 

.all: \
    .\German.hlp

.ipf.hlp:
    @echo " Compile::IPF Compiler "
    ipfc.exe %s

.\German.hlp: \
    ".\German.ipf"
