/*
   File   : NList.cpp
   Purpose: Define own list for collections
*/

#include <igline.hpp>
#include <igrect.hpp>
#include <igbundle.hpp>
#include <igstring.hpp>
#include <igrect.hpp>

#define INCL_WINWINDOWMGR
#include <os2.h>

#include "NList.hpp"

#define MINCOLWIDTH 4 // minimum column width
#define DRAGWIDTH   3 // width of column dragable space (left/right)


// Class  : NList
// Purpose: list of items (like container detail view)
NList::NList(unsigned long windowId,IWindow* ownerparentWnd,Boolean bSetSeparator)
   : ICanvas(windowId,ownerparentWnd,ownerparentWnd)
   , IScrollHandler()
   , IPaintHandler()
   , IMouseHandler()
   , IKeyboardHandler()
   , IResizeHandler()
   , istHeader   (50,this,this)
   , istList     (51,this,this,IRectangle(),IWindow::visible|IWindow::clipSiblings|IWindow::clipChildren)
   , isbLeftRight(52,this,this,IRectangle(),IScrollBar::horizontal|IWindow::visible)
   , isbUpDown   (53,this,this,IRectangle(),IScrollBar::vertical  |IWindow::visible)
{
   // Initial data
   sNoOfColumns=0;
   dragAction=NList::none;
   lOldSplitXPos=-1;
   bDisabled=False;
   bSeparator=bSetSeparator;
   sColumnsWidth=0;
   setBackgroundColor(IColor(IColor::white));
   // Handlers
   IMouseHandler   ::handleEventsFor(&istHeader   );
   IMouseHandler   ::handleEventsFor(&istList     );
   IPaintHandler   ::handleEventsFor(this         );
   IPaintHandler   ::handleEventsFor(&istHeader   );
   IPaintHandler   ::handleEventsFor(&istList     );
   IScrollHandler  ::handleEventsFor(this         );
   IMenuHandler    ::handleEventsFor(&istHeader   );
   IMenuHandler    ::handleEventsFor(&istList     );
   IKeyboardHandler::handleEventsFor(this         );
   IKeyboardHandler::handleEventsFor(&istHeader   );
   IKeyboardHandler::handleEventsFor(&istList     );
   IKeyboardHandler::handleEventsFor(&isbLeftRight);
   IKeyboardHandler::handleEventsFor(&isbUpDown   );
   IResizeHandler  ::handleEventsFor(this         );
   // Done
}

NList::~NList() { }

void NList::disableInput()
{
   bDisabled=True;
   isbLeftRight.disable();
   isbUpDown.disable();
   istList.disable();
   istHeader.disable();
}
void NList::enableInput()
{
   bDisabled=False;
   isbLeftRight.enable();
   isbUpDown.enable();
   istList.enable();
   istHeader.enable();
   layout(); // Make sure updown wasn't enabled falsely
}

void NList::setEmptyText(const IString& isSetEmptyText)
{
   isEmptyText=isSetEmptyText;
}

IWindow* NList::listWindow()
{
   return(&istList);
}

ISize NList::calcMinimumSize() const
{
   return(ISize(IScrollBar::systemScrollBarWidth(True)+2
               ,IScrollBar::systemScrollBarWidth(False)+2));
}

Boolean NList::windowResize(IResizeEvent& event)
{
   layout();
   return(True);
}

void NList::dataHasChanged()
{
   layout();
   istList.refresh();
}

ICanvas& NList::layout()
{
   long winHeight=rect().height()-2;
   long winWidth =rect().width()-2;
   long sbWidth=IScrollBar::systemScrollBarWidth(True);
   long sbHeight=IScrollBar::systemScrollBarWidth(False);
   // do the layout for header field
   long headerHeight=istHeader.font().maxDescender()+istHeader.font().maxAscender()+2+2;
   istHeader.moveSizeTo( IRectangle( IPoint(1,1+winHeight-headerHeight)
                                   , ISize (winWidth,   headerHeight) ) );
   // Enable/Disable horizontal scroll bar
   if ((sColumnsWidth<=winWidth-sbWidth)||(noOfRows()==0))  {
      isbLeftRight.hide().disable();
      sbHeight=0;
      }
   else {
      isbLeftRight.setVisibleCount(winWidth-sbWidth)
                  .setScrollableRange(IRange(1,sColumnsWidth))
                  .moveTo(IPoint(1,1))
                  .sizeTo( ISize(winWidth-sbWidth,sbHeight))
                  .show()
                  .enable();
      }
   // ... and for the list region
   long listHeight=winHeight-headerHeight-sbHeight;
   istList.moveTo(IPoint(1,1+sbHeight))
          .sizeTo(ISize(winWidth-sbWidth,listHeight));
   // ... and the vertical scroll-bar
   isbUpDown.moveTo(IPoint(1+winWidth-sbWidth,1))
            .sizeTo( ISize(sbWidth,winHeight-headerHeight));
   long rows=listHeight / rowHeight();
   if (rows>=noOfRows())
      isbUpDown.setVisibleCount(noOfRows())
               .setScrollableRange(IRange(1,1))
               .moveScrollBoxTo(1)
               .disable();
   else
      isbUpDown.setVisibleCount(rows)
               .setScrollableRange(IRange(1,noOfRows()))
               .enable(bDisabled==False);
   // ...  signal scrolling to virtual function
   positionsHaveChanged();
   // Done
   return(*this);
}

NList& NList::addColumn(const IString& isName,short sWidth)
{
   // Create new column information
   short width=sWidth>0 ? sWidth : font().textWidth(isName)*3+3+3;
   isColumn      [sNoOfColumns]=isName;
   ipColumnMetric[sNoOfColumns]=IPair(sColumnsWidth, width );
   sColumnsWidth+=width;
   sNoOfColumns++;
   // .. done with this one
   return(*this);
}

short NList::columnCount()
{
   return(sNoOfColumns);
}

IString NList::columnName(short which)
{
   if ((which<1)||(which>sNoOfColumns))
      return(IString());
   return(IString(isColumn[which-1]));
}


short NList::columnWidth(short which)
{
   if ((which>sNoOfColumns)||(which<1))
      return(0);
   return(ipColumnMetric[which-1].coord2());
}

short NList::leftmostBit()
{
   return(isbLeftRight.isVisible() ? isbLeftRight.scrollBoxPosition() : 1 );
}

short NList::topmostRow()
{
   return(isbUpDown.scrollBoxPosition());
}

short NList::visibleRows()
{
   return(isbUpDown.visibleCount());
}

short NList::noOfRows()
{
   return(0);
}

short NList::rowHeight()
{
   return(istList.font().maxCharHeight());
}

short NList::minColumnWidth(short sColumn)
{
   return(0);
}


void NList::drawHeader(const long xstart,IGraphicContext& context,const IRectangle& area)
{
   // Clear background of header region
   context.setFillColor(IColor(IColor::paleGray))
          .setPenColor(IColor(IColor::paleGray));
   IGRectangle grect(area);
   grect.drawOn(context);
   // Prepare drawing of header informations
   context.setPenColor(IColor(IColor::black));
   IGString igs(IString(""));
   IGLine igl(IPoint(0,0),IPoint(0,0));
   long  maxDescender=istHeader.font().maxDescender(),
         maxAscender =istHeader.font().maxAscender(),
         height      =maxDescender+maxAscender+2+2,
         width       =0,
         xpos        =0;
   Boolean down;
   // draw column headers (inclusive last dummy one)
   for (short i=1;i<=sNoOfColumns+1;i++) {
      // Calc metrics
      if (i==sNoOfColumns+1) {
         xpos =xstart+sColumnsWidth;
         width=rect().width();
         }
      else {
         xpos =xstart+ipColumnMetric[i-1].coord1();
         width=       ipColumnMetric[i-1].coord2();
         }
      // Check for pushed header column
      down=((dragAction==NList::pushing)&&(i==sActionColumn));
      // Draw Text
      context.setPenColor(IColor(IColor::black));
      IRectangle box(IPoint(xpos,0),ISize(width-1,height-1));
      igs.setText(isColumn[i-1])
         .moveTo(IPoint(xpos+3+(down?1:0),height/2-maxAscender/2-(down?1:0)))
         .setClippingRect(box)
         .drawOn(context);
      // Draw darkened edge
      if (down) context.setPenColor(IColor(IColor::white));
      igl.setStartingPoint(box.minXMinY())
         .setEndingPoint  (box.maxXMinY())
         .drawOn(context)
         .setStartingPoint(box.maxXMaxY())
         .drawOn(context);
      // Draw lighted edge
      if (down) context.setPenColor(IColor(IColor::black));
      else      context.setPenColor(IColor(IColor::white));
      igl.setStartingPoint(box.minXMinY())
         .setEndingPoint  (box.minXMaxY())
         .drawOn(context)
         .setStartingPoint(box.maxXMaxY())
         .drawOn(context);
      // Next
      }
   // Done
}

Boolean NList::drawStart(short sStartRow)
{
   return(False);
}
void NList::drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before)
{
}
void NList::drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString)
{
   // Default Row Draw
   igsString.setText(IString(sColumn)+"/"+IString(sRow))
            .moveTo(irRect.minXMinY()+IPoint(0,ifFont.maxDescender()))
            .setClippingRect(irRect)
            .drawOn(igcContext);
   // Done
}
void NList::drawEnd()
{
}
void NList::redrawRow(short sRow)
{
   short visibleRow=sRow-topmostRow()+1;
   // Maybe not visible ?
   if ( (visibleRow<=0) || (visibleRow>isbUpDown.visibleCount()) )
      return;
   // Prepare drawing data
   long  height =rowHeight(),
         ypos    =istList.rect().height()-visibleRow*rowHeight(),
         width  =0,
         xpos    =-(leftmostBit()-1);
   // Calculate visible box
   IRectangle area(IPoint(0,ypos)
                 ,ISize(istList.size().width()-1,height-1));
   // Clear bg of row
   IGraphicContext context(istList.handle());
   context.setFillColor(backgroundColor())
          .setPenColor (backgroundColor());
   IGRectangle grect(area);
   grect.drawOn(context);
   context.setPenColor (IColor(IColor::black));
   // Initialize drawing for list -> chance to cancel !
   if (!drawStart(topmostRow()))
      return;
   // ... prepare row
   IRectangle rowBox(IPoint(xpos,ypos),ISize(sColumnsWidth-1-1,height-1));
   drawRow(context,rowBox,sRow,True);
   // ... do col after col
   IGString gstring("");
   for (short col=1;col<=sNoOfColumns;col++) {
      // ... calc metric
      xpos  = -(leftmostBit()-1) + ipColumnMetric[col-1].coord1();
      width= ipColumnMetric[col-1].coord2();
      // .. draw it
      drawRowCol(context
                ,IRectangle(IPoint(xpos,ypos),ISize(width-1,height-1))
                ,col,sRow
                ,istList.font()
                ,gstring);
      // ... next
      }
   // ... draw whole row
   drawRow(context,rowBox,sRow,False);
   // Finish drawing of list -> chance to clean up !
   drawEnd();
   // Redraw that separators
   context.setPenColor (IColor(IColor::black));
   drawSeparators( -(leftmostBit()-1),context);
   // Done
}

void NList::drawSeparators(const long xstart,IGraphicContext& context)
{
   if (!bSeparator) return;
   // Draw Column separators (each at right edge of header button)
   context.setPenColor(IColor(IColor::darkGray));
   IGLine gline(IPoint(0,0),IPoint(0,0));
   long height=istList.rect().height();
   long xoff;
   for (short i=1;i<=sNoOfColumns;i++) {
      xoff=xstart+ipColumnMetric[i-1].coord1()+ipColumnMetric[i-1].coord2();
      gline.setStartingPoint( IPoint(xoff-1,0     ) )
           .setEndingPoint  ( IPoint(xoff-1,height) )
           .drawOn(context);
      }
   // Done
}

void NList::drawList(const long xstart,IGraphicContext& context,const IRectangle& area)
{
   // Erase background
   context.setFillColor(backgroundColor())
          .setPenColor (backgroundColor());
   IGRectangle grect(area);
   grect.drawOn(context);
   context.setPenColor(IColor(IColor::black));
   // Prepare drawing data
   long  lineHeight  =rowHeight(),
         yPosition   =istList.rect().height()-lineHeight,
         columnWidth =0;
   IFont listFont(istList.font());
   IGString gstring("");
   // Initialize drawing for list -> chance to cancel !
   if (!drawStart(topmostRow())) {
      // Draw a reminder
      if (isEmptyText.length()>0)
         gstring.setText(isEmptyText)
                .moveTo(istList.rect().centerXCenterY()-IPoint(listFont.textWidth(isEmptyText)/2,listFont.maxCharHeight()/2))
                .drawOn(context);
      return;
      }
   // Draw Row after Row
   for (short row=topmostRow(),c=1;c<=isbUpDown.visibleCount();c++,row++) {
      // ... prepare row
      IRectangle rowBox(IPoint(xstart,yPosition),ISize(sColumnsWidth-1-1,lineHeight-1));
      drawRow(context,rowBox,row,True);
      // ... do col after col
      for (short col=1;col<=sNoOfColumns;col++) {
         drawRowCol(context
                   ,IRectangle(IPoint(xstart+ipColumnMetric[col-1].coord1(),yPosition   )
                              ,ISize (     ipColumnMetric[col-1].coord2()-1,lineHeight-1))
                   ,col,row
                   ,listFont
                   ,gstring);
         }
      // ... draw whole row
      drawRow(context,rowBox,row,False);
      // ... next round
      yPosition-=lineHeight;
      if (yPosition<0) break;
      }
   // Finish drawing of list -> chance to clean up !
   drawEnd();
   // Draw Column separators
   drawSeparators(xstart,context);
   // Done
}


Boolean NList::paintWindow(IPaintEvent &event)
{
   // Check if we're visible ... otherwise finished
   if (!isShowing())
      return(True);
   // Check if the header font has changed
   if (istHeader.font().maxSize()!=font().maxSize())
      setFont(istHeader.font());
   // Prepare graphic context
   IGraphicContext context(event.presSpaceHandle());
   long xstart=-(leftmostBit()-1);
   // Draw header/list section
   if (event.dispatchingWindow()==this) {
      IGLine igl(IPoint(0,0),IPoint(0,0));
      IRectangle box(rect().size()-ISize(1,1));
      // Draw darkened edge
      context.setPenColor(IColor(IColor::darkGray));
      igl.setStartingPoint(box.minXMinY())
         .setEndingPoint  (box.minXMaxY())
         .drawOn(context)
         .setStartingPoint(box.maxXMaxY())
         .drawOn(context);
      // Draw lighted edge
      context.setPenColor(IColor(220,220,220));
      igl.setStartingPoint(box.minXMinY())
         .setEndingPoint  (box.maxXMinY())
         .drawOn(context)
         .setStartingPoint(box.maxXMaxY())
         .drawOn(context);
      // Done
      return(True);
      }
   if (event.dispatchingWindow()==&istHeader) {
      // Draw Header
      drawHeader(xstart,context,event.rect());
      // Done
      return(True);
      }
   if (event.dispatchingWindow()==&istList) {
      // Draw List
      drawList(xstart,context,event.rect());
      // Done
      return(True);
      }
   // Done
   return(True);
}

void NList::drawSplit(const long xpos)
{
   // Restore old position line
   IGLine line(IPoint(lOldSplitXPos,istList.rect().minY())
              ,IPoint(lOldSplitXPos,rect().maxY()        ));
   IGraphicContext context(handle());
   context.setMixMode(IGraphicBundle::xor)
          .setPenColor(IColor(IColor::white));;
   line.drawOn(context);
   // check for valid new position
   if ((xpos<0)||(xpos>istList.rect().maxX())) {
      lOldSplitXPos=-1;
      return;
      }
   // Draw new position line
   line.setStartingPoint(IPoint(xpos,istList.rect().minY()))
       .setEndingPoint  (IPoint(xpos,rect().maxY()        ));
   line.drawOn(context);
   // Remember new pos as old
   lOldSplitXPos=xpos;
}

IRectangle NList::columnHeaderBox(short which)
{
   if ((which<1)||(which>sNoOfColumns))
      return(IRectangle());
   IPoint pos(ipColumnMetric[which-1].coord1()-leftmostBit(),0           );
   ISize size(ipColumnMetric[which-1].coord2() ,istHeader.rect().height());
   return(IRectangle(pos,size));
}

void NList::columnHeaderClicked(short which)
{
}

Boolean NList::mouseMoved( IMouseEvent& event)
{
   // Regarding running dragging operation
   if (dragAction==NList::dragging) {
      // Calculate position of dragged column
      long xpos=-(leftmostBit()-1)+ipColumnMetric[sActionColumn-1].coord1();
      // Calculate new width of dragged column
      long newWidth=max(event.mousePosition().x()-xpos,max(MINCOLWIDTH,minColumnWidth(sActionColumn)));
      long sized=newWidth-ipColumnMetric[sActionColumn-1].coord2();
      sColumnsWidth+=sized;
      ipColumnMetric[sActionColumn-1]+=IPair(0,sized);
      // Calculate metrics of following columns
      for (short i=sActionColumn+1;i<=sNoOfColumns;i++)
         ipColumnMetric[i-1]+=IPair(sized,0);
      // Draw visual split
      drawSplit(xpos+newWidth);
      return(False);
      }
   // Regarding none|pushable|pushing|dragable ... initialize next action
   DragAction newAction=NList::none;
   short newActionColumn=0;
   // Go through columns ...
   long rightBorder;
   long mpos=event.mousePosition().x();
   if (IRectangle(istHeader.rect().size()).contains(event.mousePosition()))
   for (short i=1;i<=sNoOfColumns;i++) {
       // ... relative to right border of column i
       rightBorder= -(leftmostBit()-1) + ipColumnMetric[i-1].coord1() + ipColumnMetric[i-1].coord2();
       // ... new dragable situation (only when not pushing) ?
       if ( (dragAction!=NList::pushing) && (mpos > rightBorder-DRAGWIDTH) && (mpos < rightBorder+DRAGWIDTH) ) {
          newAction=NList::dragable;
          newActionColumn=i;
          break;
          }
       // ... push situation ?
       if (mpos < rightBorder) {
          newActionColumn=i;
          newAction=NList::pushable;    // already pushing won't be disturbed
          break;
          }
       // ... Next column
       }
   // Use investigation
   if (dragAction==NList::pushing) {
      if (newActionColumn!=sActionColumn) {
         short old=sActionColumn;
         sActionColumn=newActionColumn;
         istHeader.refresh(columnHeaderBox(old),True);
         istHeader.refresh(columnHeaderBox(sActionColumn),True);
         }
      }
   else {
      dragAction=newAction;
      sActionColumn=newActionColumn;
      }
   // Done
   return(False);
}

Boolean NList::mousePointerChange( IMousePointerEvent& event)
{
   // Disabled ?
   if (bDisabled) {
      event.setMousePointer(ISystemPointerHandle(ISystemPointerHandle::wait));
      return(True);
      }
   // Header ?
   if (event.windowId()==istHeader.id()) {
      // ... check dragging situation
      switch (dragAction) {
         case NList::dragable:
         case NList::dragging: // ... pointer to WE arrow
              event.setMousePointer(ISystemPointerHandle(ISystemPointerHandle::sizeHorizontal));
              break;
         default:              // ... default pointer
              event.setMousePointer(event.defaultMousePointer());
              break;
         }
      return(True);
      }
   // Done
   return(False);
}

short NList::positionToRow(IPoint pos)
{
   return((istList.rect().height()-pos.y())/rowHeight()+topmostRow());
}

Boolean NList::mouseClicked(IMouseClickEvent& event)
{
   // Only first button
   if ((event.mouseButton()!=IMouseClickEvent::button1)||bDisabled)
      return(False);
   // Header ??
   if (event.controlWindow()==&istHeader) {
      // Only up and down
      if (event.mouseAction()==IMouseClickEvent::click) return(False) ;
      // Check dragging situation
      switch (dragAction) {
         case NList::pushable : // not dragable ... could be push
            if (event.mouseAction()!=IMouseClickEvent::down) break;
            dragAction=NList::pushing;
            istHeader.refresh(columnHeaderBox(sActionColumn),True);
            istHeader.capturePointer();
            break;
         case NList::pushing : // user is pushing and could release button
            if (event.mouseAction()!=IMouseClickEvent::up) break;
            istHeader.releasePointer();
            dragAction=NList::none;
            istHeader.refresh(columnHeaderBox(sActionColumn),True);
            if (sActionColumn>0) columnHeaderClicked(sActionColumn);
            break;
         case NList::dragable: // ... user points at drag-point
            if (event.mouseAction()!=IMouseClickEvent::down) break;
            istHeader.capturePointer();
            dragAction=NList::dragging;
            lOldSplitXPos=-1;
            break;
         case NList::dragging: // ... user is dragging and could release button
            if (event.mouseAction()!=IMouseClickEvent::up) break;
            istHeader.releasePointer();
            dragAction=NList::none;
            layout();
            istHeader.refresh();
            istList.refresh();
            break;
         }
      return(False);
      }
   // List click or double-click ??
   if ( (event.controlWindow()==&istList)
      &&(!bDisabled)) {
      // ... check for click
      short clickRow=positionToRow(event.mousePosition());
      if (clickRow<=noOfRows()) {
         switch (event.mouseAction()) {
            case IMouseClickEvent::click :
                 click(clickRow);break;
            case IMouseClickEvent::doubleClick :
                 doubleClick(clickRow);break;
            }
         }
      return(False);
      // ... weird click
      }
   // Done
   return(False);
}

void NList::click(short sRow) { }
void NList::doubleClick(short sRow) { }

IPopUpMenu* NList::rowPopup(short sRow)
{
   return(NULL); // Has to be overriden
}

IPopUpMenu* NList::listPopup()
{
   return(NULL); // Has to be overriden
}

Boolean NList::makePopUpMenu(IMenuEvent& event)
{
   if (bDisabled) return(False);
   // Create PopUpMenu Object and fill it
   IPopUpMenu* pMenu;
   short row=positionToRow(event.mousePosition());
   if ( (event.controlWindow()==&istList)
      &&(row<=noOfRows())
      &&(event.mousePosition().x()<sColumnsWidth))
      pMenu=rowPopup(row);
   else
      pMenu=listPopup();
   if (!pMenu) return(False);
   // Show PopUp
   pMenu->setAutoDeleteObject(true);
   pMenu->show(IWindow::mapPoint(event.mousePosition(),event.controlWindow()->handle(),handle()));
   // Done it
   return(True);
}

void NList::columnChange(Boolean left)
{
   if (!isbLeftRight.isVisible()) return;
   // Find column that is shown on the left side right now
   for (short i=1;i<=sNoOfColumns;i++)
      if (leftmostBit()<ipColumnMetric[i-1].coord1()+ipColumnMetric[i-1].coord2()) break;
   // ... scroll
   if (left) {
      if ( (i>1) && (ipColumnMetric[i-1].coord1()==leftmostBit()-1) )
         i--;
      scrollHorizontal(ipColumnMetric[i-1].coord1()-leftmostBit()+1);
      }
   else
      scrollHorizontal(ipColumnMetric[i-1].coord1()+ipColumnMetric[i-1].coord2()+1-leftmostBit());
   // Done
}

Boolean NList::virtualKeyPress(IKeyboardEvent& event)
{
   if (bDisabled) return(True);
   // In case of no release check key
   if (!event.isUpTransition()) switch (event.virtualKey()) {
      case IKeyboardEvent::left     :
           columnChange(True);
           break;
      case IKeyboardEvent::right    :
           columnChange(False);
           break;
      case IKeyboardEvent::up       :
           scrollVertical(-1);
           break;
      case IKeyboardEvent::down     :
           scrollVertical(1);
           break;
      case IKeyboardEvent::pageUp   :
           scrollVertical(-16);
           break;
      case IKeyboardEvent::pageDown :
           scrollVertical(16);
           break;
      case IKeyboardEvent::home   :
           scrollVertical(-topmostRow());
           break;
      case IKeyboardEvent::end :
           scrollVertical(noOfRows());
           break;
      }
   // Done
   istList.setFocus();
   return(True);
}

Boolean NList::lineUp(IScrollEvent& event)
{
   scrollVertical(-1);
   return(True);
}
Boolean NList::lineDown(IScrollEvent& event)
{
   scrollVertical(1);
   return(True);
}
Boolean NList::pageUp(IScrollEvent& event)
{
   scrollVertical(-isbUpDown.visibleCount());
   return(True);
}
Boolean NList::pageDown(IScrollEvent& event)
{
   scrollVertical(isbUpDown.visibleCount());
   return(True);
}

Boolean NList::lineLeft(IScrollEvent& event)
{
   scrollHorizontal(-1);
   return(False);
}
Boolean NList::lineRight(IScrollEvent& event)
{
   scrollHorizontal(1);
   return(False);
}
Boolean NList::pageLeft(IScrollEvent& event)
{
   columnChange(True);
   return(True);
}
Boolean NList::pageRight(IScrollEvent& event)
{
   columnChange(False);
   return(True);
}
Boolean NList::scrollBoxTrack(IScrollEvent& event)
{
   if (event.scrollBarWindow()==&isbLeftRight)
      scrollHorizontal(event.scrollAmount());
   if (event.scrollBarWindow()==&isbUpDown)
      scrollVertical(event.scrollAmount());
   return(True);
}

LONG _System WinScrollWindow(HWND hwnd,LONG dx,LONG dy,RECTL* prclScroll,RECTL* prclClip,HRGN hrgnUpdate,RECTL* prclUpdate,ULONG rgfsw);

void NList::scrollVertical(long amount)
{
   if (!isbUpDown.isShowing()) return;
   // Scroll the bar
   long pos=topmostRow(),
        max=isbUpDown.scrollBoxRange().upperBound();
   if ((amount<0)&&(pos+amount<  1)) amount= -pos+1;
   if ((amount>0)&&(pos+amount>max)) amount=max-pos;
   isbUpDown.moveScrollBoxTo(pos+amount);
   // ...  signal scrolling to virtual function
   positionsHaveChanged();
   // ... copy list
   _RECTL rectlClip,rectlListRefresh;
   rectlClip.xLeft  =0;
   rectlClip.xRight =istList.rect().width();
   rectlClip.yBottom=istList.rect().height()-(isbUpDown.visibleCount()*rowHeight());
   rectlClip.yTop   =istList.rect().height();
   WinScrollWindow(istList.handle(),0,amount*rowHeight(),NULL,&rectlClip,NULL,&rectlListRefresh,0);
   // ... refresh invalidated area
   istList.refresh(IRectangle(rectlListRefresh.xLeft ,rectlListRefresh.yBottom
                             ,rectlListRefresh.xRight,rectlListRefresh.yTop   )
                  ,true);
   // Done
}

void NList::scrollHorizontal(long amount)
{
   if (!isbLeftRight.isShowing()) return;
   // Scroll the bar
   long pos=leftmostBit(),
        max=isbLeftRight.scrollBoxRange().upperBound();
   if ((amount<0)&&(pos+amount<  1)) amount=-pos+1 ;
   if ((amount>0)&&(pos+amount>max)) amount=max-pos;
   isbLeftRight.moveScrollBoxTo(pos+amount);
   // ...  signal scrolling to virtual function
   positionsHaveChanged();
   // ... copy list
   _RECTL rectlClip,rectlListRefresh;
   rectlClip.xLeft  =0;
   rectlClip.xRight =istList.rect().width();
   rectlClip.yBottom=0;
   rectlClip.yTop   =istList.rect().height();
   WinScrollWindow(istList.handle(),-amount,0,NULL,&rectlClip,NULL,&rectlListRefresh,0);
   // ... copy header
   _RECTL rectlHeaderRefresh;
   rectlClip.xLeft  =0;
   rectlClip.xRight =istHeader.rect().width();
   rectlClip.yBottom=0;
   rectlClip.yTop   =istHeader.rect().height();
   WinScrollWindow(istHeader.handle(),-amount,0,NULL,&rectlClip,NULL,&rectlHeaderRefresh,0);
   // ... refresh invalidated areas
   istHeader.refresh(IRectangle(rectlHeaderRefresh.xLeft ,rectlHeaderRefresh.yBottom
                               ,rectlHeaderRefresh.xRight,rectlHeaderRefresh.yTop   )
                    ,true);
   istList.refresh(IRectangle(rectlListRefresh.xLeft ,rectlListRefresh.yBottom
                             ,rectlListRefresh.xRight,rectlListRefresh.yTop   )
                  ,true);
   // Done
}

void NList::positionsHaveChanged()
{
   // Intentionally left empty
}

