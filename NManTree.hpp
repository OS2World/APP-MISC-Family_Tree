/*
   File   : ManTree.hpp
   Purpose: Define Classes for showin persons in a Tree
*/

#ifndef _NPersonTREE_HPP
#define _NPersonTREE_HPP

#include <iprofile.hpp>
#include <ireslib.hpp>
#include <icolor.hpp>
#include <ifont.hpp>
#include <ivport.hpp>
#include <istattxt.hpp>
#include <isizehdr.hpp>
#include <ipainhdr.hpp>
#include <imenuhdr.hpp>
#include <imphdr.hpp>
#include <ireslock.hpp>

//#define ZOOMSTEPS 8

#include "NDragDrop.hpp"
#include "NMankind.hpp"

typedef struct {
   IPoint pos;
   PNPerson   pman;
} NPersonINSTANCE;

typedef NPersonINSTANCE* PNPersonINSTANCE;

typedef struct {
   PNPersonINSTANCE pmaninstance[1];
} RELINSTANCE;

typedef RELINSTANCE* PRELINSTANCE;

// class  : NManTree
// purpose: Project Mankind on a flat 2-dimensional field
class NManTree : public IViewPort,
                 public IPaintHandler,
                 public IResizeHandler,
                 public NMankindHandler,
                 public IMenuHandler,
                 public IMousePointerHandler,
                 public NDragDropHandler
{
public:
   NManTree(unsigned long windowId,IWindow* pParentOwner,PNMankind pSetNMankind);
   ~NManTree();
   static NManTree* pNManTree;
   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);
   void disableInput();
   void enableInput();

   const IString& getLastFileDrop();
   const ISize& getTreeSize();
   void         printOn(IGraphicContext& igcToPrintOn
                       ,const IRectangle& irPrintRectangle
                       ,const double zoomX, const double zoomY);

   virtual void collectParm(IString& isParm);
   virtual void spreadParm(const IString& isParm);
   virtual void structureChanged();
   virtual void manChanged      (NMankindHandler* pInitiator);
   virtual void manAdded        (PNPerson which,Boolean more);
   virtual void manAdded        (SEQOFNPerson& which);
   virtual void disconnentMen   ();
   virtual void actualChanged   (PNPerson pOld,PNPerson pNew);
   virtual void originChanged   (PNPerson pOld,PNPerson pNew);

   virtual PNPerson calcManToDrag(IPoint where);
   virtual void fileDropped(IString isFile);

   enum Depth  { ancestors,descendants,siblings,totalancestors,totaldescendants };
   enum Dir    { horizontal,vertical                     };
   enum Line   { diagonal,orthogonal                     };
   enum Detail { none=0,firstName=1,dates=2,pageBorder=4 };
   enum Color  { back,lines,boxes,emphasis,texts         };

          void           changeZoom  (const double xySetZoom);
   static Boolean        changeDepth (Depth which,unsigned short depth   );
   static Boolean        changeDir   (Dir  setDir                        );
   static Boolean        changeLine  (Line setLine                       );
   static Boolean        changeDetail(Detail setDetail,Detail delDetail  );
   static Boolean        changeFont  (const IFont& setFont               );
   static Boolean        changeColor (Color which,IColor::Color setColor );
   static Boolean        resetColors ();
          double         getZoom();
   static unsigned short getDepth    (Depth which);
   static Dir            getDir      ();
   static Line           getLine     ();
   static Detail         getDetail   ();
   static IFont&         getFont     ();
   static IColor::Color  getColor    (Color which);

private:
   static Detail         detail          ;
   static Line           line            ;
   static Dir            dir             ;
   static unsigned short depthAncestors  ;
   static unsigned short depthDescendants;
   static unsigned short depthSiblings   ;
   static unsigned short totalAncestors  ;
   static unsigned short totalDescendants;
   static IFont          ifont           ;
   static IColor::Color  colorback       ;
   static IColor::Color  colorlines      ;
   static IColor::Color  colorboxes      ;
   static IColor::Color  coloremphasis   ;
   static IColor::Color  colortexts      ;

   IPrivateResource* pLockInstances;
   ICanvas          drawingArea;
   PNMankind        pNMankind;
   PNPersonINSTANCE     pManInstances;
   unsigned short   numberOfManInstances;
   PRELINSTANCE     pRelInstances;
   unsigned short   numberOfRelInstances;
   long             fontLineHeight;
   long             fontBaseOffset;
   long             manBoxHeight;
   long             manBoxWidth;
   long             distGenerations;
   long             distSiblings;
   long             distPartners;
   long             distAddPartners;
   Boolean          isHori;
   ISize            isTree;
   IPoint           ipTranslate;
   double           xyZoom;
   Boolean          bDisabled;
   Boolean          foundMan;
   Boolean          userIsDragging;
   IPoint           ipStartDrag;
   IString          isLastFileDrop;

   void buildTree           ();
   void buildAncestorsOf    (PNPerson pman
                            ,IPointArray& ipaMinMax
                            ,PNPersonINSTANCE& pmaninstance
                            ,PRELINSTANCE& prelinstance
                            ,unsigned short depth);
   void buildDescendantsOf  (PNPerson pman
                            ,IPointArray& ipaMinMax
                            ,PNPersonINSTANCE& pmaninstance
                            ,PRELINSTANCE& prelinstance
                            ,unsigned short depth
                            ,PNPerson pOnlyPartner=0);
   void calcDescendantInfoOf(PNPerson pman
                            ,unsigned short& neededManInstances
                            ,unsigned short& neededRelInstances
                            ,unsigned short depth
                            ,PNPerson pOnlyPartner=0);
   void calcAncestorInfoOf  (PNPerson pman
                            ,unsigned short& neededManInstances
                            ,unsigned short& neededRelInstances
                            ,unsigned short depth);

   PNPersonINSTANCE drawMan     (PNPersonINSTANCE pinstance
                            ,IGraphicContext& igc
                            ,IRectangle& visibleBox
                            ,Boolean isPrint=False);
   PRELINSTANCE drawRelation(PRELINSTANCE pinstance
                            ,IGraphicContext& igc
                            ,IRectangle& visibleBox
                            ,Boolean isPrint=False);

   PNPerson         findMan     (const IPoint& point);

protected:
   virtual Boolean paintWindow(IPaintEvent &event);
   virtual Boolean windowResize(IResizeEvent& ireEvent);
   virtual Boolean makePopUpMenu(IMenuEvent& imeEvent);
   virtual Boolean mouseClicked(IMouseClickEvent& imceEvent);
   virtual Boolean mouseMoved(IMouseEvent& imeEvent);
   virtual Boolean mousePointerChange(IMousePointerEvent& event);
};


#endif
