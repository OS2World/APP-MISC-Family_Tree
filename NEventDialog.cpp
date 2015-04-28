/*
   File   : EventDialog.cpp
   Purpose: Define Classes for the event list window

   IMessageBox mbox(this);
   mbox.show("Hey ! You've dropped "+which->getFirstName()+" "+which->getName()
            ,IMessageBox::informationIcon|IMessageBox::okButton|IMessageBox::moveable);
*/

#include <imenu.hpp>
#include <itimer.hpp>
#include <imsgbox.hpp>

#include "NMainWindow.hpp"
#include "NEventDialog.hpp"
#include "Main.hpp"
#include "FTree.h"

// Class  : NEventList
// Purpose: Window Class for showing events in a list
class NEventList : public NList,
                   public NMankindHandler,
                   public ICommandHandler,
                   public NDragDropHandler
{
public:
   NEventList(unsigned long windowId,IWindow* piwOwner,NMankind* pSetNMankind,short* pSetColWidth);
   virtual ~NEventList();

   virtual void eventChanged(NMankindHandler* pInitiator);
   virtual void eventAdded(PNEvent which,Boolean more);
   virtual void eventAdded(SEQOFNEvent& which);
   virtual void actualEventChanged(PNEvent pOld,PNEvent pNew);
   virtual void disconnentEvents();

   PNEvent calcEventToDrag(IPoint where);

   short getSortBy();
   void  setSortBy(short sSet);
   static NEventList* pNEventList;
protected:
   virtual short noOfRows();

   virtual Boolean  drawStart(short sStartRow);
   virtual void     drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before);
   virtual void     drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString);
   virtual void     drawEnd();

   void    click      (short sRow);
   void    doubleClick(short sRow);
   virtual IPopUpMenu* rowPopup   (short sRow);
   virtual IPopUpMenu* listPopup  ();

   virtual Boolean command(ICommandEvent& event);
private:
   virtual void columnHeaderClicked(short which);

   NMankind* pNMankind;

   IPrivateResource*  pLockSequences;
   Boolean            bDisabled;
   SEQOFNEvent         seqEvent;
   SEQOFNEvent::Cursor seqEventCursor;
   SEQOFNEvent::Cursor seqRowCursor;
   SEQOFNEvent::Cursor seqTempCursor;
   unsigned long      seqEventIdx;
   short              sSortBy;
};

NEventList* NEventList::pNEventList=NULL;

int _Optlink comparePNEvent(const void* key,const void* element) {
   switch (NEventList::pNEventList->getSortBy()) {
      // Name
      case 1:
         return( strcmpi( (*((PNEvent*)key))->getName() , (*((PNEvent*)element))->getName() ) );
      // FromDate
      case 2 :
         if ( (*((PNEvent*)key))->getFrom() < (*((PNEvent*)element))->getFrom() )
            return(-1);
         return(1);
      // ToDate
      case 3 :
         if ( (*((PNEvent*)key))->getTo() < (*((PNEvent*)element))->getTo() )
            return(-1);
         return(1);
      // Place
      case 4 :
         return( strcmpi( (*((PNEvent*)key))->getPlace() , (*((PNEvent*)element))->getPlace() ) );
      // ... one case left
      }
   // case 5 : Description
  return( strcmpi( (*((PNEvent*)key))->getDescription() , (*((PNEvent*)element))->getDescription() ) );
   // Done
}

NEventList::NEventList(unsigned long windowId,IWindow* piwOwner,NMankind* pSetNMankind,short* pSetColWidth)
   : NList(windowId,piwOwner)
   , NMankindHandler()
   , ICommandHandler()
   , NDragDropHandler()
   , seqEventCursor(seqEvent)
   , seqTempCursor(seqEvent)
   , seqRowCursor(seqEvent)
{
   // VARs
   pNMankind=pSetNMankind;
   pLockSequences=new IPrivateResource();
   seqEventIdx=0;
   sSortBy=1;
   pNEventList=this;
   setEmptyText(NApplication::pDLLResource->loadString(SI_ClickForEvent));
   // Column init
   for (short i=1;i<=5;i++)
      addColumn(NApplication::pDLLResource->loadString(SI_EventsName+i-1),pSetColWidth[i-1]);
   dataHasChanged();
   // Handlers
   pNMankind->addMankindHandler(this);
   ICommandHandler::handleEventsFor(this);
   NDragDropHandler::handleDragFor(listWindow(),NDragDropHandler::Event);
   // Done
}

PNEvent NEventList::calcEventToDrag(IPoint where)
{
   // Look for dragged on in visible area
   short clickRow=positionToRow(where);
   if (clickRow>noOfRows()) return(NULL);
   // Identify object
   seqTempCursor.copy(seqEventCursor);
   for (short i=seqEventIdx;i!=clickRow;i++)
       seqTempCursor.setToNext();
   PNEvent pevent=seqEvent.elementAt(seqTempCursor);
   // ... activate it
   click(clickRow);
   // Done
   return(pevent);
}

NEventList::~NEventList()
{
   pNMankind->removeMankindHandler(this);
   delete pLockSequences;
}

Boolean NEventList::command(ICommandEvent& event)
{
   // Analyse event
   switch (event.commandId()) {
      case MI_EventListNew :
         pNMankind->addEvent(NApplication::pDLLResource->loadString(SI_Event)
                             +" # "+IString(seqEvent.numberOfElements()+1));
         break;
      case MI_EventListDel :
         pNMankind->delEvent(pNMankind->getActualEvent());
         break;
      }
   // Done
   return(True);
}

void NEventList::columnHeaderClicked(short which)
{
   setSortBy(which);
}

short NEventList::getSortBy()
{
   return(sSortBy);
}
void NEventList::setSortBy(short sSet)
{
   // Remember sort
   sSortBy=max(1,sSet);
   if (seqEvent.numberOfElements()==0) return;
   // Lock list
   pLockSequences->lock();
   // Check for Sort necessary and invalidate Cursor
   if (seqEvent.numberOfElements()>1) {
      // Malloc and fill sort buffer
      PPNEvent ppevent=(PPNEvent)malloc(sizeof(PNEvent)*seqEvent.numberOfElements());
      SEQOFNEvent::Cursor tempCursor(seqEvent);
      unsigned long i=0;
      forCursor (tempCursor) ppevent[i++]=seqEvent.elementAt(tempCursor);
      seqEvent.removeAll();
      // Sort it
      qsort(ppevent,i,sizeof(PNEvent),comparePNEvent);
      // Refill
      for (unsigned long j=0;j<i;j++) seqEvent.add(ppevent[j]);
      // Done
      free(ppevent);
   }
   // Re-Prepare vars
   seqEventCursor.setToFirst();
   seqEventIdx=1;
   // Unlock list
   pLockSequences->unlock();
   // ... refresh window (if not more to come)
   dataHasChanged();
   // Done
}

void NEventList::eventAdded(PNEvent which,Boolean more)
{
   // Lock list
   pLockSequences->lock();
   // Find right place to add
   PNEvent pkey;
   forCursor (seqTempCursor) {
      pkey=seqEvent.elementAt(seqTempCursor);
      if (comparePNEvent(&which,&pkey)<0) break;
      }
   // ... and add it
   if (seqTempCursor.isValid()) {
      seqEvent.addAsPrevious(which,seqTempCursor);
      seqEventIdx++;
      }
   else seqEvent.add(which);
   // ... make a valid cursor
   if (seqEventIdx==0) seqEventIdx=1; // maybe first event at all
   seqEventCursor.setToFirst();
   for (short i=1;i<seqEventIdx;i++)
      seqEventCursor.setToNext();
   // ... release list
   pLockSequences->unlock();
   // ... refresh window (if not more to come)
   if (!more) dataHasChanged();
   // Done
}

void NEventList::eventAdded(SEQOFNEvent& which)
{
   // Lock list
   pLockSequences->lock();
   // add event
   seqEvent.addAllFrom(which);
   // ... make a valid cursor
   seqEventIdx=1;
   seqEventCursor.setToFirst();
   // ... release list
   pLockSequences->unlock();
   // ... resort list
   setSortBy(getSortBy());
   // Done
}

void NEventList::disconnentEvents()
{
   // Lock list
   pLockSequences->lock();
   // ... remove all
   seqEventIdx=0;
   seqEvent.removeAll();
   seqEventCursor.invalidate();
   // .. release list
   pLockSequences->unlock();
   // ... refresh window (if not more to come)
   dataHasChanged();
   // Done
}

short NEventList::noOfRows()
{
   return(seqEvent.numberOfElements());
}

Boolean NEventList::drawStart(short sStartRow)
{
   // Check for valid list
   if (!seqEventCursor.isValid()) return(False);
   // Lock list
   pLockSequences->lock();
   // Locate starting row
   while (seqEventIdx>sStartRow) {
      seqEventCursor.setToPrevious();
      seqEventIdx--;
      }
   while (seqEventIdx<sStartRow) {
      seqEventCursor.setToNext();
      seqEventIdx++;
   }
   // Ready to go
   return(True);
}
void NEventList::drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before)
{
   // Prepare Row ?
   if (before) {
      // Find Row
      seqRowCursor.copy(seqEventCursor);
      for (short i=seqEventIdx;i!=sRow;i++)
         seqRowCursor.setToNext();
      // Check for actual
      if (pNMankind->getActualEvent()==seqEvent.elementAt(seqRowCursor)) {
         igcContext.setPenColor(IColor(IColor::darkGreen));
         IGRectangle igr(irRect);
         igr.drawOn(igcContext);
         }
      // ... and prep color
      igcContext.setPenColor(IColor(IColor::black));
      }
   // Done for this row
}
void NEventList::drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString)
{
   // Prep drawing
   igsString.setClippingRect(irRect);
   IPoint pos(irRect.minXMinY()+IPoint(3,ifFont.maxDescender()));
   // ... calc Event
   PNEvent pevent=seqEvent.elementAt(seqRowCursor);

   switch (sColumn) {
      case 1 : // Name
               if (pevent->getName().length()==0) return;
               igsString.setText(pevent->getName());
               break;
      case 2 : // From
               if (pevent->getFrom().hasInfo())
                  pevent->getFrom().drawOn(igcContext,pos,ifFont,igsString);
               return;
      case 3 : // To
               if (pevent->getTo().hasInfo())
                  pevent->getTo().drawOn(igcContext,pos,ifFont,igsString);
               return;
      case 4 : // Place
               if (pevent->getPlace().length()==0) return;
               igsString.setText(pevent->getPlace());
               break;
      case 5 : // Description
               if (pevent->getDescription().length()==0) return;
               igsString.setText(pevent->getDescription());
               break;
      }
   // Drawing the string still to do
   igsString.moveTo(pos)
            .drawOn(igcContext);
   // Done
}

void NEventList::drawEnd()
{
   // Release list
   pLockSequences->unlock();
}

void NEventList::click(short sRow)
{
   // Look for clicked on in visible area
   seqTempCursor.copy(seqEventCursor);
   for (short i=seqEventIdx;i!=sRow;i++)
      seqTempCursor.setToNext();
   // ... activate it
   pNMankind->makeActualEvent(seqEvent.elementAt(seqTempCursor));
   // Done
}
void NEventList::doubleClick(short sRow)
{
}
IPopUpMenu* NEventList::rowPopup(short sRow)
{
   click(sRow);
   // Do the menu
   IPopUpMenu* pmenu=new IPopUpMenu(IResourceId(MI_EventList,*NApplication::pDLLResource),this);
   // Return menu
   return(pmenu);
}
IPopUpMenu* NEventList::listPopup()
{
   // Do the menu
   IPopUpMenu* pmenu=new IPopUpMenu(IResourceId(MI_EventList,*NApplication::pDLLResource),this);
   pmenu->disableItem(MI_EventListDel);
   // Return menu
   return(pmenu);
}

void NEventList::actualEventChanged(PNEvent pOld,PNEvent pNew)
{
   // Make sure, we have a valid sequence of men
   if (!seqEventCursor.isValid()) return;
   // Look for changees in visible field
   seqTempCursor.copy(seqEventCursor);
   for (short i=seqEventIdx;i<seqEventIdx+visibleRows();i++) {
      if ( (seqEvent.elementAt(seqTempCursor)==pOld)
         ||(seqEvent.elementAt(seqTempCursor)==pNew))
         redrawRow(i);
      seqTempCursor.setToNext();
      }

   // Check if we are too LOW now
   PNEvent pkey;long scrolled=1;
   SEQOFNEvent::Cursor loopCursor(seqEvent);
   loopCursor.copy(seqEventCursor);
   for (loopCursor.setToNext();loopCursor.isValid();loopCursor.setToNext()) {
      pkey=seqEvent.elementAt(loopCursor);
      if (pkey==pNew) {
         if (scrolled>=visibleRows())
            scrollVertical(scrolled-visibleRows()+1);
         return;
         }
      if (comparePNEvent(&pNew,&pkey)<0) break;
      scrolled++;
      }
   // Check if we are too HIGH now
   loopCursor.copy(seqEventCursor);
   for (loopCursor.setToPrevious();loopCursor.isValid();loopCursor.setToPrevious()) {
      pkey=seqEvent.elementAt(loopCursor);
      if (pkey==pNew) {
         scrollVertical(-scrolled);
         return;
         }
      if (comparePNEvent(&pNew,&pkey)>0) break;
      scrolled++;
      }
   // Done
}

void NEventList::eventChanged(NMankindHandler* pInitiator)
{
   // Lock list
   pLockSequences->lock();
   // Resort list (insert sort)
   PNEvent pcomp;
   PNEvent pdrop;
   Boolean dropped=False;
   SEQOFNEvent::Cursor posCursor(seqEvent),dropCursorLo(seqEvent),dropCursorHi(seqEvent);
   forCursor (posCursor) {
      dropCursorHi.copy(posCursor);
      while (1) {
         // ... get actual last event to be dropped
         pdrop=seqEvent.elementAt(dropCursorHi);
         // .. calc one before
         dropCursorLo.copy(dropCursorHi);
         dropCursorLo.setToPrevious();
         if (!dropCursorLo.isValid()) break;
         pcomp=seqEvent.elementAt(dropCursorLo);
         // ... compare
         if (comparePNEvent(&pdrop,&pcomp)>=0) break;
         seqEvent.replaceAt(dropCursorHi,pcomp);
         seqEvent.replaceAt(dropCursorLo,pdrop);
         dropCursorHi.setToPrevious();
         // .. flag dropping
         dropped=True;
         }
      // Next event
      }
   // Redraw all if item was dropped
   if (dropped) {
      pLockSequences->unlock();
      dataHasChanged();
      }
   // ... otherwise only changed
   else {
      // Look for changee in visible field
      posCursor.copy(seqEventCursor);
      for (short i=seqEventIdx;i<seqEventIdx+visibleRows();i++) {
         if (seqEvent.elementAt(posCursor)->changedThisTime())
            redrawRow(i);
         posCursor.setToNext();
         }
      // ... release list
      pLockSequences->unlock();
      }
   // Done
}

// Class  : NQueryInvolved
// Purpose: Ask for role of involved person

#define ID_CANVAS      1
#define ID_GRPPERSON   10
#define ID_GRPINVOLVED 20
#define ID_PERSON      11
#define ID_INVOLVED    21
#define ID_OKBUT       2
#define ID_CANCELBUT   3


NQueryInvolved::NQueryInvolved(long windowId,IWindow* pOwner,PNPerson pSetPerson,IString isSetRole)
   : IFrameWindow(windowId
                 ,NULL,pOwner,IRectangle()
                 ,IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder)
   , ICommandHandler()
   , imccCanvas       (ID_CANVAS     ,this,this)
   , igbInvolvedPerson(ID_GRPPERSON  ,&imccCanvas,&imccCanvas)
   , igbInvolvedRole  (ID_GRPINVOLVED,&imccCanvas,&imccCanvas)
   , iefInvolvedPerson(ID_PERSON     ,&imccCanvas,&imccCanvas)
   , iefInvolvedRole  (ID_INVOLVED   ,&imccCanvas,&imccCanvas)
   , ipbOK            (ID_OKBUT      ,&imccCanvas,&imccCanvas)
   , ipbCancel        (ID_CANCELBUT  ,&imccCanvas,&imccCanvas)
{
   pPerson=pSetPerson;
   // Layout
   igbInvolvedPerson.setText(NApplication::pDLLResource->loadString(SI_InvolvedPerson));
   igbInvolvedRole  .setText(NApplication::pDLLResource->loadString(SI_InvolvedAs));
   iefInvolvedPerson.disableDataUpdate()
                    .setText(pPerson->getNameDotFirstName())
                    .setBackgroundColor(IColor(IColor::paleGray));
   iefInvolvedRole  .setText(isSetRole)
                    .enableTabStop();
   ipbOK            .enableDefault()
                    .setText(NApplication::pDLLResource->loadString(SI_OK))
                    .enableTabStop();
   ipbCancel        .setText(NApplication::pDLLResource->loadString(SI_Cancel))
                    .enableTabStop();
   imccCanvas.setDefaultCell(ISize(8,8));
   imccCanvas.addToCell(&igbInvolvedPerson, 2, 2, 6, 4)
             .addToCell(&igbInvolvedRole  , 2, 6, 6, 4)
             .addToCell(&iefInvolvedPerson, 3, 4, 4, 1)
             .addToCell(&iefInvolvedRole  , 3, 8, 4, 1)
             .addToCell(&ipbOK            , 3,11      )
             .addToCell(&ipbCancel        , 5,11      )
             .setRowHeight  (12,imccCanvas.defaultCell().height())
             .setColumnWidth( 7,imccCanvas.defaultCell().width ());
   imccCanvas.setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
   // Handler
   ICommandHandler::handleEventsFor(this);
   // ... show it
   ISize size=imccCanvas.minimumSize();
   moveSizeToClient(IRectangle(IPoint(pOwner->rect().centerXCenterY()-size/2),size));
   setClient(&imccCanvas);
   iefInvolvedRole.setFocus();
}

NQueryInvolved::~NQueryInvolved() {}

Boolean NQueryInvolved::command(ICommandEvent& event)
{
   // ... investigate buttons
   switch (event.commandId()) {
      case ID_OKBUT :
         isResult=iefInvolvedRole.text();
         dismiss(True);
         break;
      case ID_CANCELBUT :
         dismiss(False);
         break;
      }
   // Done
   return(False);
}



// Class  : NInvolvedList
// Purpose: window class for showing persons involved in an event
class NInvolvedList : public NList,
                      public ICommandHandler
{
public:
   NInvolvedList(unsigned long windowId,IWindow* piwOwner);
   void  setEvent(PNEvent pSetEvent);
   short getActualIdx();
protected:
   virtual short noOfRows();

   virtual Boolean  drawStart(short sStartRow);
   virtual void     drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before);
   virtual void     drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString);
   virtual void     drawEnd();
   virtual short    rowHeight();
   virtual short    minColumnWidth(short sColumn);
   virtual Boolean  command(ICommandEvent& event);

   virtual IPopUpMenu* rowPopup(short sRow);
   virtual void        click(short sRow);
private:
   PNEvent   pEvent;
   IGBitmap igbLink;
   short    sActual;
};


#define BMP_WIDTH  12
#define BMP_HEIGHT 16

NInvolvedList::NInvolvedList(unsigned long windowId,IWindow* piwOwner)
   : NList(windowId,piwOwner)
   , igbLink(IResourceId(RES_BmpLinkMan))
{
   igbLink.setTransparentColor(IColor(0xff,0x00,0xff));
   pEvent=NULL;
   sActual=0;
   addColumn(NApplication::pDLLResource->loadString(SI_Person));
   addColumn(NApplication::pDLLResource->loadString(SI_InvolvedAs));
   setEmptyText(NApplication::pDLLResource->loadString(SI_DropPersonHere));
   ICommandHandler::handleEventsFor(this);
}


short NInvolvedList::getActualIdx()
{
   return(sActual);
}


void NInvolvedList::setEvent(PNEvent pSetEvent)
{
   pEvent=pSetEvent;
   sActual=1;
   dataHasChanged();
}

short NInvolvedList::minColumnWidth(short sColumn)
{
   return(3+BMP_WIDTH+2);
}

short NInvolvedList::rowHeight()
{
   return(max(NList::rowHeight(),BMP_HEIGHT));
}

short NInvolvedList::noOfRows()
{
   if (!pEvent) return(0);
   return(pEvent->numMan());
}

Boolean NInvolvedList::drawStart(short sStartRow)
{
   if ((!pEvent)||(pEvent->numMan()==0)) return(False);
   return(True);
}

void NInvolvedList::drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before)
{
   // Prepare Row ?
   if ((before)&&(sRow==sActual)) {
      igcContext.setPenColor(IColor(IColor::paleGray));
      IGRectangle igr(irRect);
      igr.drawOn(igcContext);
      igcContext.setPenColor(IColor(IColor::black));
      }
   // Done
}
void NInvolvedList::drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString)
{
   short sOffset=3;
   // Draw text
   IString isText;
   if (sColumn==1) {
      // Draw Link symbol
         igbLink.moveTo(irRect.minXCenterY()+IPoint(3,-BMP_HEIGHT/2+1))
                .drawOn(igcContext);
      sOffset+=BMP_WIDTH+2;
      // Prep text
      isText=pEvent->getMan(sRow)->getNameDotFirstName();
      }
   else
      isText=pEvent->getRoleOfMan(sRow);
   // Draw Text
   igsString.setClippingRect(irRect)
            .setText(isText)
            .moveTo(irRect.minXMinY()+IPoint(sOffset,ifFont.maxDescender()))
            .drawOn(igcContext);
   // Done
}
void NInvolvedList::drawEnd()
{
}

void NInvolvedList::click(short sRow)
{
   if (sRow!=sActual) {
      short tmp=sActual;
      sActual=sRow;
      redrawRow(tmp);
      redrawRow(sActual);
      }
}

IPopUpMenu* NInvolvedList::rowPopup(short sRow)
{
   click(sRow);
   // Do the menu
   IPopUpMenu* pmenu=new IPopUpMenu(IResourceId(MI_EventEdit,*NApplication::pDLLResource),this);
   if (sRow==1)
      pmenu->disableItem(MI_EventEditUp);
   if (pEvent->numMan()==sRow)
      pmenu->disableItem(MI_EventEditDown);
   // Return menu
   return(pmenu);
}

Boolean NInvolvedList::command(ICommandEvent& event)
{
   Boolean processed=False;
   // Check event control id
   switch (event.commandId()) {
      case MI_EventEditUp  :
         pEvent->swapMan(sActual,sActual-1);
         sActual--;
         processed=True;
         break;
      case MI_EventEditDown:
         pEvent->swapMan(sActual,sActual+1);
         sActual++;
         processed=True;
         break;
      }
   if (processed) {
     dataHasChanged();
     return(True);
     }
   // Done
   return(False);
}


// Class  : NEventEdit
// Purpose: Window Class for editing events
class NEventEdit : public NNotebook,
                   public NMankindHandler,
                   public NDragDropHandler,
                   public ICommandHandler
{
public:
   NEventEdit(unsigned long windowId,IWindow* piwOwner,NMankind* pSetNMankind,IFrameWindow* pSetFrame);
   virtual ~NEventEdit();
   void disableInput();
   void enableInput();

   virtual void manDropped(PNPerson which);
   virtual void eventChanged(NMankindHandler* pInitiator);
protected:
   virtual Boolean command(ICommandEvent& event);
private:
   void setMyEvent(PNEvent pSet);
   void triggerChange(unsigned long timer);

   virtual void eventAdded(PNEvent which,Boolean more);
   virtual void eventAdded(SEQOFNEvent& which);
   virtual void actualEventChanged(PNEvent pOld,PNEvent pNew);
   virtual void disconnentEvents();

   IMultiCellCanvas imccEventData;
   IStaticText istEvent;       NTextEntry    nteEvent;
   IStaticText istDateFrom;    NDateControl  ndcDateFrom;
   IStaticText istDateTo;      NDateControl  ndcDateTo;
   IStaticText istPlace;       NTextEntry    ntePlace;
   IStaticText istDescription; NTextEntry    nteDescription;
   NInvolvedList nilInvolved;
   NContainer  ncSources;

   NMankind* pNMankind;
   PNEvent    pMyEvent;

   ITimer    itTimer;
   Boolean   bDisabled;
   IFrameWindow* pFrame;
   NQueryInvolved* pQuery;
};


NEventEdit::NEventEdit(unsigned long windowId,IWindow* piwOwner,NMankind* pSetNMankind,IFrameWindow* pSetFrame)
   : NNotebook(windowId,piwOwner,piwOwner)
   , NMankindHandler()
   , NDragDropHandler()
   , ICommandHandler()
   , imccEventData  (1,this,this)
   ,  istEvent      (10,&imccEventData,&imccEventData,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,  istDateFrom   (11,&imccEventData,&imccEventData,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,  istDateTo     (12,&imccEventData,&imccEventData,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,  istPlace      (13,&imccEventData,&imccEventData,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,  istDescription(14,&imccEventData,&imccEventData,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,  nteEvent      (15,&imccEventData,&imccEventData)
   ,  ndcDateFrom   (16,&imccEventData,&imccEventData)
   ,  ndcDateTo     (17,&imccEventData,&imccEventData)
   ,  ntePlace      (18,&imccEventData,&imccEventData)
   ,  nteDescription(19,&imccEventData,&imccEventData)
   , nilInvolved     (2,this)
   , ncSources      (3,this,this)
   , itTimer        ()
{
   // VARs
   pFrame=pSetFrame;
   pNMankind=pSetNMankind;
   pQuery=NULL;
   pMyEvent=NULL;
   bDisabled=False;

   // Layout
   ncSources.setEmptyText("Not implemented !"); // Drop documented sources here !

   setPageBackgroundColor    (backgroundColor());
   setMajorTabBackgroundColor(backgroundColor());

   INotebook::PageSettings page(INotebook::PageSettings::autoPageSize|INotebook::PageSettings::majorTab|INotebook::PageSettings::statusTextOn);
   page.setTabBitmap(IResourceId(RES_BmpPageEvent));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_EventsTab1));
   addLastPage(page,&imccEventData);
   page.setTabBitmap(IResourceId(RES_BmpPagePersons));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_EventsTab2));
   addLastPage(page,&nilInvolved);
   page.setTabBitmap(IResourceId(RES_BmpPageDocsA));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_EventsTab3));
   addLastPage(page,&ncSources);

   setMajorTabSize(ISize(24+12,16+8));

   istEvent.setText      (NApplication::pDLLResource->loadString(SI_EventsName));
   istDateFrom.setText   (NApplication::pDLLResource->loadString(SI_EventsFrom));
   istDateTo.setText     (NApplication::pDLLResource->loadString(SI_EventsTo));
   istPlace.setText      (NApplication::pDLLResource->loadString(SI_EventsPlace));
   istDescription.setText(NApplication::pDLLResource->loadString(SI_EventsDescription));

   imccEventData.addToCell(&istEvent      ,2, 2,1,1)
                .addToCell(&istDateFrom   ,2, 4,1,1)
                .addToCell(&istDateTo     ,2, 6,1,1)
                .addToCell(&istPlace      ,2, 8,1,1)
                .addToCell(&istDescription,2,10,1,1)
                .addToCell(&nteEvent      ,4, 2,1,1)
                .addToCell(&ndcDateFrom   ,4, 4,1,1)
                .addToCell(&ndcDateTo     ,4, 6,1,1)
                .addToCell(&ntePlace      ,4, 8,1,1)
                .addToCell(&nteDescription,4,10,1,1)
                .setColumnWidth(4,0,True)
                .setColumnWidth(5,imccEventData.defaultCell().width())
                .setRowHeight  (11,imccEventData.defaultCell().height());
   // Handlers
   pNMankind->addMankindHandler(this);
   itTimer.start(new ITimerMemberFn<NEventEdit>(*this,NEventEdit::triggerChange),2*1000);
   NDragDropHandler::handleDropFor(nilInvolved.listWindow(),NDragDropHandler::Man);
   ICommandHandler::handleEventsFor(this);
   // Done
}

NEventEdit::~NEventEdit()
{
   pNMankind->removeMankindHandler(this);
   itTimer.stop();
}

void NEventEdit::eventAdded(PNEvent which,Boolean more)
{
   setMyEvent(pNMankind->getActualEvent());
   // Done for me
}
void NEventEdit::eventAdded(SEQOFNEvent& which)
{
   setMyEvent(pNMankind->getActualEvent());
   // Done for me
}

void NEventEdit::eventChanged(NMankindHandler* pInitiator)
{
   if ( (pInitiator==this)
      ||(!pMyEvent)
      ||(!pMyEvent->changedThisTime())) return;
   PNEvent ptmp=pMyEvent;
   pMyEvent=NULL;
   setMyEvent(ptmp);
}

void NEventEdit::actualEventChanged(PNEvent pOld,PNEvent pNew)
{
   // Change my event to reflect change of actual
   setMyEvent(pNew);
   // Done for me
}

void NEventEdit::disconnentEvents()
{
   pMyEvent=NULL;
   setMyEvent(NULL);
}


void NEventEdit::disableInput()
{
   bDisabled=True;
   nteEvent      .disableInput();
   ndcDateFrom   .disableInput();
   ndcDateTo     .disableInput();
   ntePlace      .disableInput();
   nteDescription.disableInput();
   nilInvolved     .disableInput();
   ncSources     .disableInput();
   if (pQuery) pQuery->dismiss();
}
void NEventEdit::enableInput()
{
   if (pMyEvent) {
      nteEvent      .enableInput();
      ndcDateFrom   .enableInput();
      ndcDateTo     .enableInput();
      ntePlace      .enableInput();
      nteDescription.enableInput();
      nilInvolved     .enableInput();
      ncSources     .enableInput();
      }
   bDisabled=False;
}

void NEventEdit::setMyEvent(PNEvent pSet)
{
   // Check for data of my actual event
   PNEvent pold=pMyEvent;
   if (pold) {
      pMyEvent=NULL;
      pNMankind->beginChange();
      if (nteEvent      .hasChanged()) pold->setName       (nteEvent      .text   ());
      if (ndcDateFrom   .hasChanged()) pold->setFrom       (ndcDateFrom   .getDate());
      if (ndcDateTo     .hasChanged()) pold->setTo         (ndcDateTo     .getDate());
      if (ntePlace      .hasChanged()) pold->setPlace      (ntePlace      .text   ());
      if (nteDescription.hasChanged()) pold->setDescription(nteDescription.text   ());
      pNMankind->endChange(this);
      }
   // Go for my new event
   if (pSet) {
      pMyEvent=pSet;
      if (pMyEvent!=pold) {
         nteEvent      .setText    (pMyEvent->getName       ());
         nteEvent      .enableInput();
         nteEvent      .hasChanged ();
         ndcDateFrom   .setDate    (pMyEvent->getFrom       ());
         ndcDateFrom   .enableInput();
         ndcDateFrom   .hasChanged ();
         ndcDateTo     .setDate    (pMyEvent->getTo         ());
         ndcDateTo     .enableInput();
         ndcDateTo     .hasChanged ();
         ntePlace      .setText    (pMyEvent->getPlace      ());
         ntePlace      .enableInput();
         ntePlace      .hasChanged ();
         nteDescription.setText    (pMyEvent->getDescription());
         nteDescription.enableInput();
         nteDescription.hasChanged ();
         nilInvolved   .setEvent   (pMyEvent);
         nilInvolved   .enableInput();
         ncSources     .enableInput();
         }
      }
   else {
         if (pQuery) pQuery->dismiss();
         nteEvent      .setText("")     ;
         nteEvent      .disableInput()  ;
         ndcDateFrom   .setDate(NDate());
         ndcDateFrom   .disableInput()  ;
         ndcDateTo     .setDate(NDate());
         ndcDateTo     .disableInput()  ;
         ntePlace      .setText("")     ;
         ntePlace      .disableInput()  ;
         nteDescription.setText("")     ;
         nteDescription.disableInput()  ;
         nilInvolved   .disableInput()  ;
         nilInvolved   .setEvent(NULL)  ;
         ncSources     .disableInput()  ;
      }
   // Done
}

void NEventEdit::triggerChange(unsigned long timer)
{
   if ((isValid())&&(isVisible)&&(!bDisabled)) setMyEvent(pMyEvent);
}

void NEventEdit::manDropped(PNPerson which)
{
   // Check if that man already belongs to the event
   for (short i=1;i<=pMyEvent->numMan();i++)
      if (pMyEvent->getMan(i)==which) return;
   // Prepare Dialog for involvement
   pQuery= new NQueryInvolved(IC_DEFAULT_FRAME_ID,pFrame,which);
   pQuery->showModally();
   if (pQuery->result()!=True) return;
   // Add new man to event
   pNMankind->beginChange();
   pMyEvent->addMan(which,pQuery->isResult);
   pNMankind->endChange(this);
   nilInvolved.setEvent(pMyEvent);
   // Done
}

Boolean NEventEdit::command(ICommandEvent& event)
{
   Boolean rc=False;
   switch (event.commandId()) {
      case MI_EventEditDel :
         // Remove man from event
         pNMankind->beginChange();
         pMyEvent->delMan(nilInvolved.getActualIdx());
         pNMankind->endChange(this);
         nilInvolved.setEvent(pMyEvent);
         rc=True;
         break;
      case MI_EventEditJump :
         pNMankind->makeActual(pMyEvent->getMan(nilInvolved.getActualIdx()));
         rc=True;
         break;
      case MI_EventEditChange :
         PNPerson pinvolved=pMyEvent->getMan(nilInvolved.getActualIdx());
         if (!pinvolved) break;
         // Prepare Dialog for involvement
         pQuery= new NQueryInvolved(IC_DEFAULT_FRAME_ID
                                   ,pFrame
                                   ,pinvolved
                                   ,pMyEvent->getRoleOfMan(nilInvolved.getActualIdx()));
         pQuery->showModally();
         if (pQuery->result()!=True) break;
         // Add new man to event
         pNMankind->beginChange();
         pMyEvent->changeRoleOfMan(nilInvolved.getActualIdx(),pQuery->isResult);
         pNMankind->endChange(this);
         nilInvolved.setEvent(pMyEvent);
         // Done
         rc=True;
         break;
      }
   // Done
   return(rc);
}



// Class  : NEventDialog
// Purpose: Dialog Window Class for Mankind-List
NEventDialog* NEventDialog::pNEventDialog=NULL;
IFont         NEventDialog::ifProfileFont("Helv",8);
IRectangle    NEventDialog::irProfileRectangle;
Boolean       NEventDialog::bProfileMaxed=False;
Boolean       NEventDialog::bProfileFloat=False;
short         NEventDialog::sProfileEventCols[5]={0,0,0,0,0};

void NEventDialog::loadFromProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("NEvents");
   irProfileRectangle=IRectangle(profile.integerWithKey("MINX")
                                ,profile.integerWithKey("MINY")
                                ,profile.integerWithKey("MAXX")
                                ,profile.integerWithKey("MAXY"));
   bProfileMaxed=profile.integerWithKey("MAXED");
   ifProfileFont=IFont(profile.elementWithKey("FONTNAME")
                      ,profile.integerWithKey("FONTSIZE"));
   for (short i=1;i<=5;i++)
      sProfileEventCols[i-1]=profile.integerWithKey("COL"+IString(i));
}
void NEventDialog::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("NEvents");
   profile.addOrReplaceElementWithKey("MINX",irProfileRectangle.minX());
   profile.addOrReplaceElementWithKey("MINY",irProfileRectangle.minY());
   profile.addOrReplaceElementWithKey("MAXX",irProfileRectangle.maxX());
   profile.addOrReplaceElementWithKey("MAXY",irProfileRectangle.maxY());
   profile.addOrReplaceElementWithKey("MAXED",bProfileMaxed);
   profile.addOrReplaceElementWithKey("FONTNAME",ifProfileFont.name());
   profile.addOrReplaceElementWithKey("FONTSIZE",ifProfileFont.pointSize());
   for (short i=1;i<=5;i++)
      profile.addOrReplaceElementWithKey("COL"+IString(i),sProfileEventCols[i-1]);
}

NEventDialog::NEventDialog(IWindow* piwOwner,NMankind* pSetNMankind)
   : IFrameWindow(IResourceId(ID_NEventsWindow)
                 ,IWindow::desktopWindow()
                 ,bProfileFloat ? NULL : piwOwner
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::sizingBorder
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::windowList
                  |IFrameWindow::minimizeButton
                  |IFrameWindow::maximizeButton
                  |IFrameWindow::noMoveWithOwner
                 ,NApplication::pDLLResource->loadString(SI_EventTitle))
   , IFrameHandler()

   , iscCanvas(1,this,this,IRectangle(),ISplitCanvas::vertical|IWindow::visible)

{
   setFont(IFont("Helv",8));
   pNMankind=pSetNMankind;
   setIcon(RES_IconFTree);
   setClient(&iscCanvas);
   bDisabled=False;
   setAutoDeleteObject(true);
   pNEventDialog=this;
   NMainWindow::pNMainWindow->ihwHelp.setAssociatedWindow(this);
   // Setup Layout
   pNEventList    =new NEventList(11,&iscCanvas,pSetNMankind,sProfileEventCols);
   pNEventEdit    =new NEventEdit(12,&iscCanvas,pSetNMankind,this);

   // Handler Stuff
   IFrameHandler::handleEventsFor(this);
   // SetUp Position & Size & Font
   if (  (irProfileRectangle.width()>0)
      && (irProfileRectangle.height()>0) )
      moveSizeTo(irProfileRectangle);
   else
      moveSizeTo(IRectangle(NMainWindow::pNMainWindow->rect().bottomCenter()
                           ,ISize(NMainWindow::pNMainWindow->rect().width()/2
                                 ,NMainWindow::pNMainWindow->rect().height())));
   if (bProfileMaxed)
      maximize();
   // Done
   show();
} /* end NEventDialog::NEventDialog */

NEventDialog::~NEventDialog()
{
   // Update profile information about widths
   for (short i=1;i<=5;i++)
      sProfileEventCols[i-1]=pNEventList->columnWidth(i);
   // Report destruction to MainWindow
   pNEventDialog=NULL;
   delete pNEventList;
   delete pNEventEdit;
} /* end NEventDialog::~NEventDialog */

void NEventDialog::changeFloat(Boolean set)
{
   bProfileFloat=set;
   if (pNEventDialog)
      pNEventDialog->setOwner( bProfileFloat ? NULL : NMainWindow::pNMainWindow );
}

Boolean NEventDialog::isFloat()
{
   return(bProfileFloat);
}


Boolean NEventDialog::saved(IFrameEvent& frameEvent)
{
   // Save Frame Status
   if ((!isMinimized())&&(!isMaximized()))
      irProfileRectangle=rect();
   bProfileMaxed=isMaximized();
   return(False);
}

Boolean NEventDialog::closed(IFrameEvent& frameEvent)
{
   if (bDisabled) return(True);
   // Done
   return(False);
} /* end NEventDialog::closed() */

void NEventDialog::disableInput()
{
   bDisabled=True;
   pNEventEdit->disableInput();
   pNEventList->disableInput();
}
void NEventDialog::enableInput()
{
   bDisabled=False;
   pNEventEdit->enableInput();
   pNEventList->enableInput();
}



