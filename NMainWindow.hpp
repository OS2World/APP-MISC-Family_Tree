/*
   File   : MainWindow.hpp
   Purpose: Define Classes for Interface of Application
*/

#ifndef _MAINWINDOW_HPP_
#define _MAINWINDOW_HPP_

#define WM_CLOSE 0x0029

// Include User Interface Class Library class headers
#include <iframe.hpp>
#include <icmdhdr.hpp>
#include <imenubar.hpp>
#include <isubmenu.hpp>
#include <ivport.hpp>
#include <ipopmenu.hpp>
#include <imenuhdr.hpp>
#include <imphdr.hpp>
#include <igrafctx.hpp>
#include <isizehdr.hpp>
#include <imcelcv.hpp>
#include <ifocshdr.hpp>
#include <islider.hpp>
#include <ifiledlg.hpp>
#include <iexcbase.hpp>
#include <iframhdr.hpp>
#include <ihelp.hpp>
#include <imsgbox.hpp>
#include <iobservr.hpp>
#include <inotify.hpp>
#include <inotifev.hpp>
#include <ititle.hpp>
#include <ithread.hpp>
#include <ireslock.hpp>
#include <iscroll.hpp>
#include <iaccel.hpp>
#include <iprofile.hpp>
#include <iflyhhdr.hpp>
#include <iflytext.hpp>

#include "NMisc.hpp"
#include "NMankind.hpp"
#include "NManTree.hpp"
#include "NListDialog.hpp"
#include "NEditDialog.hpp"
#include "NEventDialog.hpp"
#include "NAboutDialog.hpp"
#include "NOptionsDialog.hpp"
#include "NSearchDialog.hpp"

unsigned long _System DosBeep(unsigned long,unsigned long);

// Class  : NMainWindow
// Purpose: Main Window class for ftree.exe
class NMainWindow : public IFrameWindow,
                    public IFrameHandler,
                    public IMenuHandler,
                    public IMousePointerHandler,
                    public IEditHandler,
                    public NMankindHandler
{
public:
   static NMainWindow* pNMainWindow;
   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);

   NMainWindow();
   virtual ~NMainWindow();

   IFlyText            iFlyText;
   IFlyOverHelpHandler iFlyOverHelpHandler;

   NButtonBar  nbbButtonBar;
   NButton     nbOpen;
   NButton     nbSave;
   NButton     nbPrint;
   NButton     nbOptions;
   NButton     nbLocate;
   NButton     nbHoriVert;
   ISlider     isZoom;
   NButton     nbAncestors;
   NButton     nbDescendants;
   NButton     nbSiblings;
   NStatusLine nslMessage;
   NButton     nbTopBottom;

   NMankind mankind;
   NManTree mantree;

   IMenuBar imbMenuBar;
   ITitle itTitle;
   IHelpWindow ihwHelp;
   void threadFnSave();
   void threadFnLoad();
   void threadFnPrint();
   void printData();
   void newData();
   void saveDialog(const IString& isFile=IString());
   void loadDialog(const IString& isFile=IString());
   Boolean startSave(const IString& isFile);
   Boolean startLoad(const IString& isFile);
   void disableInput();
   void enableInput();
   void doneWithRexx();

   static void changeDoBackup(Boolean set);
   static Boolean isDoBackup();
   static void changeImportPreserve(Boolean set);
   static Boolean isImportPreserve();
   static void changeBorderlessButtons(Boolean set);
   static Boolean isBorderlessButtons();

   void fillManMenu (IMenu* ipmPopMenu);
   void fillViewMenu(IMenu* pMenu);

   virtual void originChanged (PNPerson pOld,PNPerson pNew);
   virtual void progressReport(const IString& isReport);

   virtual Boolean closed(IFrameEvent& frameEvent);
   virtual Boolean saved(IFrameEvent& frameEvent);
protected:
   virtual Boolean command(ICommandEvent& iceEvent);
   virtual Boolean edit(IControlEvent& ireEvent);
   virtual Boolean menuShowing(IMenuEvent& menuEvent,ISubmenu& submenuAboutToShow);
private:
   void addPartner(PNPerson pmanPartner=NULL);
   void addParents();
   void addChild(short int iPartner);
   void addRememberedChild(short int iPartner);
   void delPartner(short int iPartner);
   void delChilds(short int iPartner);
   void delParents();
   void delThis();
   void calcMessageText();
   void startRexx(unsigned number);
   IPoint ipStartDrag;
   void checkThreadTask();
   IThread threadLoadSavePrint;
   IAccelerator iaAccel;
   Boolean bDisabled;
   static IRectangle irProfileRectangle;
   static Boolean bProfileMaxed;
   static Boolean bProfileEdit;
   static Boolean bProfileList;
   static Boolean bProfileEvents;
   static Boolean bProfileButtonBarTop;
   static Boolean bProfileButtonBarVis;
   static Boolean bProfileDoBackup;
   static Boolean bProfileImportPreserve;
   static Boolean bProfileBorderlessButtons;
};

#endif
