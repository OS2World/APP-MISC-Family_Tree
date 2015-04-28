/*
   File   : EventDialog.hpp
   Purpose: Define Classes for Interface of Application
*/

#ifndef _NEventDIALOG_HPP_
#define _NEventDIALOG_HPP_


// Include User Interface Class Library class headers
#include <iframe.hpp>
#include <ipopmenu.hpp>
#include <iframhdr.hpp>
#include <imenuhdr.hpp>
#include <icmdhdr.hpp>
#include <iaccel.hpp>
#include <ikeyhdr.hpp>
#include <icustbut.hpp>
#include <imcelcv.hpp>
#include <imphdr.hpp>
#include <ivport.hpp>
#include <istattxt.hpp>
#include <imenuhdr.hpp>
#include <isplitcv.hpp>
#include <igroupbx.hpp>


#include "NMisc.hpp"


// Class  : NQueryInvolved
// Purpose: Ask for role of involved person
class NQueryInvolved : public IFrameWindow,
                       public ICommandHandler
{
public:
   NQueryInvolved(long windowId,IWindow* pOwner,PNPerson pSetInvolved,IString isSetRole=IString());
   virtual ~NQueryInvolved();
   IString isResult;
private:
   IMultiCellCanvas imccCanvas;
   IGroupBox        igbInvolvedPerson,igbInvolvedRole;
   IEntryField      iefInvolvedPerson,iefInvolvedRole;
   IPushButton      ipbCancel,ipbOK;
   PNPerson             pPerson;
protected:
   virtual Boolean command(ICommandEvent& event);
};

// Class  : NEventDialog
// Purpose: Dialog Window Class for Event-List
class NEventList;
class NEventEdit;

class NEventDialog : public IFrameWindow,
                     public IFrameHandler
{
public:
   static NEventDialog* pNEventDialog;
   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);

   NEventDialog(IWindow* piwOwner,NMankind* pSetNMankind);
   virtual ~NEventDialog();
   void disableInput();
   void enableInput();

   static void changeFloat(Boolean set);
   static Boolean isFloat();
private:
   Boolean bDisabled;

   NMankind* pNMankind;

   ISplitCanvas iscCanvas;
   NEventList* pNEventList;
   NEventEdit* pNEventEdit;

   static IFont ifProfileFont;
   static IRectangle irProfileRectangle;
   static Boolean bProfileMaxed;
   static Boolean bProfileFloat;
   static short sProfileEventCols[5];
protected:
   virtual Boolean closed(IFrameEvent& frameEvent);
   virtual Boolean saved(IFrameEvent& frameEvent);
};


#endif
