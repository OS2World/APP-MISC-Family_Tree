/*
   File   : guimisc.hpp
   Purpose: Define own simple GUI Classes
*/

#include "FTree.h"
#include "Main.hpp"
#include "NDragDrop.hpp"
#include "NPointersTo.h"

#ifndef _GUIMISC_HPP_
#define _GUIMISC_HPP_

// Include User Interface Class Library class headers
#include <ientryfd.hpp>
#include <ifont.hpp>
#include <istattxt.hpp>
#include <imle.hpp>
#include <ifocshdr.hpp>
#include <ikeyhdr.hpp>
#include <istring.hpp>
#include <imcelcv.hpp>
#include <irect.hpp>
#include <iedithdr.hpp>
#include <iobservr.hpp>
#include <inotifev.hpp>
#include <ihandle.hpp>
#include <ibmpctl.hpp>
#include <igbitmap.hpp>
#include <imsgbox.hpp>
#include <ivport.hpp>
#include <ikeyhdr.hpp>
#include <ipushbut.hpp>
#include <icustbut.hpp>
#include <icustbhd.hpp>
#include <imoushdr.hpp>
#include <imcelcv.hpp>
#include <iframe.hpp>
#include <icmdhdr.hpp>
#include <isetcv.hpp>
#include <ipainhdr.hpp>
#include <itbar.hpp>
#include <isizehdr.hpp>
#include <inotebk.hpp>
#include <icnrctl.hpp>
#include <icnrhdr.hpp>
#include <icnrehdr.hpp>
#include <icnrmhdr.hpp>
#include <icnrdihd.hpp>
#include <iscroll.hpp>
#include <iscrlhdr.hpp>
#include <imphdr.hpp>
#include <imoushdr.hpp>
#include <ipopmenu.hpp>
#include <i3statbx.hpp>
#include <iselhdr.hpp>

#define DID_OK 1
#define DID_CANCEL 2

class NStatusLine;
class NTextEntry;
class NMemoField;
class NBitmapControl;
class NButton;
class NQuestion;
class NColorControl;
class NButtonBar;

// Class  : NNotebook
// Purpose: New Notebook with color tabs
class NNotebook : public INotebook
{
public:
   NNotebook(unsigned long id,IWindow* pParent,IWindow* pOwner);
};

// Class  : NContainer
// Purpose: Container with empty text in center
class NContainer : public IContainerControl,
                   public ICnrDrawHandler,
                   public IResizeHandler
{
public:
   NContainer(unsigned long id
             ,IWindow* parent
             ,IWindow* owner
             ,const IRectangle& location = IRectangle()
             ,const Style& style = defaultStyle()
             ,const Attribute& attribute = defaultAttribute());
   NContainer& setEmptyText(const IString& isSet);
   void disableInput();
   void enableInput();
   virtual Boolean drawBackground( ICnrDrawBackgroundEvent& event);
protected:
   virtual Boolean windowResize(IResizeEvent& event);
private:
   IString isEmptyText;
};

// Class  : NFileBox
// Purpose: Container with Files in it

class NFileBox : public NContainer,
                 public ICnrHandler,
                 public ICnrEditHandler,
                 public ICnrMenuHandler,
                 public ICommandHandler,
                 public NDragDropHandler
{
public:
   NFileBox(unsigned long windowId,IWindow* parent,IWindow* owner,Boolean setRelative);
   virtual ~NFileBox();
   virtual void fileDropped(IString isFile);
   virtual Boolean enter(ICnrEnterEvent& event);
   virtual Boolean endEdit(ICnrEndEditEvent& event);
   void calcIcon(IContainerObject* obj,const IString& file);
   void clear();
   void add(const IString& file,Boolean warning);
   IString get(unsigned short num);
   unsigned short count();
   Boolean hasChanged();
   IString isDropName;
private:
   void open(IContainerObject* obj);
   Boolean bChanged;
   Boolean bRelative;
   IContainerObject* pPopObject;
protected:
   virtual Boolean makePopUpMenu(IMenuEvent& menuEvent);
   virtual Boolean command(ICommandEvent& event);
};

// Class  : NButtonBar
// Purpose: ViewPort incl. Canvas for Buttons
class NButtonBar : public ICanvas,
                   public IPaintHandler,
                   public IResizeHandler
{
public:
   NButtonBar(unsigned long windowId,IWindow* parentowner);
   ~NButtonBar();
   NButtonBar& addChild(IControl* pChild,Boolean more=False);
   NButtonBar& setGrowableRow(short sSetGrowableRow);
protected:
   virtual ISize calcMinimumSize() const;
   virtual NButtonBar& layout();
   virtual Boolean paintWindow(IPaintEvent& event);
   virtual Boolean windowResize(IResizeEvent& event);
private:
   IControl* pChildren[16];
   short    sChildren;
   short    sGrowableRow;
   long     lMaxHeight;
   long     lTotalWidth;
};

// Class  : NColorControl
// Purpose: display a set of colors to choose from
class NColorControl : public IMultiCellCanvas,
                      public IMouseHandler,
                      public IPaintHandler
{
public:
   NColorControl(unsigned long windowId,IWindow* pOwnerParent);
   ~NColorControl();
   void select(unsigned short int sel);
   unsigned short int getSelected();
private:
   void drawEmphasis(IGraphicContext& igc,unsigned short int which);

   unsigned short int selected;
   IStaticText istColor11;
   IStaticText istColor21;
   IStaticText istColor31;
   IStaticText istColor41;
   IStaticText istColor12;
   IStaticText istColor22;
   IStaticText istColor32;
   IStaticText istColor42;
   IStaticText istColor13;
   IStaticText istColor23;
   IStaticText istColor33;
   IStaticText istColor43;
   IStaticText istColor14;
   IStaticText istColor24;
   IStaticText istColor34;
   IStaticText istColor44;
protected:
   virtual Boolean mouseClicked(IMouseClickEvent& event);
   virtual Boolean paintWindow(IPaintEvent& event);
};

// Class  : NQuestion
// Purpose: question params from user
class NQuestion : public IFrameWindow,
                  public ICommandHandler,
                  public IKeyboardHandler
{
public:
   NQuestion(unsigned long windowId
            ,IWindow* pOwner
            ,const IString& isTitle
            ,const IPoint ipPos);
   ~NQuestion();
   IString getText();
   void setText(const IString& isSet);
private:
   IMultiCellCanvas iCanvas;
   IEntryField      iefText;
   IPushButton      ipbOK;
   IPushButton      ipbCANCEL;
protected:
   virtual Boolean command(ICommandEvent& event);
   Boolean virtualKeyPress(IKeyboardEvent& event);
};


// Class  : NButton
// Purpose: special (latchable) button
class NButton : public ICanvas
              , public IPaintHandler
              , public IMouseHandler
{
public:
   NButton(unsigned long windowId,IWindow* pParent,IWindow* pOwner,Boolean bSetAlwaysBorder=False);
   ~NButton();
   Boolean  hasChanged(Boolean setChanged=False);
   void     disableInput();
   void     enableInput();
   void     setLeftAlign(Boolean set=True);
   void     setAlwaysBorder(Boolean set);
   NButton& setBitmap(unsigned long id);
   NButton& setLatchedBitmap(unsigned long id);
   NButton& enableLatching(Boolean setEnable=True);
   NButton& latch(Boolean setLatched=True);
   Boolean  isLatched();
   NButton& setString(const IString& isSet);
   IString  getString();
   void     click();
private:
   Boolean  bChanged;
   Boolean  bDisabled;
   Boolean  bLeftAlign;
   Boolean  bDown;
   Boolean  bClick;
   Boolean  bLatched;
   Boolean  bLatchable;
   Boolean  bAlwaysBorder;
   IGBitmap* pBitmap;
   IGBitmap* pLatchedBitmap;
   IString  isString;
protected:
   virtual ISize calcMinimumSize() const;
   virtual Boolean paintWindow(IPaintEvent& event);
   virtual Boolean mouseMoved(IMouseEvent& event);
   virtual Boolean mouseClicked(IMouseClickEvent& event);
};

// Class  : NStatusLine
// Purpose: Status Line text control with border
class NStatusLine : public ICanvas,
                    public IPaintHandler
{
public:
   NStatusLine(unsigned long windowId,IWindow* ownerparentWnd);
   virtual ~NStatusLine();
   NStatusLine& setLimit(const long setValue);
   NStatusLine& setText (const IString& setValue);
protected:
   virtual Boolean paintWindow(IPaintEvent& event);
   virtual ISize calcMinimumSize() const;
private:
   IString isText;
   long    lLimit;
};

// Class  : NTextEntry
// Purpose: Special IEntryField
class NTextEntry : public IEntryField,
                   public IFocusHandler
{
public:
   NTextEntry(unsigned long windowId,IWindow* parent,IWindow* owner);
   virtual ~NTextEntry();
   void disableInput();
   void enableInput();
protected:
   virtual ISize calcMinimumSize() const;
   virtual Boolean gotFocus(IControlEvent& iceEvent);
   virtual Boolean lostFocus(IControlEvent& iceEvent);
};

// Class  : NNumEntry
// Purpose: numeric IEntryField
class NNumEntry : public IEntryField,
                  public IFocusHandler,
                  public IKeyboardHandler,
                  public IResizeHandler,
                  public IScrollHandler
{
public:
   NNumEntry(unsigned long windowId,IWindow* parent,IWindow* owner);
   virtual ~NNumEntry();
   void disableInput();
   void enableInput();
protected:
   virtual ISize calcMinimumSize() const;
   virtual Boolean gotFocus(IControlEvent& iceEvent);
   virtual Boolean lostFocus(IControlEvent& iceEvent);
   virtual Boolean characterKeyPress(IKeyboardEvent& event);
   virtual Boolean windowResize(IResizeEvent& event);
   virtual Boolean lineDown(IScrollEvent& event);
   virtual Boolean lineUp  (IScrollEvent& event);
   virtual Boolean scrollEnd(IScrollEvent& event);
private:
   IScrollBar* pScroll;
};


// Class  : NYesNoControl
// Purpose: Own yes no selector
class NYesNoControl : public I3StateCheckBox,
                      public ISelectHandler
{
public:
   enum Value { Unknown, Yes, No };
   NYesNoControl(unsigned long windowId,IWindow* pParent,IWindow* pOwner);
   virtual ~NYesNoControl();
   Boolean        hasChanged();
   Value          value();
   NYesNoControl& setValue(Value vSet);
   virtual ITextControl& setLayoutDistorted( unsigned long layoutAttributeOn,unsigned long layoutAttributeOff);
protected:
   virtual Boolean selected(IControlEvent& event);
private:
   Boolean bChanged;
};



// Class  : NBitmapControl
// Purpose: Own BitmapControl with Viewport
class NBitmapControl : public IViewPort,
                       public NDragDropHandler,
                       public IMouseHandler
{
public:
   NBitmapControl(unsigned long windowId,IWindow* ownerparentWnd,Boolean setRelative);
   virtual ~NBitmapControl();
   virtual void fileDropped(IString isFile);
   Boolean setLocation(IString isSetLocation,Boolean noWarning);
   IString getLocation();
   Boolean hasChanged();
   void disableInput();
   void enableInput();
private:
   Boolean        bRelative;
   Boolean        bChanged;
   ISetCanvas     iCanvas;
   IBitmapControl iBmp;
   NQuestion*     pNQuestion;
protected:
   virtual Boolean mouseClicked(IMouseClickEvent& event);
};

// Class  : NFocusHandler
// Purpose: Own Focushandler
class NFocusHandler : public IHandler
{
public:
   NFocusHandler();
   virtual ~NFocusHandler();
protected:
   virtual Boolean focusChanged(Boolean received);
   virtual Boolean dispatchHandlerEvent(IEvent& event);
};


// Class  : NDate
// Purpose: Do own Date eg. 00-00-1870
class NDate
{
public:
   NDate();
   NDate(unsigned short sSetDay,unsigned short sSetMonth,unsigned short sSetYear,unsigned short sSetModifier=0);
   NDate(const NDate& from);
   ~NDate();

   friend Boolean operator< (const NDate& d1, const NDate& d2);
   friend Boolean operator<= (const NDate& d1, const NDate& d2);
   NDate& operator= (const NDate& d);

   Boolean hasInfo()    const;
   unsigned short day()         const;
   unsigned short month()       const;
   unsigned short year()        const;
   unsigned short modifier()    const;
   unsigned long  days()        const;

   friend ostream& operator<< (ostream& oStream, NDate& d);
   friend istream& operator>> (istream& iStream, NDate& d);
   IString asString() const;

   enum Digit { digDay=0,digMonth=1,digYear=2 };

   void   drawOn(IGraphicContext& igcContext,const IPoint& ipPos,const IFont& ifFont,IGString& igsText,Boolean bAlwaysSeps=False) const;
   static ISize drawSize(const IFont& ifFont);
   static IRange digitMetric(Digit which,const IFont& ifFont);

   static NDate::Digit firstDigit();
   static NDate::Digit lastDigit();
   static NDate::Digit nextDigit(NDate::Digit from);
   static NDate::Digit prevDigit(NDate::Digit from);

   static char modifiers[];
private:
   unsigned char  cDay;
   unsigned char  cMonth;
   unsigned short sYear;
};

// Class  : NDateControl
// Purpose: Own entry control for dates
class NDateControl : public ICanvas,
                     public IKeyboardHandler,
                     public IPaintHandler,
                     public IMouseHandler,
                     public NFocusHandler,
                     public ICommandHandler,
                     public IMenuHandler
{
public:
   NDateControl(unsigned long windowId,IWindow* pParent,IWindow* pOwner);
   virtual ~NDateControl();
   NDateControl& setDate(const NDate& dSetDate);
   const NDate getDate();
   Boolean hasChanged();
   void disableInput();
   void enableInput();
   void startOver();
private:
   IStaticText istDate;
   unsigned short sValue[3];
   unsigned short sModifier;
   NDate::Digit   digActual;
   short          sDigOffset;
   Boolean        bChanged;
   Boolean        bDisabled;
   Boolean        bKeepPosition;
   void           makeCursor(Boolean appear);
   void           drawDate(IGraphicContext& context,Boolean focus);
protected:
   virtual Boolean  command(ICommandEvent& event);
   virtual Boolean  makePopUpMenu(IMenuEvent& event);
   virtual ISize    calcMinimumSize() const;
   virtual ICanvas& layout();
   virtual Boolean  paintWindow(IPaintEvent& event);
   virtual Boolean  mouseClicked(IMouseClickEvent& event);
   virtual Boolean  focusChanged(Boolean received);
   virtual Boolean  virtualKeyPress(IKeyboardEvent& event);
   virtual Boolean  characterKeyPress(IKeyboardEvent& event);
};

// Class  : NMemoField
// Purpose: Special IMultiLineEdit
class NMemoField : public IMultiLineEdit,
                   public IFocusHandler
{
public:
   NMemoField(unsigned long windowId,IWindow* ownerparentWnd);
   virtual ~NMemoField();
   virtual ISize calcMinimumSize() const;
   void disableInput();
   void enableInput();
protected:
   virtual Boolean gotFocus(IControlEvent& iceEvent);
   virtual Boolean lostFocus(IControlEvent& iceEvent);
};

#endif
