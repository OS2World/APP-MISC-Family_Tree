/*
   File   : Print.hpp
   Purpose: Handle Printing
*/

#ifndef _PRINT_HPP_
#define _PRINT_HPP_


// ICLUI Includes
#include <iprofile.hpp>
#include <iframe.hpp>
#include <imcelcv.hpp>
#include <istattxt.hpp>
#include <icombobx.hpp>
#include <ipushbut.hpp>
#include <icmdhdr.hpp>
#include <iselhdr.hpp>
#include <ientryfd.hpp>
#include <icheckbx.hpp>
#include <islider.hpp>
#include <iedithdr.hpp>
#include <ikeyhdr.hpp>

// OS2 Includes
#define INCL_DEV
#define INCL_SPL
#define INCL_SPLDOSPRINT
#include <os2.h>


class NPrintDialog;
class NPrinter;
class NPrintQueue;

typedef NPrintQueue*  PNPrintQueue;
typedef PNPrintQueue* PPNPrintQueue;

// Class   : NPrinter
// Purpose : Enable Printer Support for DeviceContexts
class NPrinter
{
   friend class NPrintDialog;
public:
   NPrinter();
   ~NPrinter();

   Boolean startPrinting(IWindow* pOwner
                        ,const ISize& isSetDocumentSize
                        ,IString& isSetDocumentTitle);
   IGraphicContext* graphicContext();
   void newPage();
   void stopPrinting();

   ISize getPageCmSize();
   ISize getPageSize();
   ISize getPages();

   double getZoomX();
   double getZoomY();

   ULONG count();
private:
   Boolean isRealPages();
   ISize getRealPages();
   void setPages(const ISize& isSetPages);

   void setZoom(double dZoomSetX,double dZoomSetY);

   double dZoomX,dZoomY;
   ISize isDocumentSize;
   IString isDocumentTitle;
   ISize isPages;

   Boolean setCurrentPrintQueue(long lNew);
   long getCurrentPrintQueueIdx();
   PNPrintQueue getCurrentPrintQueue();
   IString queueName();
   ISize resolution();
   void changeProperties();

   LONG alDevInfo[39];

   PPNPrintQueue ppPrintQueue;
   long lPrintQueueCount;
   long lCurrentPrintQueue;

   HDC hdcPrinter;
   HPS hpsPrinter;
   IGraphicContext* igcPrinter;
};


#endif

