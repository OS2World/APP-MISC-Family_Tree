/*
   File   : ListDialog.cpp
   Purpose: Define Classes for the Mankind list window
*/

#include <imenu.hpp>
#include <ilistbox.hpp>
#include <iselhdr.hpp>
#include <igroupbx.hpp>

#include "NListDialog.hpp"
#include "Main.hpp"
#include "FTree.h"

 #define INCL_WINSWITCHLIST      /* Window Task Switch Functions */
 #include <os2.h>

// Class  : NQuerySort
// Purpose: query sorting criterias from user
class NQuerySort : public IFrameWindow,
                   public ICommandHandler,
                   public ISelectHandler
{
public:
   NQuerySort(long windowId,IWindow* pOwner,NManList* pManList);
   virtual ~NQuerySort();
   IString isResult;
private:
   IMultiCellCanvas imccCanvas;
   IGroupBox        igbLeft,igbRight;
   IListBox         ilbLeft,ilbRight;
   NButton          nbToLeft,nbToRight,nbUp,nbDown;
   IPushButton      ipbOK,ipbCANCEL;
protected:
   virtual Boolean command(ICommandEvent& event);
   virtual Boolean enter(IControlEvent& event);
   virtual Boolean selected(IControlEvent& event);
};

#define ID_CANVAS  1
#define ID_LBOX    2
#define ID_RBOX    3
#define ID_LLIST   4
#define ID_RLIST   5
#define ID_TOLBUT  6
#define ID_TORBUT  7
#define ID_UPBUT   8
#define ID_DWNBUT  9
#define ID_OKBUT  10
#define ID_CBUT   11

NQuerySort::NQuerySort(long windowId,IWindow* pOwner,NManList* pManList)
   : IFrameWindow(windowId
                 ,NULL,pOwner,IRectangle()
                 ,IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder
                 ,NApplication::pDLLResource->loadString(SI_ListSortBy))
   , ICommandHandler()
   , ISelectHandler()
   , imccCanvas(ID_CANVAS,this,this)
   , igbLeft   (ID_LBOX,&imccCanvas,&imccCanvas)
   , igbRight  (ID_RBOX,&imccCanvas,&imccCanvas)
   , ilbLeft   (ID_LLIST,&imccCanvas,&imccCanvas,IRectangle(),IWindow::visible)
   , ilbRight  (ID_RLIST,&imccCanvas,&imccCanvas,IRectangle(),IWindow::visible)
   , nbToLeft  (ID_TOLBUT,&imccCanvas,&imccCanvas,True)
   , nbToRight (ID_TORBUT,&imccCanvas,&imccCanvas,True)
   , nbUp      (ID_UPBUT,&imccCanvas,&imccCanvas,True)
   , nbDown    (ID_DWNBUT,&imccCanvas,&imccCanvas,True)
   , ipbOK     (ID_OKBUT,&imccCanvas,&imccCanvas,IRectangle(),IPushButton::classDefaultStyle|IPushButton::defaultButton)
   , ipbCANCEL (ID_CBUT,&imccCanvas,&imccCanvas)
{
   // Layout
   setFont(IFont("Helv",8));

   igbLeft  .setText(NApplication::pDLLResource->loadString(SI_ListSortFields));
   igbRight .setText(NApplication::pDLLResource->loadString(SI_ListSortSequence));
   nbToLeft .setBitmap(RES_BmpLEFT)
            .disableInput();
   nbToRight.setBitmap(RES_BmpRIGHT)
            .disableInput();
   nbUp     .setBitmap(RES_BmpUP)
            .disableInput();
   nbDown   .setBitmap(RES_BmpDOWN)
            .disableInput();
   ipbOK    .setText(NApplication::pDLLResource->loadString(SI_OK));
   ipbCANCEL.setText(NApplication::pDLLResource->loadString(SI_Cancel));
   ilbLeft  .setMinimumRows(8)
            .setMinimumCharacters(16);
   ilbRight .setMinimumCharacters(16);
   imccCanvas.setDefaultCell(ISize(7,7));
   imccCanvas.addToCell(&igbLeft   , 2, 2, 6, 7)
             .addToCell(&igbRight  ,11, 2, 7, 7)
             .addToCell(&ilbLeft   , 3, 4, 4, 4)
             .addToCell(&ilbRight  ,12, 4, 5, 2)
             .addToCell(&nbToLeft  , 9, 5      )
             .addToCell(&nbToRight , 9, 7      )
             .addToCell(&ipbOK     , 3,10      )
             .addToCell(&ipbCANCEL , 5,10      )
             .addToCell(&nbUp      ,13, 7      )
             .addToCell(&nbDown    ,15, 7      )
             .setRowHeight  (11,imccCanvas.defaultCell().height())
             .setColumnWidth(18,imccCanvas.defaultCell().width ());
   imccCanvas.setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
   // Add sorting keys to right list
   short num;
   IString isSort=pManList->getSortBy();
   for (unsigned i=1;i<=isSort.length();i++) {
      num=(int)isSort[i];
      ilbRight.setItemHandle(ilbRight.addAsLast(pManList->columnName(num)),num);
      }
   // ... add remaining sorting keys to left
   for (i=1;i<=pManList->columnCount();i++)
      if (!isSort.includes((char)i))
         ilbLeft.setItemHandle(ilbLeft.addAsLast(pManList->columnName(i)),i);
   // Handler
   ISelectHandler::handleEventsFor(&ilbLeft);
   ISelectHandler::handleEventsFor(&ilbRight);
   ICommandHandler::handleEventsFor(this);
   // ... show it
   ISize size=imccCanvas.minimumSize();
   moveSizeToClient(IRectangle(IPoint(pOwner->rect().centerXCenterY()-size/2),size));
   setClient(&imccCanvas);
   ilbLeft.setFocus();
   // Done
}

NQuerySort::~NQuerySort() {}

Boolean NQuerySort::command(ICommandEvent& event)
{
   // Prepare ...
   IListBox *pfrom=&ilbLeft,
            *pto  =&ilbRight;
   // ... investigate buttons
   switch (event.commandId()) {
      case ID_OKBUT : { // Prepare result
                      for (short i=1;i<=ilbRight.count();i++)
                         isResult+=IString((char)ilbRight.itemHandle(i-1));
                      dismiss(True);
                      } break;
      case ID_CBUT  : dismiss(False);break;
      case ID_TOLBUT: pfrom=&ilbRight;
                      pto  =&ilbLeft;
      case ID_TORBUT: { // Lookup item
                      short item=pfrom->selection();
                      if (item==IBaseListBox::notFound) break;
                      // ... move it
                      pto->setItemHandle(pto->addAsLast(pfrom->itemText(item))
                                        ,pfrom->itemHandle(item));
                      pfrom->remove(item);
                      // ... prepare next item in from list
                      if (pfrom->count()>0) pfrom->select(0);
                      // ... check for empty lists
                      if (ilbRight.count()==0) {
                         ipbOK.disable();
                         nbToLeft.disableInput();
                         }
                      else ipbOK.enable();
                      if (ilbLeft.count()==0)
                         nbToRight.disableInput();
                      } break;
      case ID_DWNBUT: // Up&Down
      case ID_UPBUT : { // Lookup item
                      short item=ilbRight.selection();
                      if (item==IBaseListBox::notFound) break;
                      // ... move it
                      IString itemText=ilbRight.itemText(item);
                      short itemHandle=ilbRight.itemHandle(item);
                      ilbRight.remove(item);
                      if (event.commandId()==ID_DWNBUT) item++;
                      else item--;
                      ilbRight.setItemHandle(ilbRight.add(item,itemText)
                                            ,itemHandle)
                              .select(item);
                      }break;
      }
   // Done
   return(False);
}

Boolean NQuerySort::enter(IControlEvent& event)
{
   // Find out from/to listbox
   switch (event.controlId()) {
      case ID_LLIST : nbToRight .click();break;
      case ID_RLIST : nbToLeft.click();break;
      }
   // Done
   return(True);
}

Boolean NQuerySort::selected(IControlEvent& event)
{
   switch (event.controlId()) {
      case ID_LLIST : // Right is out
                      ISelectHandler::disable();
                      ilbRight.deselectAll();
                      ISelectHandler::enable();
                      // Buttons influence
                      nbToRight.enableInput();
                      nbToLeft .disableInput();
                      nbUp     .disableInput();
                      nbDown   .disableInput();
                      break;
      case ID_RLIST : // Left list is out
                      ISelectHandler::disable();
                      ilbLeft .deselectAll();
                      ISelectHandler::enable();
                      // Buttons influence
                      nbToLeft .enableInput();
                      nbToRight.disableInput();
                      // ... Up/Down too
                      if ((ilbRight.selection()>0)&&(ilbRight.selection()!=IBaseListBox::notFound))
                           nbUp.enableInput();
                      else nbUp.disableInput();
                      if (ilbRight.selection()<ilbRight.count()-1)
                           nbDown.enableInput();
                      else nbDown.disableInput();
                      break;
      }
   return(False);
}



// Class  : NListDialog
// Purpose: Dialog Window Class for Mankind-List
NListDialog*  NListDialog::pNListDialog=NULL;
IFont         NListDialog::ifProfileFont("Helv",8);
IRectangle    NListDialog::irProfileRectangle;
Boolean       NListDialog::bProfileMaxed=False;
IString       NListDialog::isProfileSortBy(IString((char)3)+IString((char)4));
Boolean       NListDialog::bProfileFloat=False;
short         NListDialog::sProfileColWidth[8]={0,0,0,0,0,0,0,0};

void NListDialog::loadFromProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("LIST");
   irProfileRectangle=IRectangle(profile.integerWithKey("MINX")
                                ,profile.integerWithKey("MINY")
                                ,profile.integerWithKey("MAXX")
                                ,profile.integerWithKey("MAXY"));
   bProfileMaxed=profile.integerWithKey("MAXED");
   ifProfileFont=IFont(profile.elementWithKey("FONTNAME")
                      ,profile.integerWithKey("FONTSIZE"));
   isProfileSortBy=profile.elementWithKey("SORT");
   bProfileFloat=profile.integerWithKey("FLOAT");
   for (short i=1;i<=8;i++)
      sProfileColWidth[i-1]=profile.integerWithKey("COL"+IString(i));
}
void NListDialog::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("LIST");
   profile.addOrReplaceElementWithKey("MINX",irProfileRectangle.minX());
   profile.addOrReplaceElementWithKey("MINY",irProfileRectangle.minY());
   profile.addOrReplaceElementWithKey("MAXX",irProfileRectangle.maxX());
   profile.addOrReplaceElementWithKey("MAXY",irProfileRectangle.maxY());
   profile.addOrReplaceElementWithKey("MAXED",bProfileMaxed);
   profile.addOrReplaceElementWithKey("FONTNAME",ifProfileFont.name());
   profile.addOrReplaceElementWithKey("FONTSIZE",ifProfileFont.pointSize());
   profile.addOrReplaceElementWithKey("SORT",isProfileSortBy);
   profile.addOrReplaceElementWithKey("FLOAT",bProfileFloat);
   for (short i=1;i<=8;i++)
      profile.addOrReplaceElementWithKey("COL"+IString(i),sProfileColWidth[i-1]);
}

NListDialog::NListDialog(IWindow* piwOwner,NMankind* pSetNMankind)
   : IFrameWindow(IResourceId(ID_NListWindow)
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
                 ,NApplication::pDLLResource->loadString(SI_ListTitle))
   , IFrameHandler()
   , ICommandHandler()

   , nManList(1,this,pSetNMankind,ifProfileFont,sProfileColWidth,isProfileSortBy)
{
   pNMankind=pSetNMankind;
   setIcon(RES_IconFTree);
   // Prepare for deletion
   bDisabled=False;
   setAutoDeleteObject(true);
   pNListDialog=this;
   NMainWindow::pNMainWindow->ihwHelp.setAssociatedWindow(this);
   setClient(&nManList);
   // SetUp HandlerStuff
   //removeDefaultHandler(); // Remove DefaultFrameHandler for my one
   IFrameHandler::handleEventsFor(this);
   ICommandHandler::handleEventsFor(this);
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
} /* end NListDialog::NListDialog */

NListDialog::~NListDialog()
{
   // Update profile information about widths
   for (short i=1;i<=8;i++)
      sProfileColWidth[i-1]=nManList.columnWidth(i);
   // Report destruction to MainWindow
   pNListDialog=NULL;
} /* end NListDialog::~NListDialog */

void NListDialog::changeFloat(Boolean set)
{
   bProfileFloat=set;
   if (pNListDialog)
      pNListDialog->setOwner( bProfileFloat ? NULL : NMainWindow::pNMainWindow );
}

Boolean NListDialog::isFloat()
{
   return(bProfileFloat);
}

Boolean NListDialog::saved(IFrameEvent& frameEvent)
{
   // Save Frame Status
   if ((!isMinimized())&&(!isMaximized()))
      irProfileRectangle=rect();
   bProfileMaxed=isMaximized();
   ifProfileFont=nManList.font();
   isProfileSortBy=nManList.getSortBy();
   return(False);
}

Boolean NListDialog::closed(IFrameEvent& frameEvent)
{
   if (bDisabled) {
      DosBeep(200,200);
      return(True);
      }
   // Done
   return(False);
} /* end NListDialog::closed() */

void NListDialog::disableInput()
{
   bDisabled=True;
   nManList.disableInput();
}
void NListDialog::enableInput()
{
   bDisabled=False;
   nManList.enableInput();
}

Boolean NListDialog::askForSorting()
{
   // Prepare Dialog
   NQuerySort sortDialog(IC_DEFAULT_FRAME_ID,this,&nManList);
   sortDialog.showModally();
   if (sortDialog.result()==True)
      nManList.setSortBy(sortDialog.isResult);
   // Done
   return(False);
}

Boolean NListDialog::command(ICommandEvent& event)
{
   // Check for list specific commands ...
   switch (event.commandId()) {
      case MI_ManlistSort     :
         askForSorting();
         return(True);
      }
   // ... mankind specific command !
   NMainWindow::pNMainWindow->postEvent(event);
   return(True);
}

