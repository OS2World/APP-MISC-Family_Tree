/*
   File   : NManList.hpp
   Purpose: Define Class for showing persons in an own list window
*/


#ifndef _NNPersonLIST_HPP
#define _NNPersonLIST_HPP

#include "NMankind.hpp"
#include "NList.hpp"
#include "NDragDrop.hpp"


#include <imsgbox.hpp>
#include <imcelcv.hpp>
#include <iscroll.hpp>
#include <iscrlhdr.hpp>
#include <ipainhdr.hpp>
#include <igbitmap.hpp>
#include <ireslib.hpp>
#include <ifont.hpp>
#include <igrect.hpp>
#include <igline.hpp>
#include <igstring.hpp>
#include <icolor.hpp>
#include <istattxt.hpp>
#include <iprofile.hpp>
#include <ireslock.hpp>


// Class  : NNManList
// Purpose: List all NPersons
class NManList : public NList,
                 public NMankindHandler,
                 public NDragDropHandler
{
public:
   static NManList* pNManList;
   NManList(unsigned long windowIdentifier
          ,IWindow* piwParentOwner
          ,NMankind* pSetNMankind
          ,IFont& initialFont
          ,short* pSetColWidth
          ,IString isSetSortBy=IString());
   virtual ~NManList();

   void setSortBy(const IString& isSetSort);
   const IString& getSortBy();
   IString isSortBy;

   virtual void  manChanged(NMankindHandler* pInitiator);
   virtual void  manAdded(PNPerson which,Boolean more);
   virtual void  manAdded(SEQOFNPerson& which);
   virtual void  disconnentMen();
   virtual void  actualChanged(PNPerson pOld,PNPerson pNew);
   virtual void  originChanged(PNPerson pOld,PNPerson pNew);

   virtual PNPerson calcManToDrag(IPoint where);
protected:
   virtual short noOfRows();
   virtual short rowHeight();
   virtual short minColumnWidth(short sColumn);

   virtual Boolean  drawStart(short sStartRow);
   virtual void     drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before);
   virtual void     drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString);
   virtual void     drawEnd();

private:
   void    click      (short sRow);
   void    doubleClick(short sRow);
   virtual IPopUpMenu* rowPopup   (short sRow);
   virtual IPopUpMenu* listPopup  ();
   virtual void columnHeaderClicked(short which);

   NMankind* pNMankind;

   IPrivateResource* pLockSequences;
   Boolean           bDisabled;
   SEQOFNPerson          seqMan;
   SEQOFNPerson::Cursor  seqManCursor;
   unsigned long     seqManIdx;
   SEQOFNPerson::Cursor  seqTempCursor;
   SEQOFNPerson::Cursor  seqRowCursor;

   IGBitmap igbMale;
   IGBitmap igbFemale;
   IGBitmap igbUnknown;
};


#endif
