/*
   File   : AboutDialog.hpp
   Purpose: Define Classes for About Dialog
*/

#ifndef _ABOUT_HPP_
#define _ABOUT_HPP_

// Constants
#define DID_OK 1
#define DID_CANCEL 2

// ICLUI Includes
#include <iprofile.hpp>
#include <iframe.hpp>
#include <imcelcv.hpp>
#include <istattxt.hpp>
#include <icombobx.hpp>
#include <ipushbut.hpp>
#include <icmdhdr.hpp>
#include <iselhdr.hpp>
#include <ientryfd.hpp>
#include <icheckbx.hpp>
#include <islider.hpp>
#include <iedithdr.hpp>
#include <iiconctl.hpp>
#include <ikeyhdr.hpp>

// Class  : NAboutDialog
// Purpose: Dialog Window Class for About
/*
  +---------------------------------+  +---------------------------------+
  | Family Tree 1.0c                |  | Family Tree                     |
  | (C) 1996 Nils Meier             |  | (C) 1996 Nils Meier             |
  |                                 |  |                                 |
  | You can register here :         |  | This program is registered to : |
  |     Name [.................]    |  |   Name [MXXSTRICKLAND...]       |
  |     Code [.................]    |  |   Code [-------/--------]       |
  | Please read the online-help for |  |                                 |
  | information on how to register. |  |  ! Thank you for registering !  |
  |                                 |  |                                 |
  | [Register] [See you later]      |  | [Register] [See you later]      |
  +---------------------------------+  +---------------------------------+
*/
class NAboutDialog : public IFrameWindow,
                     public ICommandHandler,
                     public IKeyboardHandler
{
public:
   NAboutDialog(IWindow* pOwner);
   virtual ~NAboutDialog();
   IMultiCellCanvas imccCanvas;

   IStaticText istFTree;
   IIconControl iicFTree;
   IStaticText istCopyright;
   IStaticText istYouCan;
   IStaticText istUser;
   IEntryField iefUser;
   IStaticText istCode;
   IEntryField iefCode;
   IStaticText istPlease1;
   IStaticText istPlease2;

   IPushButton ipbRegister;
   IPushButton ipbSeeYa;
protected:
   Boolean command(ICommandEvent& event);
   Boolean virtualKeyPress(IKeyboardEvent& event);
private:
   static Boolean checkRegistration(const IString& isUser,const IString& isCode);
   void thanks(const IString& isSetUser);
};

#endif

