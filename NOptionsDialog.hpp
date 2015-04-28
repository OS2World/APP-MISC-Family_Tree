/*
   File   : OptionsDialog.hpp
   Purpose: Define Classes for Options Interface
*/

#ifndef _OPTIONSDIALOG_HPP_
#define _OPTIONSDIALOG_HPP_

// Include User Interface Class Library class headers
#include <iframe.hpp>
#include <iframhdr.hpp>

unsigned long _System DosBeep(unsigned long,unsigned long);
unsigned long _System DosSleep(unsigned long);

class NOptionsWindow;

// Class  : NOptionsDialog
// Purpose: Dialog Window Class for Options Setting
class NOptionsDialog : public IFrameWindow,
                       public IFrameHandler
{
public:
   static NOptionsDialog* pNOptionsDialog;
   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);

   NOptionsDialog();
   virtual ~NOptionsDialog();
   void turnToPage(short page);
   void disableInput();
   void enableInput();
private:
   static IFont ifProfileFont;
   static IRectangle irProfileRectangle;

   NOptionsWindow* pOptions;
   Boolean bDisabled;

   virtual Boolean saved(IFrameEvent& frameEvent);
};


#endif
