/*
   File   : NDragDrop.hpp
   Purpose: Definitions for own DragDrop support
*/

#include "FTree.h"
#include "NMankind.hpp"

#ifndef _DRAGDROP_HPP_
#define _DRAGDROP_HPP_

#include <idmprov.hpp>
#include <idmitem.hpp>
#include <idmevent.hpp>
#include <idmhndlr.hpp>

// Class  : NDragDropHandler
// Purpose: Provide direct manipulation
class NDragDropHandler : public IDMItemProvider {
public:
   enum Type { Man, Event, File };
   enum Op   { All, Copy, Move, Link };

   NDragDropHandler();
   void handleDragFor(IWindow* pwindow,Type setDragType);
   void handleDropFor(IWindow* pwindow,Type setDropType,const IString& isSetSuffix=IString(),Op setDropOp=Copy);

   virtual PNPerson   calcManToDrag(IPoint where);
   virtual PNEvent calcEventToDrag(IPoint where);
   virtual void   manDropped(PNPerson which);
   virtual void   eventDropped(PNEvent which);
   virtual void   fileDropped(IString isFile);

   // Internal methods
   virtual Boolean provideSourceItems(const IDMSourceOperation::Handle& sourceOperation);
   virtual IDMItem::Handle provideTargetItemFor(const IDMItem::Handle& dragItem);

   virtual Boolean provideLeaveSupport(IDMTargetLeaveEvent& event);
   virtual Boolean provideEnterSupport(IDMTargetEnterEvent& event);
private:
   Type dragType;
   Type dropType;
   Op   dropOp;
   IString isSuffix;
};

#endif
