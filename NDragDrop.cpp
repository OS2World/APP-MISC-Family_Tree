/*
   File   : NDragDrop.cpp
   Purpose: Definitions for own DragDrop support
*/

#include <idmimage.hpp>
#include <iostream.h>

#include "NDragDrop.hpp"

unsigned long _System DosBeep(unsigned long,unsigned long);

// Class  : NDragFileItem
// Purpose: Drag object for encapsulating file DragDrop
class NDragFileItem : public IDMItem {
public:
   NDragFileItem(const IDMItem::Handle &item,NDragDropHandler* pSetDest);
   Boolean targetDrop(IDMTargetDropEvent &event);
private:
   NDragDropHandler* pDestination;
};

NDragFileItem::NDragFileItem(const IDMItem::Handle &item,NDragDropHandler* pSetDest)
   : IDMItem(item)
{
   pDestination=pSetDest;
}
Boolean NDragFileItem::targetDrop(IDMTargetDropEvent &event)
{
   // Signal drop to DragDrop handler
   pDestination->fileDropped(containerName()+sourceName());
   // Done
   return(True);
}

// Class  : NDragManItem
// Purpose: Drag object for encapsulating man DragDrop
class NDragManItem : public IDMItem {
public:
   NDragManItem(IDMSourceOperation *pSourceOp,PNPerson pmanToDrag);
   NDragManItem(const IDMItem::Handle &item,NDragDropHandler* pSetDest);
   Boolean targetDrop(IDMTargetDropEvent &event);
private:
   NDragDropHandler* pDestination;
};

NDragManItem::NDragManItem(const IDMItem::Handle &item,NDragDropHandler* pSetDest)
   : IDMItem(item)
{
   pDestination=pSetDest;
}
NDragManItem::NDragManItem(IDMSourceOperation *pSourceOp,PNPerson pmanToDrag)
   : IDMItem(pSourceOp,"FTREE_NPerson",IDMItem::linkable|IDMItem::moveable,none)
{
   setObject(pmanToDrag);
   addRMF(rmfFrom(IDM::rmLibrary, IDMItem::rfForThisProcess()));
   //addRMF(rmfFrom(IDM::rmDiscard,IDM::rfUnknown));
   pSourceOp->setImageStyle(IDM::stack3AndFade);
   IDMImage img(IResourceId(RES_IconDragMan),True);
   setImage(img);
}
Boolean NDragManItem::targetDrop(IDMTargetDropEvent &event) {
   // Signal drop to DragDrop handler
   pDestination->manDropped((PNPerson)object());
   // Done
   return(True);
}

// Class  : NDragEventItem
// Purpose: Drag object for encapsulating event DragDrop
class NDragEventItem : public IDMItem {
public:
   NDragEventItem(IDMSourceOperation *pSourceOp,PNEvent peventToDrag);
   NDragEventItem(const IDMItem::Handle &item,NDragDropHandler* pSetDest);

   Boolean targetDrop(IDMTargetDropEvent &event);
private:
   NDragDropHandler* pDestination;
};
NDragEventItem::NDragEventItem(const IDMItem::Handle &item,NDragDropHandler* pSetDest)
   : IDMItem(item)
{
   pDestination=pSetDest;
}
NDragEventItem::NDragEventItem(IDMSourceOperation *pSourceOp,PNEvent peventToDrag)
   : IDMItem(pSourceOp,"FTREE_NEvent",IDMItem::linkable|IDMItem::moveable,none)
{
   setObject(peventToDrag);
   addRMF(rmfFrom(IDM::rmLibrary, IDMItem::rfForThisProcess()));
   addRMF(rmfFrom(IDM::rmDiscard,IDM::rfUnknown));
   pSourceOp->setImageStyle(IDM::stack3AndFade);
   IDMImage img(IResourceId(RES_IconDragEvent),True);
   setImage(img);
}
Boolean NDragEventItem::targetDrop(IDMTargetDropEvent &event) {
   // Signal drop to DragDrop handler
   pDestination->eventDropped((PNEvent)object());
   // Done
   return(True);
}


// Class  : NDragDropHandler
// Purpose: Provide direct manipulation

NDragDropHandler::NDragDropHandler()
   : IDMItemProvider()
{
   dragType=Man;
   dropType=Man;
}

void NDragDropHandler::handleDragFor(IWindow* pwindow,Type setDragType)
{
   dragType=setDragType;
   pwindow->setItemProvider(this);
   IDMHandler::enableDragFrom(pwindow);
}

void NDragDropHandler::handleDropFor(IWindow* pwindow,Type setDropType,const IString& isSetSuffix,Op setDropOp)
{
   isSuffix=isSetSuffix;
   dropType=setDropType;
   dropOp  =setDropOp;
   pwindow->setItemProvider(this);
   IDMHandler::enableDropOn(pwindow);
}

PNPerson NDragDropHandler::calcManToDrag(IPoint where)
{
   return(NULL);
}

PNEvent NDragDropHandler::calcEventToDrag(IPoint where)
{
   return(NULL);
}

void NDragDropHandler::manDropped(PNPerson which)
{
}

void NDragDropHandler::eventDropped(PNEvent which)
{
}

void NDragDropHandler::fileDropped(IString isFile)
{
}


Boolean NDragDropHandler::provideSourceItems(const IDMSourceOperation::Handle& sourceOperation)
{
   // Check for type of drag support
   switch (dragType) {
      // ... man drag
      case Man : {
         // Calculate NPerson object to be dragged
         PNPerson pmanToDrag=calcManToDrag(sourceOperation->position());
         if (!pmanToDrag) return(False);
         // Create ManDragItem to be dragged
         NDragManItem* pitem=new NDragManItem(sourceOperation,pmanToDrag);
         if (!pitem) return(False);
         sourceOperation->addItem(pitem);
         sourceOperation->setOperation(IDMOperation::link);
         break;
         }
      // event drag
      case Event : {
         // Calculate NEvent object to be dragged
         PNEvent peventToDrag=calcEventToDrag(sourceOperation->position());
         if (!peventToDrag) return(False);
         // Create EventDragItem to be dragged
         NDragEventItem* pitem=new NDragEventItem(sourceOperation,peventToDrag);
         if (!pitem) return(False);
         sourceOperation->addItem(pitem);
         sourceOperation->setOperation(IDMOperation::link);
         break;
         }
      // unknown
      default : return(False);
      }
   // Done
   return(True);
}

IDMItem::Handle NDragDropHandler::provideTargetItemFor(const IDMItem::Handle& dragItem)
{
   IDMItem::Handle handle;
   // Check for type of drop support
   switch (dropType) {
      // ... man drop
      case Man :
         handle=new NDragManItem(dragItem,this);
         // Set default link
         handle->targetOperation()->setOperation(IDMOperation::link);
         break;
      case Event :
         handle=new NDragEventItem(dragItem,this);
         // Set default link
         handle->targetOperation()->setOperation(IDMOperation::link);
         break;
      case File :
         handle=new NDragFileItem(dragItem,this);
         break;
      default :
         return(dragItem);
      }
   return(handle);
}

Boolean NDragDropHandler::provideLeaveSupport(IDMTargetLeaveEvent& event)
{
   return(False);
}
Boolean NDragDropHandler::provideEnterSupport(IDMTargetEnterEvent& event)
{
   IDMTargetOperation* pTargetOp=IDMTargetOperation::targetOperation();
   // Is target disabled ?
   if (!pTargetOp->targetWindow()->isEnabled()) {
      event.setDropIndicator(IDM::notOk);
      return(False);
      }
   // Check for type of drop support
   switch (dropType) {
      // ... man drop
      case Man :
         // Check if object is a family tree man
         if (!pTargetOp->item(1)->hasType("FTREE_NPerson")) break;
         // Check if we can change default drag to link
         if (pTargetOp->operation()==IDMOperation::drag)
            pTargetOp->setOperation(IDMOperation::link);
         // Check for actual link or (default) link
         if (pTargetOp->operation()!=IDMOperation::link) break;
         // Done
         return(True);
      // ... man drop
      case Event :
         // Check if object is a family tree man
         if (!pTargetOp->item(1)->hasType("FTREE_NEvent")) break;
         // Check if we can change default drag to link
         if (pTargetOp->operation()==IDMOperation::drag)
            pTargetOp->setOperation(IDMOperation::link);
         // Check for actual link or (default) link
         if (pTargetOp->operation()!=IDMOperation::link) break;
         // Done
         return(True);
      // ... file drop
      case File :
         // cout << pTargetOp->item(1)->rmfs() << "\n";
         // Check if object is a file
         if (pTargetOp->item(1)->nativeRM()!=(char*)IDM::rmFile)
            break;
         // Check for suffix .fam
         if ( (isSuffix.length()>0                                            )
            &&(!pTargetOp->item(1)->sourceName().lowerCase().includes(isSuffix)) )
            break;
         // Calc wanted operation
         long operation;
         switch (dropOp) {
            case Copy : operation=IDMOperation::copy;break;
            case Move : operation=IDMOperation::move;break;
            case Link : operation=IDMOperation::link;break;
            }
         // Check if we can change default drag to wanted op
         if (pTargetOp->operation()==IDMOperation::drag)
            pTargetOp->setOperation(operation);
         // Check for actual copy or (default) link
         if (pTargetOp->operation()!=operation) break;
         // Done
         return(True);
      // .. unknown
      }
   // No link -> OneWay!
   event.setDropIndicator(IDM::notOk);
   return(False);
}


