/*
   File   : AboutDialog.cpp
   Purpose: Dialog for About information
*/

#include <imsgbox.hpp>

#include "NAboutDialog.hpp"
#include "FTree.h"
#include "Main.hpp"


// Class  : NAboutDialog
// Purpose: Dialog Window Class for AboutInfo
NAboutDialog::NAboutDialog(IWindow* pOwner)
   : IFrameWindow(IResourceId(ID_NAboutDialog)
                 ,IWindow::desktopWindow()
                 ,pOwner
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::noMoveWithOwner
                  |IFrameWindow::shellPosition
                 ,NApplication::pDLLResource->loadString(SI_AboutTitle))
   , ICommandHandler()
   , IKeyboardHandler()
   , imccCanvas(ID_NAboutDialog+1,this,this)

   , istFTree(ID_NAboutDialog+10,&imccCanvas,&imccCanvas)
   , iicFTree(ID_NAboutDialog+2,&imccCanvas,&imccCanvas,IResourceId(RES_IconFTree))

   , istCopyright(ID_NAboutDialog+20,&imccCanvas,&imccCanvas)

   , istYouCan(ID_NAboutDialog+30,&imccCanvas,&imccCanvas)

   , istUser(ID_NAboutDialog+40,&imccCanvas,&imccCanvas)
   , iefUser(ID_NAboutDialog+41,&imccCanvas,&imccCanvas)

   , istCode(ID_NAboutDialog+50,&imccCanvas,&imccCanvas)
   , iefCode(ID_NAboutDialog+51,&imccCanvas,&imccCanvas)

   , istPlease1(ID_NAboutDialog+60,&imccCanvas,&imccCanvas)

   , istPlease2(ID_NAboutDialog+70,&imccCanvas,&imccCanvas)

   , ipbRegister(ID_NAboutDialog+80,&imccCanvas,&imccCanvas,IRectangle()
             ,IWindow::visible|IPushButton::defaultButton)
   , ipbSeeYa(ID_NAboutDialog   +81,&imccCanvas,&imccCanvas)
{
   setFont(IFont("Helv",8));
   // 1st Row
   istFTree.setText(NApplication::pDLLResource->loadString(SI_AboutHead));
   // 2nd Row
   istCopyright.setText(NApplication::pDLLResource->loadString(SI_AboutCopyright));
   // 3nd Row
   istYouCan.setLimit(32)
            .setText(NApplication::pDLLResource->loadString(SI_AboutRegisterHere));
   // 4th Row
   istUser.setAlignment(IStaticText::centerRight)
          .setText("User ");
   iefUser.setLimit(16)
          .enableTabStop();
   // 5th Row
   istCode.setAlignment(IStaticText::centerRight)
          .setText("Code ");
   iefCode.setLimit(16)
          .enableTabStop();
   // 6th Row
   istPlease1.setAlignment(IStaticText::centerCenter)
             .setLimit(32)
             .setText(NApplication::pDLLResource->loadString(SI_AboutPlease1));
   // 7th Row
   istPlease2.setAlignment(IStaticText::centerCenter)
             .setLimit(32)
             .setText(NApplication::pDLLResource->loadString(SI_AboutPlease2));
   // 8th Row
   ipbRegister.setText(NApplication::pDLLResource->loadString(SI_AboutDoRegister))
              .enableTabStop();
   ipbSeeYa   .setText(NApplication::pDLLResource->loadString(SI_AboutLater));
   // Check for registerd
   if (NApplication::isRegisteredUser.size()>0)
      thanks(NApplication::isRegisteredUser);
   // Setup Layout
   setClient(&imccCanvas);

   imccCanvas.addToCell(&istFTree    ,2,2,3,1);
   imccCanvas.addToCell(&iicFTree    ,5,2,1,3);

   imccCanvas.addToCell(&istCopyright,2,3,3,1);

   imccCanvas.addToCell(&istYouCan   ,2,4,3,1);

   imccCanvas.addToCell(&istUser     ,2,6,1,1);
   imccCanvas.addToCell(&iefUser     ,3,6,2,1);

   imccCanvas.addToCell(&istCode     ,2,8,1,1);
   imccCanvas.addToCell(&iefCode     ,3,8,2,1);

   imccCanvas.addToCell(&istPlease1  ,2,10,4,1);

   imccCanvas.addToCell(&istPlease2  ,2,11,4,1);

   imccCanvas.addToCell(&ipbRegister ,2,13,2,1);
   imccCanvas.addToCell(&ipbSeeYa    ,4,13,2,1);

   imccCanvas.setColumnWidth( 2,0,True);
   imccCanvas.setColumnWidth( 6,imccCanvas.defaultCell().width() );
   imccCanvas.setRowHeight  (14,imccCanvas.defaultCell().height());
   // Done
   moveSizeToClient(IRectangle(IPoint(0,0),imccCanvas.minimumSize()));
   moveTo(pOwner->position()+pOwner->size()/2-size()/2);
   // Handler
   ICommandHandler::handleEventsFor(this);
   IKeyboardHandler::handleEventsFor(this);
}
NAboutDialog::~NAboutDialog() {}

void NAboutDialog::thanks(const IString& isSetUser)
{
   istYouCan.setText(NApplication::pDLLResource->loadString(SI_AboutIsRegTo));
   istPlease1.setText("");
   istPlease2.setText(NApplication::pDLLResource->loadString(SI_AboutThanks));
   iefUser.setText(isSetUser);
   iefCode.setText("*******/********");
   ipbRegister.disable();
}

Boolean NAboutDialog::virtualKeyPress(IKeyboardEvent& event)
{
   if (event.virtualKey()==IKeyboardEvent::esc)
      ipbSeeYa.click();
   return(False);
}

Boolean NAboutDialog::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      // Print Button
      case ID_NAboutDialog+80 :
         if (checkRegistration(iefUser.text(),iefCode.text())) {
            thanks(iefUser.text());
            NApplication::isRegisteredUser=iefUser.text();
            }
         else {
            IMessageBox msgBox(NULL);
            msgBox.show(NApplication::pDLLResource->loadString(SI_AboutVerify)
                       ,IMessageBox::information);
            }
         break;
      // Cancel Button
      case ID_NAboutDialog+81 :
         dismiss(DID_CANCEL);
         break;
      }
   // Done everything
   return(True);
}

Boolean NAboutDialog::checkRegistration(const IString& isUser,const IString& isCode)
{
   // Check for username & code ex.
   if ( (isUser.size()==0)
      ||(isUser.size()>16)
      ||(isCode.size()!=16))
      return(False);
   // Calc Code from user-field
   unsigned long check1=1234567;  // 7 Ziffern
   unsigned long check2=87654321; // 8 Ziffern

   IString isMod1("FAMILY");
   for (unsigned i=1;i<=isUser.size();i++)
      check1-=(isUser[i]*isMod1[(i%isMod1.size())+1]);
   IString isMod2("TREE");
   for (i=1;i<=isUser.size();i++)
      check2-=(isUser[i]*isMod2[(i%isMod2.size())+1]);
   // Check with entries
   if ( (isCode.subString(1,7)!=IString(check1))
      ||(isCode.subString(9,8)!=IString(check2))   )
      return(False);
   // O.K.
   return(True);
}

