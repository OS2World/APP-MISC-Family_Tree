/*
   File   : Rexx.hpp
   Purpose: Handle Rexx Calls
*/

#ifndef _REXX_HPP_
#define _REXX_HPP_

#define DID_OK 1
#define DID_CANCEL 2
#define NO_ERROR 0

#define MAX_REMEMBER 16

// ICLUI Includes
#include <iprofile.hpp>
#include <iframe.hpp>
#include <imcelcv.hpp>
#include <istattxt.hpp>
#include <ipushbut.hpp>
#include <icmdhdr.hpp>
#include <iselhdr.hpp>
#include <ientryfd.hpp>
#include <icheckbx.hpp>
#include <islider.hpp>
#include <iedithdr.hpp>
#include <ikeyhdr.hpp>
#include <imle.hpp>
#include <ithread.hpp>
#include <ititle.hpp>
#include <iframhdr.hpp>
#include <ilistbox.hpp>
#include <icombobx.hpp>

#ifdef INCL_INTERNAL

#include "NMankind.hpp"
#include "NMisc.hpp"

class NRexx;
class NRexxDialog;

// Class   : NRexxOutput
// Purpose : Output of Rexx Run
/*
 +-Run Rexx----------------+
 |+-----------------------+|
 ||                       ||
 |+-----------------------+|
 |[OK][Cancel][Save][Print]|
 +-------------------------+
*/
class NRexxOutput : public IFrameWindow,
                    public ICommandHandler,
                    public IKeyboardHandler,
                    public IFrameHandler
{
public:
   NRexxOutput(IWindow* pOwner,IString& isSetScript,IString& isSetName);
   virtual ~NRexxOutput();
private:
   ITitle itTitle;
   IMultiCellCanvas imccCanvas;
   IListBox ilbOutput;
   IPushButton ipbDone;
   IPushButton ipbCancel;
   IPushButton ipbSave;
   IPushButton ipbPrint;
   IPushButton ipbCopy;

   IString isScript;
   void startRexxing();
   void rexxing();
   void reading();
   IThread threadRexx;
   IThread threadRead;

   HFILE hfRead,hfWrite,hfSTDOUT,hfSTDERR;
protected:
   Boolean command(ICommandEvent& event);
   Boolean virtualKeyPress(IKeyboardEvent& event);
   Boolean NRexxOutput::closed(IFrameEvent& frameEvent);
};


// Class   : NRexxDialog
// Purpose : User Interface for NRexx
/*
 +-Select Rexx-------------+
 | [Name...............][V]|
 | Name:   Anniversary...  |
 | Author: Nils Meier      |
 | Note:   Statistic about |
 |         lifetime of all |
 |         dead persons in |
 |         tree.           |
 | [Run][Cancel][Remember] |
 +-------------------------+
*/
class NRexxDialog : public IFrameWindow,
                    public ICommandHandler,
                    public ISelectHandler,
                    public IKeyboardHandler
{
public:
   NRexxDialog(IWindow* pOwner);
   virtual ~NRexxDialog();
   void findScripts();
private:
   IMultiCellCanvas imccCanvas;
   IStaticText istScript;
   IComboBox   icbScript;
   IStaticText istName;
   IEntryField iefName;
   IStaticText istAuthor;
   IEntryField iefAuthor;
   IStaticText istNote;
   IMultiLineEdit imleNote;
   IPushButton ipbRun;
   IPushButton ipbRemember;
   IPushButton ipbCancel;
protected:
   Boolean command(ICommandEvent& event);
   Boolean selected(IControlEvent& event);
   Boolean virtualKeyPress(IKeyboardEvent& event);
};

#endif

// Class   : NRexx
// Purpose : Enable Rexx Support
class NRexx {
   friend class NRexxOutput;
public:
   NRexx();
   ~NRexx();
   void loadFromProfile(IProfile& profile);
   void saveToProfile(IProfile& profile);
   Boolean startRexx(IWindow *pOwner,unsigned number=0);
   Boolean doYouRemember(const IString& isFtx);
   unsigned getRememberCount();
   const IString& getRemembered(unsigned number,Boolean truename=False);
   void remember(const IString& isFtx,const IString& isName);
   void setScript(const IString& isSetFtx,const IString& isSetName);
   static NRexx* pNRexx;
private:
   unsigned rememberCount;
   IString *pisRememberFtx[MAX_REMEMBER];
   IString *pisRememberName[MAX_REMEMBER];
   IString isFtx,
           isName;
   static IFont ifProfileFont;
   static IRectangle irProfileRectangle;
};

#endif
