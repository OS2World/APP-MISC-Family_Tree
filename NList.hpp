/*
   File   : NList.hpp
   Purpose: Define own list class for collections
*/

#ifndef _NLIST_HPP_
#define _NLIST_HPP_

// Include User Interface Class Library class headers
#include <ifont.hpp>
#include <istattxt.hpp>
#include <istring.hpp>
#include <irect.hpp>
#include <icustbut.hpp>
#include <icustbhd.hpp>
#include <imoushdr.hpp>
#include <icmdhdr.hpp>
#include <ipainhdr.hpp>
#include <iscroll.hpp>
#include <iscrlhdr.hpp>
#include <imphdr.hpp>
#include <imoushdr.hpp>
#include <ipopmenu.hpp>
#include <icanvas.hpp>
#include <imenuhdr.hpp>
#include <imenuevt.hpp>
#include <ikeyhdr.hpp>
#include <ipopmenu.hpp>
#include <isizehdr.hpp>


// Class  : NList
// Purpose: list of items (like container detail view)
class NList : public ICanvas,
              public IPaintHandler,
              public IScrollHandler,
              public IMouseHandler,
              public IMenuHandler,
              public IKeyboardHandler,
              public IResizeHandler
{
public:
   NList(unsigned long windowId,IWindow* ownerparentWnd,Boolean bSetSeparator=True);
   virtual ~NList();
   NList&   addColumn(const IString& isName,short sWidth=0);
   short    columnWidth(short which);
   IString  columnName(short which);
   short    columnCount();
   void     disableInput();
   void     enableInput();
   IWindow* listWindow();
   void     setEmptyText(const IString& isSetEmptyText);
protected:
   virtual ISize    calcMinimumSize() const;
   virtual ICanvas& layout();
   virtual Boolean  paintWindow(IPaintEvent &event);
   virtual Boolean  mousePointerChange( IMousePointerEvent& event);
   virtual Boolean  mouseMoved( IMouseEvent& event);
   virtual Boolean  mouseClicked(IMouseClickEvent& event);
   virtual Boolean  makePopUpMenu(IMenuEvent& event);
   virtual Boolean  lineDown(IScrollEvent& event);
   virtual Boolean  lineLeft(IScrollEvent& event);
   virtual Boolean  lineRight(IScrollEvent& event);
   virtual Boolean  lineUp(IScrollEvent& event);
   virtual Boolean  pageDown(IScrollEvent& event);
   virtual Boolean  pageLeft(IScrollEvent& event);
   virtual Boolean  pageRight(IScrollEvent& event);
   virtual Boolean  pageUp(IScrollEvent& event);
   virtual Boolean  scrollBoxTrack(IScrollEvent& event);
   virtual Boolean  virtualKeyPress(IKeyboardEvent& event);
   virtual Boolean  windowResize(IResizeEvent& event);

   virtual short    noOfRows();
   virtual short    rowHeight();
   virtual short    minColumnWidth(short sColumn);

   short leftmostBit();
   short topmostRow();
   short visibleRows();
   void  redrawRow(short sRow);
   void  dataHasChanged();

   virtual Boolean  drawStart(short sStartRow);
   virtual void     drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before);
   virtual void     drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString);
   virtual void     drawEnd();

   void    scrollHorizontal(long amount);
   void    scrollVertical(long amount);
   virtual void positionsHaveChanged();

   short   positionToRow(IPoint pos);
private:

   void    drawHeader    (const long xstart,IGraphicContext& context,const IRectangle& area);
   void    drawSeparators(const long xstart,IGraphicContext& context);
   void    drawList      (const long xstart,IGraphicContext& context,const IRectangle& area);
   void    drawSplit     (const long xpos);
   void    columnChange  (Boolean left);

   IRectangle columnHeaderBox(short which);
   virtual void columnHeaderClicked(short which);

   virtual void    click        (short sRow);
   virtual void    doubleClick  (short sRow);
   virtual IPopUpMenu* rowPopup (short sRow);
   virtual IPopUpMenu* listPopup();

   enum DragAction { none , dragable, dragging, pushable, pushing };

   DragAction  dragAction;
   short       sActionColumn;
   short       sNoOfColumns;
   IPair       ipColumnMetric[10];
   short       sColumnsWidth;
   IString     isColumn[10];
   long        lOldSplitXPos;
   Boolean     bDisabled;
   IString     isEmptyText;
   Boolean     bSeparator;

   ICanvas istHeader;
   ICanvas istList;
   IScrollBar  isbLeftRight;
   IScrollBar  isbUpDown;
};



#endif
