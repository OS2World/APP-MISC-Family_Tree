/*
   File   : EditDialog.cpp
   Purpose: Define Classes for the main Window
*/


#include "ihandle.hpp"

#include "NEditDialog.hpp"
#include "NUserFields.hpp"
#include "Main.hpp"
#include "FTree.h"


/*
  +--------------+------------------+
 1|NAME          | Meier...... |Sex |
 3| First name   | Nils....... |[M] |
 5|BIRTH         | 25.05.1970       |
 7| Place        | Rendsburg....... |
 9|DEATH         | --.--.----       |
11| Place        | ................ |
13|MARRAIAGE     | --.--.----  [1/1]|
15| Place        | Dren........... |
17| End          | --.--.----       |
19|OCCUPATION    | ................ |
21|ADDRESS       | ................ |
23|MEMO          | ................ |
  |              | ................ |
  |              | ................ |
  +--------------+------------------+
*/

// Class  : NManEditEventList
// Purpose: Window Class for NPerson's involvement in events
class NManEditEventList : public NList,
                          public ICommandHandler
{
public:
   NManEditEventList(unsigned long windowId,IWindow* ownerparentWnd,long colMetric=0);
   void setMan(PNPerson pSetMan);
   short getActualIdx();
   long getColMetric();
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
   PNPerson     pMan;
   IGBitmap igbLink;
   short    sActual;
};


#define BMP_WIDTH  15
#define BMP_HEIGHT 16


NManEditEventList::NManEditEventList(unsigned long windowId,IWindow* ownerparentWnd,long colMetric)
   : NList(windowId,ownerparentWnd)
   , igbLink(IResourceId(RES_BmpLinkEvent))
{
   setAutoDeleteObject(true);
   igbLink.setTransparentColor(IColor(0xff,0x00,0xff));
   pMan=NULL;
   sActual=0;
   setEmptyText(NApplication::pDLLResource->loadString(SI_DropEventHere));
   addColumn(NApplication::pDLLResource->loadString(SI_Event)     ,(colMetric>>16)&0xffff);
   addColumn(NApplication::pDLLResource->loadString(SI_InvolvedAs),(colMetric>> 0)&0xffff);
   ICommandHandler::handleEventsFor(this);
}

long NManEditEventList::getColMetric()
{
   return((columnWidth(1)<<16)+columnWidth(2));
}

void NManEditEventList::setMan(PNPerson pSetMan)
{
   pMan=pSetMan;
   sActual=1;
   dataHasChanged();
}

short NManEditEventList::getActualIdx()
{
   return(sActual);
}


short NManEditEventList::minColumnWidth(short sColumn)
{
   return(3+BMP_WIDTH+2);
}

short NManEditEventList::rowHeight()
{
   return(max(NList::rowHeight(),BMP_HEIGHT));
}

short NManEditEventList::noOfRows()
{
   if (!pMan) return(0);
   return(pMan->numEvents());
}

Boolean NManEditEventList::drawStart(short sStartRow)
{
   if ((!pMan)||(pMan->numEvents()==0)) return(False);
   return(True);
}

void NManEditEventList::drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before)
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
void NManEditEventList::drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString)
{
   // Draw actual idx
   short xoffset=3;
   // Draw text
   IString isText;
   if (sColumn==1) {
      // Draw Link symbol
      igbLink.moveTo(irRect.minXCenterY()+IPoint(xoffset,-BMP_HEIGHT/2+1))
             .drawOn(igcContext);
      xoffset+=BMP_WIDTH+2;
      // prep text
      isText=pMan->getEvent(sRow)->getName();
      }
   else
      isText=pMan->getEvent(sRow)->getRoleOfMan(pMan);
   // Draw Text
   igsString.setClippingRect(irRect)
            .setText(isText)
            .moveTo(irRect.minXMinY()+IPoint(xoffset,ifFont.maxDescender()))
            .drawOn(igcContext);
   // Done
}
void NManEditEventList::drawEnd()
{
}

void NManEditEventList::click(short sRow)
{
   if (sRow!=sActual) {
      short tmp=sActual;
      sActual=sRow;
      redrawRow(tmp);
      redrawRow(sActual);
      }
}

IPopUpMenu* NManEditEventList::rowPopup(short sRow)
{
   click(sRow);
   // Do the menu
   IPopUpMenu* pmenu=new IPopUpMenu(IResourceId(MI_ManEditEventList,*NApplication::pDLLResource),this);
   // Return menu
   return(pmenu);
}

Boolean NManEditEventList::command(ICommandEvent& event)
{
   // Done
   return(False);
}


// Class  : NManEdit
// Purpose: Window Class for NPerson's involvement in events
class NManEdit : public NNotebook,
                 public ICommandHandler,
                 public NMankindHandler,
                 public IResizeHandler,
                 public NDragDropHandler
{
public:
   NManEdit(unsigned long windowId,IWindow* piwOwner,IFrameWindow* pSetFrame
           ,NMankind* pSetNMankind
           ,Boolean bSetRelativePaths
           ,Boolean bSetWordWrap
           ,Boolean bSetFocusGrab
           ,const IFont& ifSet
           ,long userListMetric
           ,long eventListMetric);
   ~NManEdit();

   void    disableInput();
   void    enableInput();

   virtual void eventDropped(PNEvent which);

   virtual void doChangesNow();
   virtual void manChanged(NMankindHandler* pInitiator);
   virtual void manAdded(PNPerson which,Boolean more);
   virtual void manAdded(SEQOFNPerson& which);
   virtual void disconnentMen();

   virtual void actualChanged(PNPerson pOld,PNPerson pNew);
   virtual void originChanged(PNPerson pOld,PNPerson pNew);
   virtual void structureChanged();

   long    getUserListMetric();
   long    getEventListMetric();

   Boolean bFocusGrab;
protected:
   virtual Boolean command(ICommandEvent& event);
   virtual Boolean windowResize(IResizeEvent& event);
private:
   // Functions
   void   setMan(NPerson* pmanSet);
   void   storeInfo();
   void   loadInfo();
   void   clear();
   void triggerChange(unsigned long timer);

   // Data
   IFlyText            iFlyText;
   IFlyOverHelpHandler iFlyOverHelpHandler;

   NPerson*            pmanMan;
   short int           iMarriage;
   Boolean             bDisabled;
   NMankind*           pNMankind;
   ITimer              itTimer;
   IFrameWindow*       pFrame;
   NQueryInvolved*     pQuery;

   // PAGE: Standard Fields
   IViewPort ivpPerson;
   IMultiCellCanvas imccPerson;
   IStaticText istName;          NTextEntry nteName;         IStaticText istSex;
   IStaticText istFirstName;     NTextEntry nteFirstName;    NButton nbSex;
   IStaticText istBirthDate;     NDateControl  ndcBirthDate;
   IStaticText istBirthPlace;    NTextEntry nteBirthPlace;
   IStaticText istDeathDate;     NDateControl  ndcDeathDate;
   IStaticText istDeathPlace;    NTextEntry    nteDeathPlace;
   IStaticText istMarriageDate;  NDateControl  ndcMarriageDate; NButton       nbMarriage;
   IStaticText istMarriagePlace; NTextEntry    nteMarriagePlace;
   IStaticText istDivorceDate;   NDateControl  ndcDivorceDate;
   IStaticText istOccupation;    NTextEntry    nteOccupation;
   IStaticText istAddress;       NTextEntry    nteAddress;
   IStaticText istMemo;          NMemoField    nmfMemo;
   IStaticText istPicture;       NBitmapControl nbcPicture;
   // PAGE: User Fields
   NUserFields* pUserFields;
   // PAGE: Documented Sources
   NContainer ncDocumentedSources;
   // PAGE: Events
   NManEditEventList* pelEvents;
   // PAGE: Files Attachments
   IViewPort ivpAttach;
   IMultiCellCanvas imccAttach;
   NFileBox nfbFiles;
   // No more pages
};


// Specific IDs for commands (<10)
#define BUT_SEX      1000
#define BUT_MARRIAGE 1001


NManEdit::NManEdit(unsigned long windowId,IWindow* piwOwner,IFrameWindow* pSetFrame
                  ,NMankind* pSetNMankind
                  ,Boolean bSetRelativePaths
                  ,Boolean bSetWordWrap
                  ,Boolean bSetFocusGrab
                  ,const IFont& ifSet
                  ,long userListMetric
                  ,long eventListMetric)
   : NNotebook(windowId,piwOwner,piwOwner)
   , ICommandHandler()
   , NMankindHandler()
   , IResizeHandler()
   , itTimer()
   , iFlyText(9,piwOwner)
   , iFlyOverHelpHandler(&iFlyText)
   // PAGE: Person
   ,ivpPerson       (10,this,this,IRectangle(),IViewPort::classDefaultStyle|IViewPort::noViewWindowFill)
   ,imccPerson      (11,&ivpPerson,&ivpPerson,IRectangle(IPoint(0,0),ISize(10,10)),IWindow::noStyle)
   ,istName         (12,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,nteName         (13,&imccPerson,&imccPerson)
   ,istSex          (14,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,istFirstName    (15,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,nteFirstName    (16,&imccPerson,&imccPerson)
   ,nbSex           (BUT_SEX,&imccPerson,&imccPerson,True)
   ,istBirthDate    (18,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,ndcBirthDate    (19,&imccPerson,&imccPerson)
   ,istBirthPlace   (20,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,nteBirthPlace   (21,&imccPerson,&imccPerson)
   ,istDeathDate    (22,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,ndcDeathDate    (23,&imccPerson,&imccPerson)
   ,istDeathPlace   (24,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,nteDeathPlace   (25,&imccPerson,&imccPerson)
   ,istMarriageDate (26,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,ndcMarriageDate (27,&imccPerson,&imccPerson)
   ,nbMarriage      (BUT_MARRIAGE,&imccPerson,&imccPerson,True)
   ,istMarriagePlace(29,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,nteMarriagePlace(30,&imccPerson,&imccPerson)
   ,istDivorceDate  (31,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,ndcDivorceDate  (32,&imccPerson,&imccPerson)
   ,istOccupation   (33,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,nteOccupation   (34,&imccPerson,&imccPerson)
   ,istAddress      (35,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::vertCenter|IStaticText::left)
   ,nteAddress      (36,&imccPerson,&imccPerson)
   ,istMemo         (37,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::top|IStaticText::left)
   ,nmfMemo         (38,&imccPerson)
   ,istPicture      (39,&imccPerson,&imccPerson,IRectangle(),IWindow::visible|IStaticText::top|IStaticText::left)
   ,nbcPicture      (40,&imccPerson,bSetRelativePaths)
   // PAGE: User Fields (dynamic)
   // PAGE: Documented Sources
   ,ncDocumentedSources(60,this,this)
   // PAGE: Events (dynamic)
   // PAGE: Attachments
   ,ivpAttach       (80,this,this,IRectangle(),IViewPort::classDefaultStyle|IViewPort::noViewWindowFill)
   ,imccAttach      (81,&ivpAttach,&ivpAttach,IRectangle(),IWindow::noStyle)
   ,nfbFiles        (82,&imccAttach,&imccAttach,bSetRelativePaths)
   // NO MORE PAGES
{
   // Initial data
   bFocusGrab=bSetFocusGrab;
   pQuery=NULL;
   pFrame=pSetFrame;
   pNMankind=pSetNMankind;
   pmanMan=NULL;
   iMarriage=0;
   bDisabled=False;
   setFont(ifSet);
   setAutoDeleteObject(true);
   // PAGE: Person
   imccPerson.disableUpdate();
   istName.setText(NApplication::pDLLResource->loadString(SI_Name));
   nteName.setMinimumSize(ISize());
   nteFirstName.setMinimumSize(ISize());
   istSex.setText(NApplication::pDLLResource->loadString(SI_Sex));
   istFirstName.setText(NApplication::pDLLResource->loadString(SI_FirstName));
   nbSex.setBitmap(RES_BmpSexUnknown)
        .enableTabStop();
   istBirthDate.setText(NApplication::pDLLResource->loadString(SI_BirthDate));
   istBirthPlace.setText(NApplication::pDLLResource->loadString(SI_BirthPlace));
   istDeathDate.setText(NApplication::pDLLResource->loadString(SI_DeathDate));
   istDeathPlace.setText(NApplication::pDLLResource->loadString(SI_DeathPlace));
   nbMarriage.enableTabStop();
   istMarriageDate.setText(NApplication::pDLLResource->loadString(SI_MarriageDate));
   istMarriagePlace.setText(NApplication::pDLLResource->loadString(SI_MarriagePlace));
   istDivorceDate.setText(NApplication::pDLLResource->loadString(SI_DivorceDate));
   istOccupation.setText(NApplication::pDLLResource->loadString(SI_Occupation));
   istAddress.setText(NApplication::pDLLResource->loadString(SI_Address));
   istMemo.setText(NApplication::pDLLResource->loadString(SI_Memo));
   nmfMemo.enableWordWrap(bSetWordWrap);
   istPicture.setText(NApplication::pDLLResource->loadString(SI_Bitmap));

   imccPerson.setDefaultCell(ISize(3,2));
   imccPerson.addToCell(&istName         ,1, 1,2,1)
             .addToCell(&nteName         ,3, 1,1,1)
             .addToCell(&istSex          ,4, 1,1,1)
             .addToCell(&istFirstName    ,2, 3,1,1)
             .addToCell(&nteFirstName    ,3, 3,1,1)
             .addToCell(&nbSex           ,4, 2,1,2)

             .addToCell(&istBirthDate    ,1, 5,2,1)
             .addToCell(&ndcBirthDate    ,3, 5,2,1)
             .addToCell(&istBirthPlace   ,2, 7,1,1)
             .addToCell(&nteBirthPlace   ,3, 7,2,1)

             .addToCell(&istDeathDate    ,1, 9,2,1)
             .addToCell(&ndcDeathDate    ,3, 9,2,1)
             .addToCell(&istDeathPlace   ,2,11,1,1)
             .addToCell(&nteDeathPlace   ,3,11,2,1)

             .addToCell(&istMarriageDate ,1,13,2,1)
             .addToCell(&ndcMarriageDate ,3,13,1,1)
             .addToCell(&nbMarriage      ,4,13,1,1)
             .addToCell(&istMarriagePlace,2,15,1,1)
             .addToCell(&nteMarriagePlace,3,15,2,1)
             .addToCell(&istDivorceDate  ,2,17,1,1)
             .addToCell(&ndcDivorceDate  ,3,17,2,1)

             .addToCell(&istOccupation   ,1,19,2,1)
             .addToCell(&nteOccupation   ,3,19,2,1)
             .addToCell(&istAddress      ,1,21,2,1)
             .addToCell(&nteAddress      ,3,21,2,1)
             .addToCell(&istMemo         ,1,23,2,1)
             .addToCell(&nmfMemo         ,3,23,2,1)
             .addToCell(&istPicture      ,1,25,2,1)
             .addToCell(&nbcPicture      ,3,25,2,1)

             .setColumnWidth(3,0,True)
             .setColumnWidth(5,imccPerson.columnWidth(1))
             .setRowHeight(23,16,True)
             .setRowHeight(25,16,True)

             .enableUpdate();
   // PAGE: User fields
   pUserFields=new NUserFields(50,this,pNMankind,userListMetric);
   // PAGE: Documented Sources
   ncDocumentedSources.setEmptyText(NApplication::pDLLResource->loadString(SI_NotImplemented));
   // PAGE: Events
   pelEvents=new NManEditEventList(70,this,eventListMetric);
   // PAGE: Attachments
   imccAttach.disableUpdate();
   imccAttach.setDefaultCell(ISize(3,2));
   imccAttach.addToCell(&nfbFiles  ,1,1,1,1);
   imccAttach.setColumnWidth(1, 0,True);
   imccAttach.setRowHeight  (1,64,True );
   imccAttach.enableUpdate();
   // Notebook
   setPageBackgroundColor    (backgroundColor());
   setMajorTabBackgroundColor(backgroundColor());
   PageSettings page(PageSettings::autoPageSize
                    |PageSettings::majorTab
                    |PageSettings::statusTextOn);

   page.setTabBitmap(IResourceId(RES_BmpPageMan));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_TabStandard));
   addLastPage(page,&ivpPerson);

   page.setTabBitmap(IResourceId(RES_BmpPageManPlus));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_TabUser));
   addLastPage(page,pUserFields);

   page.setTabBitmap(IResourceId(RES_BmpPageDocsA));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_TabSources));
   addLastPage(page,&ncDocumentedSources);

   page.setTabBitmap(IResourceId(RES_BmpPageEventsA));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_TabEvents));
   addLastPage(page,pelEvents);

   page.setTabBitmap(IResourceId(RES_BmpPageFilesA));
   page.setStatusText(NApplication::pDLLResource->loadString(SI_TabAttachments));
   addLastPage(page,&ivpAttach);

   setMajorTabSize(ISize(19+12,15+8));
   // Handlers
   ICommandHandler::handleEventsFor(this);
   pNMankind->addMankindHandler(this);
   IResizeHandler::handleEventsFor(&ivpPerson);
   IResizeHandler::handleEventsFor(&ivpAttach);
   NDragDropHandler::handleDropFor(pelEvents->listWindow(),NDragDropHandler::Event);
   iFlyOverHelpHandler.handleEventsFor(&nbMarriage);
   itTimer.start(new ITimerMemberFn<NManEdit>(*this,NManEdit::triggerChange),2*1000);
   // Done
};

NManEdit::~NManEdit()
{
   itTimer.stop();
   // Save actual
   pNMankind->removeMankindHandler(this);
}

long NManEdit::getUserListMetric()
{
   return(pUserFields->getColMetric());
}
long NManEdit::getEventListMetric()
{
   return(pelEvents->getColMetric());
}


void NManEdit::disableInput()
{
   bDisabled=True;
   // Disable
   nteName.disableInput();
   nteFirstName.disableInput();
   nbSex.disableInput();
   ndcBirthDate.disableInput();
   nteBirthPlace.disableInput();
   ndcDeathDate.disableInput();
   nteDeathPlace.disableInput();
   nteOccupation.disableInput();
   nteAddress.disableInput();
   nmfMemo.disableInput();
   nbcPicture.disableInput();
   nbMarriage.disableInput();
   ndcMarriageDate.disableInput();
   nteMarriagePlace.disableInput();
   ndcDivorceDate.disableInput();
   nfbFiles.disableInput();
   pelEvents->disableInput();
   pUserFields->disableInput();
   if (pQuery) pQuery->dismiss();
}
void NManEdit::enableInput()
{
   // Enable
   nteName.enableInput();
   nteFirstName.enableInput();
   nbSex.enableInput();
   ndcBirthDate.enableInput();
   nteBirthPlace.enableInput();
   ndcDeathDate.enableInput();
   nteDeathPlace.enableInput();
   nteOccupation.enableInput();
   nteAddress.enableInput();
   nmfMemo.enableInput();
   nbcPicture.enableInput();
   nfbFiles.enableInput();
   if (iMarriage>0) {
      nbMarriage.enableInput();
      ndcMarriageDate.enableInput();
      nteMarriagePlace.enableInput();
      ndcDivorceDate.enableInput();
      }
   pelEvents->enableInput();
   pUserFields->enableInput();
   bDisabled=False;
}

Boolean NManEdit::windowResize(IResizeEvent& event)
{
   if (event.controlWindow()==&ivpPerson) {
      ISize isMin=imccPerson.minimumSize();
      // Check if ViewPort is too small
      unsigned long xmod=0;
      if (event.newSize().height()<isMin.height())
         xmod=IScrollBar::systemScrollBarWidth(True);
      unsigned long ymod=0;
      if (event.newSize().width()<isMin.width())
         ymod=IScrollBar::systemScrollBarWidth(False);

      imccPerson.sizeTo(ISize(max(event.newSize().width()-xmod,isMin.width())
                             ,max(event.newSize().height()-ymod,isMin.height()))  );
      }
   if (event.controlWindow()==&ivpAttach) {
      ISize isMin=imccAttach.minimumSize();
      imccAttach.sizeTo(ISize(max(event.newSize().width(),isMin.width())
                             ,max(event.newSize().height(),isMin.height()))  );
      }
   // Done
   return(False);
}

// Mankind Handlers
void NManEdit::manChanged(NMankindHandler* pInitiator)
{
   if ( (pInitiator==this)
      ||(!pmanMan)
      ||(!pmanMan->changedThisTime())) return;
   PNPerson ptmp=pmanMan;
   pmanMan=NULL;
   setMan(ptmp);
}

void NManEdit::doChangesNow()
{
   setMan(pmanMan);
}
void NManEdit::manAdded(PNPerson which,Boolean more)
{
   if ((!more)&&(pmanMan==NULL)) {
      setMan(pNMankind->getActual());
      }
   else {
      // just show the change, o.k. ?
      storeInfo();
      iMarriage=pmanMan->getNoOfPartners();
      loadInfo();
      }
}
void NManEdit::manAdded(SEQOFNPerson& which)
{
   if (pmanMan==NULL) setMan(pNMankind->getActual());
   else setMan(pmanMan); // just show the change, o.k. ?
}
void NManEdit::disconnentMen() {
   pmanMan=NULL;
}
void NManEdit::actualChanged(PNPerson pOld,PNPerson pNew)
{
   setMan(pNew);
   if (bFocusGrab) imccPerson.setFocus();
}
void NManEdit::originChanged(PNPerson pOld,PNPerson pNew)
{
   if (pmanMan==NULL) setMan(pNMankind->getActual());
   else setMan(pmanMan); // just show the change, o.k. ?
}

void NManEdit::triggerChange(unsigned long timer)
{
   if ((isValid())&&(isVisible())&&(!bDisabled)) storeInfo();
}

void NManEdit::storeInfo()
{
   if (!pmanMan) return;
   // Write standard data
   pNMankind->beginChange();
   if (nteName.hasChanged())
      pmanMan->setName(nteName.text());
   if (nteFirstName.hasChanged())
      pmanMan->setFirstName(nteFirstName.text());
   if (nbSex.hasChanged())
      switch (nbSex.getString().asInt()) {
         case RES_BmpSexMale  : pmanMan->setSex(1); break;
         case RES_BmpSexFemale: pmanMan->setSex(2); break;
         default           : pmanMan->setSex(0); break;
         }
   if (ndcBirthDate.hasChanged())
      pmanMan->setBirthDate(ndcBirthDate.getDate());
   if (nteBirthPlace.hasChanged())
      pmanMan->setBirthPlace(nteBirthPlace.text());
   if (ndcDeathDate.hasChanged())
      pmanMan->setDeathDate(ndcDeathDate.getDate());
   if (nteDeathPlace.hasChanged())
      pmanMan->setDeathPlace(nteDeathPlace.text());
   if (nteOccupation.hasChanged())
      pmanMan->setOccupation(nteOccupation.text());
   if (nteAddress.hasChanged())
      pmanMan->setAddress(nteAddress.text());
   if (nmfMemo.isChanged())
      pmanMan->setMemo(nmfMemo.text().remove(4096+1));
   if (nbcPicture.hasChanged())
      pmanMan->setBitmap(nbcPicture.getLocation());
   if (nfbFiles.hasChanged()) {
      pmanMan->delAllFiles();
      for (unsigned short i=1;i<=nfbFiles.count();i++)
         pmanMan->addFile(nfbFiles.get(i));
      }
   // Check for changed information
   if (iMarriage>0) {
      if (nteMarriagePlace.hasChanged())
         pmanMan->setMarriagePlace(iMarriage,nteMarriagePlace.text());
      if (ndcDivorceDate.hasChanged())
         pmanMan->setDivorceDate(iMarriage,ndcDivorceDate.getDate());
      if (ndcMarriageDate.hasChanged())
         pmanMan->setMarriageDate(iMarriage,ndcMarriageDate.getDate());
      }
   // Tell Mankind that we're ready
   pNMankind->endChange(this);
   // Done with save
}

void NManEdit::loadInfo()
{
   if (!pmanMan) return;
   // Set standard data
   nteName.setText(pmanMan->getName());
   nteName.hasChanged();
   nteFirstName.setText(pmanMan->getFirstName());
   nteFirstName.hasChanged();
   switch (pmanMan->getSex()) {
      case 1 : nbSex.setBitmap(RES_BmpSexMale); break;
      case 2 : nbSex.setBitmap(RES_BmpSexFemale); break;
      default: nbSex.setBitmap(RES_BmpSexUnknown); break;
      }
   nbSex.hasChanged();
   ndcBirthDate.setDate(pmanMan->getBirthDate());
   ndcBirthDate.hasChanged();   // Reset change flag
   nteBirthPlace.setText(pmanMan->getBirthPlace());
   nteBirthPlace.hasChanged();   // Reset change flag
   ndcDeathDate.setDate(pmanMan->getDeathDate());
   ndcDeathDate.hasChanged();   // Reset change flag
   nteDeathPlace.setText(pmanMan->getDeathPlace());
   nteDeathPlace.hasChanged();   // Reset change flag
   nteOccupation.setText(pmanMan->getOccupation());
   nteOccupation.hasChanged();  // Reset change flag
   nteAddress.setText(pmanMan->getAddress());
   nteAddress.hasChanged();  // Reset change flag
   nmfMemo.setText(pmanMan->getMemo());
   nmfMemo.resetChangedFlag();  // Reset change flag
   nbcPicture.setLocation(pmanMan->getBitmap(),False);
   nbcPicture.hasChanged();     // Reset change flag
   // Do Marriages
   if (iMarriage>0) {
      iFlyOverHelpHandler.setHelpText(nbMarriage.handle(),pmanMan->getPartnerName(iMarriage));

      nbMarriage.setString(IString(iMarriage)+"/"
                         +IString(pmanMan->getNoOfPartners()));
      ndcMarriageDate.setDate(pmanMan->getMarriageDate(iMarriage));
      ndcMarriageDate.hasChanged();
      nteMarriagePlace.setText(pmanMan->getMarriagePlace(iMarriage));
      nteMarriagePlace.hasChanged();
      ndcDivorceDate.setDate(pmanMan->getDivorceDate(iMarriage));
      ndcDivorceDate.hasChanged();

      if (!bDisabled) {
         nbMarriage.enableInput();
         ndcMarriageDate.enableInput();
         nteMarriagePlace.enableInput();
         ndcDivorceDate.enableInput();
         }
      }
   else {
      iFlyOverHelpHandler.removeHelpText(nbMarriage.handle());

      nbMarriage.setString("-/-");
      ndcMarriageDate.setDate(NDate());
      nteMarriagePlace.setText(IString());
      ndcDivorceDate.setDate(NDate());
      nbMarriage.disableInput();
      ndcMarriageDate.disableInput();
      nteMarriagePlace.disableInput();
      ndcDivorceDate.disableInput();
      }
   // User Fields
   pUserFields->setMan(pmanMan);
   // Events
   pelEvents->setMan(pmanMan);
   // Attachments
   nfbFiles.setRefreshOff();
   nfbFiles.clear();
   if (pmanMan->getNoOfFiles()>0) {
      for (unsigned short i=1;i<=pmanMan->getNoOfFiles();i++)
         nfbFiles.add(*pmanMan->getFile(i),False);
      setTabBitmap(lastPage(),IResourceId(RES_BmpPageFilesB));
      }
   else
      setTabBitmap(lastPage(),IResourceId(RES_BmpPageFilesA));
   nfbFiles.hasChanged();
   nfbFiles.setRefreshOn()
           .refresh();
   // Done with set
   if (pQuery) pQuery->dismiss();
}

void NManEdit::clear()
{
   nteName.removeAll();
   nteFirstName.removeAll();
   nteBirthPlace.removeAll();
   ndcBirthDate.setDate(NDate());
   ndcDeathDate.setDate(NDate());
   ndcMarriageDate.setDate(NDate());
   ndcDivorceDate.setDate(NDate());
   nteOccupation.removeAll();
   nteAddress.removeAll();
   nmfMemo.removeAll();
   nbcPicture.setLocation(IString(),False);
   nfbFiles.clear();
   setTabBitmap(lastPage(),IResourceId(RES_BmpPageFilesA));

   nbMarriage.setString("-/-");
   ndcMarriageDate.setDate(NDate());
   nteMarriagePlace.setText(IString());
   ndcDivorceDate.setDate(NDate());
}

void NManEdit::setMan(NPerson* pmanSet)
{
   // Save old
   storeInfo();
   // Check for same
   if (pmanMan==pmanSet) return;
   // Remember man
   pmanMan=pmanSet;
   // Get new
   if (pmanMan) {
      iMarriage=pmanMan->getNoOfPartners();
      loadInfo();
      }
   else clear();
   // Done
}

Boolean NManEdit::command(ICommandEvent& event)
{
   // Check command
   switch (event.commandId()) {
      // Event List
      case MI_ManEditEventListDel : {
         // ... remove event from man
         pNMankind->beginChange();
         PNEvent pevent=pmanMan->getEvent(pelEvents->getActualIdx());
         pevent->delMan(pmanMan);
         pNMankind->endChange(this);
         } break;
      case MI_ManEditEventListChange: {
         // Prepare Dialog for involvement
         PNEvent pevent=pmanMan->getEvent(pelEvents->getActualIdx());
         pQuery=new NQueryInvolved(IC_DEFAULT_FRAME_ID
                                  ,pFrame
                                  ,pmanMan
                                  ,pevent->getRoleOfMan(pmanMan));
         pQuery->showModally();
         if (pQuery->result()!=True) break;
         // Add new man to event
         pNMankind->beginChange();
         pevent->changeRoleOfMan(pmanMan,pQuery->isResult);
         pNMankind->endChange(this);
         // Done
         break;
         }
      case MI_ManEditEventListJump: {
         pNMankind->makeActualEvent(pmanMan->getEvent(pelEvents->getActualIdx()));
         // Done
         break;
         }
      // Buttons
      case BUT_SEX : {
         short int next;
         switch (nbSex.getString().asInt()) {
            case RES_BmpSexMale    : next=RES_BmpSexFemale; break;
            case RES_BmpSexFemale  : next=RES_BmpSexUnknown; break;
            case RES_BmpSexUnknown : next=RES_BmpSexMale; break;
            }
         nbSex.setBitmap(next);
         return(True);
         }
      case BUT_MARRIAGE : {
         short int next=iMarriage+1;
         if (next>pmanMan->getNoOfPartners()) next=1;
         if (iMarriage!=next) {
            storeInfo();
            iMarriage=next;
            loadInfo();
            }
         return(True);
         }
      // Done
      }
   // Done everything
   return(False);
}


void NManEdit::structureChanged()
{
   storeInfo();
   iMarriage=pmanMan->getNoOfPartners();
   loadInfo();
}

void NManEdit::eventDropped(PNEvent which)
{
   // Check if our man already belongs to the event
   for (short i=1;i<=pmanMan->numEvents();i++)
      if (pmanMan->getEvent(i)==which) return;
   // Prepare Dialog for involvement
   pQuery=new NQueryInvolved(IC_DEFAULT_FRAME_ID,pFrame,pmanMan);
   pQuery->showModally();
   if (pQuery->result()!=True) return;
   // Add new man to event
   pNMankind->beginChange();
   which->addMan(pmanMan,pQuery->isResult);
   pNMankind->endChange(this);
   // Update event list
   pelEvents->setMan(pmanMan);
   // Done
}


// Class  : NEditDialog
// Purpose: Dialog Window Class for NPerson
NEditDialog* NEditDialog::pNEditDialog=NULL;
IFont        NEditDialog::ifProfileFont("Helv",8);
IRectangle   NEditDialog::irProfileRectangle;
Boolean      NEditDialog::bProfileRelativePaths=True;
Boolean      NEditDialog::bProfileFocusAfterClick=False;
Boolean      NEditDialog::bProfileMemoWordWrap  =False;
Boolean      NEditDialog::bProfileFloat=False;
long         NEditDialog::lProfileUserListMetric=0;
long         NEditDialog::lProfileEventListMetric=0;

void NEditDialog::loadFromProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("EDIT");
   irProfileRectangle=IRectangle(profile.integerWithKey("MINX")
                                ,profile.integerWithKey("MINY")
                                ,profile.integerWithKey("MAXX")
                                ,profile.integerWithKey("MAXY"));
   ifProfileFont=IFont(profile.elementWithKey("FONTNAME")
                      ,profile.integerWithKey("FONTSIZE"));
   bProfileRelativePaths=profile.integerWithKey("RELATIVE");
   bProfileFocusAfterClick=profile.integerWithKey("AUTOFOCUS");
   bProfileMemoWordWrap=profile.integerWithKey("MEMOWW");
   bProfileFloat=profile.integerWithKey("FLOAT");
   lProfileUserListMetric=profile.integerWithKey("ULISTMETRIC");
   lProfileEventListMetric=profile.integerWithKey("ELISTMETRIC");
}
void NEditDialog::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("EDIT");
   profile.addOrReplaceElementWithKey("MINX",irProfileRectangle.minX());
   profile.addOrReplaceElementWithKey("MINY",irProfileRectangle.minY());
   profile.addOrReplaceElementWithKey("MAXX",irProfileRectangle.maxX());
   profile.addOrReplaceElementWithKey("MAXY",irProfileRectangle.maxY());
   profile.addOrReplaceElementWithKey("FONTNAME",ifProfileFont.name());
   profile.addOrReplaceElementWithKey("FONTSIZE",ifProfileFont.pointSize());
   profile.addOrReplaceElementWithKey("RELATIVE",bProfileRelativePaths);
   profile.addOrReplaceElementWithKey("AUTOFOCUS",bProfileFocusAfterClick);
   profile.addOrReplaceElementWithKey("MEMOWW",bProfileMemoWordWrap);
   profile.addOrReplaceElementWithKey("FLOAT",bProfileFloat);
   profile.addOrReplaceElementWithKey("ULISTMETRIC",lProfileUserListMetric);
   profile.addOrReplaceElementWithKey("ELISTMETRIC",lProfileEventListMetric);
}


NEditDialog::NEditDialog(IWindow* piwOwner,NMankind* pSetNMankind)
   : IFrameWindow(IResourceId(ID_NEditWindow)
                 ,IWindow::desktopWindow()
                 ,bProfileFloat ? NULL : piwOwner
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::sizingBorder
                  |IFrameWindow::titleBar
                  |IFrameWindow::noMoveWithOwner
                  |IFrameWindow::dialogBackground
                 ,NApplication::pDLLResource->loadString(SI_EditTitle))
   ,IFrameHandler()
   ,itTitle(this)
{
   // SetUp Data
   setFont(IFont("Helv",8));
   setIcon(RES_IconFTree);
   pNEditDialog=this;
   bDisabled=False;
   NMainWindow::pNMainWindow->ihwHelp.setAssociatedWindow(this);

   pNManEdit=new NManEdit(1,this,this,pSetNMankind
                         ,bProfileRelativePaths
                         ,bProfileMemoWordWrap
                         ,bProfileFocusAfterClick
                         ,ifProfileFont
                         ,lProfileUserListMetric
                         ,lProfileEventListMetric);
   setClient(pNManEdit);

   // SetUp Position & Size & Font
   if (  (irProfileRectangle.width()>0)
      && (irProfileRectangle.height()>0) )
      moveSizeTo(irProfileRectangle);
   else {
      ISize is=pNManEdit->minimumSize();
      moveSizeToClient(IRectangle(NMainWindow::pNMainWindow->rect().centerXCenterY()-is/2
                                 ,is));
      }

   // Handler removeDefaultHandler(); // Remove DefaultFrameHandler for my one
   IFrameHandler::handleEventsFor(this);
   // Done
   show();
   pNManEdit->setFocus();
} /* end NEditDialog::NEditDialog */

NEditDialog::~NEditDialog() { }

void NEditDialog::changeFloat(Boolean set)
{
   bProfileFloat=set;
   if (pNEditDialog)
      pNEditDialog->setOwner( bProfileFloat ? NULL : NMainWindow::pNMainWindow );
}

Boolean NEditDialog::isFloat()
{
   return(bProfileFloat);
}

void NEditDialog::disableInput()
{
   bDisabled=True;
   pNManEdit->disableInput();
}
void NEditDialog::enableInput()
{
   pNManEdit->enableInput();
   bDisabled=False;
}

Boolean NEditDialog::saved(IFrameEvent& frameEvent)
{
   // Save Frame Status
   irProfileRectangle=rect();
   ifProfileFont=pNManEdit->font();
   lProfileUserListMetric=pNManEdit->getUserListMetric();
   lProfileEventListMetric=pNManEdit->getEventListMetric();
   return(False);
}

Boolean NEditDialog::closed(IFrameEvent& frameEvent)
{
   if (bDisabled) {
      DosBeep(200,200);
      return(True);
      }
   // Save all contained data
   pNManEdit->doChangesNow();
   // Report destruction to ownerpointer
   pNEditDialog=NULL;
   // Done
   return(False);
}

void NEditDialog::changeRelativePaths(Boolean set)
{
   bProfileRelativePaths=set;
   if (pNEditDialog) pNEditDialog->close();
}
Boolean NEditDialog::isRelativePaths()
{
   return(bProfileRelativePaths);
}
void NEditDialog::changeFocusAfterClick(Boolean set)
{
   bProfileFocusAfterClick=set;
   if (pNEditDialog) pNEditDialog->close();
}
Boolean NEditDialog::isFocusAfterClick()
{
   return(bProfileFocusAfterClick);
}
void NEditDialog::changeMemoWordWrap(Boolean set)
{
   bProfileMemoWordWrap=set;
   if (pNEditDialog) pNEditDialog->close();
}
Boolean NEditDialog::isMemoWordWrap()
{
   return(bProfileMemoWordWrap);
}





