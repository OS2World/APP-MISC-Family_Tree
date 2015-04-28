#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <fstream.h>
#include <ireslib.hpp>
#include <ifont.hpp>
#include <irect.hpp>

class NPrinter;
class NRexx;

// Class  : NApplication
// Purpose: Global Application-Variables
class NApplication
{
public:
   static IString isTree;
   static fstream fsFileStream;
   static IString isRegisteredUser;
   static IDynamicLinkLibrary *pDLLResource;
   static NPrinter* pNPrinter;
   static NRexx nrRexx;
   static char cDateSeparator;
   static char cDateLayout;
   static void initLocale();
   static void changeLocale(char dateSep,char DateFmt);
};


#endif
