/*
   File   : ListDialog.hpp
   Purpose: Define Classes for Interface of Application
*/

#ifndef _LISTDIALOG_HPP_
#define _LISTDIALOG_HPP_


// Include User Interface Class Library class headers
#include <iframe.hpp>
#include <ipopmenu.hpp>
#include <iframhdr.hpp>
#include <imenuhdr.hpp>
#include <icmdhdr.hpp>
#include <iaccel.hpp>
#include <ikeyhdr.hpp>
#include <imcelcv.hpp>
#include <ivport.hpp>
#include <istattxt.hpp>
#include <imenuhdr.hpp>

#include "NMisc.hpp"
#include "NManList.hpp"
#include "NMainWindow.hpp"


// Class  : NListDialog
// Purpose: Dialog Window Class for Mankind-List
class NListDialog : public IFrameWindow,
                    public IFrameHandler,
                    public ICommandHandler
{
public:
   static NListDialog* pNListDialog;
   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);

   NListDialog(IWindow* piwOwner,NMankind* pSetNMankind);
   virtual ~NListDialog();
   void disableInput();
   void enableInput();

   static void changeFloat(Boolean set);
   static Boolean isFloat();
private:
   Boolean askForSorting();

   Boolean bDisabled;

   NMankind* pNMankind;
   NManList  nManList;

   static IFont ifProfileFont;
   static IRectangle irProfileRectangle;
   static Boolean bProfileMaxed;
   static IString isProfileSortBy;
   static Boolean bProfileFloat;
   static short sProfileColWidth[8];
protected:
   virtual Boolean closed(IFrameEvent& frameEvent);
   virtual Boolean saved(IFrameEvent& frameEvent);
   virtual Boolean command(ICommandEvent& event);
};

#endif
