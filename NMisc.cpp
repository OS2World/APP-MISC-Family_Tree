/*
   File   : guimisc.cpp
   Purpose: Define simple GUI Classes
*/

#include <igline.hpp>
#include <igrect.hpp>
#include <igbundle.hpp>
#include <igstring.hpp>
#include <iscroll.hpp>
#include <ipopmenu.hpp>
#include <igrect.hpp>
#include <igstring.hpp>

#include "NMisc.hpp"
#include "NMainWindow.hpp"

#include "igrafctx.hpp"
#include "igrect.hpp"
#include "ievtdata.hpp"

unsigned long _System DosSleep(unsigned long);
unsigned long _System DosBeep(unsigned long,unsigned long);

#define INCL_WINWINDOWMGR
#define INCL_WINCURSORS
#include <os2.h>

IString parseFile(IString& file,Boolean relative,IWindow* pOwner)
{
   // Warn about no Name -> no relative
   if ((relative)&&(NApplication::isTree.length()==0)) {
      NMainWindow::pNMainWindow->disableInput();
      IMessageBox mbox(pOwner);
      mbox.show(NApplication::pDLLResource->loadString(SI_BmpGiveName)
               ,IMessageBox::informationIcon|IMessageBox::okButton|IMessageBox::moveable);
      NMainWindow::pNMainWindow->enableInput();
      return(IString(file));
      }
   // Missing path in file -> return full qualified
   if ((file.indexOf(':')==0)&&(file[(unsigned)1]!='\\'))
      return(IString(NApplication::isTree.subString(1,NApplication::isTree.lastIndexOf('\\'))
                     +file));
   // Full qualified path -> no relative no work
   if (!relative)
      return(IString(file));
   // Full qualified path -> try to make relative
   IString dir(NApplication::isTree.subString(1,NApplication::isTree.lastIndexOf('\\')).upperCase()),
           result(file);
   result.upperCase();
   if (dir.isAbbreviationFor(result)) {
      file.remove(1,dir.length());
      return(result);
      }
   // Warn about no (sub-)dir -> no relative
   NMainWindow::pNMainWindow->disableInput();
   IMessageBox mbox(pOwner);
   mbox.show(NApplication::pDLLResource->loadString(SI_BmpDoRelative)
            ,IMessageBox::informationIcon|IMessageBox::okButton|IMessageBox::moveable);
   NMainWindow::pNMainWindow->enableInput();
   return(file);
}


// Class  : NNotebook
// Purpose: New Notebook with color tabs
NNotebook::NNotebook(unsigned long id,IWindow* pParent,IWindow* pOwner)
   : INotebook(WinCreateWindow(pParent->handle(),WC_NOTEBOOK
                              ,NULL,0x0800|WS_VISIBLE,0,0,0,0
                              ,pOwner->handle()
                              ,HWND_BOTTOM,id,NULL,NULL))
{
   setAutoDestroyWindow(True);
   setPageButtonSize(ISize(21,21));
}



// Class  : NContainer
// Purpose: Container with empty text in center
NContainer::NContainer(unsigned long id,
                       IWindow* parent,
                       IWindow* owner,
                       const IRectangle& location,
                       const Style& style,
                       const Attribute& attribute)
   : IContainerControl(id,parent,owner,location,style,
                       IContainerControl::Attribute(attribute|handleDrawBackground))
{
   ICnrDrawHandler::handleEventsFor(this);
   IResizeHandler::handleEventsFor(this);
}

void NContainer::disableInput()
{
   IContainerControl::disable();
}
void NContainer::enableInput()
{
   IContainerControl::enable();
}

NContainer& NContainer::setEmptyText(const IString& isSet)
{
   isEmptyText=isSet;
   return(*this);
}

Boolean NContainer::windowResize(IResizeEvent& event)
{
   refresh(IWindow::paintAll);
   return(False);
}


Boolean NContainer::drawBackground(ICnrDrawBackgroundEvent& event)
{
   event.setResult(True);
   // Do own draw with hint
   IGraphicContext igc(event.itemPresSpaceHandle());
   igc.setPenColor(backgroundColor())
      .setFillColor(backgroundColor());

   IGRectangle igrTemp(event.itemRect());
   igrTemp.drawOn(igc);

   // Check if there are objects
   if (objectCount()>0) return(True);

   // Give hint
   igc.setPenColor(foregroundColor());
   IGString igsTemp(isEmptyText);
   igsTemp.moveTo(IPoint(size().width()/2,size().height()/2)
                 -IPoint(font().textWidth(igsTemp.text())/2
                        ,(font().maxCharHeight()-font().maxDescender())/2) ) //+font().maxDescender()
          .drawOn(igc);
   // I've done the job
   return(True);
}

// Class  : NFileBox
// Purpose: Container with Files in it

NFileBox::NFileBox(unsigned long windowId
                  ,IWindow* parent
                  ,IWindow* owner
                  ,Boolean setRelative)
   : NContainer(windowId,parent,owner,IRectangle()
                ,IWindow::visible
                 |IContainerControl::singleSelection
                 |IContainerControl::autoPosition
                 |IContainerControl::noSharedObjects)
   , ICnrHandler()
   , ICnrEditHandler()
   , ICnrMenuHandler()
   , ICommandHandler()
{
   pPopObject=NULL;
   bRelative=setRelative;
   setDeleteObjectsOnClose(True);
   ICnrHandler::handleEventsFor(this);
   ICnrEditHandler::handleEventsFor(this);
   NDragDropHandler::handleDropFor(this,NDragDropHandler::File,IString(),NDragDropHandler::Link);
   ICnrMenuHandler::handleEventsFor(this);
   ICommandHandler::handleEventsFor(this);
   setEmptyText(NApplication::pDLLResource->loadString(SI_DropFileHere));
   bChanged=False;
}

NFileBox::~NFileBox() { }

void NFileBox::fileDropped(IString isFile)
{
   isDropName=isFile;
   postEvent(WM_COMMAND,MI_FileBoxDrop);
}

Boolean NFileBox::hasChanged()
{
   // Check if FTree is registered -> otherwise no durable change
   if (NApplication::isRegisteredUser.size()==0) return(False);
   // Check Change
   Boolean result=bChanged;
   bChanged=False;
   return(result);
}

Boolean NFileBox::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      case MI_FileBoxDrop:
         if (isDropName.length()>0) {
           add(isDropName,True);
           isDropName.remove(1);
           }
         return(True);
      case MI_FileBoxNew:
         add("New",False);
         return(True);
      case MI_FileBoxClear:
         clear();
         return(True);
      case MI_FileBoxOpen:
         if (pPopObject)
            open(pPopObject);
         pPopObject=NULL;
         return(True);
      case MI_FileBoxChange:
         if (pPopObject)
            editObject(pPopObject);
         pPopObject=NULL;
         return(True);
      case MI_FileBoxFirst:
         if (pPopObject) {
            removeObject(pPopObject);
            addObjectAfter(pPopObject,NULL);
            }
         pPopObject=NULL;
         bChanged=True;
         return(True);
      case MI_FileBoxDel:
         if (pPopObject) {
            removeObject(pPopObject);
            delete pPopObject;
            }
         pPopObject=NULL;
         bChanged=True;
         return(True);
      }
   // Done
   return(False);
}

Boolean NFileBox::makePopUpMenu(IMenuEvent& event)
{
   //  PopUp ?
   IPopUpMenu *pMenu=new IPopUpMenu(IResourceId(MI_FileBox,*NApplication::pDLLResource),this);
   if (!pMenu) return(False);
   // Fill menu
   pPopObject=popupMenuObject();
   if (pPopObject) {
      pMenu->disableItem(MI_FileBoxNew);
      pMenu->disableItem(MI_FileBoxClear);
      }
   else {
      pMenu->disableItem(MI_FileBoxOpen);
      pMenu->disableItem(MI_FileBoxChange);
      pMenu->disableItem(MI_FileBoxFirst);
      pMenu->disableItem(MI_FileBoxDel);
      }
   // Show Menu
   pMenu->setAutoDeleteObject(true);
   pMenu->show(event.mousePosition());
   return(True);
}

void NFileBox::clear()
{
   bChanged=True;
   deleteAllObjects();
}

void NFileBox::calcIcon(IContainerObject* obj,const IString& file)
{
   IPointerHandle handle=IPointerHandle(WinLoadFileIcon(file,False));
   if (!handle) handle=ISystemPointerHandle(ISystemPointerHandle::question);
   obj->setIcon(handle);
   obj->refresh();
}

Boolean NFileBox::endEdit(ICnrEndEditEvent& event)
{
   bChanged=True;
   IString file(event.newText());
   calcIcon(event.object(),parseFile(file,bRelative,this));
   event.object()->setIconText(file);
   // Check if FTree is registered -> otherwise notify non durable changes
   if (NApplication::isRegisteredUser.size()==0) {
      NMainWindow::pNMainWindow->disableInput();
      IMessageBox mbox(this);
      mbox.show(NApplication::pDLLResource->loadString(SI_FeatureNotAvailable2)
               ,IMessageBox::warningIcon|IMessageBox::okButton|IMessageBox::systemModal);
      NMainWindow::pNMainWindow->enableInput();
      }
   return(True);
}

void NFileBox::open(IContainerObject* obj)
{
   // Do System OPEN
   IString file(obj->iconText());
   unsigned long handle=WinQueryObject(parseFile(file,False,this));
   if ((!handle)||(!WinSetObjectData(handle,(PCSZ)"OPEN=DEFAULT"))) {
      DosBeep(200,200);
      return;
      }
   obj->setInUse();
   DosSleep(100);
   obj->removeInUse();
   // Done
}

Boolean NFileBox::enter(ICnrEnterEvent& event)
{
   if (event.validObject()) open(event.object());
   return(True);
}

void NFileBox::add(const IString& file,Boolean warning)
{
   IString name(file);
   // Check if FTree is registered -> otherwise notify non durable changes
   if (NApplication::isRegisteredUser.size()==0) {
      if (warning) {
         NMainWindow::pNMainWindow->disableInput();
         IMessageBox mbox(this);
         mbox.show(NApplication::pDLLResource->loadString(SI_FeatureNotAvailable2)
                  ,IMessageBox::warningIcon|IMessageBox::okButton|IMessageBox::systemModal);
         NMainWindow::pNMainWindow->enableInput();
         }
      else name=NApplication::pDLLResource->loadString(SI_FeatureNotAvailable1);
      }
   // Add it
   bChanged=True;
   IContainerObject* obj;
   obj=new IContainerObject;
   calcIcon(obj,parseFile(name,warning&bRelative,this));
   obj->setIconText(name);
   obj->enableDataUpdate(); // let it be editable
   addObject(obj);
   refresh();
   // Done
}
IString NFileBox::get(unsigned short num)
{
   return(objectAt(num-1)->iconText());
}
unsigned short NFileBox::count()
{
   return(objectCount());
}

// Class  : NButtonBar
// Purpose: Canvas for Buttons
NButtonBar::NButtonBar(unsigned long windowId,IWindow* parentowner)
   : ICanvas(windowId,parentowner,parentowner)
   , IPaintHandler()
   , IResizeHandler()
{
   lTotalWidth=1+1+1;
   lMaxHeight=1+1;
   sChildren=0;
   sGrowableRow=0;
   setBackgroundColor(IColor(IColor::paleGray));
   IPaintHandler::handleEventsFor(this);
   IResizeHandler::handleEventsFor(this);
}
NButtonBar::~NButtonBar() {}

ISize NButtonBar::calcMinimumSize() const
{
   // Return result
   return(ISize(lTotalWidth,lMaxHeight));
}

NButtonBar& NButtonBar::setGrowableRow(short sSetGrowableRow)
{
   sGrowableRow=sSetGrowableRow;
   layout();
   return(*this);
}

NButtonBar& NButtonBar::layout()
{
   // Go through children
   long xpos=1,ypos=1;
   ISize size;
   for (short i=1;i<=sChildren;i++) {
      // .. get child preferred size
      size=pChildren[i-1]->minimumSize();
      // .. check if this window can be streched
      if ((i==sGrowableRow)&&(rect().width()>lTotalWidth))
         size+=ISize(rect().width()-lTotalWidth,0);
      // calc pos and move/size
      if (pChildren[i-1]->isGroup()) xpos+=8;
      ypos = lMaxHeight/2-size.height()/2;
      pChildren[i-1]->moveTo(IPoint(xpos,ypos)).sizeTo(size);
      // next
      xpos+=size.width()+1;
      }
   // Done
   return(*this);
}

NButtonBar& NButtonBar::addChild(IControl* pChild,Boolean more)
{
   // Remember new child
   pChildren[sChildren]=pChild;
   sChildren++;
   // Max Height ?
   ISize size(pChild->minimumSize());
   lMaxHeight  =max(lMaxHeight,1+size.height()+1);
   lTotalWidth+=size.width()+(pChild->isGroup()?8:1);
   // re-layout ?
   if (!more) layout();
   return(*this);
   // Done
}

Boolean NButtonBar::paintWindow(IPaintEvent& event)
{
   // Initial clear
   IGraphicContext context(event.presSpaceHandle());
   context.setPenColor (IColor(184,184,184))
          .setFillColor(IColor(204,204,204));
   IGRectangle box(IRectangle(rect().size()-1));
   box.drawOn(context);
   // border
   IGLine line(IPoint(0,0),IPoint(0,rect().height()-1));
   context.setPenColor(IColor(224,224,224));
   line.drawOn(context)
       .setStartingPoint(IPoint(rect().size()-1))
       .drawOn(context);
   // Go through children
   long xpos=1;
   ISize size;
   for (short i=1;i<=sChildren;i++) {
      size=pChildren[i-1]->size();
      if (pChildren[i-1]->isGroup()) {
         xpos+=4;
         context.setPenColor(IColor(IColor::darkGray));
         line.setStartingPoint(IPoint(xpos,1))
             .setEndingPoint  (IPoint(xpos,lMaxHeight-1-1))
             .drawOn(context);
         xpos++;
         context.setPenColor(IColor(IColor::white));
         line.setStartingPoint(IPoint(xpos,1))
             .setEndingPoint  (IPoint(xpos,lMaxHeight-1-1))
             .drawOn(context);
         xpos+=4-1;
         }
      xpos+=size.width()+1;
      }
   // Done
   return(True);
}

Boolean NButtonBar::windowResize(IResizeEvent& event)
{
   layout();
   return(True);
}


// Class  : NColorControl
// Purpose: display a set of colors to choose from
NColorControl::NColorControl(unsigned long windowId,IWindow* pOwnerParent)
   : IMultiCellCanvas(windowId,pOwnerParent,pOwnerParent)
   , IMouseHandler()
   , IPaintHandler()
   , istColor11( 1,this,this)
   , istColor21( 2,this,this)
   , istColor31( 3,this,this)
   , istColor41( 4,this,this)
   , istColor12( 5,this,this)
   , istColor22( 6,this,this)
   , istColor32( 7,this,this)
   , istColor42( 8,this,this)
   , istColor13( 9,this,this)
   , istColor23(10,this,this)
   , istColor33(11,this,this)
   , istColor43(12,this,this)
   , istColor14(13,this,this)
   , istColor24(14,this,this)
   , istColor34(15,this,this)
   , istColor44(16,this,this)
{
   selected=0;
   // Layout
   setDefaultCell(ISize(5,5));
   IStaticText* pistColor=&istColor11;
   for (short int i=0;i<=15;i++) {
      addToCell(&pistColor[i],((i&3)+1)*2,((i/4)+1)*2);
      pistColor[i].setFillColor(IColor(IColor::Color(i)))
                  .setMinimumSize(ISize(21,21));
      IMouseHandler::handleEventsFor(&pistColor[i]);
      }
   IPaintHandler::handleEventsFor(this);
   setColumnWidth(9,defaultCell().width());
   setRowHeight  (9,defaultCell().height());
   // Done
}
NColorControl::~NColorControl()
{
}

void NColorControl::select(unsigned short int sel)
{
   IGraphicContext igcTemp(handle());
   drawEmphasis(igcTemp,selected);
   selected=sel;
   drawEmphasis(igcTemp,selected);
}

unsigned short int NColorControl::getSelected()
{
   return(selected);
}

void NColorControl::drawEmphasis(IGraphicContext& igc,unsigned short int which)
{
   if (which>15) return;
   igc.setDrawOperation(IGraphicBundle::frame )
      .setPenType      (IGraphicBundle::dot   )
      .setMixMode      (IGraphicBundle::invert);
   IRectangle  irOld=(&istColor11+which)->rect();
   IGRectangle igrTemp(IRectangle(IPoint(irOld.minXMinY()-IPoint(3,3))
                                 ,IPoint(irOld.maxXMaxY()+IPoint(2,2)) ) );
   igrTemp.drawOn(igc);
}

#define WM_COMMAND 0x0020

Boolean NColorControl::mouseClicked(IMouseClickEvent& event)
{
   if ( (event.mouseAction()==IMouseClickEvent::click)
      &&(event.mouseButton()==IMouseClickEvent::button1)) {
      // Remove old box
      IGraphicContext igcTemp(handle());
      drawEmphasis(igcTemp,selected);
      // Calc new box
      selected=event.window()->id()-1;
      // Draw new Box
      drawEmphasis(igcTemp,selected);
      // Done
      postEvent(WM_COMMAND
               ,IEventData(id())
               ,IEventData(True,0));
      }
   return(False);
}

Boolean NColorControl::paintWindow(IPaintEvent& event)
{
   // First a clear
   event.clearBackground(backgroundColor());
   // Draw rectangle
   IGraphicContext igcTemp(event.presSpaceHandle());
   drawEmphasis(igcTemp,selected);
   // Done
   return(True);
}


// Class  : NQuestion
// Purpose: question params from user
NQuestion::NQuestion(unsigned long windowId
                    ,IWindow* pOwner
                    ,const IString& isTitle
                    ,const IPoint ipPos)
   : IFrameWindow(IResourceId(windowId)
                 ,IWindow::desktopWindow()
                 ,pOwner
                 ,IRectangle()
                 ,IFrameWindow::dialogBackground
                  //|IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder
                  |IFrameWindow::titleBar
                 ,isTitle)
   , ICommandHandler()
   , IKeyboardHandler()
   , iCanvas  (1,this,this)
   , iefText  (2,&iCanvas,&iCanvas,IRectangle()
              ,IEntryField::classDefaultStyle|IControl::tabStop)
   , ipbOK    (3,&iCanvas,&iCanvas,IRectangle()
              ,IWindow::visible|IPushButton::defaultButton|IControl::tabStop)
   , ipbCANCEL(4,&iCanvas,&iCanvas,IRectangle()
              ,IPushButton::classDefaultStyle|IControl::tabStop)
{
   // Data
   ipbOK.setText("OK");
   ipbCANCEL.setText("Cancel");
   iefText.setLimit(128);
   // Layout
   setClient(&iCanvas);
   iCanvas.addToCell(&iefText,    2,2,2,1);
   iCanvas.addToCell(&ipbOK,      2,3,1,1);
   iCanvas.addToCell(&ipbCANCEL,  3,3,1,1);
   iCanvas.setRowHeight(4,iCanvas.defaultCell().height());
   iCanvas.setColumnWidth(4,iCanvas.defaultCell().width());
   // Handlers
   ICommandHandler::handleEventsFor(this);
   IKeyboardHandler::handleEventsFor(this);
   // Done
   moveSizeToClient(IRectangle(IPoint(0,0),iCanvas.minimumSize()));
   moveTo(ipPos-size()/2);
//   moveTo(mapPoint(pOwner->position()+pOwner->size()/2-size()/2
//                  ,pOwner->handle(),IWindow::desktopWindow()->handle() ) );
}

NQuestion::~NQuestion()
{
}

Boolean NQuestion::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      // OK Button
      case 3 : dismiss(DID_OK); break;
      // Cancel Button
      case 4 : dismiss(DID_CANCEL); break;
      }
   // Done everything
   return(True);
}

Boolean NQuestion::virtualKeyPress(IKeyboardEvent& event)
{
   if (event.virtualKey()==IKeyboardEvent::esc)
      ipbCANCEL.click();
   // I'm not the last handler
   return(False);
}

void NQuestion::setText(const IString& isSet)
{
   iefText.setText(isSet);
}

IString NQuestion::getText()
{
   return(IString(iefText.text()));
}

// Class  : NYesNoControl
// Purpose: Own yesno selector
NYesNoControl::NYesNoControl(unsigned long windowId,IWindow* pParent,IWindow* pOwner)
   : I3StateCheckBox(windowId,pParent,pOwner,IRectangle()
                    ,IWindow::visible|IButton::noPointerFocus)
   , ISelectHandler()
{
   // data
   setValue(Unknown);
   bChanged=False;
   setBackgroundColor(pParent->backgroundColor());
   disableAutoSelect();
   // handlers
   ISelectHandler::handleEventsFor(this);
   // done
}
NYesNoControl::~NYesNoControl()
{
}

ITextControl& NYesNoControl::setLayoutDistorted( unsigned long layoutAttributeOn,unsigned long layoutAttributeOff)
{
   return(*this);
}

NYesNoControl::Value NYesNoControl::value()
{
   if (isHalftone()) return(Unknown);
   if (isSelected()) return(Yes);
                     return(No);
}

NYesNoControl& NYesNoControl::setValue(Value vSet)
{
   deselect();
   switch (vSet) {
      case Yes : setText(NApplication::pDLLResource->loadString(SI_Yes    ));select();break;
      case No  : setText(NApplication::pDLLResource->loadString(SI_No     ));break;
      default  : setText(NApplication::pDLLResource->loadString(SI_Unknown));selectHalftone();break;
      }
   bChanged=True;
   return(*this);
}

Boolean NYesNoControl::hasChanged()
{
   Boolean result=bChanged;
   bChanged=False;
   return(result);
}

Boolean NYesNoControl::selected(IControlEvent& event)
{
   if (isHalftone()) { setValue(Yes); return(True); }
   if (isSelected()) { setValue(No ); return(True); }
   setValue(Unknown); return(False);
}



// Class  : NButton
// Purpose: special (latchable) button
NButton::NButton(unsigned long windowId,IWindow* pParent,IWindow* pOwner,Boolean bSetAlwaysBorder)
   : ICanvas(windowId,pParent,pOwner)
   , IPaintHandler()
   , IMouseHandler()
{
   disableGroup();
   setBackgroundColor(IColor(IColor::paleGray));
   // Handler
   IPaintHandler::handleEventsFor(this);
   IMouseHandler::handleEventsFor(this);
   // VARs
   bDisabled=False;
   bLeftAlign=False;
   bChanged=False;
   bLatched=False;
   bLatchable=False;
   bClick=False;
   bDown=False;
   pBitmap=NULL;
   pLatchedBitmap=NULL;
   bAlwaysBorder=bSetAlwaysBorder;
   // Done
}

void NButton::setAlwaysBorder(Boolean set)
{
   bAlwaysBorder=set;
   refresh();
}

NButton::~NButton()
{
   if (pBitmap) delete pBitmap;
   if (pLatchedBitmap) delete pLatchedBitmap;
}

NButton& NButton::setBitmap(unsigned long id)
{
   if (pBitmap) delete pBitmap;
   pBitmap=new IGBitmap(IResourceId(id));
   pBitmap->setTransparentColor(IColor(0xff,0x00,0xff));
   setString(IString(id));
   return(*this);
}

NButton& NButton::setLatchedBitmap(unsigned long id)
{
   if (pLatchedBitmap) delete pLatchedBitmap;
   pLatchedBitmap=new IGBitmap(IResourceId(id));
   pLatchedBitmap->setTransparentColor(IColor(0xff,0x00,0xff));
   return(*this);
}

void NButton::disableInput()
{
   if (bDisabled) return;
   bDisabled=True;
   IPushButton::disable();
   refresh(IRectangle(IPoint(1,1),ISize(rect().size()-1)),True);
}
void NButton::enableInput()
{
   if (!bDisabled) return;
   bDisabled=False;
   IPushButton::enable();
   refresh(IRectangle(IPoint(1,1),ISize(rect().size()-1)),True);
}
void NButton::setLeftAlign(Boolean set)
{
   bLeftAlign=set;
}

Boolean NButton::hasChanged(Boolean setChanged)
{
   Boolean result=bChanged;
   bChanged=setChanged;
   return(result);
}

IString NButton::getString()
{
   return(IString(isString));
}

NButton& NButton::setString(const IString& isSet)
{
   bChanged=True;
   isString=isSet;
   refresh();
   return(*this);
}

ISize NButton::calcMinimumSize() const
{
   // Text ...
   if (pBitmap==NULL)
      return(ISize(font().textWidth(isString)+6,font().maxCharHeight()+2) );
   // ... or BMP
   return(pBitmap->size()+ISize(4,4));
}

Boolean NButton::isLatched()
{
   return(bLatched);
}

NButton& NButton::latch(Boolean setLatched)
{
   bLatchable=True;
   bLatched=setLatched;
   refresh();
   return(*this);
}

NButton& NButton::enableLatching(Boolean setEnable)
{
   bLatchable=setEnable;
   bLatched=bLatched&&bLatchable;
   return(*this);
}

void NButton::click()
{
   if (bLatchable) bLatched=bLatched?False:True;
   postEvent(WM_COMMAND,IEventData(id()),IEventData(True,0));
}

Boolean NButton::mouseClicked(IMouseClickEvent& event)
{
   if (bDisabled) return(True);
   // Check if mouse button 1
   if (event.mouseButton()!=IMouseClickEvent::button1) return(True);
   // Mouse pressed
   Boolean paint=False;
   switch (event.mouseAction()) {
      case IMouseClickEvent::doubleClick :
      case IMouseClickEvent::down :
         bDown=True;
         bClick=True;
         paint=True;
         break;
      case IMouseClickEvent::up :
         if (!bClick) return(True);
         if ((!bAlwaysBorder)||(bDown)) paint=True;
         bDown=False;
         bClick=False;
         releasePointer();
         // ... Maybe resulting command ?
         if (event.windowUnderPointer()==handle()) click();
         break;
      }
   // Done
   if (paint) refresh(IWindow::paintAllImmediate);
   return(True);
}

Boolean NButton::mouseMoved(IMouseEvent& event)
{
   Boolean wantDown=False;
   // Check if the mouse pointer belongs to this
   if (hasPointerCaptured()) {
      // ... mouse is clicked ?
      if (bClick) {
         // ... mouse over this or not
         if (event.windowUnderPointer()==handle())
            wantDown=True;
         else
            wantDown=False;
         }
      // ... mouse is not clicked !
      else {
         // ... not over this anymore ?
         if (event.windowUnderPointer()!=handle()) {
            bDown=False;
            releasePointer();
            if (!bAlwaysBorder) refresh();
            return(True);
            }
         wantDown=False;
         }
      // ... make sure Button's (not)down
      if (bDown!=wantDown) {
         bDown=wantDown;
         refresh();
         }
      return(True);
      }
   // ... so the pointer isn't captured ... maybe now if not alwaysborder ?
   if (event.windowUnderPointer()==handle()) {
      // ... capture it
      try { capturePointer(); } catch (IException ex) {}
      if (!bAlwaysBorder) refresh();
      }
   return(True);
}

Boolean NButton::paintWindow(IPaintEvent& event)
{

   Boolean pointed=hasPointerCaptured(),
           down=bDown||bLatched,
           border=(pointed&&(!bDisabled))||bLatched||bAlwaysBorder;
   IGraphicContext context(event.presSpaceHandle());
   // If button is latched, draw lightened bg
   if (!bLatched||pointed) context.setFillColor(IColor(IColor::paleGray));
   else                    context.setFillColor(IColor(220,220,220));
   // Draw background
   context.setDrawOperation(IGraphicBundle::fill);
   IRectangle box(IPoint(0,0),ISize(size()-1));
   IGRectangle igr(box);
   igr.drawOn(context);
   // Draw Text/Bitmap
   if (pBitmap) {
      // ... calc bmp and draw it
      IGBitmap* pbmp;
      if (bLatched&&pLatchedBitmap) pbmp=pLatchedBitmap;
      else pbmp=pBitmap;
      pbmp->moveTo(rect().size()/2-pbmp->size()/2+ISize(down?1:0,down?-1:0))
           .drawOn(context);
      // ... disabled state ??
      if (bDisabled) {
         context.setFillPattern(IGraphicBundle::dense7);
         igr.setEnclosingRect(box).drawOn(context);
         }
      // .. drawn
      }
   else {
      context.setPenColor(IColor(bDisabled?IColor::darkGray:IColor::black));
      IGString igs(isString);
      igs.moveTo(IPoint( (bLeftAlign?3:box.width()/2-font().textWidth(isString)/2) + (down?1:0)
                       ,box.height()/2-font().maxAscender()/2+font().maxDescender() + (down?-1:0)))
         .setClippingRect(box)
         .drawOn(context);
      }
   // Draw border
   IColor::Color col1=IColor::paleGray,
                 col2=IColor::paleGray;
   if (border) {
      if (!down) { col1=IColor::darkGray;col2=IColor::white   ; }
      else       { col1=IColor::white   ;col2=IColor::darkGray; }
      }
   context.setPenColor(IColor(col1));
   IGLine igl(box.minXMinY(),box.maxXMinY());   //  __
   igl.drawOn(context)
      .setStartingPoint(box.maxXMaxY())         //    |
      .drawOn(context);
   context.setPenColor(IColor(col2));
   igl.setEndingPoint  (box.minXMaxY())         //  ^^
      .drawOn(context)
      .setStartingPoint(box.minXMinY())         // |
      .drawOn(context);
   // Done
   return(True);
}


// Class  : NStatusLine
// Purpose: Status Line text control with frame
NStatusLine::NStatusLine(unsigned long windowId,IWindow* ownerparentWnd)
   : ICanvas(windowId,ownerparentWnd,ownerparentWnd)
   , IPaintHandler()
{
   // VARs
   lLimit=0;
   setForegroundColor(IColor(IColor::darkBlue));
   // Handlers
   IPaintHandler::handleEventsFor(this);
}
NStatusLine::~NStatusLine() { }

NStatusLine& NStatusLine::setLimit(const long setValue)
{
   lLimit=setValue;
   return(*this);
}
NStatusLine& NStatusLine::setText (const IString& setValue)
{
   // Remember value
   isText=setValue;
   // ... show Change
   if (!isVisible()) return(*this);

   IRectangle box(1,1,rect().width()-2,rect().height()-2);
   refresh(box,True);
   // Done
   return(*this);
}

Boolean NStatusLine::paintWindow(IPaintEvent& event)
{
   // Draw Background
   event.clearBackground(IColor(IColor::paleGray));
   // Draw Text
   IRectangle box(size()-ISize(1,1));
   IGraphicContext context(event.presSpaceHandle());
   context.setPenColor(foregroundColor());
   IGString igs(isText);
   igs.moveTo(IPoint(3,box.height()/2-font().maxAscender()/2+font().maxDescender()))
      .setClippingRect(box)
      .drawOn(context);
   // Draw frame
   context.setPenColor(IColor(IColor::darkGray));
   IGLine igl(box.minXMinY(),box.minXMaxY());
   igl.drawOn(context);
   igl.setStartingPoint(box.maxXMaxY());
   igl.drawOn(context);
   context.setPenColor(IColor(IColor::white));
   igl.setEndingPoint  (box.maxXMinY());
   igl.drawOn(context);
   igl.setStartingPoint(box.minXMinY());
   igl.drawOn(context);
   // Done
   return(True);
}

ISize NStatusLine::calcMinimumSize() const
{
   return(ISize( (lLimit>0 ? lLimit*font().maxSize().width() : font().textWidth(isText) ) +6
               ,font().maxCharHeight()+2) );
}


// Class  : NBitmapControl
// Purpose: Own BitmapControl
NBitmapControl::NBitmapControl(unsigned long windowId
                              ,IWindow* ownerparentWnd
                              ,Boolean setRelative)
   : IViewPort(windowId,ownerparentWnd,ownerparentWnd)
   , NDragDropHandler()
   , IMouseHandler()
   , iCanvas(1,this,this,IRectangle()
            ,IWindow::visible|ISetCanvas::centerVerticalAlign|ISetCanvas::centerAlign)
   , iBmp(2,&iCanvas,&iCanvas)
{
   pNQuestion=NULL;
   bChanged=False;
   bRelative=setRelative;
   // Bitmap 1
   IGBitmap bmp(RES_BmpDefault);
   iBmp.sizeTo(bmp.size());
   iBmp.setBitmap(bmp.handle());
   iCanvas.setText("<>");
   // Enable Drag&Drop
   NDragDropHandler::handleDropFor(&iBmp,NDragDropHandler::File,IString(),NDragDropHandler::Link);
   IMouseHandler::handleEventsFor(&iBmp);
}
NBitmapControl::~NBitmapControl() {
}

Boolean NBitmapControl::setLocation(IString isSetLocation,Boolean warning) {
   bChanged=True;
   // Check for no Location
   if (isSetLocation.length()==0) {
      iCanvas.setText("<>");
      IGBitmap bmp(RES_BmpDefault);
      iBmp.sizeTo(bmp.size());
      iBmp.setBitmap(bmp.handle());
      setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
      return(True);
      }
   // Any question window close
   if (pNQuestion) pNQuestion->dismiss(DID_CANCEL);
   // Calc file
   IString name(isSetLocation),
           path(parseFile(name,warning&bRelative,this));
   // Construct dropped .BMP file name from this dragitem and build BMP
   try {
      // Create Bitmap
      IGBitmap bmp(path);
      // if not successfull -> wrong drop
      if (!bmp.handle()) return(False);
      // Get BMP
      iBmp.sizeTo(bmp.size());
      iBmp.setBitmap(bmp.handle());
      setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
      iCanvas.setText(name);
      // Done
      return(True);
      }
   catch (IException& exc) {
      // Warning means user set it -> don't store new, keep old
      if (warning) {
         iCanvas.setText("<>");
         IGBitmap bmp(RES_BmpDefault);
         iBmp.sizeTo(bmp.size());
         iBmp.setBitmap(bmp.handle());
         setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
         }
      // NoWarning means program set it -> store new, show Questionmark
      else {
         iCanvas.setText(name);
         IGBitmap bmp(RES_BmpNotFound);
         iBmp.sizeTo(bmp.size());
         iBmp.setBitmap(bmp.handle());
         setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
         }
      }
   return(False);
}

void NBitmapControl::disableInput() {
   iBmp.disable();
}
void NBitmapControl::enableInput() {
   iBmp.enable();
}

IString NBitmapControl::getLocation() {
   if (iCanvas.text()=="<>") return(IString());
   return(iCanvas.text());
}

Boolean NBitmapControl::hasChanged() {
   Boolean bReturn=bChanged;
   bChanged=False;
   return(bReturn);
}

Boolean NBitmapControl::mouseClicked(IMouseClickEvent& event) {
   switch (event.mouseAction()) {
      case IMouseClickEvent::click :
         pNQuestion = new NQuestion(12345
                                   ,this
                                   ,NApplication::pDLLResource->loadString(SI_EnterPath)
                                   ,mapPoint(position()+size()/2,handle(),IWindow::desktopWindow()->handle()) );
         if (pNQuestion) {
            if (iCanvas.text()!="<>")
               pNQuestion->setText(iCanvas.text());
            pNQuestion->setFocus();
            pNQuestion->showModally();
            IString isTemp=pNQuestion->getText();
            Boolean ok=pNQuestion->result()==DID_OK;
            delete pNQuestion;pNQuestion=NULL;
            if (ok) setLocation(isTemp,True);
            }
         break;
      }
   return(False);
}

void NBitmapControl::fileDropped(IString isFile)
{
   setLocation(isFile,True);
}

// Class  : NTextEntry
// Purpose: Special IEntryField
NTextEntry::NTextEntry(unsigned long windowId,IWindow* parent,IWindow* owner)
   : IEntryField(windowId,parent,owner,IRectangle()
                ,IWindow::visible
                 |IEntryField::autoScroll
                 |IEntryField::leftAlign)
   , IFocusHandler()
{
   // Layout
   enableTabStop();
   setBackgroundColor(IColor(223,223,223));
   setDisabledBackgroundColor(IColor(223,223,223));
   setLimit(256);
   // Handlers
   IFocusHandler::handleEventsFor(this);
}
NTextEntry::~NTextEntry() { }
void NTextEntry::disableInput()
{
   setForegroundColor(IColor(IColor::paleGray));
   IEntryField::disable();
}
void NTextEntry::enableInput()
{
   setForegroundColor(IColor(IColor::black));
   IEntryField::enable();
}
ISize NTextEntry::calcMinimumSize() const
{
   return(ISize(0,IEntryField::calcMinimumSize().height()));
}
Boolean NTextEntry::gotFocus(IControlEvent& iceEvent)
{
   if (IWindow::isEnabled())
      setBackgroundColor(IColor(IColor::white));
   return(False);
}
Boolean NTextEntry::lostFocus(IControlEvent& iceEvent)
{
   if (IWindow::isEnabled())
      setBackgroundColor(IColor(223,223,223));
   return(False);
}


// Class  : NNumEntry
// Purpose: Special IEntryField
NNumEntry::NNumEntry(unsigned long windowId,IWindow* parent,IWindow* owner)
   : IEntryField(windowId,parent,owner,IRectangle()
                ,IWindow::visible
                 |IEntryField::autoScroll
                 |IEntryField::rightAlign
                 |IWindow::clipChildren)
   , IFocusHandler()
   , IKeyboardHandler()
   , IResizeHandler()
   , IScrollHandler()
{
   // Layout
   enableTabStop();
   setBackgroundColor(IColor(223,223,223));
   setDisabledBackgroundColor(IColor(223,223,223));
   setLimit(256);

   pScroll=new IScrollBar(1234,this,this);
   pScroll->setScrollBar(IRange(1,3),1)
           .moveScrollBoxTo(2);
   // Handlers
   IFocusHandler::handleEventsFor(this);
   IResizeHandler::handleEventsFor(this);
   IKeyboardHandler::handleEventsFor(this);
   IScrollHandler::handleEventsFor(this);
}
NNumEntry::~NNumEntry()
{
   delete pScroll;
}
void NNumEntry::disableInput()
{
   setForegroundColor(IColor(IColor::paleGray));
   IEntryField::disable();
}
void NNumEntry::enableInput()
{
   setForegroundColor(IColor(IColor::black));
   IEntryField::enable();
}
ISize NNumEntry::calcMinimumSize() const
{
   return(ISize(0,IEntryField::calcMinimumSize().height()));
}
Boolean NNumEntry::gotFocus(IControlEvent& iceEvent)
{
   if (IWindow::isEnabled())
      setBackgroundColor(IColor(IColor::white));
   return(False);
}
Boolean NNumEntry::lostFocus(IControlEvent& iceEvent)
{
   if (IWindow::isEnabled())
      setBackgroundColor(IColor(223,223,223));
   return(False);
}

Boolean NNumEntry::characterKeyPress(IKeyboardEvent& event)
{
   // Make sure only numeric values are entered
   if ((event.character()<'0')||(event.character()>'9')) return(True);
   return(False);
}

Boolean NNumEntry::windowResize(IResizeEvent& event)
{
   long sw=IScrollBar::systemScrollBarWidth();
   pScroll->moveTo(IPoint(0,0))
           .sizeTo(ISize (sw,event.newSize().height()));
   return(False);
}

Boolean NNumEntry::lineDown(IScrollEvent& event)
{
   setText(IString(max(0,text().asInt()-1)));
   return(False);
}

Boolean NNumEntry::lineUp  (IScrollEvent& event)
{
   setText(IString(max(0,text().asInt()+1)));
   return(False);
}

Boolean NNumEntry::scrollEnd(IScrollEvent& event)
{
   setFocus();
   return(False);
}


// Class  : NMemoField
// Purpose: Special IMultiLineEdit
NMemoField::NMemoField(unsigned long windowId,IWindow* ownerparentWnd)
   : IMultiLineEdit(windowId,ownerparentWnd,ownerparentWnd,IRectangle()
                   ,IMultiLineEdit::visible
                    //|IMultiLineEdit::horizontalScroll
                    |IMultiLineEdit::verticalScroll
                    //|IMultiLineEdit::border
                    |IMultiLineEdit::ignoreTab)
   , IFocusHandler()
{
   enableTabStop();
   setBackgroundColor(IColor(223,223,223));
   setLimit(4*1024);
   IFocusHandler::handleEventsFor(this);
}

NMemoField::~NMemoField() { }

void NMemoField::disableInput()
{
   IMultiLineEdit::disable();
}
void NMemoField::enableInput()
{
   IMultiLineEdit::enable();
}
Boolean NMemoField::gotFocus(IControlEvent& iceEvent)
{
   if (IWindow::isEnabled())
      setBackgroundColor(IColor(IColor::white));
   return(False);
} /* end NMemoField::gotFocus() */

Boolean NMemoField::lostFocus(IControlEvent& iceEvent)
{
   if (IWindow::isEnabled())
      setBackgroundColor(IColor(223,223,223));
   return(False);
} /* end NMemoField::lostFocus() */

ISize NMemoField::calcMinimumSize() const
{
   return(ISize(0,font().maxCharHeight()*5+8));
}

// Class  : NFocusHandler
// Purpose: Focus handler for any window
#define WM_FOCUSCHANGE 0x0043
NFocusHandler::NFocusHandler() : IHandler() {}
NFocusHandler::~NFocusHandler() {}
Boolean NFocusHandler::focusChanged(Boolean received) { return(False); }
Boolean NFocusHandler::dispatchHandlerEvent(IEvent& event) {
   // Focus message ?
   if (event.eventId()==WM_SETFOCUS)
      return(focusChanged(event.parameter2()));
   // Not for me
   return(False);
}


// Class  : NDate
// Purpose: Do own Date eg. 25-05-1970 + Modifier ' ~<>?'
char NDate::modifiers[]=" ñ><?";  //" ?";

NDate::NDate() {
   cDay=0;cMonth=0;sYear=0;
}
NDate::NDate(unsigned short sSetDay,unsigned short sSetMonth,unsigned short sSetYear,unsigned short sSetModifier) {
   cDay=sSetDay;cMonth=sSetMonth;sYear=((sSetModifier>4?0:sSetModifier)<<13) | min((1<<13)-1,sSetYear);
}
NDate::NDate(const NDate& from) {
   *this=from;
}
NDate::~NDate() {
}
IString NDate::asString() const {
   IString isDay,isMonth,isYear,isMod(" ");
   if (day()>0) isDay=IString(day()).rightJustify(2,'0');
   else isDay="--";
   if (month()>0) isMonth=IString(month()).rightJustify(2,'0');
   else isMonth="--";
   if ((year()&0x1fff)>0) isYear=IString(year()&0x1fff).rightJustify(4,'0');
   else isYear="----";
   if (modifier()!=0) isMod=IString(modifiers[modifier()]);
   switch (NApplication::cDateLayout) {
      case 'D':
         return( isMod
                +isDay  +IString(NApplication::cDateSeparator)
                +isMonth+IString(NApplication::cDateSeparator)
                +isYear                                  );
      case 'M':
         return( isMod
                +isMonth+IString(NApplication::cDateSeparator)
                +isDay  +IString(NApplication::cDateSeparator)
                +isYear                                  );
      }
      //case 'Y':
         return( isMod
                +isYear +IString(NApplication::cDateSeparator)
                +isMonth+IString(NApplication::cDateSeparator)
                +isDay                                   );
   // Done
}
ostream& operator<< (ostream& oStream, NDate& d)
{
   oStream.write((char*)&d.cDay,2*sizeof(UCHAR)+sizeof(USHORT));
   // Done
   return(oStream);
}
istream& operator>> (istream& iStream, NDate& d)
{
   iStream.read((char*)&d.cDay,2*sizeof(UCHAR)+sizeof(USHORT));
   // Done
   return(iStream);
}
Boolean operator< (const NDate& d1, const NDate& d2) {
   return((*((ULONG*)&d1.cDay)&0x1fffffff)<(*((ULONG*)&d2.cDay)&0x1fffffff));
}
Boolean operator<= (const NDate& d1, const NDate& d2) {
   return((*((ULONG*)&d1.cDay)&0x1fffffff)<=(*((ULONG*)&d2.cDay)&0x1fffffff));
}
NDate& NDate::operator= (const NDate& d) {
   *((ULONG*)&cDay)=*((ULONG*)&d.cDay);return(*this);
}
Boolean NDate::hasInfo() const {
   return( *((ULONG*)&cDay)!=0 );
}
unsigned short NDate::day() const {
   return(cDay);
}
unsigned short NDate::month() const {
   return(cMonth);
}
unsigned short NDate::year() const {
   return(sYear&0x1fff);
}
unsigned short NDate::modifier() const {
   return(sYear>>13);
}
unsigned long NDate::days() const {
   return(((long)cDay)+((long)cMonth)*31+((long)(sYear&0x1fff))*365);
}

void NDate::drawOn(IGraphicContext& igcContext,const IPoint& ipPos,const IFont& ifFont,IGString& igsText,Boolean bAlwaysSeps) const
{
   IString num;
   short offset;
   // ... modifier
   if (modifier()>0) {
      igsText.setText(IString(modifiers[modifier()]))
             .moveTo(ipPos)
             .drawOn(igcContext);
      }
   // ... day
   num=IString(day());
   switch (day()) {
      case 0 :
         if (!bAlwaysSeps) break;
         num="";
      default:
         offset=ifFont.textWidth(num);
         if (NDate::lastDigit()!=NDate::digDay) num+=IString(NApplication::cDateSeparator);
         igsText.setText(num)
                .moveTo(ipPos+IPair(digitMetric(digDay,ifFont).coord2()-offset,0))
                .drawOn(igcContext);
      }
   // ... month
   num=IString(month());
   switch (month()) {
      case 0 :
         if (!bAlwaysSeps) break;
         num="";
      default:
         offset=ifFont.textWidth(num);
         if (NDate::lastDigit()!=NDate::digMonth) num+=IString(NApplication::cDateSeparator);
         igsText.setText(num)
                .moveTo(ipPos+IPair(digitMetric(digMonth,ifFont).coord2()-offset,0))
                .drawOn(igcContext);
      }
   // ... year
   num=IString(year());
   switch (year()) {
      case 0 :
         if (!bAlwaysSeps) break;
         num="";
      default:
         offset=ifFont.textWidth(num);
         if (NDate::lastDigit()!=NDate::digYear) num+=IString(NApplication::cDateSeparator);
         igsText.setText(num)
                .moveTo(ipPos+IPair(digitMetric(digYear,ifFont).coord2()-offset,0))
                .drawOn(igcContext);
      }
   // Done
}

ISize NDate::drawSize(const IFont& ifFont)
{
   return(ISize(ifFont.charWidth('2')*11,ifFont.maxCharHeight()));
}

IRange NDate::digitMetric(Digit which,const IFont& ifFont)
{
   // Calc layout           D M Y   D M Y   D M Y
   static short pos[3][3]={{1,4,7},{4,1,7},{9,6,1}};
   static short width[3]={2,2,4};

   short format; // DMY
   switch (NApplication::cDateLayout) {
      case 'M': format=1;break;  // MDY
      case 'Y': format=2;break;  // YMD
      default : format=0;break;  // DMY
      }

   return(IRange(pos[format][which]
                ,pos[format][which]+width[which])*ifFont.charWidth('2'));
}

NDate::Digit NDate::firstDigit()
{
   switch (NApplication::cDateLayout) {
      case 'M': return(digMonth);
      case 'Y': return(digYear);
      }
   //case 'D': return(digDay)
   return(digDay);
}
NDate::Digit NDate::lastDigit()
{
   if (NApplication::cDateLayout=='Y') return(digDay);
   return(digYear);
}
NDate::Digit NDate::nextDigit(NDate::Digit from)
{
   static NDate::Digit next[3][3]={ {digMonth,digYear,digYear } // DMY
                                  , {digYear ,digDay ,digYear } // MDY
                                  , {digDay  ,digDay ,digMonth}};// YMD
   short i;
   switch (NApplication::cDateLayout) {
      case 'Y': i=2;break;
      case 'M': i=1;break;
      case 'D': i=0;break;
      }
   return(next[i][from]);
}
NDate::Digit NDate::prevDigit(NDate::Digit from)
{
   static NDate::Digit prev[3][3]={ {digDay  ,digDay  ,digMonth} // DMY
                                  , {digMonth,digMonth,digDay  } // MDY
                                  , {digMonth,digYear ,digYear }};// YMD
   short i;
   switch (NApplication::cDateLayout) {
      case 'Y': i=2;break;
      case 'M': i=1;break;
      case 'D': i=0;break;
      }
   return(prev[i][from]);
}

// Class  : NDateControl
// Purpose: Special Date entry control that looks like this
//     0123456789
// +--+----------+
// | >|01.01.1996|
// +--+----------+
NDateControl::NDateControl(unsigned long windowId,IWindow* pParent,IWindow* pOwner)
   : ICanvas(windowId,pParent,pOwner)
   , IKeyboardHandler()
   , IMouseHandler()
   , NFocusHandler()
   , ICommandHandler()
   , IMenuHandler()
   , istDate(1,this,this,IRectangle(),IWindow::visible|IControl::tabStop)
{
   // VARs
   digActual=NDate::digDay;
   sDigOffset=0;
   sValue[NDate::digDay  ]=0;
   sValue[NDate::digMonth]=0;
   sValue[NDate::digYear ]=0;
   sModifier=0;
   bChanged=False;
   bDisabled=False;
   bKeepPosition=False;
   // Handlers
   IKeyboardHandler::handleEventsFor(this);
   IPaintHandler::handleEventsFor(&istDate);
   IMouseHandler::handleEventsFor(&istDate);
   NFocusHandler::handleEventsFor(&istDate);
   ICommandHandler::handleEventsFor(this);
   IMenuHandler::handleEventsFor(&istDate);
   // Done
}

NDateControl::~NDateControl() { }

ISize NDateControl::calcMinimumSize() const
{
   return(NDate::drawSize(istDate.font())+ISize(2+2,1+2+2));
}

ICanvas& NDateControl::layout()
{
   // Run default layout (tabbing order) routine
   ICanvas::layout();
   // Position Text field
   istDate.sizeTo(NDate::drawSize(istDate.font())+ISize(2+2,2+2+1))
          .moveTo(IPoint(0,0));
   // Done
   return(*this);
}

NDateControl& NDateControl::setDate(const NDate& dSetDate)
{
   // Transfer data to date entry
   bChanged=False;
   sValue[NDate::digDay  ]=dSetDate.day();
   sValue[NDate::digMonth]=dSetDate.month();
   sValue[NDate::digYear ]=dSetDate.year();
   sModifier              =dSetDate.modifier();
   digActual=NDate::firstDigit();
   sDigOffset=0;
   istDate.refresh();
   // ... done
   return(*this);
}
const NDate NDateControl::getDate()
{
   return(NDate(sValue[NDate::digDay  ]
               ,sValue[NDate::digMonth]
               ,sValue[NDate::digYear ]
               ,sModifier));
}
Boolean NDateControl::hasChanged()
{
   Boolean rc=bChanged;
   bChanged=False;
   return(rc);
}
void NDateControl::disableInput()
{
   bDisabled=True;
   istDate.disable();
   ICanvas::disable();
}
void NDateControl::enableInput()
{
   bDisabled=False;
   istDate.enable();
   ICanvas::enable();
}

void NDateControl::startOver()
{
   digActual=NDate::firstDigit();
   sDigOffset=0;
   istDate.refresh();
}


void NDateControl::drawDate(IGraphicContext& context,Boolean focus)
{
   makeCursor(False);
   // Clear text background
   if (focus) context.setPenColor(IColor(IColor::white));
   else context.setPenColor(IColor(223,223,223));
   context.setFillColor(context.penColor());
   IGRectangle back(istDate.rect().size()-ISize(1,1));
   back.drawOn(context);
   // Draw date
   IFont dateFont(istDate.font());
   context.setPenColor(IColor(IColor::black));
   NDate date(sValue[NDate::digDay  ]
             ,sValue[NDate::digMonth]
             ,sValue[NDate::digYear ]
             ,sModifier);
   IGString igsTemp("");
   date.drawOn(context
              ,istDate.rect().minXCenterY()-IPoint(0,dateFont.maxAscender()/2)
              ,dateFont
              ,igsTemp
              ,!bDisabled); // Always separators when not disabled
   // ... disabled state ??
   if (bDisabled) {
      context.setFillPattern(IGraphicBundle::dense6)
             .setDrawOperation(IGraphicBundle::fill)
             .setFillColor(IColor(223,223,223))
             .setPenColor(IColor(223,223,223));
      back.drawOn(context);
      }
   // Done
   if (focus) makeCursor(True);
}

Boolean NDateControl::paintWindow(IPaintEvent& event)
{
   IGraphicContext context(event.presSpaceHandle());
   drawDate(context,istDate.hasFocus());
   // Done
   return(True);
}

Boolean NDateControl::mouseClicked(IMouseClickEvent& event)
{
   if (bDisabled) return(True);
   // Selection into date ?
   if (event.mouseAction()==IMouseClickEvent::down) {
      // ... which position ?
      if (event.mouseButton()==IMouseClickEvent::button1) {
         IFont listFont(istDate.font());
         if (NDate::digitMetric(NDate::digDay  ,listFont).includes(event.mousePosition().x()))
            digActual=NDate::digDay;
         if (NDate::digitMetric(NDate::digMonth,listFont).includes(event.mousePosition().x()))
            digActual=NDate::digMonth;
         if (NDate::digitMetric(NDate::digYear ,listFont).includes(event.mousePosition().x()))
            digActual=NDate::digYear;
         sDigOffset=0;
         }
      // ... have to set focus ?
      if (!istDate.hasFocus()) {
         bKeepPosition=True;
         istDate.setFocus();
         }
      // ... otherwise just cursor
      else makeCursor(True);
      }
   // ... done
   return(True);
}

Boolean NDateControl::focusChanged(Boolean received)
{
   // Create cursor
   if (!bKeepPosition) digActual=NDate::firstDigit();
   // Redraw window, so focus is indiacted
   IGraphicContext context(istDate.handle());
   drawDate(context,received);
   // Done
   bKeepPosition=False;
   return(True);
}

void NDateControl::makeCursor(Boolean appear)
{
   // destroy cursor in case it belonged to this
   WinDestroyCursor(istDate.handle());
   // create cursor
   if (!appear) return;
   IRange range(NDate::digitMetric(digActual,istDate.font()));
   WinCreateCursor(istDate.handle()
                  ,range.coord1(),1
                  ,range.coord2()-range.coord1(),1
                  ,CURSOR_HALFTONE|CURSOR_FLASH
                  ,NULL);
   WinShowCursor(istDate.handle(),True);
   // Done
}

Boolean NDateControl::virtualKeyPress(IKeyboardEvent& event)
{
   if (bDisabled) return(True);
   // Maybe just release
   if (event.isUpTransition())
      return(False);
   // Check for cursor keys
   Boolean change=False,
           redraw=False;
   switch (event.virtualKey()) {
      case IKeyboardEvent::left     :
           digActual=NDate::prevDigit(digActual);redraw=True;
           break;
      case IKeyboardEvent::right    :
           digActual=NDate::nextDigit(digActual);redraw=True;
           break;
      case IKeyboardEvent::up       :
           sValue[digActual]+=1;change=True;
           break;
      case IKeyboardEvent::down     :
           sValue[digActual]-=1;change=True;
           break;
      case IKeyboardEvent::deleteKey:
           sValue[digActual]=0;change=True;
           break;
      case IKeyboardEvent::backSpace:
           digActual=NDate::prevDigit(digActual);
           sValue[digActual]=0;change=True;
           break;
      case IKeyboardEvent::esc      :
           sValue[NDate::digDay  ]=0;
           sValue[NDate::digMonth]=0;
           sValue[NDate::digYear ]=0;
           sModifier=0;
           digActual=NDate::firstDigit();
           change=True;
           break;
      default                       :
           return(False);
      }
   if (change) {
      bChanged=True;
      sValue[NDate::digDay  ]=max(0,min(31  ,sValue[NDate::digDay  ]));
      sValue[NDate::digMonth]=max(0,min(12  ,sValue[NDate::digMonth]));
      sValue[NDate::digYear ]=max(0,min(4096,sValue[NDate::digYear ]));
      }
   if (change||redraw)
      istDate.refresh(IWindow::paintAllImmediate);
   sDigOffset=0;
   // Done
   return(True);
}

Boolean NDateControl::characterKeyPress(IKeyboardEvent& event)
{
   if (bDisabled) return(True);
   // Investigate key
   char key=event.character()-'0';
   if ((key<0)||(key>9)) return(True);
   bChanged=True;
   Boolean next=False,beep=True;
   // Maybe Ctrl+Modifier ?
   if (event.isCtrlDown())
      sModifier=key>4 ? 0 : key;
   // ... number for date information
   else switch (digActual) {
      case NDate::digDay :
         switch (sDigOffset) {
            case 0 : sValue[digActual]=0;
            case 1 : sValue[digActual]=min(31,sValue[digActual]*10+key);
                     sDigOffset++;
                     beep=False;
            case 2 : if (sDigOffset==2) next=True;
            }
         break;
      case NDate::digMonth :
         switch (sDigOffset) {
            case 0 : sValue[digActual]=0;
            case 1 : sValue[digActual]=min(12,sValue[digActual]*10+key);
                     sDigOffset++;
                     beep=False;
            case 2 : if (sDigOffset==2) next=True;
            }
         break;
      case NDate::digYear :
         switch (sDigOffset) {
            case 0 : sValue[digActual]=0;
            case 1 :
            case 2 :
            case 3 : sValue[digActual]=min(4096,sValue[digActual]*10+key);
                     sDigOffset++;
                     beep=False;
            case 4 : if (sDigOffset==4) next=True;
            }
         break;
      }
   if (next) {
      if (digActual==NDate::lastDigit()) {
         if (beep) DosBeep(100,100);
         }
      else {
         digActual=NDate::nextDigit(digActual);
         sDigOffset=0;
         }
      }
   // ... draw Result and re-enable Cursor
   istDate.refresh(IWindow::paintAllImmediate);
   // Done
   return(True);
}

Boolean NDateControl::command(ICommandEvent& event)
{
   bChanged=True;
   sModifier=event.commandId()-MI_Date-1;
   istDate.refresh();
   return(True);
}
Boolean NDateControl::makePopUpMenu(IMenuEvent& event)
{
   if (bDisabled) return(False);
   // Create PopUpMenu Object
   IPopUpMenu* pMenu;
   pMenu=new IPopUpMenu(IResourceId(MI_Date,*NApplication::pDLLResource),this);
   pMenu->checkItem(MI_Date+sModifier+1);
   pMenu->disableItem(MI_Date+sModifier+1);
   // ... no success -> done
   if (!pMenu)
      return(False);
   // ... fill data
   // Show PopUp
   pMenu->setAutoDeleteObject(true);
   pMenu->show(event.mousePosition());
   // Done it
   return(True);
}

