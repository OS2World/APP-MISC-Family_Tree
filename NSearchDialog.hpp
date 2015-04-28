/*
   File   : NSearchDialog.hpp
   Purpose: Define classes for Searching Interface
*/

#ifndef _SEARCHDIALOG_HPP_
#define _SEARCHDIALOG_HPP_

// Include User Interface Class Library class headers
#include <iframe.hpp>
#include <ivport.hpp>
#include <ipopmenu.hpp>
#include <imphdr.hpp>
#include <imcelcv.hpp>
#include <ifocshdr.hpp>
#include <iframhdr.hpp>
#include <imsgbox.hpp>
#include <iobservr.hpp>
#include <icmdhdr.hpp>
#include <iradiobt.hpp>
#include <ititle.hpp>
#include <inotebk.hpp>
#include <icheckbx.hpp>
#include <iradiobt.hpp>
#include <iselhdr.hpp>
#include <isetcv.hpp>
#include <ipushbut.hpp>
#include <icombobx.hpp>

#include "NMisc.hpp"
#include "NMankind.hpp"
#include "NMainWindow.hpp"
#include "NUserFields.hpp"

unsigned long _System DosBeep(unsigned long,unsigned long);
unsigned long _System DosSleep(unsigned long);

class NSearch;

// Class  : NSearchDialog
// Purpose: Dialog Window Class for Options Setting
class NSearchDialog : public IFrameWindow,
                      public IFrameHandler
{
public:
   static NSearchDialog* pNSearchDialog;
   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);
   static void generate(IWindow* piwOwner,NMankind* pSetNMankind);

   NSearchDialog(IWindow* piwOwner,NMankind* pSetNMankind);
   virtual ~NSearchDialog();

   void disableInput();
   void enableInput();
private:
   static IFont ifProfileFont;
   static IRectangle irProfileRectangle;
   Boolean bDisabled;

   NSearch* pNSearch;
   NMankind* pNMankind;
protected:
   virtual Boolean saved(IFrameEvent& frameEvent);
};


#endif
