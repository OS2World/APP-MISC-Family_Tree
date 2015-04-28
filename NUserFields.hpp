/*
   File   : NUserFields.hpp
   Purpose: Define Classes for user fields management
*/

#ifndef _USERFIELDS_HPP_
#define _USERFIELDS_HPP_

// Include User Interface Class Library class headers
#include <imcelcv.hpp>
#include <icmdhdr.hpp>
#include <ipushbut.hpp>

#include "NMisc.hpp"
#include "NList.hpp"

class NUserFieldList;

// Class  : NUserFields
// Purpose: Window Class for NPerson's user defined fiels
class NUserFields : public NList,
                    public ICommandHandler,
                    public NMankindHandler
{
public:
   NUserFields(unsigned long windowId,IWindow* ownerparentWnd,NMankind* pSetNMankind,long colMetric=0);
   ~NUserFields();
   void setMan(PNPerson pSetMan);
   short getActualIdx();
   void disableInput();
   long getColMetric();
protected:
   virtual short       noOfRows();
   virtual short       rowHeight();
   virtual Boolean     drawStart(short sStartRow);
   virtual void        drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString);
   virtual IPopUpMenu* rowPopup(short sRow);
   virtual void        click(short sRow);
   virtual void        positionsHaveChanged();

   virtual void userFieldsModified();
   virtual void disconnentMen();
   virtual void doChangesNow();

   virtual Boolean command(ICommandEvent& event);
private:
   void storeInfo();
   void loadInfo();

   PNPerson  pMan;
   short     sActual;
   NMankind* pNMankind;
   ICanvas   icCanvas;
   IControl* pInput;
};


// Class  : NEditUserFields
// Purpose: Dialog Window Class for editing of user fields
class NEditUserFields : public IMultiCellCanvas,
                        public ICommandHandler,
                        public NMankindHandler
{
public:
   NEditUserFields(unsigned long windowId,IWindow* pParentOwner,IFrameWindow* pSetFrame,NMankind* pSetNMankind);
   virtual ~NEditUserFields();
   void disableInput();
   void enableInput();
private:
   void userFieldsModified();

   NMankind*       pNMankind;
   NUserFieldList* pNUserFieldList;
   IPushButton     ipbAdd;
   IPushButton     ipbChange;
   IPushButton     ipbDelete;
   IFrameWindow*   pFrame;
protected:
   virtual Boolean command(ICommandEvent& event);
};


#endif
