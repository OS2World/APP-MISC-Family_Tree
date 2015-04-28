# EnglishHlp.mak
#
# The actions included in this make file are:
#  Compile::IPF Compiler

.SUFFIXES: .hlp .ipf 

.all: \
    .\English.hlp

.ipf.hlp:
    @echo " Compile::IPF Compiler "
    ipfc.exe %s

.\English.hlp: \
    ".\English.ipf"
