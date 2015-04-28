/*
   File   : OptionsDialog.cpp
   Purpose: Logic of Classes for Options Interface
*/

#include <ivport.hpp>
#include <ipopmenu.hpp>
#include <imphdr.hpp>
#include <imcelcv.hpp>
#include <ifocshdr.hpp>
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
#include "NOptionsDialog.hpp"
#include "NEventDialog.hpp"
#include "NListDialog.hpp"
#include "NEditDialog.hpp"

#include "ifontdlg.hpp"

// Class  : NOptionsWindow
// Purpose: Dialog Window Class for Options Setting
class NOptionsWindow : public NNotebook,
                       public IFocusHandler,
                       public ICommandHandler,
                       public ISelectHandler
{
public:
   NOptionsWindow(long windowId,IWindow* pParentOwner,IFrameWindow* pFrame);
   virtual ~NOptionsWindow();
   void NOptionsWindow::turnToPageNo(short page);
private:
   IMultiCellCanvas imccLayout;
   ISetCanvas    icLines;
   IRadioButton  irbDiagonal;
   IRadioButton  irbOrthogonal;
   ISetCanvas    icVisible;
   ICheckBox     icbFirstName;
   ICheckBox     icbDates;
   ICheckBox     icbPageBorder;
   ISetCanvas    icColors;
   IComboBox     icbColors;
   NColorControl nccColors;
   IPushButton   ipbColors;
   ISetCanvas    icFont;
   IPushButton   ipbFont;

   IMultiCellCanvas imccDataAssistant;
   ICheckBox icbAddPartnerSameAddress;
   ICheckBox icbAddPartnerOppositeSex;
   ICheckBox icbAddChildSameName;
   ICheckBox icbAddChildSameAddress;
   ICheckBox icbAddParentsMaleFemale;
   ICheckBox icbAddParentsSameName;
   ICheckBox icbAddParentsSameAddress;

   IMultiCellCanvas imccMiscellaneous;
   ICheckBox     icbActivatePersonFocusToEdit;
   ICheckBox     icbRelativePaths;
   ICheckBox     icbDoBackup;
   ICheckBox     icbImportPreserve;
   ICheckBox     icbFloatEdit;
   ICheckBox     icbFloatList;
   ICheckBox     icbFloatEvents;
   ICheckBox     icbMemoWordWrap;
   ICheckBox     icbBorderlessButtons;

   ISetCanvas    icDateFormat;
   IRadioButton  irbMMDDYYYY;
   IRadioButton  irbDDMMYYYY;
   IRadioButton  irbYYYYMMDD;
   IRadioButton  irbDot;
   IRadioButton  irbMinus;
   IRadioButton  irbSlash;
   IRadioButton  irbCross;

   NEditUserFields nEditUserFields;
protected:
   virtual Boolean lostFocus(IControlEvent& iceEvent);
   virtual Boolean command(ICommandEvent& event);
   virtual Boolean selected(IControlEvent& event);
};



// Class  : NOptionsWindow
// Purpose: Dialog Window Class for Options Setting
NOptionsWindow::NOptionsWindow(long windowId,IWindow* pParentOwner,IFrameWindow* pFrame)
   : NNotebook(windowId,pParentOwner,pParentOwner)
   , IFocusHandler()
   , ICommandHandler()
   , ISelectHandler()
   // Layout Page
   , imccLayout(10,this,this)
   , icLines      (11,&imccLayout,&imccLayout,IRectangle()
                  ,IWindow::visible
                   |ISetCanvas::centerVerticalAlign
                   |ISetCanvas::leftAlign
                   |ISetCanvas::verticalDecks)
   , irbDiagonal  (12,&icLines,&icLines,IRectangle(),IWindow::visible)
   , irbOrthogonal(13,&icLines,&icLines,IRectangle(),IWindow::visible)
   , icVisible    (14,&imccLayout,&imccLayout,IRectangle()
                  ,IWindow::visible
                   |ISetCanvas::centerVerticalAlign
                   |ISetCanvas::leftAlign
                   |ISetCanvas::verticalDecks)
   , icbFirstName (15,&icVisible,&icVisible)
   , icbDates     (16,&icVisible,&icVisible)
   , icbPageBorder(17,&icVisible,&icVisible)
   , icColors     (18,&imccLayout,&imccLayout,IRectangle()
                  ,IWindow::visible
                   |ISetCanvas::centerVerticalAlign
                   |ISetCanvas::leftAlign
                   |ISetCanvas::horizontalDecks
                   |ISetCanvas::decksByGroup)
   , icbColors    (19,&icColors,&icColors,IRectangle()
                  ,IWindow::visible
                   |IControl::group
                   |IComboBox::readOnlyDropDownType
                   |IComboBox::anyData)
   , nccColors    (20,&icColors)
   , ipbColors    (21,&icColors,&icColors,IRectangle()
                  ,IWindow::visible|IButton::noPointerFocus)
   , icFont       (22,&imccLayout,&imccLayout,IRectangle()
                  ,IWindow::visible
                   |ISetCanvas::centerVerticalAlign
                   |ISetCanvas::leftAlign
                   |ISetCanvas::verticalDecks)
   , ipbFont      (23,&icFont,&icFont,IRectangle()
                  ,IWindow::visible|IButton::noPointerFocus)
   // DataAssistant Page
   , imccDataAssistant(30,this,this)
   , icbAddPartnerSameAddress (31,&imccDataAssistant,&imccDataAssistant)
   , icbAddPartnerOppositeSex (32,&imccDataAssistant,&imccDataAssistant)
   , icbAddChildSameName      (33,&imccDataAssistant,&imccDataAssistant)
   , icbAddChildSameAddress   (34,&imccDataAssistant,&imccDataAssistant)
   , icbAddParentsMaleFemale  (35,&imccDataAssistant,&imccDataAssistant)
   , icbAddParentsSameName    (36,&imccDataAssistant,&imccDataAssistant)
   , icbAddParentsSameAddress (37,&imccDataAssistant,&imccDataAssistant)
   // Misc Page
   , imccMiscellaneous(50,this,this)
   , icbActivatePersonFocusToEdit(51,&imccMiscellaneous,&imccMiscellaneous)
   , icbRelativePaths            (52,&imccMiscellaneous,&imccMiscellaneous)
   , icbDoBackup                 (53,&imccMiscellaneous,&imccMiscellaneous)
   , icbImportPreserve           (54,&imccMiscellaneous,&imccMiscellaneous)
   , icbFloatEdit                (55,&imccMiscellaneous,&imccMiscellaneous)
   , icbFloatList                (56,&imccMiscellaneous,&imccMiscellaneous)
   , icbFloatEvents              (57,&imccMiscellaneous,&imccMiscellaneous)
   , icbMemoWordWrap             (58,&imccMiscellaneous,&imccMiscellaneous)
   , icbBorderlessButtons        (59,&imccMiscellaneous,&imccMiscellaneous)
   // Date Page
   , icDateFormat      (60,this,this,IRectangle()
                       ,IWindow::visible
                        |ISetCanvas::centerVerticalAlign
                        |ISetCanvas::leftAlign
                        |ISetCanvas::verticalDecks
                        |ISetCanvas::decksByGroup
                        |ISetCanvas::packEven)
   , irbMMDDYYYY       (61,&icDateFormat     ,&icDateFormat     )
   , irbDDMMYYYY       (62,&icDateFormat     ,&icDateFormat     )
   , irbYYYYMMDD       (63,&icDateFormat     ,&icDateFormat     )
   , irbDot            (64,&icDateFormat     ,&icDateFormat     )
   , irbMinus          (65,&icDateFormat     ,&icDateFormat     )
   , irbSlash          (66,&icDateFormat     ,&icDateFormat     )
   , irbCross          (67,&icDateFormat     ,&icDateFormat     )
   // UserFields Page
   , nEditUserFields(70,this,pFrame,NMankind::pNMankind)
{
   // Prepare for deletion
   setAutoDeleteObject(true);
   // Page: Layout
   icLines      .setPad(ISize()).setMargin(ISize(4,4))
                .setText(NApplication::pDLLResource->loadString(SI_OptionsLines));
   irbDiagonal  .select(NManTree::pNManTree->getLine()==NManTree::diagonal)
                .setText(NApplication::pDLLResource->loadString(SI_OptionsDiagonal));
   irbOrthogonal.select(NManTree::pNManTree->getLine()==NManTree::orthogonal)
                .setText(NApplication::pDLLResource->loadString(SI_OptionsOrthogonal));

   icVisible     .setPad(ISize()).setMargin(ISize(4,4))
                .setText(NApplication::pDLLResource->loadString(SI_OptionsDetail));
   icbFirstName .select ((NManTree::pNManTree->getDetail()&NManTree::firstName)!=0)
                .setText(NApplication::pDLLResource->loadString(SI_OptionsFirstNames));
   icbDates     .select ((NManTree::pNManTree->getDetail()&NManTree::dates)!=0)
                .setText(NApplication::pDLLResource->loadString(SI_OptionsDates));
   icbPageBorder.select ((NManTree::pNManTree->getDetail()&NManTree::pageBorder)!=0)
                .setText("Page border");

   icColors     .setPad(ISize(2,2)).setMargin(ISize(4,4))
                .setText(NApplication::pDLLResource->loadString(SI_OptionsColor));

   icbColors    .setItemHandle(icbColors.addAsLast(NApplication::pDLLResource->loadString(SI_OptionsCBackground))
                              ,NManTree::back    );
   icbColors    .setItemHandle(icbColors.addAsLast(NApplication::pDLLResource->loadString(SI_OptionsCLines))
                              ,NManTree::lines   );
   icbColors    .setItemHandle(icbColors.addAsLast(NApplication::pDLLResource->loadString(SI_OptionsCBoxes))
                              ,NManTree::boxes   );
   icbColors    .setItemHandle(icbColors.addAsLast(NApplication::pDLLResource->loadString(SI_OptionsCEmphasis))
                              ,NManTree::emphasis);
   icbColors    .setItemHandle(icbColors.addAsLast(NApplication::pDLLResource->loadString(SI_OptionsCTexts))
                              ,NManTree::texts   );
   icbColors    .select(0)
                .setMinimumRows(7);
   nccColors    .select(NManTree::pNManTree->getColor(NManTree::Color(icbColors.itemHandle(0))));
   ipbColors    .setText(NApplication::pDLLResource->loadString(SI_OptionsCDefault));

   icFont       .setPad(ISize()).setMargin(ISize(4,4))
                .setText(NApplication::pDLLResource->loadString(SI_OptionsFont));
   ipbFont      .setText(IString(NManTree::pNManTree->getFont().pointSize())
                        +"."+NManTree::pNManTree->getFont().name());

   imccLayout   .addToCell(&icLines       ,2,2);
   imccLayout   .addToCell(&icVisible     ,2,3);
   imccLayout   .addToCell(&icFont        ,2,4);
   imccLayout   .addToCell(&icColors      ,3,2,1,3);
   // Page: Data Assistant
   NMankind::Assist temp=NMankind::pNMankind->getAssist();
   icbAddPartnerSameAddress    .select((temp&NMankind::addPartnerSameAddress)!=0)
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsAddPartSA));
   icbAddPartnerOppositeSex    .select((temp&NMankind::addPartnerOppositeSex)!=0)
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsAddPartOS));
   icbAddChildSameName         .select((temp&NMankind::addChildSameName)!=0)
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsAddChilSN));
   icbAddChildSameAddress      .select((temp&NMankind::addChildSameAddress)!=0)
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsAddChilSA));
   icbAddParentsMaleFemale     .select((temp&NMankind::addParentsMaleFemale)!=0)
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsAddPareMF));
   icbAddParentsSameName       .select((temp&NMankind::addParentsSameName)!=0)
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsAddPareSN));
   icbAddParentsSameAddress    .select((temp&NMankind::addParentsSameAddress)!=0)
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsAddPareSA));

   imccDataAssistant.addToCell(&icbAddPartnerSameAddress    ,2,2)
                    .addToCell(&icbAddPartnerOppositeSex    ,2,3)
                    .addToCell(&icbAddChildSameName         ,2,4)
                    .addToCell(&icbAddChildSameAddress      ,2,5)
                    .addToCell(&icbAddParentsMaleFemale     ,2,6)
                    .addToCell(&icbAddParentsSameName       ,2,7)
                    .addToCell(&icbAddParentsSameAddress    ,2,8);
   // Page: Miscellaneous
   icbActivatePersonFocusToEdit.select(NEditDialog::pNEditDialog->isFocusAfterClick())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsFocusToEdit));
   icbRelativePaths            .select(NEditDialog::pNEditDialog->isRelativePaths())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsRelPath));
   icbDoBackup                 .select(NMainWindow::pNMainWindow->isDoBackup())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsDoBackup));
   icbImportPreserve           .select(NMainWindow::pNMainWindow->isImportPreserve())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsImportPreserve));
   icbFloatEdit                .select(NEditDialog::pNEditDialog->isFloat())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsFloatEdit));
   icbFloatList                .select(NListDialog::pNListDialog->isFloat())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsFloatList));
   icbFloatEvents              .select(NEventDialog::pNEventDialog->isFloat())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsFloatEvents));
   icbMemoWordWrap             .select(NEditDialog::pNEditDialog->isMemoWordWrap())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsMemoWordWrap));
   icbBorderlessButtons        .select(NMainWindow::pNMainWindow->isBorderlessButtons())
                               .setText(NApplication::pDLLResource->loadString(SI_OptionsBorderlessButtons));

   imccMiscellaneous.addToCell(&icbActivatePersonFocusToEdit,2,2,2,1)
                    .addToCell(&icbRelativePaths            ,2,3,2,1)
                    .addToCell(&icbDoBackup                 ,2,4,2,1)
                    .addToCell(&icbImportPreserve           ,2,5,2,1)
                    .addToCell(&icbFloatEdit                ,2,6,2,1)
                    .addToCell(&icbFloatList                ,2,7,2,1)
                    .addToCell(&icbFloatEvents              ,2,8,2,1)
                    .addToCell(&icbMemoWordWrap             ,2,9,2,1)
                    .addToCell(&icbBorderlessButtons        ,2,10,2,1);
   // Page: Date
   icDateFormat .setPad(ISize()).setMargin(ISize(4,4))
                .setText(NApplication::pDLLResource->loadString(SI_OptionsDateFmt));
   irbMMDDYYYY  .setText("MM DD YYYY").enableGroup();
   irbDDMMYYYY  .setText("DD MM YYYY");
   irbYYYYMMDD  .setText("YYYY MM DD");
   irbDot       .setText("  .  ").enableGroup();
   irbMinus     .setText("  -  ");
   irbSlash     .setText("  /  ");
   irbCross     .setText("  x  ");
   switch (NApplication::cDateSeparator) {
      case '-': irbMinus.select();break;
      case '/': irbSlash.select();break;
      case 'x': irbCross.select();break;
      default : irbDot  .select();break;
      }
   switch (NApplication::cDateLayout) {
      case 'M': irbMMDDYYYY.select();break;
      case 'Y': irbYYYYMMDD.select();break;
      default : irbDDMMYYYY.select();break;
      }
   // Page: UserFields
   // Notebook
   setPageBackgroundColor    (backgroundColor());
   setMajorTabBackgroundColor(backgroundColor());
   setMajorTabSize  (ISize(100,30));
   INotebook::PageSettings majPage(INotebook::PageSettings::autoPageSize
                                   |INotebook::PageSettings::majorTab
                                   |INotebook::PageSettings::statusTextOn);
   majPage.setTabText(NApplication::pDLLResource->loadString(SI_OptionsLayout));
   majPage.setStatusText(NApplication::pDLLResource->loadString(SI_OptionsProgram));
   addLastPage(majPage,&imccLayout);
   majPage.setTabText(NApplication::pDLLResource->loadString(SI_OptionsDataAssistant));
   addLastPage(majPage,&imccDataAssistant);
   majPage.setTabText(NApplication::pDLLResource->loadString(SI_OptionsMiscellaneous));
   addLastPage(majPage,&imccMiscellaneous);
   majPage.setTabText(NApplication::pDLLResource->loadString(SI_OptionsDateFormat));
   addLastPage(majPage,&icDateFormat);
   majPage.setTabText(NApplication::pDLLResource->loadString(SI_OptionsUserFields));
   majPage.setStatusText(NApplication::pDLLResource->loadString(SI_OptionsTree));
   addLastPage(majPage,&nEditUserFields);
   // Handler
   ICommandHandler::handleEventsFor (this);
   ICommandHandler ::handleEventsFor(&imccLayout);
   // Layout handler
   ISelectHandler ::handleEventsFor(&irbDiagonal);
   ISelectHandler ::handleEventsFor(&irbOrthogonal);
   ISelectHandler ::handleEventsFor(&icbFirstName);
   ISelectHandler ::handleEventsFor(&icbDates);
   ISelectHandler ::handleEventsFor(&icbPageBorder);
   // Assistant Handler
   ISelectHandler ::handleEventsFor(&icbAddPartnerSameAddress    );
   ISelectHandler ::handleEventsFor(&icbAddPartnerOppositeSex    );
   ISelectHandler ::handleEventsFor(&icbAddChildSameName         );
   ISelectHandler ::handleEventsFor(&icbAddChildSameAddress      );
   ISelectHandler ::handleEventsFor(&icbAddParentsMaleFemale     );
   ISelectHandler ::handleEventsFor(&icbAddParentsSameName       );
   ISelectHandler ::handleEventsFor(&icbAddParentsSameAddress    );
   // Miscellaneous
   ISelectHandler ::handleEventsFor(&irbMMDDYYYY                 );
   ISelectHandler ::handleEventsFor(&irbDDMMYYYY                 );
   ISelectHandler ::handleEventsFor(&irbYYYYMMDD                 );
   ISelectHandler ::handleEventsFor(&irbDot                      );
   ISelectHandler ::handleEventsFor(&irbMinus                    );
   ISelectHandler ::handleEventsFor(&irbSlash                    );
   ISelectHandler ::handleEventsFor(&irbCross                    );
   ISelectHandler ::handleEventsFor(&icbActivatePersonFocusToEdit);
   ISelectHandler ::handleEventsFor(&icbRelativePaths            );
   ISelectHandler ::handleEventsFor(&icbDoBackup                 );
   ISelectHandler ::handleEventsFor(&icbImportPreserve           );
   ISelectHandler ::handleEventsFor(&icbFloatEdit                );
   ISelectHandler ::handleEventsFor(&icbFloatList                );
   ISelectHandler ::handleEventsFor(&icbFloatEvents              );
   ISelectHandler ::handleEventsFor(&icbMemoWordWrap             );
   ISelectHandler ::handleEventsFor(&icbBorderlessButtons        );
   ISelectHandler ::handleEventsFor(&icbColors                   );
//   ICommandHandler ::handleEventsFor(&nccColors);
   // UserFields
   // Done
} /* end NOptionsWindow::NOptionsWindow */

NOptionsWindow::~NOptionsWindow()
{
}

Boolean NOptionsWindow::lostFocus(IControlEvent& iceEvent)
{
   return(False);
}

void NOptionsWindow::turnToPageNo(short page)
{
   // Check for page existing
   if (page==4) page++;
   if ((page<=0)||(page>totalPages())) return;
   // Go to page
   IPageHandle run=firstPage();
   for (short i=1;i<page;i++) {
      run=nextPage(run);
      }
   turnToPage(run);
   // Done
}

Boolean NOptionsWindow::command(ICommandEvent& event)
{
   // Color changed ?
   if (event.commandId()==nccColors.id()) {
      NManTree::pNManTree->changeColor(NManTree::Color(icbColors.itemHandle(icbColors.selection()))
                                    , IColor::Color(nccColors.getSelected()                    ));

      }
   // Color rest ?
   if (event.commandId()==ipbColors.id()) {
      NManTree::pNManTree->resetColors();
      icbColors.select(0);
      nccColors.select(NManTree::pNManTree->getColor(NManTree::Color(icbColors.itemHandle(0))));
      }
   // Font ?
   if (event.commandId()==ipbFont.id()) {
      IFont ifont=NManTree::pNManTree->getFont();
      IFontDialog::Settings ifds(&ifont);
      ifds.setTitle(NApplication::pDLLResource->loadString(SI_OptionsChooseFont));
      ifds.setPreviewText("FamilyTree "+NDate(1,3,1996).asString());
      IFontDialog ifd(0,this,IFontDialog::vectorOnly,ifds);
      if (ifd.pressedOK()) {
         NManTree::pNManTree->changeFont(ifont);
         ipbFont.setText(IString(NManTree::pNManTree->getFont().pointSize())
                        +"."+NManTree::pNManTree->getFont().name());
         }
      }
   // Done
   return(True);
}

Boolean NOptionsWindow::selected(IControlEvent& event)
{
   Boolean refresh=False;
   // Layout
   if ((event.controlWindow()==&irbDiagonal)&&(!irbDiagonal.isSelected())) {
      irbDiagonal.select();irbOrthogonal.deselect();
      NManTree::pNManTree->changeLine(NManTree::diagonal);
      refresh=True;
      }
   if ((event.controlWindow()==&irbOrthogonal)&&(!irbOrthogonal.isSelected())) {
      irbOrthogonal.select();irbDiagonal.deselect();
      NManTree::pNManTree->changeLine(NManTree::orthogonal);
      refresh=True;
      }
   if (event.controlWindow()==&icbFirstName) {
      NManTree::pNManTree->changeDetail(
            NManTree::Detail(icbFirstName.isSelected()?NManTree::firstName:0)
           ,NManTree::Detail(icbFirstName.isSelected()?0:NManTree::firstName)  );
      refresh=True;
      }
   if (event.controlWindow()==&icbDates) {
      NManTree::pNManTree->changeDetail(
            NManTree::Detail(icbDates.isSelected()?NManTree::dates:0)
           ,NManTree::Detail(icbDates.isSelected()?0:NManTree::dates)  );
      refresh=True;
      }
   if (event.controlWindow()==&icbPageBorder) {
      NManTree::pNManTree->changeDetail(
            NManTree::Detail(icbPageBorder.isSelected()?NManTree::pageBorder:0)
           ,NManTree::Detail(icbPageBorder.isSelected()?0:NManTree::pageBorder)  );
      refresh=True;
      }
   if (event.controlWindow()==&icbColors) {
      switch (icbColors.selection()) {
         case 0: nccColors.select(NManTree::pNManTree->getColor(NManTree::back));    break;
         case 1: nccColors.select(NManTree::pNManTree->getColor(NManTree::lines));   break;
         case 2: nccColors.select(NManTree::pNManTree->getColor(NManTree::boxes));   break;
         case 3: nccColors.select(NManTree::pNManTree->getColor(NManTree::emphasis));break;
         case 4: nccColors.select(NManTree::pNManTree->getColor(NManTree::texts));   break;
         }
      }
   // Data Assitant
   if (event.controlWindow()==&icbAddPartnerSameAddress)
      NMankind::pNMankind->changeAssist(
            NMankind::Assist(icbAddPartnerSameAddress.isSelected()?NMankind::addPartnerSameAddress:0)
           ,NMankind::Assist(icbAddPartnerSameAddress.isSelected()?0:NMankind::addPartnerSameAddress)  );
   if (event.controlWindow()==&icbAddPartnerOppositeSex)
      NMankind::pNMankind->changeAssist(
            NMankind::Assist(icbAddPartnerOppositeSex.isSelected()?NMankind::addPartnerOppositeSex:0)
           ,NMankind::Assist(icbAddPartnerOppositeSex.isSelected()?0:NMankind::addPartnerOppositeSex)  );
   if (event.controlWindow()==&icbAddChildSameName)
      NMankind::pNMankind->changeAssist(
            NMankind::Assist(icbAddChildSameName.isSelected()?NMankind::addChildSameName:0)
           ,NMankind::Assist(icbAddChildSameName.isSelected()?0:NMankind::addChildSameName)  );
   if (event.controlWindow()==&icbAddChildSameAddress)
      NMankind::pNMankind->changeAssist(
            NMankind::Assist(icbAddChildSameAddress.isSelected()?NMankind::addChildSameAddress:0)
           ,NMankind::Assist(icbAddChildSameAddress.isSelected()?0:NMankind::addChildSameAddress)  );
   if (event.controlWindow()==&icbAddParentsMaleFemale)
      NMankind::pNMankind->changeAssist(
            NMankind::Assist(icbAddParentsMaleFemale.isSelected()?NMankind::addParentsMaleFemale:0)
           ,NMankind::Assist(icbAddParentsMaleFemale.isSelected()?0:NMankind::addParentsMaleFemale)  );
   if (event.controlWindow()==&icbAddParentsSameName)
      NMankind::pNMankind->changeAssist(
            NMankind::Assist(icbAddParentsSameName.isSelected()?NMankind::addParentsSameName:0)
           ,NMankind::Assist(icbAddParentsSameName.isSelected()?0:NMankind::addParentsSameName)  );
   if (event.controlWindow()==&icbAddParentsSameAddress)
      NMankind::pNMankind->changeAssist(
            NMankind::Assist(icbAddParentsSameAddress.isSelected()?NMankind::addParentsSameAddress:0)
           ,NMankind::Assist(icbAddParentsSameAddress.isSelected()?0:NMankind::addParentsSameAddress)  );
   // Miscellaneous
   if (event.controlWindow()==&irbMMDDYYYY) {
      NApplication::cDateLayout='M';
      refresh=True;
      }
   if (event.controlWindow()==&irbDDMMYYYY) {
      NApplication::cDateLayout='D';
      refresh=True;
      }
   if (event.controlWindow()==&irbYYYYMMDD) {
      NApplication::cDateLayout='Y';
      refresh=True;
      }
   if (event.controlWindow()==&irbDot     ) {
      NApplication::cDateSeparator='.';
      refresh=True;
      }
   if (event.controlWindow()==&irbMinus   ) {
      NApplication::cDateSeparator='-';
      refresh=True;
      }
   if (event.controlWindow()==&irbSlash   ) {
      NApplication::cDateSeparator='/';
      refresh=True;
      }
   if (event.controlWindow()==&irbCross   ) {
      NApplication::cDateSeparator='x';
      refresh=True;
      }
   if (event.controlWindow()==&icbActivatePersonFocusToEdit)
      NEditDialog::pNEditDialog->changeFocusAfterClick(icbActivatePersonFocusToEdit.isSelected());
   if (event.controlWindow()==&icbRelativePaths) {
      NEditDialog::pNEditDialog->changeRelativePaths(icbRelativePaths.isSelected());
      }
   if (event.controlWindow()==&icbDoBackup)
      NMainWindow::changeDoBackup(icbDoBackup.isSelected());
   if (event.controlWindow()==&icbImportPreserve)
      NMainWindow::changeImportPreserve(icbImportPreserve.isSelected());
   if (event.controlWindow()==&icbFloatEdit)
      NEditDialog::changeFloat(icbFloatEdit.isSelected());
   if (event.controlWindow()==&icbFloatList)
      NListDialog::changeFloat(icbFloatList.isSelected());
   if (event.controlWindow()==&icbFloatEvents)
      NEventDialog::changeFloat(icbFloatEvents.isSelected());
   if (event.controlWindow()==&icbMemoWordWrap)
      NEditDialog::changeMemoWordWrap(icbMemoWordWrap.isSelected());
   if (event.controlWindow()==&icbBorderlessButtons)
      NMainWindow::changeBorderlessButtons(icbBorderlessButtons.isSelected());
   // Done
   if (refresh) {
      NManTree::pNManTree->refresh();
      if (NEditDialog::pNEditDialog) NEditDialog::pNEditDialog->refresh();
      }
   return(False);
}

//--------------------------------------------------------------------------------------


// Class  : NOptionsWindow
// Purpose: Dialog Window Class for Options Setting
NOptionsDialog* NOptionsDialog::pNOptionsDialog;
IFont           NOptionsDialog::ifProfileFont=IFont("Helv",8);
IRectangle      NOptionsDialog::irProfileRectangle;

void NOptionsDialog::loadFromProfile(IProfile& profile)
{
   // Dialog options
   profile.setDefaultApplicationName("OPTIONS");
   irProfileRectangle=IRectangle(profile.integerWithKey("MINX")
                                ,profile.integerWithKey("MINY")
                                ,profile.integerWithKey("MAXX")
                                ,profile.integerWithKey("MAXY"));
   ifProfileFont=IFont(profile.elementWithKey("FONTNAME")
                      ,profile.integerWithKey("FONTSIZE"));
   // Done
}
void NOptionsDialog::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("OPTIONS");
   profile.addOrReplaceElementWithKey("MINX",irProfileRectangle.minX());
   profile.addOrReplaceElementWithKey("MINY",irProfileRectangle.minY());
   profile.addOrReplaceElementWithKey("MAXX",irProfileRectangle.maxX());
   profile.addOrReplaceElementWithKey("MAXY",irProfileRectangle.maxY());
   profile.addOrReplaceElementWithKey("FONTNAME",ifProfileFont.name());
   profile.addOrReplaceElementWithKey("FONTSIZE",ifProfileFont.pointSize());
}

NOptionsDialog::NOptionsDialog()
   : IFrameWindow(IResourceId(ID_NOptionsDialog)
                 ,IWindow::desktopWindow()
                 ,NMainWindow::pNMainWindow
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::sizingBorder
                  |IFrameWindow::titleBar
                  |IFrameWindow::noMoveWithOwner
                 ,NApplication::pDLLResource->loadString(SI_OptionsTitle))
   , IFrameHandler()
{
   // SetUp Data
   setIcon(RES_IconFTree);
   pNOptionsDialog=this;
   bDisabled=False;
   // Client creation
   pOptions=new NOptionsWindow(1,this,this);
   setClient(pOptions);
   NMainWindow::pNMainWindow->ihwHelp.setAssociatedWindow(this);
   // Prepare for deletion
   setAutoDeleteObject(true);
   // SetUp Position & Size & Font
   if (  (irProfileRectangle.width()>0)
      && (irProfileRectangle.height()>0) )
      moveSizeTo(irProfileRectangle);
   else {
      ISize is=pOptions->minimumSize();
      moveSizeToClient(IRectangle(NMainWindow::pNMainWindow->rect().centerXCenterY()-is/2
                                 ,is));
      }
   setFont(ifProfileFont);
   // Handler
   IFrameWindow   ::removeDefaultHandler(); // Remove DefaultFrameHandler for my one
   IFrameHandler  ::handleEventsFor     (this);
   // Done
} /* end NOptionsDialog::NOptionsDialog */

NOptionsDialog::~NOptionsDialog()
{
   // Report destruction to ownerpointer
   pNOptionsDialog=NULL;
}

Boolean NOptionsDialog::saved(IFrameEvent& frameEvent)
{
   // Save Frame Status
   irProfileRectangle=rect();
   ifProfileFont=font();
   return(False);
}

void NOptionsDialog::turnToPage(short page)
{
cout << "Turn to " << page << "," << flush;
   pOptions->turnToPageNo(page);
}

void NOptionsDialog::disableInput()
{
   bDisabled=True;
}
void NOptionsDialog::enableInput()
{
   bDisabled=False;
}

