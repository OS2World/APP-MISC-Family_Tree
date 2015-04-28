/*
   File   : ManList.cpp
   Purpose: Define Class implementation
*/

#include "FTree.h"
#include "NManList.hpp"
#include "NMainWindow.hpp"

#define BMP_WIDTH  12
#define BMP_HEIGHT 16

unsigned long _System DosBeep(unsigned long,unsigned long);


// Class  : NManList
// Purpose: List all NPersons
NManList* NManList::pNManList=NULL;

NManList::NManList(unsigned long windowIdentifier
                  ,IWindow* piwParentOwner
                  ,NMankind* pSetNMankind
                  ,IFont& initialFont
                  ,short* pSetColWidth
                  ,IString isSetSortBy)
   : NList(windowIdentifier,piwParentOwner)
   , NMankindHandler()
   , NDragDropHandler()
   , seqManCursor(seqMan)
   , seqRowCursor(seqMan)
   , seqTempCursor(seqMan)
   , igbMale(IResourceId(RES_BmpSexMale))
   , igbFemale(IResourceId(RES_BmpSexFemale))
   , igbUnknown(IResourceId(RES_BmpSexUnknown))
{
   pNMankind=pSetNMankind;
   setSortBy(isSetSortBy);
   // remember Data
   pNManList=this;
   bDisabled=False;
   seqManIdx=0;
   pLockSequences=new IPrivateResource();
   setFont(initialFont);
   igbMale.setTransparentColor(IColor(0xff,0x00,0xff));
   igbFemale.setTransparentColor(IColor(0xff,0x00,0xff));
   igbUnknown.setTransparentColor(IColor(0xff,0x00,0xff));
   // Column init
   for (short i=1;i<=8;i++)
      addColumn(NApplication::pDLLResource->loadString(SI_ListId+i-1),pSetColWidth[i-1]);
   // Handlers
   pNMankind->addMankindHandler(this);
   NDragDropHandler::handleDragFor(listWindow(),NDragDropHandler::Man);
   // Done
}

NManList::~NManList()
{
   delete pLockSequences;
   pNMankind->removeMankindHandler(this);
   pNManList=NULL;
}

void NManList::click(short sRow)
{
   // Look for clicked on in visible area
   seqTempCursor.copy(seqManCursor);
   for (short i=seqManIdx;i!=sRow;i++)
      seqTempCursor.setToNext();
   // ... activate it
   NMankind::pNMankind->makeActual(seqMan.elementAt(seqTempCursor));
   // Done
}
void NManList::doubleClick(short sRow)
{
   NMankind::pNMankind->makeOrigin();
}

IPopUpMenu* NManList::rowPopup(short sRow)
{
   // Activate person under click
   click(sRow);
   // Do the menu
   IPopUpMenu* pmenu=new IPopUpMenu(IResourceId(MI_Man,*NApplication::pDLLResource),this);
   if (pmenu) NMainWindow::pNMainWindow->fillManMenu(pmenu);
   // Return menu
   return(pmenu);
}
IPopUpMenu* NManList::listPopup()
{
   // Do the menu
   IPopUpMenu* pmenu=new IPopUpMenu(IResourceId(MI_Manlist,*NApplication::pDLLResource),this);
   // Return menu
   return(pmenu);
}

Boolean NManList::drawStart(short sStartRow)
{
   if (!seqManCursor.isValid()) return(False);
   // Lock sequence of persons, so no other thread can disturb
   pLockSequences->lock();
   // Locate starting row
   while (seqManIdx>sStartRow) {
      seqManCursor.setToPrevious();
      seqManIdx--;
      }
   while (seqManIdx<sStartRow) {
      seqManCursor.setToNext();
      seqManIdx++;
   }
   // ... ready to draw
   return(True);
}

void NManList::drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before)
{
   // Prepare Row ?
   if (before) {
      // Find Row
      seqRowCursor.copy(seqManCursor);
      for (short i=seqManIdx;i!=sRow;i++)
         seqRowCursor.setToNext();
      PNPerson pman=seqMan.elementAt(seqRowCursor);
      // Check for actual
      if (pNMankind->getActual()==pman) {
         igcContext.setPenColor(IColor(IColor::red));
         IGRectangle igr(irRect);
         igr.drawOn(igcContext);
         igcContext.setPenColor(IColor(IColor::black));
         }
      if (pNMankind->getOrigin()==pman)
         igcContext.setPenColor(IColor(IColor::red));
      }
   // Finish Row ?
   else {
      igcContext.setPenColor(IColor(IColor::black));
      }
   // Done
}

void NManList::drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString)
{
   // Calc man
   PNPerson pman=seqMan.elementAt(seqRowCursor);
   // Draw Information
   IGBitmap* pbmp=&igbUnknown;
   igsString.setClippingRect(irRect);
   IPoint pos(irRect.minXMinY()+IPoint(3,ifFont.maxDescender()));
   switch (sColumn) {
      case 1 : { // MID
               IString mid(pman->getMid());
               igsString.setText(mid);
               pos=irRect.maxXMinY()+IPoint(-3-ifFont.textWidth(mid),ifFont.maxDescender());
               } break;
      case 2 : // Sex
               switch (pman->getSex()) {
                  case 0: pbmp=&igbUnknown;break;
                  case 1: pbmp=&igbMale;   break;
                  case 2: pbmp=&igbFemale; break;
                  }
               pbmp->moveTo(irRect.centerXCenterY()-IPoint(BMP_WIDTH/2,BMP_HEIGHT/2-1))
                    .drawOn(igcContext);
               return;
      case 3 : // Name
               if (pman->getName().length()==0) return;
               igsString.setText(pman->getName());
               break;
      case 4 : // FirstName
               if (pman->getFirstName().length()==0) return;
               igsString.setText(pman->getFirstName());
               break;
      case 5 : // BirthDate
               if (pman->getBirthDate().hasInfo())
                  pman->getBirthDate().drawOn(igcContext,pos,ifFont,igsString);
               return;
      case 6 : // BirthPlace
               if (pman->getBirthPlace().length()==0) return;
               igsString.setText(pman->getBirthPlace());
               break;
      case 7 : // DeathDate
               if (pman->getDeathDate().hasInfo())
                  pman->getDeathDate().drawOn(igcContext,pos,ifFont,igsString);
               return;
      case 8 : // DeathPlace
               if (pman->getDeathPlace().length()==0) return;
               igsString.setText(pman->getDeathPlace());
               break;
      }
   // Drawing the string still to do
   igsString.moveTo(pos)
            .drawOn(igcContext);
   // Done
}

void NManList::drawEnd()
{
   // Unlock sequence of persons ... other threads won't disturb now
   pLockSequences->unlock();
}

int _Optlink comparePNPerson(const void* key,const void* element) {
   char* psort=NManList::pNManList->isSortBy;
   int rc;

   while (*psort!=0) {
      switch (*psort) {
         // ID
         case 1:
            if ( (*((PNPerson*)key))->getMid()     < (*((PNPerson*)element))->getMid() )
               return(-1);
            if ( (*((PNPerson*)element))->getMid() < (*((PNPerson*)key))->getMid()     )
               return(1);
            break;
         // Sex
         case 2:
            if ( (*((PNPerson*)key))->getSex()
               < (*((PNPerson*)element))->getSex() )
               return(-1);
            if ( (*((PNPerson*)element))->getSex()
               < (*((PNPerson*)key))->getSex()     )
               return(1);
            break;
         // Name
         case 3:
            rc=strcmpi( (*((PNPerson*)key))->getName() , (*((PNPerson*)element))->getName() );
            if (rc!=0) return(rc);
            break;
         // FirstName
         case 4 :
            rc=strcmpi( (*((PNPerson*)key))->getFirstName() , (*((PNPerson*)element))->getFirstName() );
            if (rc!=0) return(rc);
            break;
         // BirthDate
         case 5 :
            if ( (*((PNPerson*)key))->getBirthDate()
               < (*((PNPerson*)element))->getBirthDate() )
               return(-1);
            if ( (*((PNPerson*)element))->getBirthDate()
               < (*((PNPerson*)key))->getBirthDate()     )
               return(1);
            break;
         // BirthPlace
         case 6 :
            rc=strcmpi( (*((PNPerson*)key))->getBirthPlace() , (*((PNPerson*)element))->getBirthPlace() );
            if (rc!=0) return(rc);
            break;
         // DeathDate
         case 7 :
            if ( (*((PNPerson*)key))->getDeathDate()
               < (*((PNPerson*)element))->getDeathDate() )
               return(-1);
            if ( (*((PNPerson*)element))->getDeathDate()
               < (*((PNPerson*)key))->getDeathDate()     )
               return(1);
            break;
         // BirthPlace
         case 8 :
            rc=strcmpi( (*((PNPerson*)key))->getDeathPlace() , (*((PNPerson*)element))->getDeathPlace() );
            if (rc!=0) return(rc);
            break;
         // That's all
         }
      psort++;
      }
   return(0);
}

void NManList::columnHeaderClicked(short which)
{
   setSortBy(IString((char)which));
}

void NManList::setSortBy(const IString& isSetSort)
{
   // Remember sort
   isSortBy=isSetSort;
   if (isSortBy.length()==0) isSortBy=IString((char)1);
   if (seqMan.numberOfElements()==0) return;
   // Lock list
   pLockSequences->lock();
   // Check for Sort necessary and invalidate Cursor
   if (seqMan.numberOfElements()>1) {
      // Malloc and fill sort buffer
      PPNPerson ppman=(PPNPerson)malloc(sizeof(PNPerson)*seqMan.numberOfElements());
      SEQOFNPerson::Cursor tempCursor(seqMan);
      unsigned long i=0;
      forCursor (tempCursor) ppman[i++]=seqMan.elementAt(tempCursor);
      seqMan.removeAll();
      // Sort it
      qsort(ppman,i,sizeof(PNPerson),comparePNPerson);
      // Refill
      for (unsigned long j=0;j<i;j++) seqMan.add(ppman[j]);
      // Done
      free(ppman);
   }
   // Re-Prepare vars
   seqManCursor.setToFirst();
   seqManIdx=1;
   // Unlock list
   pLockSequences->unlock();
   // Done
   dataHasChanged();
}
const IString& NManList::getSortBy()
{
   return(isSortBy);
}


short NManList::rowHeight()
{
   return(max(NList::rowHeight(),BMP_HEIGHT));
}

short NManList::minColumnWidth(short sColumn)
{
   if (sColumn==2) return(1+BMP_WIDTH+1);
   return(0);
}


short NManList::noOfRows()
{
   return(seqMan.numberOfElements());
}


void NManList::manChanged(NMankindHandler* pInitiator)
{
   // Lock list
   pLockSequences->lock();
   // Resort list (insert sort)
   PNPerson pcomp;
   PNPerson pdrop;
   Boolean dropped=False;
   SEQOFNPerson::Cursor posCursor(seqMan),dropCursorLo(seqMan),dropCursorHi(seqMan);
   forCursor (posCursor) {
      dropCursorHi.copy(posCursor);
      while (1) {
         // ... get actual last man to be dropped
         pdrop=seqMan.elementAt(dropCursorHi);
         // .. calc one before
         dropCursorLo.copy(dropCursorHi);
         dropCursorLo.setToPrevious();
         if (!dropCursorLo.isValid()) break;
         pcomp=seqMan.elementAt(dropCursorLo);
         // ... compare
         if (comparePNPerson(&pdrop,&pcomp)>=0) break;
         seqMan.replaceAt(dropCursorHi,pcomp);
         seqMan.replaceAt(dropCursorLo,pdrop);
         dropCursorHi.setToPrevious();
         // .. flag dropping
         dropped=True;
         }
      // Next man
      }
   // Redraw all if item was dropped
   if (dropped) {
      pLockSequences->unlock();
      dataHasChanged();
      }
   // ... otherwise only changed
   else {
      // Look for changee in visible field
      if (isVisible()) {
         posCursor.copy(seqManCursor);
         for (short i=seqManIdx;i<seqManIdx+visibleRows();i++) {
            if (seqMan.elementAt(posCursor)->changedThisTime())
               redrawRow(i);
            posCursor.setToNext();
            }
         }
      // ... release list
      pLockSequences->unlock();
      }
   // Done
}

void NManList::manAdded(PNPerson which,Boolean more)
{
   // Lock list
   pLockSequences->lock();
   // Find right place to add
   SEQOFNPerson::Cursor tempCursor(seqMan);
   PNPerson pkey;
   forCursor (tempCursor) {
      pkey=seqMan.elementAt(tempCursor);
      if (comparePNPerson(&which,&pkey)<0) break;
      }
   // ... and add it
   if (tempCursor.isValid()) {
      seqMan.addAsPrevious(which,tempCursor);
      seqManIdx++;
      }
   else seqMan.add(which);
   // ... make a valid cursor
   seqManCursor.setToFirst();
   for (short i=1;i<seqManIdx;i++)
      seqManCursor.setToNext();
   // ... release list
   pLockSequences->unlock();
   // ... refresh window (if not more to come)
   if (!more) dataHasChanged();
   // Done
}

void NManList::manAdded(SEQOFNPerson& which)
{
   // Lock list
   pLockSequences->lock();
   // ... add new
   seqMan.addAllFrom(which);
   seqManCursor.setToFirst();
   seqManIdx=1;
   // ... release list
   pLockSequences->unlock();
   // ... resort list
   setSortBy(getSortBy());
   // Done
}

void NManList::disconnentMen()
{
   // Lock list
   pLockSequences->lock();
   // ... remove old
   seqMan.removeAll();
   seqManCursor.invalidate();
   seqManIdx=1;
   // ... update count
   pLockSequences->unlock();
   // ... refresh window (if not more to come)
   dataHasChanged();
}
void NManList::actualChanged(PNPerson pOld,PNPerson pNew)
{
   // Make sure, we have a valid sequence of men
   if (!seqManCursor.isValid()) return;
   if (!isVisible()) return;
   // Look for changees in visible field
   seqTempCursor.copy(seqManCursor);
   for (short i=seqManIdx;i<seqManIdx+visibleRows();i++) {
      if ( (seqMan.elementAt(seqTempCursor)==pOld)
         ||(seqMan.elementAt(seqTempCursor)==pNew))
         redrawRow(i);
      seqTempCursor.setToNext();
      }
   // Check if we are too LOW now
   PNPerson pkey;long scrolled=1;
   SEQOFNPerson::Cursor loopCursor(seqMan);
   loopCursor.copy(seqManCursor);
   for (loopCursor.setToNext();loopCursor.isValid();loopCursor.setToNext()) {
      pkey=seqMan.elementAt(loopCursor);
      if (pkey==pNew) {
         if (scrolled>=visibleRows())
            scrollVertical(scrolled-visibleRows()+1);
         return;
         }
      if (comparePNPerson(&pNew,&pkey)<0) break;
      scrolled++;
      }
   // Check if we are too HIGH now
   loopCursor.copy(seqManCursor);
   for (loopCursor.setToPrevious();loopCursor.isValid();loopCursor.setToPrevious()) {
      pkey=seqMan.elementAt(loopCursor);
      if (pkey==pNew) {
         scrollVertical(-scrolled);
         return;
         }
      if (comparePNPerson(&pNew,&pkey)>0) break;
      scrolled++;
      }
   // Done
}
void NManList::originChanged(PNPerson pOld,PNPerson pNew)
{
   // Look for changees in visible field
   seqTempCursor.copy(seqManCursor);
   for (short i=seqManIdx;i<seqManIdx+visibleRows();i++) {
      if ( (seqMan.elementAt(seqTempCursor)==pOld)
         ||(seqMan.elementAt(seqTempCursor)==pNew))
         redrawRow(i);
      seqTempCursor.setToNext();
      }
   // Done
}

PNPerson NManList::calcManToDrag(IPoint where)
{
   // Look for dragged on in visible area
   short clickRow=positionToRow(where);
   if (clickRow>noOfRows()) return(NULL);
   // Identify object
   seqTempCursor.copy(seqManCursor);
   for (short i=seqManIdx;i!=clickRow;i++)
       seqTempCursor.setToNext();
   PNPerson pman=seqMan.elementAt(seqTempCursor);
   // ... activate it
   pNMankind->makeActual(pman);
   // Done
   return(pman);
}

