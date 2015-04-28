// Class  : NSearchDialog
// Purpose: Dialog Window Class for Options Setting
#include "NSearchDialog.hpp"
#include "NEventDialog.hpp"
#include "NListDialog.hpp"
#include "NEditDialog.hpp"

#include <icombobx.hpp>


class NSearch : public IMultiCellCanvas,
                public ICommandHandler,
                public ISelectHandler
{
public:
   NSearch(unsigned long windowId,IWindow* pOwnerParent,IFrameWindow* pSetFrame);
protected:
   virtual Boolean command(ICommandEvent& event);
   virtual Boolean selected(IControlEvent& event);
private:
   IFrameWindow* pFrame;

   NNotebook nnbSearch;
   IMultiCellCanvas imccSearchFor;
   IRadioButton irbPersons,irbEvents;
   IComboBox icbYesNoIn;NYesNoControl nynYesNo;
   IComboBox icbTextIn;NTextEntry nteText;
   IRadioButton irbSubString,irbRegExp;
   IComboBox icbNumberIn;NNumEntry nneNumber;
   IRadioButton irbLess,irbEqual,irbMore;
   IComboBox icbDateIn;NDateControl ndcDate;
   NList nlFound;
   IPushButton  ipbStart,ipbStop,ipbClear,ipbDone;
};

#define ID_BOOK        9000
#define ID_PAGESEARCH  9010


#define ID_PERSONS     9011
#define ID_EVENTS      9012
#define ID_YESNOIN     9013
#define ID_YESNO       9014
#define ID_TEXTIN      9015
#define ID_TEXT        9016
#define ID_TEXTSUBSTR  9017
#define ID_TEXTREGEXP  9018

#define ID_NUMBERIN    9019
#define ID_NUMLESS     9020
#define ID_NUMEQUAL    9021
#define ID_NUMMORE     9022
#define ID_NUMBER      9023
#define ID_DATEIN      9024
#define ID_DATE        9025

#define ID_PAGEFOUND   9026
#define ID_START       9027
#define ID_STOP        9028
#define ID_CLEAR       9029
#define ID_DONE        9030

#define COMBOSTYLE IWindow::visible|IBaseComboBox::readOnlyDropDownType
#define RADIOSTYLE IWindow::visible|IButton::noPointerFocus
#define AUTORADIOSTYLE IWindow::visible|IButton::noPointerFocus|IRadioButton::autoSelect

NSearch::NSearch(unsigned long windowId,IWindow* pOwnerParent,IFrameWindow* pSetFrame)
   : IMultiCellCanvas(windowId,pOwnerParent,pOwnerParent)
   , ICommandHandler()
   , ISelectHandler()
   // Search Notebook
   , nnbSearch      (ID_BOOK ,this,this)
   // .. page 1
   , imccSearchFor  (ID_PAGESEARCH ,&nnbSearch,&nnbSearch)

   , irbPersons     (ID_PERSONS    ,&imccSearchFor,&imccSearchFor,IRectangle(),RADIOSTYLE|IControl::group)
   , irbEvents      (ID_EVENTS     ,&imccSearchFor,&imccSearchFor,IRectangle(),RADIOSTYLE)

   , icbYesNoIn     (ID_YESNOIN    ,&imccSearchFor,&imccSearchFor,IRectangle(),COMBOSTYLE)
   , nynYesNo       (ID_YESNO      ,&imccSearchFor,&imccSearchFor)

   , icbTextIn      (ID_TEXTIN     ,&imccSearchFor,&imccSearchFor,IRectangle(),COMBOSTYLE)
   , nteText        (ID_TEXT       ,&imccSearchFor,&imccSearchFor)
   , irbSubString   (ID_TEXTSUBSTR ,&imccSearchFor,&imccSearchFor,IRectangle(),AUTORADIOSTYLE|IControl::group)
   , irbRegExp      (ID_TEXTREGEXP ,&imccSearchFor,&imccSearchFor,IRectangle(),AUTORADIOSTYLE)

   , icbNumberIn    (ID_NUMBERIN   ,&imccSearchFor,&imccSearchFor,IRectangle(),COMBOSTYLE)
   , nneNumber      (ID_NUMBER     ,&imccSearchFor,&imccSearchFor)
   , irbLess        (ID_NUMLESS    ,&imccSearchFor,&imccSearchFor,IRectangle(),AUTORADIOSTYLE|IControl::group)
   , irbEqual       (ID_NUMEQUAL   ,&imccSearchFor,&imccSearchFor,IRectangle(),AUTORADIOSTYLE)
   , irbMore        (ID_NUMMORE    ,&imccSearchFor,&imccSearchFor,IRectangle(),AUTORADIOSTYLE)

   , icbDateIn      (ID_DATEIN     ,&imccSearchFor,&imccSearchFor,IRectangle(),COMBOSTYLE)
   , ndcDate        (ID_DATE       ,&imccSearchFor,&imccSearchFor)

   // .. page 2
   , nlFound        (ID_PAGEFOUND  ,&nnbSearch)
   // Search Footer
   , ipbStart  (ID_START,this,this)
   , ipbStop   (ID_STOP ,this,this)
   , ipbClear  (ID_CLEAR,this,this)
   , ipbDone   (ID_DONE ,this,this)
   // Done
{
   // data setup
   pFrame=pSetFrame;
   setFont(IFont("Helv",8));
   // Handlers
   ICommandHandler::handleEventsFor(this);
   ISelectHandler::handleEventsFor(&irbPersons)
                  .handleEventsFor(&irbEvents);
   // layout
   nnbSearch.setPageBackgroundColor    (backgroundColor())
            .setMajorTabBackgroundColor(backgroundColor());
   INotebook::PageSettings page(INotebook::PageSettings::autoPageSize
                               |INotebook::PageSettings::majorTab);
   page.setTabText("Search for");
   nnbSearch.addLastPage(page,&imccSearchFor);
   page.setTabText("");
   nnbSearch.addLastPage(page,&nlFound);

   imccSearchFor.setDefaultCell(ISize(3,6));
   imccSearchFor
                .addToCell(&irbPersons  ,2, 2,1,1)
                .addToCell(&irbEvents   ,4, 2,3,1)

                .addToCell(&icbYesNoIn  ,2, 5,1,1)
                .addToCell(&nynYesNo    ,4, 5,3,1)

                .addToCell(&icbTextIn   ,2, 7,1,1)
                .addToCell(&nteText     ,4, 7,3,1)
                .addToCell(&irbSubString,4, 9,2,1)
                .addToCell(&irbRegExp   ,6, 9,1,1)

                .addToCell(&icbNumberIn ,2,11,1,1)
                .addToCell(&nneNumber   ,4,11,2,1)
                .addToCell(&irbLess     ,4,13,1,1)
                .addToCell(&irbEqual    ,5,13,1,1)
                .addToCell(&irbMore     ,6,13,1,1)

                .addToCell(&icbDateIn   ,2,15,1,1)
                .addToCell(&ndcDate     ,4,15,3,1)

                .setRowHeight(16,imccSearchFor.defaultCell().height(),False)
                .setColumnWidth(7,imccSearchFor.defaultCell().width(),False);

   irbPersons .click().setText("Persons");
   irbEvents  .setText("Events");

   IColor comboColor(223,223,223);
   icbYesNoIn .setLimit(20).setBackgroundColor(comboColor);
   icbTextIn  .setLimit(20).setBackgroundColor(comboColor);
   icbNumberIn.setLimit(20).setBackgroundColor(comboColor);

   irbSubString.click().setText("Substring");
   irbRegExp   .setText("RegExp").disable();

   irbLess    .setText("<");
   irbEqual   .select().setText("=");
   irbMore    .setText(">");

   icbDateIn  .setMinimumRows(min(icbDateIn  .count(),10)+1).setLimit(15).setBackgroundColor(comboColor);

   nlFound.addColumn("Person")
          .addColumn("Field");

   ipbStart.enableDefault().setText("Start");
   ipbStop .setText("Stop ");
   ipbClear.setText("Clear");
   ipbDone .setText("Done ");

   setDefaultCell(ISize(3,2));
   addToCell(&nnbSearch, 1, 1, 8);
   addToCell(&ipbStart     , 1, 3, 1);
   addToCell(&ipbStop      , 3, 3, 1);
   addToCell(&ipbClear     , 5, 3, 1);
   addToCell(&ipbDone      , 7, 3, 1);
   setColumnWidth(8, 0,True);
   setRowHeight  (1,16,True);
   // done
}

Boolean NSearch::command(ICommandEvent& event)
{
   Boolean rc=False;
   switch (event.commandId()) {
      case ID_DONE :
         pFrame->close();
         rc=True;
         break;
      }
   return(rc);
}

Boolean NSearch::selected(IControlEvent& event)
{
   Boolean selectionsChanged=False;
   switch (event.controlId()) {
      case ID_PERSONS:
         if (irbPersons.isSelected()) break;
         irbPersons.select();
         irbEvents.select(False);
         // Found Tab Text Change
         nnbSearch.setTabText(nnbSearch.lastPage(),"Found Persons");
         // Selections in Combo
         icbYesNoIn .removeAll()
                    .addAsLast("<None>");
         icbYesNoIn .select(0);

         icbTextIn  .removeAll()
                    .addAsLast("<None>");
         icbTextIn  .addAsLast("Name");
         icbTextIn  .addAsLast("First name");
         icbTextIn  .addAsLast("Birth place");
         icbTextIn  .addAsLast("Death place");
         icbTextIn  .addAsLast("Marriage place");
         icbTextIn  .addAsLast("Occupation");
         icbTextIn  .addAsLast("Address");
         icbTextIn  .addAsLast("Memo");
         icbTextIn  .addAsLast("Picture");
         icbTextIn  .select(0);

         icbNumberIn.removeAll()
                    .addAsLast("<None>");
         icbNumberIn.addAsLast("Marriages");
         icbNumberIn.addAsLast("Children");
         icbNumberIn.addAsLast("Events involved in");
         icbNumberIn.select(0);

         icbDateIn  .removeAll()
                    .addAsLast("<None>");
         icbDateIn  .addAsLast("Birth date");
         icbDateIn  .addAsLast("Death date");
         icbDateIn  .addAsLast("Marriage date");
         icbDateIn  .addAsLast("Divorce date");
         icbDateIn  .select(0);
         // Done
         selectionsChanged=True;
         break;
      case ID_EVENTS:
         if (irbEvents.isSelected()) break;
         irbEvents.select();
         irbPersons.select(False);
         // Found Tab Text Change
         nnbSearch.setTabText(nnbSearch.lastPage(),"Found Events");
         // Selections in Combo
         icbYesNoIn .removeAll()
                    .addAsLast("<None>");
         icbYesNoIn .select(0);

         icbTextIn  .removeAll()
                    .addAsLast("<None>");
         icbTextIn  .addAsLast("Name");
         icbTextIn  .addAsLast("Place");
         icbTextIn  .addAsLast("Description");
         icbTextIn  .select(0);

         icbNumberIn.removeAll()
                    .addAsLast("<None>");
         icbNumberIn.addAsLast("Involved Persons");
         icbNumberIn.select(0);

         icbDateIn  .removeAll()
                    .addAsLast("<None>");
         icbDateIn  .addAsLast("Date from");
         icbDateIn  .addAsLast("Date to");
         icbDateIn  .select(0);
         // Done
         selectionsChanged=True;
         break;
      }
   // General Combo Settings
   if (selectionsChanged) {
      icbYesNoIn .setMinimumRows(min(icbYesNoIn .count(),10)+1);
      icbTextIn  .setMinimumRows(min(icbTextIn  .count(),10)+1);
      icbNumberIn.setMinimumRows(min(icbNumberIn.count(),10)+1);
      icbDateIn  .setMinimumRows(min(icbDateIn  .count(),10)+1);
      }
   // Done
   return(False);
}



// Class  : NSearchDialog
// Purpose: Dialog Window Class for Options Setting
NSearchDialog*  NSearchDialog::pNSearchDialog=NULL;
IFont           NSearchDialog::ifProfileFont("Helv",8);
IRectangle      NSearchDialog::irProfileRectangle;

void NSearchDialog::loadFromProfile(IProfile& profile)
{
   // Dialog options
   profile.setDefaultApplicationName("SEARCH");
   irProfileRectangle=IRectangle(profile.integerWithKey("MINX")
                                ,profile.integerWithKey("MINY")
                                ,profile.integerWithKey("MAXX")
                                ,profile.integerWithKey("MAXY"));
   ifProfileFont=IFont(profile.elementWithKey("FONTNAME")
                      ,profile.integerWithKey("FONTSIZE"));
   // Done
}
void NSearchDialog::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("SEARCH");
   profile.addOrReplaceElementWithKey("MINX",irProfileRectangle.minX());
   profile.addOrReplaceElementWithKey("MINY",irProfileRectangle.minY());
   profile.addOrReplaceElementWithKey("MAXX",irProfileRectangle.maxX());
   profile.addOrReplaceElementWithKey("MAXY",irProfileRectangle.maxY());
   profile.addOrReplaceElementWithKey("FONTNAME",ifProfileFont.name());
   profile.addOrReplaceElementWithKey("FONTSIZE",ifProfileFont.pointSize());
}

void NSearchDialog::generate(IWindow* piwOwner,NMankind* pSetNMankind)
{
   if (pNSearchDialog)
      pNSearchDialog->setFocus();
   else
      new NSearchDialog(piwOwner,pSetNMankind);
}


NSearchDialog::NSearchDialog(IWindow* piwOwner,NMankind* pSetNMankind)
   : IFrameWindow(IResourceId(ID_NSearchDialog)
                 ,IWindow::desktopWindow()
                 ,piwOwner
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::dialogBorder
                  |IFrameWindow::titleBar
                  |IFrameWindow::noMoveWithOwner
                 ,"Search")
   , IFrameHandler()
{
   // SetUp Data
   setIcon(RES_IconFTree);
   pNSearchDialog=this;
   bDisabled=False;
   pNMankind=pSetNMankind;
   setAutoDeleteObject(true);
   // Layout
   pNSearch = new NSearch(1,this,this);
   setClient(pNSearch);
   NMainWindow::pNMainWindow->ihwHelp.setAssociatedWindow(this);
   // SetUp Position & Size & Font
   setFont(ifProfileFont);
   ISize is=pNSearch->minimumSize();
   IPoint pos=irProfileRectangle.minXMinY();
   if (pos==IPoint(0,0)) pos=NMainWindow::pNMainWindow->rect().centerXCenterY()-is/2;
   moveSizeToClient(IRectangle(pos,is));
   // Handler
   IFrameWindow   ::removeDefaultHandler(); // Remove DefaultFrameHandler for my one
   IFrameHandler  ::handleEventsFor     (this);
   // Done
   show();
   setFocus();
}

NSearchDialog::~NSearchDialog()
{
   // Report destruction to ownerpointer
   pNSearchDialog=NULL;
}

void NSearchDialog::disableInput()
{
   bDisabled=True;
}
void NSearchDialog::enableInput()
{
   bDisabled=False;
}

Boolean NSearchDialog::saved(IFrameEvent& frameEvent)
{
   // Save Frame Status
   irProfileRectangle=rect();
   ifProfileFont=font();
   return(False);
}


