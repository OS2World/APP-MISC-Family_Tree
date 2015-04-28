/*
   File   : Print.cpp
   Purpose: Handle Printing
*/

#include <imsgbox.hpp>
#include <ithread.hpp>
#include <iexcbase.hpp>
#include <ipoint.hpp>
#include <igrafctx.hpp>
#include <igstring.hpp>
#include <igrect.hpp>
#include <igbundle.hpp>
#include <ihandle.hpp>

#include <math.h>

#include "NPrint.hpp"
#include "FTree.h"
#include "Main.hpp"

// Class  : NPrintQueue
// Purpose: Encapsulate PM Queues in C++
class NPrintQueue
{
public:
   NPrintQueue(const IString& isSetName,
               const IString& isSetDriverName,
               const IString& isSetComment,
               PDRIVDATA      pSetDriverData);
   ~NPrintQueue();
   IString& getName();
   IString& getDriverName();
   IString& getComment();
   PDRIVDATA getDriverData(long lNewSize=0);
private:
   IString isName,isDriverName,isComment;
   PDRIVDATA pDriverData;
};

NPrintQueue::NPrintQueue(const IString& isSetName,
                         const IString& isSetDriverName,
                         const IString& isSetComment,
                         PDRIVDATA      pSetDriverData)
{
   isName=isSetName;
   isDriverName=isSetDriverName;
   isComment=isSetComment;
   pDriverData=(PDRIVDATA)malloc(pSetDriverData->cb);
   memcpy(pDriverData,pSetDriverData,pSetDriverData->cb);
}

NPrintQueue::~NPrintQueue()
{
   free(pDriverData);
}

IString& NPrintQueue::getName()
{
   return(isName);
}
IString& NPrintQueue::getDriverName()
{
   return(isDriverName);
}
IString& NPrintQueue::getComment()
{
   return(isComment);
}
PDRIVDATA NPrintQueue::getDriverData(long lNewSize)
{
   // Check if new size is bigger than old
   if (pDriverData->cb<lNewSize) {
      // Get new space
      PDRIVDATA pNew=(PDRIVDATA)malloc(lNewSize);
      // Copy old
      memcpy(pNew,pDriverData,pDriverData->cb);
      // change ptr
      free(pDriverData);
      pDriverData=pNew;
      }
   return(pDriverData);
}


// Class  : NPrintDialog
// Purpose: Dialog Window Class for Printing
/*
 +------------------------------+
 |Printer [                   ] |
 |Printersize (cm) 29.3 x 14.0  |
 |Resolution (dpi)  300 x  300  |
 |Pages []fixed   [  1] x [ 1]  |
 |Zoom            |--[]-------| |
 |[Print] [Properties] [Cancel] |
 +------------------------------+
*/

class NPrintDialog : public IFrameWindow,
                     public ICommandHandler,
                     public ISelectHandler,
                     public IEditHandler,
                     public IKeyboardHandler
{
public:
   NPrintDialog(IWindow* pOwner,NPrinter* pSetNPrinter);
   virtual ~NPrintDialog();
   IMultiCellCanvas imccCanvas;

   IStaticText istPrinter;
   IComboBox icbCombo;

   IStaticText istPrinterSize1;
   IStaticText istCmWidth;
   IStaticText istPrinterSize2;
   IStaticText istCmHeight;

   IStaticText istPrinterResolution1;
   IStaticText istResWidth;
   IStaticText istPrinterResolution2;
   IStaticText istResHeight;

   IStaticText istPages1;
   ICheckBox   icbPages;
   IEntryField iefWidth;
   IStaticText istPages2;
   IEntryField iefHeight;

   IStaticText istZoom1;
   IStaticText istZoom2;
   ISlider     islZoom;

   IPushButton ipbPrint;
   IPushButton ipbProperties;
   IPushButton ipbCancel;
protected:
   Boolean command(ICommandEvent& event);
   Boolean selected(IControlEvent& event);
   Boolean edit(IControlEvent& event);
   Boolean virtualKeyPress(IKeyboardEvent& event);
private:
   void setInfo();
   NPrinter* pNPrinter;
};

NPrintDialog::NPrintDialog(IWindow* pOwner,NPrinter* pSetNPrinter)
   : IFrameWindow(IResourceId(ID_NPrintDialog)
                 ,IWindow::desktopWindow()
                 ,pOwner
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::noMoveWithOwner
                  |IFrameWindow::shellPosition
                 ,NApplication::pDLLResource->loadString(SI_PrintTitle))
   , ICommandHandler()
   , ISelectHandler()
   , IEditHandler()
   , IKeyboardHandler()
   , imccCanvas(ID_NPrintDialog+1,this,this)
   , istPrinter(ID_NPrintDialog+2,&imccCanvas,&imccCanvas)
   , icbCombo(ID_NPrintDialog+3,&imccCanvas,&imccCanvas,IRectangle()
             ,IWindow::visible|IBaseComboBox::readOnlyDropDownType)

   , istPrinterSize1(ID_NPrintDialog+10,&imccCanvas,&imccCanvas)
   , istCmWidth(ID_NPrintDialog+11,&imccCanvas,&imccCanvas)
   , istPrinterSize2(ID_NPrintDialog+12,&imccCanvas,&imccCanvas)
   , istCmHeight(ID_NPrintDialog+13,&imccCanvas,&imccCanvas)

   , istPrinterResolution1(ID_NPrintDialog+20,&imccCanvas,&imccCanvas)
   , istResWidth(ID_NPrintDialog+21,&imccCanvas,&imccCanvas)
   , istPrinterResolution2(ID_NPrintDialog+22,&imccCanvas,&imccCanvas)
   , istResHeight(ID_NPrintDialog+23,&imccCanvas,&imccCanvas)

   , istPages1(ID_NPrintDialog+30,&imccCanvas,&imccCanvas)
   , icbPages(ID_NPrintDialog+31,&imccCanvas,&imccCanvas)
   , iefWidth(ID_NPrintDialog+32,&imccCanvas,&imccCanvas)
   , istPages2(ID_NPrintDialog+33,&imccCanvas,&imccCanvas)
   , iefHeight(ID_NPrintDialog+34,&imccCanvas,&imccCanvas)

   , istZoom1(ID_NPrintDialog+40,&imccCanvas,&imccCanvas)
   , istZoom2(ID_NPrintDialog+41,&imccCanvas,&imccCanvas)
   , islZoom(ID_NPrintDialog+42,&imccCanvas,&imccCanvas,IRectangle()
             ,4,0,ISlider::classDefaultStyle|ISlider::snapToTickMark)

   , ipbPrint(ID_NPrintDialog+80,&imccCanvas,&imccCanvas,IRectangle()
             ,IWindow::visible|IPushButton::defaultButton)
   , ipbProperties(ID_NPrintDialog+81,&imccCanvas,&imccCanvas)
   , ipbCancel(ID_NPrintDialog+82,&imccCanvas,&imccCanvas)
{
   setFont(IFont("Helv",8));
   // 1st Row
   istPrinter.setText(NApplication::pDLLResource->loadString(SI_PrintPrinter));
   icbCombo  .enableTabStop();
   // 2nd Row
   istPrinterSize1.setText(NApplication::pDLLResource->loadString(SI_PrintPageSize))
                  .setAlignment(IStaticText::centerLeft);
   istCmWidth     .setLimit(4)
                  .setAlignment(IStaticText::centerRight)
                  .setForegroundColor(IColor(IColor::black));
   istPrinterSize2.setText(IString("x"))
                  .setAlignment(IStaticText::centerCenter);
   istCmHeight    .setLimit(4)
                  .setAlignment(IStaticText::centerRight)
                  .setForegroundColor(IColor(IColor::black));
   // 3rd Row
   istPrinterResolution1.setText(NApplication::pDLLResource->loadString(SI_PrintDPI))
                        .setAlignment(IStaticText::centerLeft);
   istResWidth          .setLimit(4)
                        .setAlignment(IStaticText::centerRight)
                        .setForegroundColor(IColor(IColor::black));
   istPrinterResolution2.setText(IString("x"))
                        .setAlignment(IStaticText::centerCenter);
   istResHeight         .setLimit(4)
                        .setAlignment(IStaticText::centerRight)
                        .setForegroundColor(IColor(IColor::black));
   // 4th Row
   istPages1.setText(NApplication::pDLLResource->loadString(SI_PrintPages))
            .setAlignment(IStaticText::centerLeft);
   icbPages .setText(NApplication::pDLLResource->loadString(SI_PrintFit))
            .setForegroundColor(istPages1.foregroundColor());
   iefWidth .setAlignment(IEntryField::right)
            .enableTabStop()
            .setMinimumSize(ISize());
   istPages2.setText(IString("x"))
            .setAlignment(IStaticText::centerCenter);
   iefHeight.setAlignment(IEntryField::right)
            .enableTabStop()
            .setMinimumSize(ISize());
   // 5th Row
   istZoom1.setText(NApplication::pDLLResource->loadString(SI_PrintZoom))
           .setAlignment(IStaticText::centerLeft);
   istZoom2.setLimit(4)
           .setText(IString("x1"))
           .setAlignment(IStaticText::centerRight);
   // 6th Row
   ipbPrint     .setText(NApplication::pDLLResource->loadString(SI_PrintDoPrint))
                .enableTabStop();
   ipbProperties.setText(NApplication::pDLLResource->loadString(SI_PrintDoProps));
   ipbCancel    .setText(NApplication::pDLLResource->loadString(SI_PrintDoCancel));
   // Setup Layout
   setClient(&imccCanvas);
   imccCanvas.addToCell(&istPrinter,2,2,1,1);
   imccCanvas.addToCell(&icbCombo,  3,2,4,1);

   imccCanvas.addToCell(&istPrinterSize1,2,4,2,1);
   imccCanvas.addToCell(&istCmWidth,     4,4,1,1);
   imccCanvas.addToCell(&istPrinterSize2,5,4,1,1);
   imccCanvas.addToCell(&istCmHeight,    6,4,1,1);

   imccCanvas.addToCell(&istPrinterResolution1,2,6,2,1);
   imccCanvas.addToCell(&istResWidth          ,4,6,1,1);
   imccCanvas.addToCell(&istPrinterResolution2,5,6,1,1);
   imccCanvas.addToCell(&istResHeight         ,6,6,1,1);

   imccCanvas.addToCell(&istPages1,2,8,1,1);
   imccCanvas.addToCell(&icbPages ,3,8,1,1);
   imccCanvas.addToCell(&iefWidth ,4,8,1,1);
   imccCanvas.addToCell(&istPages2,5,8,1,1);
   imccCanvas.addToCell(&iefHeight,6,8,1,1);

   imccCanvas.addToCell(&istZoom1,2,10,1,1);
   imccCanvas.addToCell(&istZoom2,3,10,1,1);
   imccCanvas.addToCell(&islZoom ,4,10,3,1);

   imccCanvas.addToCell(&ipbPrint,     2,12,1,1);
   imccCanvas.addToCell(&ipbProperties,3,12,1,1);
   imccCanvas.addToCell(&ipbCancel,    4,12,3,1);

   imccCanvas.setColumnWidth( 7,imccCanvas.defaultCell().width() );
   imccCanvas.setRowHeight  (13,imccCanvas.defaultCell().height());
   // Done
   moveSizeToClient(IRectangle(IPoint(0,0),imccCanvas.minimumSize()));
   moveTo(pOwner->position()+pOwner->size()/2-size()/2);
   // Init Queue Information  (should be after Combo has received a Size)
   pNPrinter=pSetNPrinter;
   if (pNPrinter->count()>0) {
      long oldCurrent=pNPrinter->getCurrentPrintQueueIdx();
      for (long i=1;i<=pNPrinter->count();i++) {
         pNPrinter->setCurrentPrintQueue(i);
         ULONG ulRc=icbCombo.addAscending(pNPrinter->queueName());
         icbCombo.setItemHandle(ulRc,(ULONG)i);
         if (i==oldCurrent)
            icbCombo.select(ulRc);
         // Next Queue
         }
      setInfo();
      }
   // Handler
   ICommandHandler::handleEventsFor(this);
   ISelectHandler::handleEventsFor(&icbCombo);
   ISelectHandler::handleEventsFor(&icbPages);
   IEditHandler::handleEventsFor(&islZoom);
   IEditHandler::handleEventsFor(&iefWidth);
   IEditHandler::handleEventsFor(&iefHeight);
   IKeyboardHandler::handleEventsFor(this);
}
NPrintDialog::~NPrintDialog() {}

void NPrintDialog::setInfo()
{
   IEditHandler::disable();
   ISelectHandler::disable();

   pNPrinter->setCurrentPrintQueue(icbCombo.itemHandle(icbCombo.selection()));
   // Enable/Disable
   if (pNPrinter->isRealPages()) {
      icbPages.select(False);
      iefWidth.disableDataUpdate()
              .setBackgroundColor(IColor(IColor::paleGray));
      iefHeight.disableDataUpdate()
               .setBackgroundColor(IColor(IColor::paleGray));
      islZoom.enable();
      }
   else {
      icbPages.select(True);
      iefWidth.enableDataUpdate()
              .resetBackgroundColor();
      iefHeight.enableDataUpdate()
               .resetBackgroundColor();
      islZoom.disable();
      }
   // mm Size
   ISize isSize=pNPrinter->getPageCmSize();
   istCmWidth.setText( IString(isSize.width() /100) +  ","
                      +IString(isSize.width() %100)       );
   istCmHeight.setText( IString(isSize.height()/100) +  ","
                       +IString(isSize.height()%100)       );
   // dpi Res
   ISize isRes=pNPrinter->resolution();
   istResWidth.setText( IString(isRes.width()) );
   istResHeight.setText( IString(isRes.height()) );
   // Pages Count
   if (pNPrinter->isRealPages()) {
      iefWidth.setText(IString(pNPrinter->getRealPages().width()));
      iefHeight.setText(IString(pNPrinter->getRealPages().height()));
      }
   else {
      iefWidth.setText(IString(pNPrinter->getPages().width()));
      iefHeight.setText(IString(pNPrinter->getPages().height()));
      }
   // Zoom (check for user zooming or wantedpage zooming)
   if (pNPrinter->isRealPages()) {
      istZoom2.setText("x"+IString(pNPrinter->getZoomX())+"  ");
      islZoom.moveArmToTick(pNPrinter->getZoomX()-1);
      }
   else {
      istZoom2.setText(IString("x-  "));
      islZoom.moveArmToTick(0);
      }
   // Done
   ISelectHandler::enable();
   IEditHandler::enable();
}

Boolean NPrintDialog::virtualKeyPress(IKeyboardEvent& event)
{
   if (event.virtualKey()==IKeyboardEvent::esc)
      ipbCancel.click();
   // I'm not the last handler
   return(False);
}

Boolean NPrintDialog::selected(IControlEvent& event)
{
   if (event.controlWindow()==&icbPages) {
      if (icbPages.isSelected())
         pNPrinter->setPages(pNPrinter->getRealPages());
      else
         pNPrinter->setPages(ISize(0,0));
      }
   setInfo();
   // I'm not the last handler
   return(False);
}
Boolean NPrintDialog::edit(IControlEvent& event)
{
   if ((event.controlWindow()==&iefWidth)||(event.controlWindow()==&iefHeight)) {
      long width=iefWidth.text().asInt();if (width<1) width=1;
      long height=iefHeight.text().asInt();if (height<1) height=1;
      pNPrinter->setPages(ISize(width,height));
      }
   if (event.controlWindow()==&islZoom) {
      pNPrinter->setZoom(islZoom.armTickOffset()+1,islZoom.armTickOffset()+1);
      }
   setInfo();
   // I'm not the last handler
   return(False);
}

Boolean NPrintDialog::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      // Print Button
      case ID_NPrintDialog+80 :
         dismiss(DID_OK);
         break;
      // Properties Button
      case ID_NPrintDialog+81 :
         pNPrinter->changeProperties();
         setInfo();
         break;
      // Cancel Button
      case ID_NPrintDialog+82 :
         dismiss(DID_CANCEL);
         break;
      }
   // Done everything
   return(True);
}


// Class   : NPrinter
// Purpose : Enable Printer Support for DeviceContexts
NPrinter::NPrinter()
{
   // Setup Printer Queue Data
   ppPrintQueue=NULL;
   lPrintQueueCount=0;
   lCurrentPrintQueue=0;

   hdcPrinter=NULL;
   hpsPrinter=NULL;
   igcPrinter=NULL;
   dZoomX=1;dZoomY=1;
   // Get count of total number of print queues and the size of the
   //  buffer needed (cbNeeded) to hold them.
   ULONG ulQueueTotalCount,ulQueueInfoCount,ulBytesNeeded,ulRc;
   ulRc=SplEnumQueue(NULL,                // pszComputerName - local machine
                     3,                   // information level 3 requested
                     NULL,                // pBuf - NULL for first call
                     0L,                  // cbBuf - 0 to get size needed
                     &ulQueueInfoCount,   // number of queues returned
                     &ulQueueTotalCount,  // total number of queues
                     &ulBytesNeeded,      // number bytes needed for info
                     NULL );              // reserved
   if (ulQueueTotalCount==0L) {
      cout <<  NApplication::pDLLResource->loadString(SI_PrintErrNoQueuesDef);
      return;
      }
   // Allocate memory to store the enumerated queue information
   // and call again to get print queue data
   PPRQINFO3 pQueueInfo=(PPRQINFO3)malloc(ulBytesNeeded);
   ulRc=SplEnumQueue(NULL,                // pszComputerName - local machine
                     3,                   // information level 3 requested
                     pQueueInfo,          // pBuf - is big enough now
                     ulBytesNeeded,       // cbBuf - size of pBuf
                     &ulQueueInfoCount,   // number of queues returned
                     &ulQueueTotalCount,  // total number of queues
                     &ulBytesNeeded,      // number bytes needed for info
                     NULL );              // reserved
   // Create NPrintQueues
   if (ulQueueTotalCount>0) {
      lPrintQueueCount=ulQueueInfoCount;
      ppPrintQueue=(PPNPrintQueue)malloc(lPrintQueueCount*sizeof(PNPrintQueue));
      for (lCurrentPrintQueue=1;lCurrentPrintQueue<=lPrintQueueCount;lCurrentPrintQueue++) {
         ppPrintQueue[lCurrentPrintQueue-1]=
         new NPrintQueue(IString(pQueueInfo[lCurrentPrintQueue-1].pszName)
                        ,IString(pQueueInfo[lCurrentPrintQueue-1].pszDriverName)
                        ,IString(pQueueInfo[lCurrentPrintQueue-1].pszComment)
                        ,        pQueueInfo[lCurrentPrintQueue-1].pDriverData);
         }
      // Set initial data afer successfull init
      lCurrentPrintQueue=1;
      }
   // free temporary
   free(pQueueInfo);
   // Done
}
NPrinter::~NPrinter()
{
   // Free Queues
   if (ppPrintQueue) {
      for (lCurrentPrintQueue=1;lCurrentPrintQueue<=lPrintQueueCount;lCurrentPrintQueue) {
         delete getCurrentPrintQueue();
         }
      free(ppPrintQueue);
      }
}

long NPrinter::getCurrentPrintQueueIdx()
{
   return(lCurrentPrintQueue);
}

PNPrintQueue NPrinter::getCurrentPrintQueue()
{
   if ((!ppPrintQueue)||(!lCurrentPrintQueue)) return(NULL);
   return(ppPrintQueue[lCurrentPrintQueue-1]);
}

Boolean NPrinter::setCurrentPrintQueue(long lNew)
{
   // get new PrintQueue
   if ((lNew<=0)||(lPrintQueueCount<lNew)) return(False);
   lCurrentPrintQueue=lNew;
   PNPrintQueue pQueue=getCurrentPrintQueue();
   if (!pQueue) return(False);
   // Initialize DEVOPENSTRUC for PrinterDeviceContext
   DEVOPENSTRUC dopPrinter;
   memset((PVOID)&dopPrinter, 0, sizeof(dopPrinter));
   dopPrinter.pszLogAddress = pQueue->getName();       // "HPLaserJ"
   dopPrinter.pdriv         = pQueue->getDriverData();
   IString isTemp           = pQueue->getDriverName(); // "LASERJET.HP LaserJet IIIP"
   if (isTemp.indexOf('.')>0) isTemp.remove(isTemp.indexOf('.'));
   dopPrinter.pszDriverName = isTemp;
   // Open device context for printer
   HDC hdcPrinter = DevOpenDC(IThread::current().anchorBlock(),
                              OD_INFO, // Get Information
                              (PSZ)"*",
                              3L,
                              (PDEVOPENDATA)&dopPrinter,
                              (HDC)NULLHANDLE);
   if (!hdcPrinter) {
      IMessageBox msgBox(NULL);
      msgBox.show(NApplication::pDLLResource->loadString(SI_PrintErrDevOpenFailed)
                  +pQueue->getName()
                 ,IMessageBox::catastrophic);
      lCurrentPrintQueue=0;
      return(False);
      }
   // Query Printer Characteristics
   ULONG ulRc=DevQueryCaps(hdcPrinter,
                           0L,
                           (LONG)39,
                           alDevInfo);
   if (!ulRc) {
      IMessageBox msgBox(NULL);
      msgBox.show(NApplication::pDLLResource->loadString(SI_PrintErrQueryCapsFailed)
                  +pQueue->getName()
                 ,IMessageBox::catastrophic);
      lCurrentPrintQueue=0;
      }
   // Close device context
   DevCloseDC(hdcPrinter);
   // Done
   return(ulRc!=0);
}

ULONG NPrinter::count()
{
   return(lPrintQueueCount);
}

void NPrinter::changeProperties()
{
   // Check for PrinterInfos
   PNPrintQueue pQueue=getCurrentPrintQueue();
   if (!pQueue) return;
   // Get Printer Data
   IString isName  =pQueue->getName();         // Queue Name "LaserJ"
   IString isDriver=pQueue->getDriverName();   // "LASERJET.HP LaserJet IIIP"
   IString isDevType;
   unsigned i;
   if ( (i=isDriver.indexOf('.')) > 0) {
      isDevType=isDriver.subString(i+1);
      isDriver.remove(i);
      }
   long newlen=
   DevPostDeviceModes(IThread::current().anchorBlock(),
                      NULL,                        // Query length
                      isDriver,                    // Driver Name
                      isDevType,                   // Device type
                      isName,                      // Queue Name "LaserJ"
                      DPDM_POSTJOBPROP);

   PDRIVDATA pData=pQueue->getDriverData(newlen);

   DevPostDeviceModes(IThread::current().anchorBlock(),
                      pData,                                    // Driver Data
                      isDriver,                                 // Driver Name
                      isDevType,                                // Device type
                      isName,                                   // Queue Name "LaserJ"
                      DPDM_POSTJOBPROP);
   // Done
}

IString NPrinter::queueName()
{
   // Check for PrinterInfos
   PNPrintQueue pQueue=getCurrentPrintQueue();
   if (!pQueue) return(IString());
   // Use printer comment if possible, else use queue name for display
   return(pQueue->getComment().length()>0 ?
          pQueue->getComment()          :
          pQueue->getName()               );
}

ISize NPrinter::getPageCmSize()
{
   // Check for PrinterInfos
   if (!lCurrentPrintQueue) return(ISize());
   // SIZE : Width in pels / pels per meter * 10000 = size in 0.1 millimeters
   ULONG ulTemp1=  alDevInfo[CAPS_WIDTH] * 10000 * 10
           / alDevInfo[CAPS_HORIZONTAL_RESOLUTION];
         ulTemp1=  (ulTemp1 + (ulTemp1%10 >=5 ? 10 : 0) ) / 10 -1;
   ULONG ulTemp2=  alDevInfo[CAPS_HEIGHT] * 10000 * 10
           / alDevInfo[CAPS_VERTICAL_RESOLUTION];
         ulTemp2= (ulTemp2 + (ulTemp2%10 >=5 ? 10 : 0) ) / 10 -1;
   // Done
   return(ISize(ulTemp1,ulTemp2));
}

ISize NPrinter::getPageSize()
{
   // Check for PrinterInfos
   if (!lCurrentPrintQueue) return(ISize());
   // SIZE : Width in pels / pels per meter * 10000 = size in 0.1 millimeters
/*
   ULONG ulTemp1=  alDevInfo[CAPS_WIDTH] * 10000 * 10
           / alDevInfo[CAPS_HORIZONTAL_RESOLUTION];
         ulTemp1=  (ulTemp1 + (ulTemp1%10 >=5 ? 10 : 0) ) / 10 -1;
   ULONG ulTemp2=  alDevInfo[CAPS_HEIGHT] * 10000 * 10
           / alDevInfo[CAPS_VERTICAL_RESOLUTION];
         ulTemp2= (ulTemp2 + (ulTemp2%10 >=5 ? 10 : 0) ) / 10 -1;
*/
   ULONG ulTemp1=  alDevInfo[CAPS_WIDTH]-1;
   ULONG ulTemp2=  alDevInfo[CAPS_HEIGHT]-1;
   // Done
   return(ISize(ulTemp1,ulTemp2));
}

Boolean NPrinter::isRealPages()
{
   return(isPages==ISize(0,0));
}

ISize NPrinter::getRealPages()
{
   // Check for PrinterInfos
   if (!lCurrentPrintQueue) return(ISize());
   // PAGES :
   ISize isPageSize=getPageSize();
   ULONG width=ceil( ((double)isDocumentSize.width()) * dZoomX / isPageSize.width() );
   ULONG height=ceil( ((double)isDocumentSize.height()) * dZoomY / isPageSize.height() );
   // Done
   return(ISize(width,height));
}

void NPrinter::setPages(const ISize& isSetPages)
{
   isPages=isSetPages;
}
ISize NPrinter::getPages()
{
   // check for adjusted pages
   if (isPages!=ISize(0,0))
      return(isPages);
   // Done
   return(getRealPages());
}
double NPrinter::getZoomX()
{
   // Check for adjusted size
   if (isPages!=ISize(0,0)) {
      return( ((double)getPageSize().width()) * isPages.width() / isDocumentSize.width() );
      }
   return(dZoomX);
}
double NPrinter::getZoomY()
{
   // Check for adjusted size
   if (isPages!=ISize(0,0)) {
      return( ((double)getPageSize().height()) * isPages.height() / isDocumentSize.height() );
      }
   return(dZoomY);
}
void NPrinter::setZoom(double dZoomSetX,double dZoomSetY)
{
   dZoomX=dZoomSetX;
   dZoomY=dZoomSetY;
}

ISize NPrinter::resolution()
{
   // Check for PrinterInfos
   if (!lCurrentPrintQueue) return(ISize());
   // RESOLUTION : Pels per meter / cm per meter * centimeter per inch
   ULONG ulTemp1= alDevInfo[CAPS_HORIZONTAL_RESOLUTION] * 2.539998 * 10 / 100;
         ulTemp1= (ulTemp1 + (ulTemp1%10 >=5 ? 10 : 0) ) / 10;
   ULONG ulTemp2= alDevInfo[CAPS_VERTICAL_RESOLUTION] * 2.539998 * 10 / 100;
         ulTemp2= (ulTemp2 + (ulTemp2%10 >=5 ? 10 : 0) ) / 10;
   // Done
   return(ISize(ulTemp1,ulTemp2));
}

Boolean NPrinter::startPrinting(IWindow* pOwner
                               ,const ISize& isSetDocumentSize
                               ,IString& isSetDocumentTitle)
{
   // Check for PrinterInfos (successfull init) or already started
   if ((!ppPrintQueue)||(hdcPrinter))
      return(False);
   // Remember
   isDocumentSize=isSetDocumentSize;
   isDocumentTitle=isSetDocumentTitle;
   // Setup NPrintDialog
   NPrintDialog npdDialog(pOwner,this);
   npdDialog.setFocus();
   npdDialog.showModally();
   if (npdDialog.result()!=DID_OK)
      return(False);
   // Done
   return(True);
}

void NPrinter::stopPrinting()
{
   // End Document
   DevEscape(hdcPrinter,DEVESC_ENDDOC,0L,(PSZ)NULL,(PLONG)NULL,(PSZ)NULL);
   // Close everything
   delete igcPrinter;
   GpiAssociate(hpsPrinter, (HDC)NULLHANDLE);
   GpiDestroyPS(hpsPrinter);
   DevCloseDC(hdcPrinter);
   hdcPrinter=NULL;
   igcPrinter=NULL;
}

IGraphicContext* NPrinter::graphicContext()
{
   // Check if already running
   if (igcPrinter)
      return(igcPrinter);
   // Get Queue
   PNPrintQueue pQueue=getCurrentPrintQueue();
   // Initialize DEVOPENSTRUC for PrinterDeviceContext
   DEVOPENSTRUC dopPrinter;
   memset((PVOID)&dopPrinter, 0, sizeof(dopPrinter));

   dopPrinter.pszLogAddress = pQueue->getName();    // "HPLaserJ"
   dopPrinter.pdriv         = pQueue->getDriverData();
   IString isTemp=IString(pQueue->getDriverName()); // "LASERJET.HP LaserJet IIIP"
   if (isTemp.indexOf('.')>0) isTemp.remove(isTemp.indexOf('.'));
   dopPrinter.pszDriverName = isTemp;

   dopPrinter.pszComment    = IString("Family Tree - Printing");
   dopPrinter.pszDataType   = (PSZ)"PM_Q_STD";
   dopPrinter.pszQueueProcParams = (PSZ)"";
   dopPrinter.pszSpoolerParams = (PSZ)"";
   // Open device context for printer
   hdcPrinter = DevOpenDC(IThread::current().anchorBlock(),
                          OD_QUEUED,
                          (PSZ)"*",
                          7L, // was 9
                          (PDEVOPENDATA)&dopPrinter,
                          (HDC)NULLHANDLE);
   if (!hdcPrinter) {
      IMessageBox msgBox(NULL);
      msgBox.show(NApplication::pDLLResource->loadString(SI_PrintErrDevOpenFailed)
                  +pQueue->getName()
                 ,IMessageBox::catastrophic);
      return(NULL);
      }
   // create presentation space
//   SIZEL sizel=ISize(0,0).asSIZEL();
   SIZEL sizel=getPageSize().asSIZEL();
   hpsPrinter = GpiCreatePS(IThread::current().anchorBlock(),
                            hdcPrinter,
                            &sizel,
                            PU_PELS | GPIA_ASSOC | GPIT_NORMAL);
   if (!hpsPrinter) {
      IMessageBox msgBox(NULL);
      msgBox.show(NApplication::pDLLResource->loadString(SI_PrintErrCreatePSFailed)
                 ,IMessageBox::catastrophic);
      DevCloseDC(hdcPrinter);
      hdcPrinter=NULL;
      return(NULL);
      }
   // Start Document
   IString isTitle=IString("FamilyTree\n")+isDocumentTitle;
   DevEscape(hdcPrinter,DEVESC_STARTDOC
            ,isTitle.size(),isTitle
            ,0L,(PSZ)NULL);
   // Make GraphicContext
   igcPrinter=new IGraphicContext(IPresSpaceHandle(hpsPrinter));
   // Done
   return(igcPrinter);
}

void NPrinter::newPage()
{
   DevEscape(hdcPrinter,DEVESC_NEWFRAME,0L,(PBYTE)NULL,(PLONG)NULL,(PBYTE)NULL );
}


