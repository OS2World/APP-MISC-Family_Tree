/*
   File   : EditDialog.hpp
   Purpose: Define Classes for Interface of Application
*/

#ifndef _EDITDIALOG_HPP_
#define _EDITDIALOG_HPP_

// Include User Interface Class Library class headers
#include <iframe.hpp>
#include <ivport.hpp>
#include <ipopmenu.hpp>
#include <imoushdr.hpp>
#include <imcelcv.hpp>
#include <ifocshdr.hpp>
#include <iframhdr.hpp>
#include <imsgbox.hpp>
#include <iobservr.hpp>
#include <icmdhdr.hpp>
#include <iradiobt.hpp>
#include <ititle.hpp>
#include <isizehdr.hpp>
#include <itimer.hpp>

#include "NMankind.hpp"
#include "NMisc.hpp"
#include "NMainWindow.hpp"

unsigned long _System DosBeep(unsigned long,unsigned long);
unsigned long _System DosSleep(unsigned long);

// Class  : NEditDialog
// Purpose: Dialog Window Class for Man-Editing

class NManEdit;


class NEditDialog : public IFrameWindow,
                    public IFrameHandler
{
public:
   static NEditDialog* pNEditDialog;
   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);

   NEditDialog(IWindow* piwOwner,NMankind* pSetNMankind);
   virtual ~NEditDialog();

   void    disableInput();
   void    enableInput();

   static void    changeRelativePaths(Boolean set);
   static Boolean isRelativePaths();
   static void    changeFocusAfterClick(Boolean set);
   static Boolean isFocusAfterClick();
   static void changeMemoWordWrap(Boolean set);
   static Boolean isMemoWordWrap();

   static void changeFloat(Boolean set);
   Boolean isFloat();
protected:
   virtual Boolean closed(IFrameEvent& frameEvent);
   virtual Boolean saved(IFrameEvent& frameEvent);

   static IFont      ifProfileFont;
   static IRectangle irProfileRectangle;
   static Boolean    bProfileRelativePaths;
   static Boolean    bProfileFocusAfterClick;
   static Boolean    bProfileMemoWordWrap;
   static Boolean    bProfileFloat;
   static long       lProfileUserListMetric;
   static long       lProfileEventListMetric;
private:
   ITitle    itTitle;
   NManEdit* pNManEdit;
   Boolean bDisabled;
};

#endif
