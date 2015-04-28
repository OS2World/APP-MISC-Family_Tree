/*
   File   : MainWindow.cpp
   Purpose: Define Classes for the main Window
*/

#include "imnitem.hpp"
#include "iradiobt.hpp"
#include "ispinnum.hpp"
#include "ipushbut.hpp"

#include "NMainWindow.hpp"
#include "Main.hpp"
#include "NPrint.hpp"
#include "NRexx.hpp"
#include "FTree.h"

unsigned long _System DosSleep(unsigned long msec);
unsigned long _System DosBeep(unsigned long,unsigned long);

#define MAXCHILDREN 50
#define MAXPARTNERS 50

// Class  : NQueryGenerations
// Purpose: query number of generations
class NQueryGenerations : public IFrameWindow,
                          public ICommandHandler,
                          public ISelectHandler
{
public:
   NQueryGenerations(long windowId,IWindow* pOwner
                    ,const IString& isExplanation
                    ,const unsigned short sSetMaximum
                    ,const unsigned short sSetCurrent
                    ,const IString& isTitle);
   unsigned short sResult;
private:
   unsigned short sMaximum,sCurrent;
   IMultiCellCanvas   imccCanvas;
   IGroupBox          igbExplain;
   IMultiLineEdit     imleExplain;
   IRadioButton       irbMaximum;
   IRadioButton       irbCustom;
   INumericSpinButton insCustom;
   IPushButton        ipbOK,ipbCANCEL;
protected:
   virtual Boolean command(ICommandEvent& event);
   virtual Boolean selected(IControlEvent& event);
};

#define ID_CANVAS   1
#define ID_GROUP    2
#define ID_TEXT     3
#define ID_SMAXIMUM 4
#define ID_SCUSTOM  5
#define ID_NCUSTOM  6
#define ID_OKBUT    7
#define ID_CBUT     8

NQueryGenerations::NQueryGenerations(long windowId,IWindow* pOwner
                                    ,const IString& isExplanation
                                    ,const unsigned short sSetMaximum
                                    ,const unsigned short sSetCurrent
                                    ,const IString& isTitle)
   : IFrameWindow(windowId
                 ,NULL,pOwner,IRectangle()
                 ,IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder
                 ,isTitle)
   , ICommandHandler()
   , ISelectHandler()
   , imccCanvas (ID_CANVAS  ,this       ,this       )
   , igbExplain (ID_GROUP   ,&imccCanvas,&imccCanvas)
   , imleExplain(ID_TEXT    ,&imccCanvas,&imccCanvas,IRectangle()
                ,IMultiLineEdit::readOnly|IMultiLineEdit::wordWrap|IWindow::visible)
   , irbMaximum (ID_SMAXIMUM,&imccCanvas,&imccCanvas)
   , irbCustom  (ID_SCUSTOM ,&imccCanvas,&imccCanvas)
   , insCustom  (ID_NCUSTOM ,&imccCanvas,&imccCanvas)
   , ipbOK      (ID_OKBUT   ,&imccCanvas,&imccCanvas,IRectangle(),IPushButton::classDefaultStyle|IPushButton::defaultButton)
   , ipbCANCEL  (ID_CBUT    ,&imccCanvas,&imccCanvas)
{
   sMaximum=sSetMaximum;
   sCurrent=sSetCurrent;
   // Layout
   setFont(IFont("Helv",8));
   setClient(&imccCanvas);

   igbExplain.setText(NApplication::pDLLResource->loadString(SI_QueryGenerationsPlease));
   imleExplain.setText(isExplanation)
              .setBackgroundColor(backgroundColor())
              .setMinimumSize(imleExplain.minimumSize()*5/9);

   irbMaximum.setText(NApplication::pDLLResource->loadString(SI_QueryGenerationsMax));
   irbCustom .setText(NApplication::pDLLResource->loadString(SI_QueryGenerationsUpTo));
   insCustom .setRange(IRange(0,sMaximum))
             .setLimit(3)
             .setAlignment(IBaseSpinButton::right)
             .setDisabledBackgroundColor(IColor(IColor::paleGray));
   ipbOK     .setText(NApplication::pDLLResource->loadString(SI_OK));
   ipbCANCEL .setText(NApplication::pDLLResource->loadString(SI_Cancel));

   imccCanvas.setDefaultCell(ISize(7,7));
   imccCanvas.addToCell(&igbExplain , 2, 2, 5, 4)
             .addToCell(&imleExplain, 3, 4, 3, 1)
             .addToCell(&irbMaximum , 3, 7, 2, 1)
             .addToCell(&irbCustom  , 3, 8, 1, 1)
             .addToCell(&insCustom  , 4, 8, 1, 1)
             .addToCell(&ipbOK      , 4,10      )
             .addToCell(&ipbCANCEL  , 5,10      )
             .setRowHeight  (11,imccCanvas.defaultCell().height())
             .setColumnWidth(7,imccCanvas.defaultCell().width ());

   ISize size=imccCanvas.minimumSize();
   moveSizeToClient(IRectangle(IPoint(pOwner->rect().centerXCenterY()-size/2),size));
   // Handler
   ISelectHandler::handleEventsFor(&irbMaximum);
   ISelectHandler::handleEventsFor(&irbCustom );
   ICommandHandler::handleEventsFor(this);
   // Prepare pre-content
   if (sCurrent==(unsigned short)-1) {
      insCustom.setValue(sMaximum);
      irbMaximum.click();
      }
   else {
      insCustom.setValue(sCurrent);
      irbCustom.click();
      }
   // Done
}

Boolean NQueryGenerations::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      case ID_OKBUT :
         if (irbMaximum.isSelected())
            sResult=(unsigned short)-1;
         else
            sResult=insCustom.value();
         dismiss(True);
         break;
      case ID_CBUT  :
         dismiss(False);
         break;
      }
   // Done
   return(False);
}

Boolean NQueryGenerations::selected(IControlEvent& event)
{
   switch (event.controlId()) {
      case ID_SMAXIMUM :
         insCustom.disable();
         break;
      case ID_SCUSTOM  :
         insCustom.enable();
         insCustom.setFocus();
         break;

      }
   return(False);
}




// Class  : NMainWindow
// Purpose: Main Window class for ftree.exe
NMainWindow* NMainWindow::pNMainWindow          =NULL;
IRectangle   NMainWindow::irProfileRectangle;
Boolean      NMainWindow::bProfileMaxed         =False;
Boolean      NMainWindow::bProfileEdit          =False;
Boolean      NMainWindow::bProfileList          =False;
Boolean      NMainWindow::bProfileEvents        =False;
Boolean      NMainWindow::bProfileButtonBarTop  =True;
Boolean      NMainWindow::bProfileButtonBarVis  =True;
Boolean      NMainWindow::bProfileDoBackup      =True;
Boolean      NMainWindow::bProfileImportPreserve=False;
Boolean      NMainWindow::bProfileBorderlessButtons=True;

void NMainWindow::loadFromProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("MAIN");
   // Check for DateInformation
   try {
      IString isTemp=profile.elementWithKey("DATE");
      if (isTemp.length()==2) NApplication::changeLocale(isTemp[(unsigned)1],isTemp[(unsigned)2]);
      }
   catch (IException) {}
   // Get Registration Information
   try {
      NApplication::isRegisteredUser=profile.elementWithKey("KIRSTEN");
      }
   catch (IException) {}
   // Other
   irProfileRectangle =IRectangle(profile.integerWithKey("MINX")
                                 ,profile.integerWithKey("MINY")
                                 ,profile.integerWithKey("MAXX")
                                 ,profile.integerWithKey("MAXY"));
   bProfileMaxed       =profile.integerWithKey("MAXED");
   bProfileEdit        =profile.integerWithKey("EDIT");
   bProfileList        =profile.integerWithKey("LIST");
   bProfileEvents      =profile.integerWithKey("EVNT");
   bProfileButtonBarTop=profile.integerWithKey("BBARTOP");
   bProfileButtonBarVis=profile.integerWithKey("BBARVIS");
   bProfileDoBackup    =profile.integerWithKey("DOBACKUP");
   bProfileImportPreserve=profile.integerWithKey("IMPPRES");
   bProfileBorderlessButtons=profile.integerWithKey("BLB");
}
void NMainWindow::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("MAIN");
   // DateInformation
   profile.addOrReplaceElementWithKey("DATE",
      IString(NApplication::cDateSeparator)+IString(NApplication::cDateLayout));
   // Registration
   if (NApplication::isRegisteredUser.size()>0) {
      profile.addOrReplaceElementWithKey("KIRSTEN",NApplication::isRegisteredUser);
      }
   // Other
   profile.addOrReplaceElementWithKey("MINX",irProfileRectangle.minX());
   profile.addOrReplaceElementWithKey("MINY",irProfileRectangle.minY());
   profile.addOrReplaceElementWithKey("MAXX",irProfileRectangle.maxX());
   profile.addOrReplaceElementWithKey("MAXY",irProfileRectangle.maxY());
   profile.addOrReplaceElementWithKey("MAXED",bProfileMaxed           );
   profile.addOrReplaceElementWithKey("EDIT",bProfileEdit             );
   profile.addOrReplaceElementWithKey("LIST",bProfileList             );
   profile.addOrReplaceElementWithKey("EVNT",bProfileEvents           );
   profile.addOrReplaceElementWithKey("BBARTOP",bProfileButtonBarTop  );
   profile.addOrReplaceElementWithKey("BBARVIS",bProfileButtonBarVis  );
   profile.addOrReplaceElementWithKey("DOBACKUP",bProfileDoBackup     );
   profile.addOrReplaceElementWithKey("IMPPRES",bProfileImportPreserve);
   profile.addOrReplaceElementWithKey("BLB",bProfileBorderlessButtons );
}

NMainWindow::NMainWindow()
   : IFrameWindow(IResourceId(ID_NMainWindow)
                 ,IWindow::desktopWindow()
                 ,NULL
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::titleBar
                  |IFrameWindow::minimizeButton
                  |IFrameWindow::maximizeButton
                  |IFrameWindow::windowList
                  |IFrameWindow::sizingBorder
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::appDBCSStatus)
   , IFrameHandler()
   , IMenuHandler()
   , IMousePointerHandler()
   , IEditHandler()

   , iFlyText(2,this)
   , iFlyOverHelpHandler(&iFlyText)

   , nbbButtonBar(1,this)
   , nbOpen       (ID_nbOpen       ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbSave       (ID_nbSave       ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbPrint      (ID_nbPrint      ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbOptions    (ID_nbOptions    ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbLocate     (ID_nbLocate     ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbHoriVert   (ID_nbHoriVert   ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbAncestors  (ID_nbAncestors  ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbDescendants(ID_nbDescendants,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , nbSiblings   (ID_nbSiblings   ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)
   , isZoom       (ID_isZoom,&nbbButtonBar,&nbbButtonBar,IRectangle(),10,0
                  ,IWindow::visible
                  |ISlider::horizontal
                  |IProgressIndicator::snapToTickMark)
   , nslMessage   (ID_nslMessage   ,&nbbButtonBar)
   , nbTopBottom  (ID_nbTopBottom  ,&nbbButtonBar,&nbbButtonBar,!bProfileBorderlessButtons)

   , mankind()
   , mantree(9999,this,&mankind)

   , imbMenuBar(IResourceId(MI_Main,*NApplication::pDLLResource),this)
   , itTitle(this)
   , ihwHelp(IResourceId(ID_NMainWindow),this)
   , iaAccel(IResourceId(ID_NMainWindow,*NApplication::pDLLResource),this)
{
cout << "2.1 " << flush;
   // First
   setIcon(RES_IconFTree);
   if (NApplication::isRegisteredUser.size()==0)
      itTitle.setObjectText(NApplication::pDLLResource->loadString(SI_MainTitle)
                           +NApplication::pDLLResource->loadString(SI_MainTitleUnregistered));
   else
      itTitle.setObjectText(NApplication::pDLLResource->loadString(SI_MainTitle));
   pNMainWindow=this;
   threadLoadSavePrint.setVariable("TASK","");
   bDisabled=False;
   ihwHelp.setAssociatedWindow(this);
cout << "2.2 " << flush;
   // SetUp ButtonBar
   nbOpen     .setBitmap       (RES_BmpOpen       );
   nbSave     .setBitmap       (RES_BmpSave       );
   nbPrint    .setBitmap       (RES_BmpPrint      );
   nbOptions  .setBitmap       (RES_BmpOptions    );
   nbLocate   .setBitmap       (RES_BmpLocate     )
              .disableInput();
   nbHoriVert .setBitmap       (RES_BmpVertical   )
              .setLatchedBitmap(RES_BmpHorizontal )
              .enableLatching  ()
              .enableGroup     ();
   nbAncestors  .setBitmap     (RES_BmpAncestors  );
   nbDescendants.setBitmap     (RES_BmpDescendants);
   nbSiblings .setBitmap       (RES_BmpNoSiblings )
              .setLatchedBitmap(RES_BmpSiblings   )
              .enableLatching  ();
   isZoom     .setMinimumSize(ISize(64,20))
              .setBackgroundColor(nbbButtonBar.backgroundColor());
   nslMessage .setLimit(60)
              .setForegroundColor(IColor(IColor::black))
              .setFont(IFont("System VIO",3));
   nbTopBottom.setBitmap         (RES_BmpTop       )
              .setLatchedBitmap(RES_BmpBottom    )
              .enableLatching  ()
              .latch(bProfileButtonBarTop==False )
              .enableGroup     ();
   nbbButtonBar.addChild(&nbOpen       ,True)
               .addChild(&nbSave       ,True)
               .addChild(&nbPrint      ,True)
               .addChild(&nbOptions    ,True)
               .addChild(&nbLocate     ,True)
               .addChild(&nbHoriVert   ,True)
               .addChild(&nbAncestors  ,True)
               .addChild(&nbDescendants,True)
               .addChild(&nbSiblings   ,True)
               .addChild(&isZoom       ,True)
               .addChild(&nslMessage   ,True)
               .addChild(&nbTopBottom  ,False)
               .setGrowableRow(11);
   // SetUp Main-Layout
   setClient(&mantree);
   if (bProfileButtonBarVis) {
      addExtension(&nbbButtonBar,bProfileButtonBarTop ?
                   IFrameWindow::aboveClient : IFrameWindow::belowClient );
cout << "2.2.1" << flush;
      useExtensionMinimumSize(&nbbButtonBar);
cout << "2.2.2" << flush;
      }
   // SetUp HandlerStuff
   //IFrameWindow::removeDefaultHandler(); // Remove DefaultFrameHandler for my one
   IFrameHandler::handleEventsFor(this);
   IMenuHandler::handleEventsFor(this);
   IEditHandler::handleEventsFor(&isZoom);
   iFlyOverHelpHandler.setInitialDelayTime(1000)
                      .setDelayTime(500)
                      .setResourceLibrary(NApplication::pDLLResource->handle())
                      .handleEventsFor(&nbbButtonBar);
   mankind.addMankindHandler(this);
cout << "2.3 " << flush;
   // Setup InfoArea
   calcMessageText();
   // SetUp Help
   try {
      ihwHelp.addLibraries(HELPFILE);
      ihwHelp.setTitle(itTitle.objectText());
      }
   catch(IException iex) {
      disableInput();
      IMessageBox msgBox(this);
      msgBox.show(NApplication::pDLLResource->loadString(SI_NoHelp),IMessageBox::information);
      imbMenuBar.disableItem(MI_MainHelp);
      enableInput();
      }
   // SetUp Position & Size
cout << "2.4 " << flush;
   if ( (irProfileRectangle.width()>0)
      &&(irProfileRectangle.height()>0))
      moveSizeTo(irProfileRectangle);
   else
      moveSizeTo(IRectangle(IPoint(0,parent()->rect().maxY()-400),ISize(640,400)));
   if (bProfileMaxed)
      maximize();
   // Edit & List Dialogs ?
cout << "2.5 " << flush;
   if (bProfileEdit) new NEditDialog(this,&mankind);
   if (bProfileList) new NListDialog(this,&mankind);
   if (bProfileEvents) new NEventDialog(this,&mankind);
   // Done
cout << "2.6 " << flush;
   show();
   setFocus();
cout << "2.7 " << flush;
} /* end IFWWindow::IFWWindow */

NMainWindow::~NMainWindow()
{
   mankind.removeMankindHandler(this);
}

Boolean NMainWindow::saved(IFrameEvent& frameEvent)
{
   // Save Frame Status
   if ((!isMinimized())&&(!isMaximized()))
      irProfileRectangle=rect();
   bProfileMaxed=isMaximized();
   return(False);
}

Boolean NMainWindow::closed(IFrameEvent& frameEvent)
{
   // Wait for IO-thread
   if (bDisabled) {
      threadLoadSavePrint.setVariable("TASK","CLOSE");
      return(True);
      }
   // Check for final data & needed saving
   if (mankind.hasChanged()) {
      IMessageBox msgBox(this);
      Boolean choice=msgBox.show(NApplication::pDLLResource->loadString(SI_SaveNow)
                                ,IMessageBox::yesNoCancelButton|IMessageBox::queryIcon|IMessageBox::moveable);
      if (choice==IMessageBox::cancel)
         return(True);
      if (choice==IMessageBox::yes) {
         threadLoadSavePrint.setVariable("TASK","CLOSE");
         saveDialog(NApplication::isTree);
         return(True);
         }
      }
   // Close other open windows
   bProfileEdit  =(NEditDialog::pNEditDialog!=NULL);
   bProfileList  =(NListDialog::pNListDialog!=NULL);
   bProfileEvents=(NEventDialog::pNEventDialog!=NULL);
   if (NEditDialog::pNEditDialog) NEditDialog::pNEditDialog->close();
   if (NListDialog::pNListDialog) NListDialog::pNListDialog->close();
   if (NEventDialog::pNEventDialog) NEventDialog::pNEventDialog->close();
   if (NOptionsDialog::pNOptionsDialog) NOptionsDialog::pNOptionsDialog->close();
   // Done
   return(False);
}

void NMainWindow::calcMessageText()
{
   // First calc Text
   IString isText;
   // Counts
   isText+=IString(mankind.getManCount())
           +NApplication::pDLLResource->loadString(SI_MsgNumberOfMan)
           +IString(mankind.getRelCount())
           +NApplication::pDLLResource->loadString(SI_MsgNumberOfRel);
   // Ancestors
   if (mantree.getDepth(NManTree::ancestors)!=(unsigned short)-1)
      isText+=IString(mantree.getDepth(NManTree::ancestors))+"/";
   isText+=IString(mantree.getDepth(NManTree::totalancestors))
           +NApplication::pDLLResource->loadString(SI_MsgGenAncestors);
   // Descendants
   if (mantree.getDepth(NManTree::descendants)!=(unsigned short)-1)
      isText+=IString(mantree.getDepth(NManTree::descendants))+"/";
   isText+=IString(mantree.getDepth(NManTree::totaldescendants))
           +NApplication::pDLLResource->loadString(SI_MsgGenDescendants);
   // Text
   nslMessage.setText(isText);
   // Check correct ToolBarButtons
   nbHoriVert.latch(mantree.getDir()==NManTree::horizontal);
   nbSiblings.latch(mantree.getDepth(NManTree::siblings)>0);
   // Do zoom slider
   IEditHandler::disable();
   try { isZoom.moveArmToTick(max(0,min(9,ceil(mantree.getZoom()*10)-1))); }
   catch (IException) {};
   IEditHandler::enable();
   // Done
}


void NMainWindow::disableInput()
{
   if (bDisabled) return;
   bDisabled=True;

   if (NEditDialog::pNEditDialog) NEditDialog::pNEditDialog->disableInput();
   if (NListDialog::pNListDialog) NListDialog::pNListDialog->disableInput();
   if (NEventDialog::pNEventDialog) NEventDialog::pNEventDialog->disableInput();
   if (NOptionsDialog::pNOptionsDialog) NOptionsDialog::pNOptionsDialog->disableInput();

   mantree.disableInput();

   imbMenuBar.disableItem(MI_MainFile);
   imbMenuBar.disableItem(MI_MainView);
   imbMenuBar.disableItem(MI_MainOptions);
   imbMenuBar.disableItem(MI_MainRexx);

   nbOpen       .disableInput();
   nbSave       .disableInput();
   nbPrint      .disableInput();
   nbOptions    .disableInput();
   //nbLocate     .disableInput();
   nbHoriVert   .disableInput();
   nbAncestors  .disableInput();
   nbDescendants.disableInput();
   nbSiblings   .disableInput();
   isZoom       .disable();
}
void NMainWindow::enableInput()
{
   if (!bDisabled) return;
   bDisabled=False;

   if (NEditDialog::pNEditDialog) NEditDialog::pNEditDialog->enableInput();
   if (NListDialog::pNListDialog) NListDialog::pNListDialog->enableInput();
   if (NEventDialog::pNEventDialog) NEventDialog::pNEventDialog->enableInput();
   if (NOptionsDialog::pNOptionsDialog) NOptionsDialog::pNOptionsDialog->enableInput();

   mantree.enableInput();

   imbMenuBar.enableItem(MI_MainFile);
   imbMenuBar.enableItem(MI_MainView);
   imbMenuBar.enableItem(MI_MainOptions);
   imbMenuBar.enableItem(MI_MainRexx);

   nbOpen       .enableInput();
   nbSave       .enableInput();
   nbPrint      .enableInput();
   nbOptions    .enableInput();
   //nbLocate     .enableInput();
   nbHoriVert   .enableInput();
   nbAncestors  .enableInput();
   nbDescendants.enableInput();
   nbSiblings   .enableInput();
   isZoom       .enable();
}

void NMainWindow::changeDoBackup(Boolean set)
{
   bProfileDoBackup=set;
}
Boolean NMainWindow::isDoBackup()
{
   return(bProfileDoBackup);
}

void NMainWindow::changeImportPreserve(Boolean set)
{
   bProfileImportPreserve=set;
}
Boolean NMainWindow::isImportPreserve()
{
   return(bProfileImportPreserve);
}

void NMainWindow::changeBorderlessButtons(Boolean set)
{
   bProfileBorderlessButtons=set;
   if (!pNMainWindow) return;
   pNMainWindow->nbOpen       .setAlwaysBorder(!set);
   pNMainWindow->nbSave       .setAlwaysBorder(!set);
   pNMainWindow->nbPrint      .setAlwaysBorder(!set);
   pNMainWindow->nbOptions    .setAlwaysBorder(!set);
   pNMainWindow->nbLocate     .setAlwaysBorder(!set);
   pNMainWindow->nbHoriVert   .setAlwaysBorder(!set);
   pNMainWindow->nbAncestors  .setAlwaysBorder(!set);
   pNMainWindow->nbDescendants.setAlwaysBorder(!set);
   pNMainWindow->nbSiblings   .setAlwaysBorder(!set);
   pNMainWindow->nbTopBottom  .setAlwaysBorder(!set);
}
Boolean NMainWindow::isBorderlessButtons()
{
   return(bProfileBorderlessButtons);
}


IBase::Boolean NMainWindow::command(ICommandEvent &iceEvent)
{
   Boolean eventProcessed(true);
   switch (iceEvent.commandId()) {
      case ID_nbLocate:
         NSearchDialog::generate(this,&mankind);
         break;
      case MI_FileNew:
         if (mankind.hasChanged()) {
            IMessageBox msgBox(this);
            Boolean choice=msgBox.show(NApplication::pDLLResource->loadString(SI_SaveNow)
                                      ,IMessageBox::yesNoCancelButton|IMessageBox::queryIcon|IMessageBox::moveable);
            if (choice==IMessageBox::cancel)
               break;
            if (choice==IMessageBox::yes) {
               threadLoadSavePrint.setVariable("TASK","NEW");
               saveDialog(NApplication::isTree);
               break;
               }
            }
         newData();
         break;
      case MI_FileOpenDropped:
         if (mankind.hasChanged()) {
            IMessageBox msgBox(this);
            Boolean choice=msgBox.show(NApplication::pDLLResource->loadString(SI_SaveNow)
                                      ,IMessageBox::yesNoCancelButton|IMessageBox::queryIcon|IMessageBox::moveable);
            if (choice==IMessageBox::cancel)
               break;
            if (choice==IMessageBox::yes) {
               threadLoadSavePrint.setVariable("TASK","DROP");;
               saveDialog(NApplication::isTree);
               break;
               }
            }
         loadDialog(mantree.getLastFileDrop());
         break;
      case MI_FileOpenStartup:
         loadDialog(NApplication::isTree);
         break;
      case MI_FileOpen:
      case ID_nbOpen:
         if (mankind.hasChanged()) {
            IMessageBox msgBox(this);
            Boolean choice=msgBox.show(NApplication::pDLLResource->loadString(SI_SaveNow)
                                      ,IMessageBox::yesNoCancelButton|IMessageBox::queryIcon|IMessageBox::moveable);
            if (choice==IMessageBox::cancel)
               break;
            if (choice==IMessageBox::yes) {
               threadLoadSavePrint.setVariable("TASK","LOAD");;
               saveDialog(NApplication::isTree);
               break;
               }
            }
         loadDialog();
         break;
      case MI_FileSave:
      case ID_nbSave:
         saveDialog(NApplication::isTree);
         break;
      case MI_FileSaveAs:
         saveDialog();
         break;
      case MI_FileClose:
         close();
         break;
      case MI_FilePrint:
      case ID_nbPrint:
         printData();
         break;
      case MI_ManOrigin:
         mankind.makeOrigin();
         break;
      case MI_ManPartnerNew:
         addPartner();
         break;
      case MI_ManPartnerRemembered:
         addPartner(mankind.getRemembered());
         break;
      case MI_ManParents:
         addParents();
         break;
      case MI_ManEdit:
         if (!NEditDialog::pNEditDialog) new NEditDialog(this,&mankind);
         else NEditDialog::pNEditDialog->close();
         break;
      case ID_nbAncestors:
      case MI_MankindAncestors: {
         NQueryGenerations query(1,this
                                ,NApplication::pDLLResource->loadString(SI_QueryGenerationsParent)
                                ,mantree.getDepth(NManTree::totalancestors)
                                ,mantree.getDepth(NManTree::ancestors)
                                ,NApplication::pDLLResource->loadString(SI_MsgGenAncestors));
         query.setFocus();
         if (DID_OK==query.showModally())
            mantree.changeDepth(NManTree::ancestors,query.sResult);
         calcMessageText();
         } break;
      case ID_nbDescendants:
      case MI_MankindDescendants: {
         NQueryGenerations query(1,this
                                ,NApplication::pDLLResource->loadString(SI_QueryGenerationsChild)
                                ,mantree.getDepth(NManTree::totaldescendants)
                                ,mantree.getDepth(NManTree::descendants)
                                ,NApplication::pDLLResource->loadString(SI_MsgGenDescendants));
         query.setFocus();
         if (DID_OK==query.showModally())
            mantree.changeDepth(NManTree::descendants,query.sResult);
         calcMessageText();
         } break;
      case ID_nbSiblings:
      case MI_MankindSiblings: {
         if (mantree.getDepth(NManTree::siblings)>0)
            mantree.changeDepth(NManTree::siblings,0);
         else
            mantree.changeDepth(NManTree::siblings,1);
         calcMessageText();
         } break;
      case MI_MankindZoomOut: {
         unsigned long z=isZoom.armTickOffset()-1;
         if (z>=isZoom.numberOfTicks(IProgressIndicator::scale1))
            z=0;
         try { isZoom.moveArmToTick(z); } catch (IException) {};
         } break;
      case MI_MankindZoomIn: {
         unsigned long z=isZoom.armTickOffset()+1;
         if (z>=isZoom.numberOfTicks(IProgressIndicator::scale1))
            z=isZoom.numberOfTicks(IProgressIndicator::scale1)-1;
         try { isZoom.moveArmToTick(z); } catch (IException) {};
         } break;
      case ID_nbHoriVert:
         if (nbHoriVert.isLatched()) mantree.changeDir(NManTree::horizontal);
         else mantree.changeDir(NManTree::vertical);
         break;
      case ID_nbTopBottom:
         removeExtension(&nbbButtonBar);
         if (nbTopBottom.isLatched()) {
            addExtension(&nbbButtonBar,IFrameWindow::belowClient);
            bProfileButtonBarTop=False;
            }
         else {
            addExtension(&nbbButtonBar,IFrameWindow::aboveClient);
            bProfileButtonBarTop=True;
            }
         useExtensionMinimumSize(&nbbButtonBar);
         break;
      case MI_MankindHori:
         nbHoriVert.latch(True);
         mantree.changeDir(NManTree::horizontal);
         break;
      case MI_MankindVert:
         nbHoriVert.latch(False);
         mantree.changeDir(NManTree::vertical);
         break;
      case MI_MankindList:
         if (NListDialog::pNListDialog==NULL)
            new NListDialog(this,&mankind);
         else NListDialog::pNListDialog->close();
         break;
      case MI_MankindEvents:
         if (NEventDialog::pNEventDialog==NULL)
            new NEventDialog(this,&mankind);
         else NEventDialog::pNEventDialog->close();
         break;
      case MI_MankindToolbar:
         if (isAnExtension(&nbbButtonBar)) {
            removeExtension(&nbbButtonBar);
            bProfileButtonBarVis=False;
            }
         else {
            addExtension(&nbbButtonBar,bProfileButtonBarTop?IFrameWindow::aboveClient:IFrameWindow::belowClient);
            useExtensionMinimumSize(&nbbButtonBar);
            bProfileButtonBarVis=True;
            }
         break;
      case MI_ManDelParents:
         delParents();
         break;
      case MI_ManDelThis:
         delThis();
         break;
      case MI_ManCenter:
         mantree.changeDir(mantree.getDir());
         break;
      case MI_ManRemember:
         mankind.remember();
         break;
      case MI_RexxStart:
         startRexx(0);
         break;
      case MI_About:
         {
         NAboutDialog nAboutDialog(this);
         nAboutDialog.setFocus();
         nAboutDialog.showModally();
         if (NApplication::isRegisteredUser.size()>0)
            itTitle.setObjectText(NApplication::pDLLResource->loadString(SI_MainTitle));
         }
         break;
      default:
         // Child wish ?
         if ((iceEvent.commandId()>MI_ManChildWith)&&(iceEvent.commandId()<(MI_ManChildWith+MAXPARTNERS))) {
            addChild(iceEvent.commandId()-MI_ManChildWith);
            break;
            }
         if ((iceEvent.commandId()>MI_ManChildNew)&&(iceEvent.commandId()<(MI_ManChildNew+MAXPARTNERS))) {
            addChild(iceEvent.commandId()-MI_ManChildNew);
            break;
            }
         if ((iceEvent.commandId()>MI_ManChildRemembered)&&(iceEvent.commandId()<(MI_ManChildRemembered+MAXPARTNERS))) {
            addRememberedChild(iceEvent.commandId()-MI_ManChildRemembered);
            break;
            }
         // Partner delete ?
         if ((iceEvent.commandId()>MI_ManDelPartner)&&(iceEvent.commandId()<MI_ManDelChilds)) {
            delPartner(iceEvent.commandId()-MI_ManDelPartner);
            break;
            }
         // Child delete ?
         if ((iceEvent.commandId()>MI_ManDelChilds)&&(iceEvent.commandId()<MI_ManDelParents)) {
            delChilds(iceEvent.commandId()-MI_ManDelChilds);
            break;
            }
         // Rexx script ?
         if ((iceEvent.commandId()>MI_RexxStart)&&(iceEvent.commandId()<=MI_RexxStart+20)) {
            startRexx(iceEvent.commandId()-MI_RexxStart);
            break;
            }
         // Options ?
         if (   ((iceEvent.commandId()>=MI_OptionsLayout)&&(iceEvent.commandId()<=MI_OptionsUserFields))
              ||( iceEvent.commandId()==ID_nbOptions)  ) {
            if (!NOptionsDialog::pNOptionsDialog) new NOptionsDialog();
            if (NOptionsDialog::pNOptionsDialog) {
               if (iceEvent.commandId()!=ID_nbOptions)
                  NOptionsDialog::pNOptionsDialog->turnToPage(iceEvent.commandId()-MI_OptionsLayout+1);
               NOptionsDialog::pNOptionsDialog->show();
               NOptionsDialog::pNOptionsDialog->positionOnSiblings();
               NOptionsDialog::pNOptionsDialog->setFocus();
               }
            break;
            }
         // Nothing found
         eventProcessed=false;
      }
   return(eventProcessed);
} // end NMainWindow::command

void NMainWindow::fillViewMenu(IMenu* pMenu)
{
   // View Mode
   if (mantree.getDepth(NManTree::siblings)>0)
      pMenu->checkItem(MI_MankindSiblings);
   // List Dialog
   if (NListDialog::pNListDialog!=NULL) pMenu->checkItem(MI_MankindList);
   // Event Dialog
   if (NEventDialog::pNEventDialog!=NULL) pMenu->checkItem(MI_MankindEvents);
   // Toolbar
   if (isAnExtension(&nbbButtonBar)) pMenu->checkItem(MI_MankindToolbar);
   // Display for direction
   switch (mantree.getDir()) {
      case NManTree::horizontal: pMenu->checkItem(MI_MankindHori);break;
      case NManTree::vertical  : pMenu->checkItem(MI_MankindVert);break;
      }
   // Done
}

void NMainWindow::fillManMenu(IMenu* pMenu)
{
   NPerson* pMan=mankind.getActual();
   IString isRemembered(NApplication::pDLLResource->loadString(SI_NoneRemembered));
   Boolean remembered=mankind.getRemembered()!=NULL;
   if (remembered) {
      isRemembered=mankind.getRemembered()->getCallFirstName()
                   +" "+mankind.getRemembered()->getName();
      isRemembered.remove(30);
      }
   // Edit Window ?
   if (NEditDialog::pNEditDialog!=NULL) pMenu->checkItem(MI_ManEdit);
   // Add Child ?
   if (!pMan->hasPartners()) pMenu->disableItem(MI_ManChild);
   else {
      for (short int i=1;i<=pMan->getNoOfPartners();i++) {
         // Add Partnername as Submenu
         pMenu->addItem(IMenuItem(MI_ManChildWith+i
                                 ,i%16==0?IMenuItem::splitWithSeparator:IMenuItem::noStyle)
                        .setText(IString(pMan->getPartnerName(i)+"  ").remove(30))
                       ,MI_ManChild);
         pMenu->addSubmenu(MI_ManChildWith+i);
         // Add New (default)
         pMenu->addItem(IMenuItem(MI_ManChildNew+i).setText(NApplication::pDLLResource->loadString(SI_New))
                       ,MI_ManChildWith+i);
         pMenu->setConditionalCascade(MI_ManChildWith+i,MI_ManChildNew+i);
         // Add Remembered
         pMenu->addItem(IMenuItem(MI_ManChildRemembered+i,IMenuItem::noStyle
                                 ,remembered&&(!pMan->hasChild(mankind.getRemembered(),i))
                                  ? IMenuItem::noAttribute : IMenuItem::disabled)
                        .setText(isRemembered)
                       ,MI_ManChildWith+i);
         // Max Children ?
         if (pMan->getNoOfChildrenWith(i)==MAXCHILDREN)
            pMenu->disableItem(MI_ManChildWith+i);
         // Next Partner
         }
      // Only one Partner ?
      if (i==2) pMenu->setConditionalCascade(MI_ManChild,MI_ManChildWith+1);
      }
   // Add Partners ?
   pMenu->addItem(IMenuItem(MI_ManPartnerRemembered).setText(isRemembered)
                 ,MI_ManPartner);
   if (pMan->getNoOfPartners()==MAXPARTNERS)
      pMenu->disableItem(MI_ManPartner);
   pMenu->setConditionalCascade(MI_ManPartner,MI_ManPartnerNew);
   if ( (!remembered                              )
      ||(pMan->hasPartner(mankind.getRemembered()))
      ||(pMan==mankind.getRemembered()            ) )
      pMenu->disableItem(MI_ManPartnerRemembered);
   // Add Parents
   if (pMan->hasParents())
      pMenu->disableItem(MI_ManParents);
   // Remember ?
   if (pMan==mankind.getRemembered()) pMenu->disableItem(MI_ManRemember);
   // Origin ?
   if (mankind.isOrigin(pMan)) pMenu->disableItem(MI_ManOrigin);
   // Delete Partner
   if (pMan->getNoOfPartners()==0) pMenu->disableItem(MI_ManDelPartner);
   else {
      for (short int i=1,j=1;i<=pMan->getNoOfPartners();i++) {
         pMenu->addItem(IMenuItem(MI_ManDelPartner+i
                                 ,j%16==0?IMenuItem::splitWithSeparator:IMenuItem::noStyle)
                       .setText(pMan->getPartnerName(i).remove(30))
                       ,MI_ManDelPartner);
         }
      if (pMan->getNoOfPartners()==1)
         pMenu->setConditionalCascade(MI_ManDelPartner,MI_ManDelPartner+1);
      }
   // Delete This
   if (mankind.getActual()==mankind.getOrigin())
     pMenu->disableItem(MI_ManDelThis);
   // Delete Childs
   if (!pMan->hasChildren()) pMenu->disableItem(MI_ManDelChilds);
   else {
      // Fill Menu with Partners that have children with Actual
      Boolean child=False;
      for (short int i=1;i<=pMan->getNoOfPartners();i++) {
         if (pMan->getNoOfChildrenWith(i)>0) {
            pMenu->addItem(IMenuItem(MI_ManDelChilds+i
                                    ,i%16==0?IMenuItem::splitWithSeparator:IMenuItem::noStyle)
                          .setText(pMan->getPartnerName(i).remove(30))
                          ,MI_ManDelChilds);
            child=True;
            }
         if ((pMan->getNoOfPartners()==1)&&(child))
            pMenu->setConditionalCascade(MI_ManDelChilds,MI_ManDelChilds+1);
         }
      }
   // Delete Parents
   if (!pMan->hasParents())
      pMenu->disableItem(MI_ManDelParents);
   // Done
}

Boolean NMainWindow::edit(IControlEvent& iceEvent)
{
   // Do the Zooming
   mantree.changeZoom(0.1+((double)isZoom.armTickOffset()/10));
   // Done
   return(True);
} /* end NMainWindow::edit */

Boolean NMainWindow::menuShowing(IMenuEvent& menuEvent,ISubmenu& submenuAboutToShow)
{
   // SetUp MenuStuff
   switch (submenuAboutToShow.id()) {
      case MI_MainFile:
         if (NApplication::pNPrinter->count()==0) submenuAboutToShow.disableItem(MI_FilePrint);
         submenuAboutToShow.disableItem(MI_FileMerge);
         break;
      case MI_MainView:
         fillViewMenu(&submenuAboutToShow);
         break;
      case MI_MainRexx:
         for (unsigned i=1;i<=NApplication::nrRexx.getRememberCount();i++) {
            submenuAboutToShow.addItem(IMenuItem(MI_RexxStart+i)
                                       .setText(IString('~')+IString(i-1).d2x().subString(2,1)+" "
                                                +NApplication::nrRexx.getRemembered(i)
                                               )
                                      ,0);
            }
         break;
      }
   // Changes can be thrown away after show
   return(True);
}  /* end NMainWindow::menuShowing */

void NMainWindow::addPartner(PNPerson pmanPartner)
{
   mankind.addManPartner(pmanPartner);
   // Done
   calcMessageText();
}
void NMainWindow::addParents()
{
   mankind.addManParents(NApplication::pDLLResource->loadString(SI_Mom),NApplication::pDLLResource->loadString(SI_Dad));
   // Done
   calcMessageText();
}
void NMainWindow::addRememberedChild(short int iPartner)
{
   // Check if we have a remembered
   if (!mankind.getRemembered()) return;
   // Check if remembered has Parents -> Warning about loss
   disableInput();
   IMessageBox msgBox(this);
   if (mankind.getRemembered()->hasParents()) {
      if (IMessageBox::yes
          !=msgBox.show(NApplication::pDLLResource->loadString(SI_ReallyStealChild1)
                        +mankind.getRemembered()->getFirstName()+" "
                        +mankind.getRemembered()->getName()
                        +NApplication::pDLLResource->loadString(SI_ReallyStealChild2)
                       ,IMessageBox::yesNoButton|IMessageBox::queryIcon|IMessageBox::moveable )) {
         enableInput();
         return;
         }
      }
   // Do the adding and wait for result (maybe loop)
   if (!mankind.addManChild(iPartner,mankind.getRemembered()))
       msgBox.show(mankind.getRemembered()->getFirstName()+" "
                   +mankind.getRemembered()->getName()
                   +NApplication::pDLLResource->loadString(SI_AncestorLoop1)
                   +mankind.getActual()->getFirstName()+" "
                   +mankind.getActual()->getName()
                   +NApplication::pDLLResource->loadString(SI_AncestorLoop2)
                  ,IMessageBox::okButton|IMessageBox::warningIcon|IMessageBox::moveable );
   enableInput();
   // Done
   calcMessageText();
}

void NMainWindow::addChild(short int iPartner)
{
   mankind.addManChild(iPartner);
   // Done
   calcMessageText();
}

void NMainWindow::delPartner(short int iPartner)
{
   disableInput();
   IMessageBox msgBox(this);
   if (IMessageBox::yes
       !=msgBox.show(NApplication::pDLLResource->loadString(SI_ReallyDelPartner1)
                     +mankind.getActual()->getFirstName()+" "
                     +mankind.getActual()->getName()
                     +NApplication::pDLLResource->loadString(SI_ReallyDelPartner2)
                    ,IMessageBox::yesNoButton|IMessageBox::queryIcon|IMessageBox::moveable )) {
      enableInput();
      return;
      }
   // Delete Childs
   mankind.deletePartner(iPartner);
   enableInput();
   // Done
   calcMessageText();
}

void NMainWindow::delChilds(short int iPartner)
{
   disableInput();
   IMessageBox msgBox(this);
   if (IMessageBox::yes
       !=msgBox.show(NApplication::pDLLResource->loadString(SI_ReallyDelChilds1)
                     +mankind.getActual()->getFirstName()+" "
                     +mankind.getActual()->getName()
                     +NApplication::pDLLResource->loadString(SI_ReallyDelChilds2)
                    ,IMessageBox::yesNoButton|IMessageBox::queryIcon|IMessageBox::moveable )) {
      enableInput();
      return;
      }
   // Delete Childs
   mankind.deleteChilds(iPartner);
   // Done
   calcMessageText();
   enableInput();
}

void NMainWindow::delThis()
{
   disableInput();
   IMessageBox msgBox(this);
   if (IMessageBox::yes
       !=msgBox.show(NApplication::pDLLResource->loadString(SI_ReallyDelThis1)
                     +mankind.getActual()->getFirstName()+" "
                     +mankind.getActual()->getName()
                     +NApplication::pDLLResource->loadString(SI_ReallyDelThis2)
                    ,IMessageBox::yesNoButton|IMessageBox::queryIcon|IMessageBox::moveable )) {
      enableInput();
      return;
      }
   // Delete Actual
   mankind.deleteMan();
   // Done
   calcMessageText();
   enableInput();
}
void NMainWindow::delParents() {
   disableInput();
   IMessageBox msgBox(this);
   if (IMessageBox::yes
       !=msgBox.show(NApplication::pDLLResource->loadString(SI_ReallyDelParents1)
                     +mankind.getActual()->getFirstName()+" "
                     +mankind.getActual()->getName()
                     +NApplication::pDLLResource->loadString(SI_ReallyDelParents2)
                    ,IMessageBox::yesNoButton|IMessageBox::queryIcon|IMessageBox::moveable )) {
      enableInput();
      return;
      }
   // Delete Parents
   mankind.deleteParents();
   // Done
   calcMessageText();
   enableInput();
}

void NMainWindow::loadDialog(const IString& isFile)
{
   disableInput();
   Boolean ask=isFile.length()==0;
   // Put together FileDialog
   IFileDialog::Settings fsettings;
   fsettings.setTitle(NApplication::pDLLResource->loadString(SI_DlgFileLoad));
   if (ask) {
      if (NApplication::isTree.length()==0) fsettings.setFileName(TREEFILE);
      else fsettings.setFileName(NApplication::isTree.subString(1,NApplication::isTree.lastIndexOf('\\'))+IString(TREEFILE));
      }
   else fsettings.setFileName(isFile);
   // Go for load Loop
   do {
      // Do we have to ask for filename ?
      if (ask) {
         IFileDialog filedialog(IWindow::desktopWindow(),this
                               ,fsettings,IFileDialog::classDefaultStyle|IFileDialog::selectableListbox);
         if (!filedialog.pressedOK()) {
            enableInput();
            return;
            }
         fsettings.setFileName(filedialog.fileName());
         }
      // Start Loading with Filename
      ask=!startLoad(fsettings.fileName());
      }
   while (ask);
   // Done
} /* end NMainWindow::loadDialog */

void NMainWindow::saveDialog(const IString& isFile)
{
   disableInput();
   Boolean ask=isFile.length()==0;
   // Put together FileDialog
   IFileDialog::Settings fsettings;
   fsettings.setTitle(NApplication::pDLLResource->loadString(SI_DlgFileSave));
   if (ask) {
      if (NApplication::isTree.length()==0) fsettings.setFileName(TREEFILE);
      else fsettings.setFileName(NApplication::isTree.subString(1,NApplication::isTree.lastIndexOf('\\'))+IString(TREEFILE));
      }
   else fsettings.setFileName(isFile);
   fsettings.setSaveAsDialog();
   // Go for save Loop
   do {
      // Do we have to ask for filename ?
      if (ask) {
         IFileDialog filedialog(IWindow::desktopWindow(),this
                               ,fsettings,IFileDialog::classDefaultStyle|IFileDialog::selectableListbox);
         if (!filedialog.pressedOK()) {
            calcMessageText();
            enableInput();
            return;
            }
         fsettings.setFileName(filedialog.fileName());
         }
      // Start Saving with Filename
      ask=!startSave(fsettings.fileName());
      }
   while (ask);
} /* end NMainWindow::saveDialog */

Boolean NMainWindow::startSave(const IString& isFile)
{
   // Make a backup
   if (bProfileDoBackup) {
      IString isBak(isFile);
      unsigned short ext=isBak.upperCase().indexOf(".FAM");
      if (ext>0) isBak.remove(ext);
      isBak+=".BAK";
      nslMessage.setText(NApplication::pDLLResource->loadString(SI_MsgDoingBackup)
                        +isBak);
      DosCopy(isFile,isBak,1);
      }
   // Go for saving
   NApplication::fsFileStream.open(isFile,ios::binary|ios::out);
   if (!NApplication::fsFileStream.good()) {
      IMessageBox msgBox(this);
      msgBox.show(NApplication::pDLLResource->loadString(SI_DlgFileError)+isFile,IMessageBox::information );
      return(False);
      }
   // Do a TREE !
   NApplication::fsFileStream << "TREE";
   // Start Threaded Saving
   NApplication::isTree=isFile;
   while (threadLoadSavePrint.isStarted()) DosSleep(100);
   threadLoadSavePrint.start(new IThreadMemberFn<NMainWindow>(*this,NMainWindow::threadFnSave),True);
   // Done for now
   return(True);
}  /* end NMainWindow::startSave */

Boolean NMainWindow::startLoad(const IString& isFile)
{
   // Open file
   NApplication::fsFileStream.open(isFile,ios::binary|ios::in|ios::nocreate);
   if (!NApplication::fsFileStream.good()) {
      IMessageBox msgBox(this);
      msgBox.show(NApplication::pDLLResource->loadString(SI_DlgFileError)+isFile
                 ,IMessageBox::information );
      return(False);
      }
   // Check FileHeader
   if (!mankind.checkHeader(NApplication::fsFileStream)) {
      IMessageBox msgBox(this);
      msgBox.show(NApplication::pDLLResource->loadString(SI_DlgFileFalse)+isFile
                 ,IMessageBox::catastrophic );
      NApplication::fsFileStream.close();
      return(False);
      }
   // Start Threaded Loading
   NApplication::isTree=isFile;
   while (threadLoadSavePrint.isStarted()) DosSleep(100);
   threadLoadSavePrint.start(new IThreadMemberFn<NMainWindow>(*this,NMainWindow::threadFnLoad),True);
   // Done for now
   return(True);
} /* end NMainWindow::startLoad */

void NMainWindow::checkThreadTask()
{
   IString task(threadLoadSavePrint.variable("TASK"));
   threadLoadSavePrint.setVariable("TASK","");
   if (task=="CLOSE") postEvent(WM_CLOSE);
   if (task=="NEW"  ) postEvent(IWindow::command,MI_FileNew);
   if (task=="LOAD" ) postEvent(IWindow::command,MI_FileOpen);
   if (task=="DROP" ) postEvent(IWindow::command,MI_FileOpenDropped);
   if (task=="REXX" ) postEvent(IWindow::command,MI_RexxStart+0);
}

void NMainWindow::threadFnSave()
{
   // Saving
   mankind.save(NApplication::fsFileStream);
   // Check for successfull save
   if (!NApplication::fsFileStream.good()) {
      IMessageBox msgBox(this);
      msgBox.show(NApplication::pDLLResource->loadString(SI_DlgFileBadWrite)+NApplication::isTree
                 ,IMessageBox::catastrophic );
      threadLoadSavePrint.setVariable("TASK","");;
      }
   // Title ...
   itTitle.setViewText(NApplication::isTree);
   NApplication::fsFileStream.close();
   // ReSetup View & List & Edit
   calcMessageText();
   enableInput();
   // Check for ThreadMessage
   checkThreadTask();
   // Done
}

void NMainWindow::threadFnLoad()
{
   // Loading
   if (mankind.load(NApplication::fsFileStream))
      itTitle.setViewText(NApplication::isTree);
   else {
      IMessageBox msgBox(this);
      msgBox.show(NApplication::pDLLResource->loadString(SI_DlgFileFalse)+NApplication::isTree
                 ,IMessageBox::catastrophic );
      NApplication::isTree=IString();
      itTitle.setViewText(IString());
      }
   NApplication::fsFileStream.close();
   // ReSetup View & List & Edit
   calcMessageText();
   // Enable
   enableInput();
   // Check for ThreadMessage
   checkThreadTask();
   // Done
}

void NMainWindow::threadFnPrint()
{
   // Get graphics context
   IGraphicContext* pigc=NApplication::pNPrinter->graphicContext();
   if (pigc) {
      // Do the Drawing
      ISize isPageSize=NApplication::pNPrinter->getPageSize();
      ISize isPages   =NApplication::pNPrinter->getPages();
      double zoomX    =NApplication::pNPrinter->getZoomX();
      double zoomY    =NApplication::pNPrinter->getZoomY();
      IPoint ipShift;
      // Go for horizontal Pages
      int number=1;
      for (int x=1;x<=isPages.width();x++)
         // ... and vertikal Pages
         for (int y=1;y<=isPages.height();y++) {
            // Show Information about page
            nslMessage.setText(NApplication::pDLLResource->loadString(SI_MsgPrinting)
                               +IString(number++)+IString(" / ")
                               +IString(isPages.width()*isPages.height()));
            // New Page (first page doesn't)
            if ((x!=1)||(y!=1)) NApplication::pNPrinter->newPage();
            // Calculate View Position for this page
            ipShift.setX(isPageSize.width()*(isPages.width()-x));
            ipShift.setY(isPageSize.height()*(isPages.height()-y));
            mantree.printOn(*pigc
                           ,IRectangle(ipShift,isPageSize)
                           ,zoomX
                           ,zoomY);
            // Next page
            }
      // Close Printer
      NApplication::pNPrinter->stopPrinting();
   }
   // ReSetup View & List & Edit
   calcMessageText();
   enableInput();
   // Check for ThreadMessage
   checkThreadTask();
   // Done
}

void NMainWindow::printData()
{
   // No Input beyond this point
   disableInput();
   // Try to get a Printer GraphicContext
   if (!NApplication::pNPrinter->startPrinting(this,mantree.getTreeSize(),NApplication::isTree)) {
      enableInput();
      return;
      }
   // Start threaded printing
   while (threadLoadSavePrint.isStarted()) DosSleep(100);
   threadLoadSavePrint.start(new IThreadMemberFn<NMainWindow>(*this,NMainWindow::threadFnPrint),True);
   // Done
}

void NMainWindow::newData()
{
   // Do it
   mankind.newMankind();
   // Data set
   NApplication::isTree=IString();
   itTitle.setViewText(IString());
   // Enable
   enableInput();
   calcMessageText();
}

void NMainWindow::startRexx(unsigned number)
{
   disableInput();
   if (mankind.hasChanged()) {
      IMessageBox msgBox(this);
      Boolean choice=msgBox.show(NApplication::pDLLResource->loadString(SI_SaveNow)
                                ,IMessageBox::yesNoCancelButton|IMessageBox::queryIcon|IMessageBox::moveable);
      if (choice==IMessageBox::cancel) {
         enableInput();
         return;
         }
      if (choice==IMessageBox::yes) {
         threadLoadSavePrint.setVariable("TASK","REXX");
         saveDialog(NApplication::isTree);
         return;
         }
      }
   if (!NApplication::nrRexx.startRexx(this,number))
      enableInput();
}

void NMainWindow::doneWithRexx()
{
   calcMessageText();
   enableInput();
}

void NMainWindow::originChanged(PNPerson pOld,PNPerson pNew)
{
   calcMessageText();
}

void NMainWindow::progressReport(const IString& isReport)
{
   nslMessage.setText(isReport);
}

