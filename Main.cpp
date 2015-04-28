// Include User Interface Class Library class headers
#include <iapp.hpp>
#include <iprofile.hpp>
#include <locale.h>

#define INCL_DOSNLS

class NListDialog;
class NEditDialog;
class NOptionsDialog;

#include "NMainWindow.hpp"
#include "NRexx.hpp"
#include "FTree.h"
#include "Main.hpp"
#include "NPrint.hpp"

// Class  : NApplication
// Purpose: Global Application-Variables
IString              NApplication::isTree;
fstream              NApplication::fsFileStream;
IString              NApplication::isRegisteredUser;
IDynamicLinkLibrary* NApplication::pDLLResource;
NPrinter*            NApplication::pNPrinter;
NRexx                NApplication::nrRexx;
char                 NApplication::cDateSeparator='.';
char                 NApplication::cDateLayout='D';

void NApplication::initLocale()
{
   // Get Locale Information
   COUNTRYCODE country={0};  // Current Country
   COUNTRYINFO cinfo={0};
   ULONG actual;
   DosQueryCtryInfo(sizeof(cinfo),&country,&cinfo,&actual);
   NApplication::cDateSeparator=cinfo.szDateSeparator[0]; // . - / x
   switch (cinfo.fsDateFmt) {
      case 0: NApplication::cDateLayout='M';break;
      case 1: NApplication::cDateLayout='D';break;
      case 2: NApplication::cDateLayout='Y';break;
      }
   // Done
}
void NApplication::changeLocale(char dateSep,char dateFmt)
{
   // Separator
   switch (dateSep) {
      case '.': break;
      case '-': break;
      case '/': break;
      case 'x': break;
      default : dateSep='.';
      }
   NApplication::cDateSeparator=dateSep;
   // Format
   switch (dateFmt) {
      case 'M': break;
      case 'D': break;
      case 'Y': break;
      default : dateFmt='D';
      }
   NApplication::cDateLayout=dateFmt;
   // Done
}

// Function: Main
// Purpose : Start the whole thing
int main (int argc, char **argv)
{
cout << "1 " << flush;
   // Try Dll & Printer
   try {
      IApplication::current().setResourceLibrary(0);
      // Try to get DllResource
      NApplication::pDLLResource=new IDynamicLinkLibrary(DLLFILE);
      }
   catch(IException iex) {
      IMessageBox msgBox(NULL);
      msgBox.show("Sorry, please recheck your libpath for .; and run install.cmd !",IMessageBox::catastrophic);
      return(1);
      }
   NApplication::pNPrinter=new NPrinter;
cout << "2 " << flush;
   // Do MessageQueue
   IThread::current().initializeGUI(3000);
cout << "3 " << flush;
   // Init Local
   NApplication::initLocale();
   // Get Profile Data
   try {
      IProfile profile(PROFILE);
      // NMainWindow Params
      try { NMainWindow::loadFromProfile(profile); } catch (IException) {}
      // Mankind
      try { NMankind::loadFromProfile(profile);    } catch (IException) {}
      // ManTree
      try { NManTree::loadFromProfile(profile);    } catch (IException) {}
      // NListDialog Params
      try { NListDialog::loadFromProfile(profile); } catch (IException) {}
      // NEditDialog Params
      try { NEditDialog::loadFromProfile(profile); } catch (IException) {}
      // NEventDialog Params
      try { NEventDialog::loadFromProfile(profile); } catch (IException) {}
      // NRexx Params
      try { NApplication::nrRexx.loadFromProfile(profile); } catch (IException) {}
      // NOptions Params
      try { NOptionsDialog::loadFromProfile(profile); } catch (IException) {}
      // NSearch Params
      try { NSearchDialog::loadFromProfile(profile); } catch (IException) {}
      }
   catch (IException) {
      }
cout << "4 " << flush;
   // Main Window Loop
   try {
      // Go for window
      new NMainWindow();
      if (!NMainWindow::pNMainWindow)
         return(1);
cout << "5 " << flush;
      // Check for PreLoad
      if (argc>1) {
         IString isFile(argv[1]);
         IString isDir(argv[0]);isDir=isDir.subString(1,isDir.lastIndexOf('\\'));
         if (isFile.indexOf(':')==0) // No path information ?
            isFile=isDir+isFile;
         NApplication::isTree=isFile;
         NMainWindow::pNMainWindow->postEvent(IWindow::command,MI_FileOpenStartup);
         }
cout << "6 " << flush;
      // Run the stuff
      IApplication::current().run();
      }
   catch(IException& iex) {
      IMessageBox msgBox(IWindow::desktopWindow());
      msgBox.show(iex.text(),IMessageBox::enterButton);
      }
   // Save Window Params
   try {
      IProfile profile(PROFILE);
      // Mankind
      NMankind::saveToProfile(profile);
      // ManTree
      NManTree::saveToProfile(profile);
      // NMainWindow Params
      NMainWindow::saveToProfile(profile);
      // NListWindow Params
      NListDialog::saveToProfile(profile);
      // NOptionsDialog Params
      NOptionsDialog::saveToProfile(profile);
      // NEditWindow Params
      NEditDialog::saveToProfile(profile);
      // NEventWindow Params
      NEventDialog::saveToProfile(profile);
      // NRexx Params
      NSearchDialog::saveToProfile(profile);
      // NRexx Params
      NApplication::nrRexx.saveToProfile(profile);
      }
   catch (IException& exc) {
      IMessageBox mbox(NULL);
      mbox.show(NApplication::pDLLResource->loadString(SI_NoSaveProfile),IMessageBox::information);
      }
   // Done
   return(0);
} /* end main */
