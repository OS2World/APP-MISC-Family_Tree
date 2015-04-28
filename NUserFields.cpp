
/*
   File   : NUserFields.cpp
   Purpose: Define implementation of classes for user fields management
*/

#include "iradiobt.hpp"
#include "ipushbut.hpp"
#include "igroupbx.hpp"

#include "NUserFields.hpp"

unsigned long _System DosBeep(unsigned long,unsigned long);

// Class  : NQueryGenerations
// Purpose: query number of generations
class NQueryField : public IFrameWindow,
                    public ICommandHandler,
                    public IEditHandler
{
public:
   NQueryField(long windowId,IWindow* pOwner,const IString& isTitle,const IString& setFieldName=IString(),NMankind::FieldType setFieldType=NMankind::Unknown);
   IString             fieldName;
   NMankind::FieldType fieldType;
private:
   IMultiCellCanvas   imccCanvas;
   IGroupBox          igbName;
   IEntryField        iefName;
   IGroupBox          igbType;
   IRadioButton       irbYesNo;
   IRadioButton       irbText;
   IRadioButton       irbNumber;
   IRadioButton       irbDate;
   IPushButton        ipbOK,ipbCANCEL;
protected:
   virtual Boolean command(ICommandEvent& event);
   virtual Boolean edit(IControlEvent& event);
};

#define ID_CANVAS   1
#define ID_GNAME    2
#define ID_NAME     3
#define ID_GTYPE    4
#define ID_YESNO    5
#define ID_TEXT     6
#define ID_NUMBER   7
#define ID_DATE     8
#define ID_OKBUT    9
#define ID_CBUT    10

NQueryField::NQueryField(long windowId,IWindow* pOwner,const IString& isTitle,const IString& setFieldName,NMankind::FieldType setFieldType)
   : IFrameWindow(windowId
                 ,NULL,pOwner,IRectangle()
                 ,IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder
                 ,isTitle)
   , ICommandHandler()
   , IEditHandler()
   , imccCanvas (ID_CANVAS,this       ,this       )
   , igbName    (ID_GNAME ,&imccCanvas,&imccCanvas)
   , iefName    (ID_NAME  ,&imccCanvas,&imccCanvas)
   , igbType    (ID_GTYPE ,&imccCanvas,&imccCanvas)
   , irbYesNo   (ID_YESNO ,&imccCanvas,&imccCanvas)
   , irbText    (ID_TEXT  ,&imccCanvas,&imccCanvas)
   , irbNumber  (ID_NUMBER,&imccCanvas,&imccCanvas)
   , irbDate    (ID_DATE  ,&imccCanvas,&imccCanvas)
   , ipbOK      (ID_OKBUT ,&imccCanvas,&imccCanvas,IRectangle(),IPushButton::classDefaultStyle|IPushButton::defaultButton)
   , ipbCANCEL  (ID_CBUT  ,&imccCanvas,&imccCanvas)
{
   fieldName=setFieldName;
   fieldType=setFieldType;
   // Layout
   setFont(IFont("Helv",8));
   setClient(&imccCanvas);

   igbName   .setText(NApplication::pDLLResource->loadString(SI_UserFieldFieldName));
   iefName   .setText(fieldName);
   igbType   .setText(NApplication::pDLLResource->loadString(SI_UserFieldFieldType));
   irbYesNo  .setText("~"+NApplication::pDLLResource->loadString(SI_UserFieldYesNo));
   irbText   .setText("~"+NApplication::pDLLResource->loadString(SI_UserFieldText));
   irbNumber .setText("~"+NApplication::pDLLResource->loadString(SI_UserFieldNumber));
   irbDate   .setText("~"+NApplication::pDLLResource->loadString(SI_UserFieldDate));
   ipbOK     .setText(NApplication::pDLLResource->loadString(SI_OK));
   ipbCANCEL .setText(NApplication::pDLLResource->loadString(SI_Cancel));

   imccCanvas.setDefaultCell(ISize(7,7));
   imccCanvas.addToCell(&igbName    , 2, 2, 5, 4)
             .addToCell(&iefName    , 3, 4, 3   )
             .addToCell(&igbType    , 2, 6, 5, 7)
             .addToCell(&irbYesNo , 3, 8, 3   )
             .addToCell(&irbText    , 3, 9, 3   )
             .addToCell(&irbNumber  , 3,10, 3   )
             .addToCell(&irbDate    , 3,11, 3   )
             .addToCell(&ipbOK      , 3,14      )
             .addToCell(&ipbCANCEL  , 5,14      )
             .setRowHeight  (15,imccCanvas.defaultCell().height())
             .setColumnWidth(7,imccCanvas.defaultCell().width ());

   ISize size=imccCanvas.minimumSize();
   moveSizeToClient(IRectangle(IPoint(pOwner->rect().centerXCenterY()-size/2),size));
   // Handler
   ICommandHandler::handleEventsFor(this);
   IEditHandler::handleEventsFor(&iefName);
   // Prepare logic
   irbText.click();
   ipbOK.enable(fieldName.length()>0);
   if (fieldType==NMankind::Unknown)
      irbText.click();
   else {
      if (fieldType==NMankind::YesNo)
           irbYesNo .select();
      else irbYesNo .disable();
      if (fieldType==NMankind::Text )
           irbText  .select();
      else irbText  .disable();
      if (fieldType==NMankind::Number)
           irbNumber.select();
      else irbNumber.disable();
      if (fieldType==NMankind::Date)
           irbDate  .select();
      else irbDate  .disable();
      }
   // Done
   iefName.setFocus();
}

Boolean NQueryField::edit(IControlEvent& event)
{
   // Check for (empty) name field
   if (event.controlWindow()==&iefName) {
      ipbOK.enable(iefName.text().length()>0);
      }
   // Done
   return(False);
}

Boolean NQueryField::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      case ID_OKBUT :
         if (irbYesNo .isSelected()) fieldType=NMankind::YesNo;
         if (irbText  .isSelected()) fieldType=NMankind::Text;
         if (irbNumber.isSelected()) fieldType=NMankind::Number;
         if (irbDate  .isSelected()) fieldType=NMankind::Date;
         fieldName=iefName.text();
         dismiss(True);
         break;
      case ID_CBUT  :
         dismiss(False);
         break;
      }
   // Done
   return(False);
}


// Class  : NUserFieldList
// Purpose: Window class for showing user fields
class NUserFieldList : public NList,
                       public NMankindHandler
{
public:
   NUserFieldList(unsigned long windowId,IWindow* ownerparentWnd,NMankind* pSetNMankind);
   ~NUserFieldList();
   short getActualIdx();
protected:
   virtual short noOfRows();

   virtual Boolean  drawStart(short sStartRow);
   virtual void     drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before);
   virtual void     drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString);
   virtual void     drawEnd();

   virtual void     click(short sRow);

   void userFieldsModified();
private:
   NMankind* pNMankind;
   short     sActual;
};

NUserFieldList::NUserFieldList(unsigned long windowId,IWindow* ownerparentWnd,NMankind* pSetNMankind)
   : NList(windowId,ownerparentWnd)
   , NMankindHandler()
{
   // data setup
   setAutoDeleteObject(true);
   pNMankind=pSetNMankind;
   sActual=pNMankind->getNoOfUserFields();
   setEmptyText(NApplication::pDLLResource->loadString(SI_UserFieldNone));
   addColumn(NApplication::pDLLResource->loadString(SI_UserFieldFieldName));
   addColumn(NApplication::pDLLResource->loadString(SI_UserFieldFieldType));
   // handlers
   pNMankind->addMankindHandler(this);
   // done
}

NUserFieldList::~NUserFieldList()
{
   pNMankind->removeMankindHandler(this);
}


void NUserFieldList::userFieldsModified()
{
   if (sActual==0) sActual++;
   sActual=min(sActual,pNMankind->getNoOfUserFields());
   dataHasChanged();
}

short NUserFieldList::getActualIdx()
{
   return(sActual);
}

short NUserFieldList::noOfRows()
{
   return(pNMankind->getNoOfUserFields());
}

Boolean NUserFieldList::drawStart(short sStartRow)
{
   if (noOfRows()==0) return(False);
   return(True);
}

void NUserFieldList::drawRow(IGraphicContext& igcContext,const IRectangle& irRect,short sRow,Boolean before)
{
   // Prepare Row ?
   if ((before)&&(sRow==sActual)) {
      igcContext.setPenColor(IColor(IColor::paleGray));
      IGRectangle igr(irRect);
      igr.drawOn(igcContext);
      igcContext.setPenColor(IColor(IColor::black));
      }
   // Done
}


void NUserFieldList::drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString)
{
   // Calc Text
   IString text;
   if (sColumn==1)text=pNMankind->getUserFieldName(sRow);
   else switch (pNMankind->getUserFieldType(sRow)) {
      case NMankind::YesNo :
         text=NApplication::pDLLResource->loadString(SI_UserFieldYesNo);
         break;
      case NMankind::Text  :
         text=NApplication::pDLLResource->loadString(SI_UserFieldText);
         break;
      case NMankind::Number:
         text=NApplication::pDLLResource->loadString(SI_UserFieldNumber);
         break;
      case NMankind::Date  :
         text=NApplication::pDLLResource->loadString(SI_UserFieldDate);
         break;
      default :
         text="?";
      }
   // Draw Text
   igsString.setClippingRect(irRect)
            .setText(text)
            .moveTo(irRect.minXMinY()+IPoint(3,ifFont.maxDescender()))
            .drawOn(igcContext);
   // Done
}
void NUserFieldList::drawEnd()
{
}

void NUserFieldList::click(short sRow)
{
   if (sRow!=sActual) {
      short tmp=sActual;
      sActual=sRow;
      redrawRow(tmp);
      redrawRow(sActual);
      }
}


// Class  : NEditUserFields
// Purpose: Dialog Window Class for editing of user fields
NEditUserFields::NEditUserFields(unsigned long windowId,IWindow* pParentOwner,IFrameWindow* pSetFrame,NMankind* pSetNMankind)
   : IMultiCellCanvas(windowId,pParentOwner,pParentOwner)
   , ICommandHandler()
   , NMankindHandler()
   , ipbAdd        (2,this,this)
   , ipbChange     (3,this,this)
   , ipbDelete     (4,this,this)
{
   pNMankind=pSetNMankind;
   pFrame=pSetFrame;
   // Layout
   pNUserFieldList=new NUserFieldList(1,this,pNMankind);
   ipbAdd   .setText(NApplication::pDLLResource->loadString(SI_New));
   ipbChange.setText(NApplication::pDLLResource->loadString(SI_Change));
   ipbDelete.setText(NApplication::pDLLResource->loadString(SI_Delete));
   setDefaultCell(ISize(4,4));
   addToCell(pNUserFieldList,2,2,1,7);
   addToCell(&ipbAdd        ,4,3,1,1);
   addToCell(&ipbChange     ,4,5,1,1);
   addToCell(&ipbDelete     ,4,7,1,1);
   setColumnWidth(2,16,True);
   setRowHeight  (2,16,True);
   setColumnWidth(5,defaultCell().width ());
   setRowHeight  (9,defaultCell().height());
   // Handlers
   ICommandHandler::handleEventsFor(this);
   pNMankind->addMankindHandler(this);
   // Logic
   ipbChange.enable(pNMankind->getNoOfUserFields()>0);
   ipbDelete.enable(pNMankind->getNoOfUserFields()>0);
   // Done
}

NEditUserFields::~NEditUserFields()
{
   pNMankind->removeMankindHandler(this);
}

void NEditUserFields::userFieldsModified()
{
   // Modify buttons
   if (pNMankind->getNoOfUserFields()) {
      ipbChange.enable();
      ipbDelete.enable();
      }
   else {
      ipbChange.disable();
      ipbDelete.disable();
      }
   // Done
}


void NEditUserFields::disableInput() {}
void NEditUserFields::enableInput() {}


Boolean NEditUserFields::command(ICommandEvent& event)
{
   short idx=pNUserFieldList->getActualIdx();

   switch (event.commandId()) {
      // Add
      case 2 : {
         // .. registered ?
         if ((pNMankind->getNoOfUserFields()>=3)&&(NApplication::isRegisteredUser.size()==0)) {
            IMessageBox mbox(pFrame);
            mbox.show(NApplication::pDLLResource->loadString(SI_UserFieldMax3)
                     ,IMessageBox::warningIcon|IMessageBox::okButton|IMessageBox::systemModal);
            break;
            }
         // .. gather information and add
         NQueryField query(1
                          ,pFrame
                          ,NApplication::pDLLResource->loadString(SI_New));
         if (DID_OK==query.showModally()) {
            pNMankind->addUserField(query.fieldName,query.fieldType);
            }
         } break;
      // Change
      case 3 : {
         NQueryField query(1
                          ,pFrame
                          ,NApplication::pDLLResource->loadString(SI_Change)
                          ,pNMankind->getUserFieldName(idx)
                          ,pNMankind->getUserFieldType(idx) );
         if (DID_OK==query.showModally()) {
            pNMankind->changeUserField(idx,query.fieldName);
            }
         } break;
      // Remove
      case 4 : {
         IMessageBox msgBox(pFrame);
         if (IMessageBox::yes
             ==msgBox.show(NApplication::pDLLResource->loadString(SI_UserFieldDelete)
                           .change("$",pNMankind->getUserFieldName(idx))
                          ,IMessageBox::warningIcon|IMessageBox::yesNoButton|IMessageBox::moveable))
            pNMankind->delUserField(idx);
         }
      // No more
      }
   // Done
   return(False);
}

// Class  : NUserFields
// Purpose: Window Class for NPerson's user defined fiels

#define VMARGIN 3
#define HMARGIN 3

NUserFields::NUserFields(unsigned long windowId,IWindow* ownerparentWnd,NMankind* pSetNMankind,long colMetric)
   : NList(windowId,ownerparentWnd,False)
   , ICommandHandler()
   , NMankindHandler()
   , icCanvas(1,listWindow(),NULL,IRectangle(),IWindow::noStyle)
{
   // data
   pNMankind=pSetNMankind;
   setAutoDeleteObject(true);
   pMan=NULL;
   sActual=0;
   pInput=NULL;
   setBackgroundColor(IColor(IColor::paleGray));
   icCanvas.setFont(font());
   // layout
   setEmptyText(NApplication::pDLLResource->loadString(SI_UserFieldNone));
   addColumn(NApplication::pDLLResource->loadString(SI_UserFieldField),(colMetric>>16)&0xffff);
   addColumn(NApplication::pDLLResource->loadString(SI_UserFieldValue),(colMetric>> 0)&0xffff);
   // handlers
   ICommandHandler::handleEventsFor(this);
   pNMankind->addMankindHandler(this);
   // done
}

long NUserFields::getColMetric()
{
   return((columnWidth(1)<<16)+columnWidth(2));
}


NUserFields::~NUserFields()
{
   pNMankind->removeMankindHandler(this);
}

void NUserFields::disableInput()
{
   storeInfo();
   // Remove open input (changes should have been written earlier)
   if (pInput) {
      delete pInput;
      pInput=NULL;
      icCanvas.hide();
      }
   sActual=0;
   // Inherited
   NList::disableInput();
   // Done
}


void NUserFields::userFieldsModified()
{
   // Remove open input (changes should have been written earlier)
   if (pInput) {
      delete pInput;
      pInput=NULL;
      icCanvas.hide();
      }
   sActual=0;
   // redraw
   dataHasChanged();
}

void NUserFields::disconnentMen()
{
   // Remove open input (Sorry!)
   if (pInput) delete pInput;
   pInput=NULL;
   sActual=0;
   // no man any more
   pMan=NULL;
   // redraw
   dataHasChanged();
}

void NUserFields::doChangesNow()
{
   storeInfo();
   dataHasChanged();
}

void NUserFields::storeInfo()
{
   // Check for input
   if (!pInput) return;
   // Begin modifications
   pNMankind->beginChange();
   // .. find out which
   Boolean setFieldToNull=True;
   switch (pNMankind->getUserFieldType(sActual)) {
      case NMankind::YesNo : {
         // Calculate YesNo Control
         NYesNoControl* pYesNo=(NYesNoControl*)pInput;
         // Check if it has changed
         if (!pYesNo->hasChanged()) return;
         // Check if it has no information
         if (pYesNo->value()==NYesNoControl::Unknown) break;
         // Calc and store information
         NUserField::YesNo set;
         switch (pYesNo->value()) {
            case NYesNoControl::Yes    : set=NUserField::Yes    ;break;
            case NYesNoControl::No     : set=NUserField::No     ;break;
            }
         pMan->setUserField(new NUserFieldYesNo(set),sActual);
         setFieldToNull=False;
         break;
         // Done
         }
      case NMankind::Text : {
         // Calculate Text Control
         NTextEntry* pText=(NTextEntry*)pInput;
         // Check if it has changed
         if (!pText->hasChanged()) return;
         // Check if it has no information
         if (pText->text().length()==0) break;
         // Calc and store information
         pMan->setUserField(new NUserFieldText(pText->text()),sActual);
         setFieldToNull=False;
         break;
         // Done
         }
      case NMankind::Number : {
         // Calculate Text Control
         NNumEntry* pNum=(NNumEntry*)pInput;
         // Check if it has changed
         if (!pNum->hasChanged()) return;
         // Check if it has no information
         if (pNum->text().length()==0) break;
         // Calc and store information
         pMan->setUserField(new NUserFieldNumber(pNum->text().asInt()),sActual);
         setFieldToNull=False;
         break;
         // Done
         }
      case NMankind::Date : {
         // Calculate Date Control
         NDateControl* pDate=(NDateControl*)pInput;
         // Check if it has changed
         if (!pDate->hasChanged()) return;
         // Check if it has no information
         if (!pDate->getDate().hasInfo()) break;
         // Calc and store information
         pMan->setUserField(new NUserFieldDate(pDate->getDate()),sActual);
         setFieldToNull=False;
         break;
         // Done
         }
      }
   // UserField to Null because of no information
   if (setFieldToNull) pMan->setUserField(NULL,sActual);
   // Tell Mankind that we're ready
   pNMankind->endChange(this);
}

void NUserFields::loadInfo()
{
   if (!pInput) return;
   // Get pointer to user field data of current man
   NUserField* pField=pMan->getUserField(sActual);
   // Update input
   switch (pNMankind->getUserFieldType(sActual)) {
      case NMankind::YesNo : {
         NYesNoControl*   pYesNo    =(NYesNoControl  *)pInput;
         NUserFieldYesNo* pUserYesNo=(NUserFieldYesNo*)pField;
         NYesNoControl::Value set=NYesNoControl::Unknown;
         if (pUserYesNo) switch (pUserYesNo->getYesNo()) {
            case NUserField::Yes : set=NYesNoControl::Yes;break;
            case NUserField::No  : set=NYesNoControl::No;break;
            }
         pYesNo->setValue(set);
         pYesNo->hasChanged();
         } break;
      case NMankind::Text : {
         NTextEntry*     pText    =(NTextEntry    *)pInput;
         NUserFieldText* pUserText=(NUserFieldText*)pField;
         IString set;
         if (pUserText) set=pUserText->getText();
         pText->setText(set);
         pText->hasChanged();
         } break;
      case NMankind::Number : {
         NNumEntry*        pNumEntry  =(NNumEntry       *)pInput;
         NUserFieldNumber* pUserNumber=(NUserFieldNumber*)pField;
         IString set;
         if (pUserNumber) set=IString(pUserNumber->getNumber());
         pNumEntry->setText(set);
         pNumEntry->hasChanged();
         } break;
      case NMankind::Date : {
         NDateControl*   pDate    =(NDateControl  *)pInput;
         NUserFieldDate* pUserDate=(NUserFieldDate*)pField;
         NDate set;
         if (pUserDate) set=pUserDate->getDate();
         pDate->setDate(set);
         pDate->hasChanged();
         } break;
      }
}


void NUserFields::setMan(PNPerson pSetMan)
{
   // Try to store some data
   storeInfo();
   // Remember new man
   pMan=pSetMan;
   // load data
   loadInfo();
   // Done
   dataHasChanged();
}

short NUserFields::getActualIdx()
{
   return(sActual);
}

short NUserFields::rowHeight()
{
   return(VMARGIN+NList::rowHeight()+VMARGIN);
}

short NUserFields::noOfRows()
{
   if (!pMan) return(0);
   return(pNMankind->getNoOfUserFields());
}

Boolean NUserFields::drawStart(short sStartRow)
{
   if (!noOfRows()) return(False);
   return(True);
}

void NUserFields::drawRowCol(IGraphicContext& igcContext,const IRectangle& irRect,short sColumn,short sRow,const IFont& ifFont,IGString& igsString)
{
   // Column 1
   if (sColumn==1) {
      // Blue
      igcContext.setPenColor(IColor(IColor::darkBlue));
      // Draw Text
      igsString.setClippingRect(irRect)
               .setText(pNMankind->getUserFieldName(sRow))
               .moveTo(irRect.minXMinY()+IPoint(3,ifFont.maxDescender()+VMARGIN))
               .drawOn(igcContext);
      return;
      }
   // Column 2
   if (!pMan) return;
   // Black
   igcContext.setPenColor(IColor(IColor::black));
   // prepare text draw
   IPoint pos(irRect.minXMinY()+IPoint(3,ifFont.maxDescender()+VMARGIN));
   igsString.setClippingRect(irRect)
            .moveTo(pos);
   // calculate data
   NUserField* pField=pMan->getUserField(sRow);
   switch (pNMankind->getUserFieldType(sRow)) {
      // Text Field
      case NMankind::Text : {
         if (!pField) return;
         NUserFieldText* ptmp=(NUserFieldText*)pField;
         igsString.setText(ptmp->getText());
         } break;
      // Number Field
      case NMankind::Number : {
         if (!pField) return;
         NUserFieldNumber* ptmp=(NUserFieldNumber*)pField;
         igsString.setText(IString(ptmp->getNumber()));
         } break;
      // YesNo Field
      case NMankind::YesNo : {
         NUserField::YesNo value;
         igsString.setText(NApplication::pDLLResource->loadString(SI_Unknown));
         if (pField) {
            NUserFieldYesNo* ptmp=(NUserFieldYesNo*)pField;
            switch (ptmp->getYesNo()) {
               case NUserField::Yes :
                  igsString.setText(NApplication::pDLLResource->loadString(SI_Yes));break;
               case NUserField::No  :
                  igsString.setText(NApplication::pDLLResource->loadString(SI_No));break;
               }
            }
         } break;
      // Date Field
      case NMankind::Date : {
         NDate date;
         if (pField) {
            NUserFieldDate* ptmp=(NUserFieldDate*)pField;
            date=ptmp->getDate();
            }
         date.drawOn(igcContext,pos,ifFont,igsString,True);
         return;
         } break;
      // done
      }
   // draw
   igsString.drawOn(igcContext);
   // Done
}


void NUserFields::click(short sRow)
{
   // Try to store data
   storeInfo();
   // Close old input
   if (pInput) {
      delete pInput;
      icCanvas.hide();
      }
   sActual=sRow;
   // Create new input
   switch (pNMankind->getUserFieldType(sActual)) {
      case NMankind::YesNo :
         pInput=new NYesNoControl(1,&icCanvas,&icCanvas); break;
      case NMankind::Text  :
         pInput=new NTextEntry   (1,&icCanvas,&icCanvas); break;
      case NMankind::Number:
         pInput=new NNumEntry   (1,&icCanvas,&icCanvas); break;
      case NMankind::Date  :
         pInput=new NDateControl (1,&icCanvas,&icCanvas); break;
      }
   pInput->setAutoDestroyWindow(True);
   // .. load info to field
   loadInfo();
   // ... position and show
   positionsHaveChanged();
   pInput->enableTabStop()
          .setFocus();
   icCanvas.show();
   // Done
}

void NUserFields::positionsHaveChanged()
{
   // Do we have an input right now ?
   if (!pInput) return;
   // .. calculation
   ISize minSize=pInput->minimumSize();
   long  yoffset=(rowHeight()-minSize.height())/2,
         width  =max(columnWidth(2)-HMARGIN*2,minSize.width()),
         height =minSize.height(),
         xpos   =columnWidth(1)+HMARGIN-leftmostBit(),
         ypos   =listWindow()->rect().height()-rowHeight()*sActual
                 +yoffset+(topmostRow()-1)*rowHeight();
   // .. do it
   pInput->moveTo(IPoint(0    ,0     ))
          .sizeTo(ISize (width,height));
   icCanvas.moveTo(IPoint(xpos ,ypos  ))
           .sizeTo(ISize (width,height));
   // .. done
}

IPopUpMenu* NUserFields::rowPopup(short sRow)
{
   click(sRow);
   return(NULL);
}

Boolean NUserFields::command(ICommandEvent& event)
{
   // Done
   return(False);
}


