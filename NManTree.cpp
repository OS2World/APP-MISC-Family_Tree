/*
   File   : ManTree.hpp
   Purpose: Define Classes for showin persons in a Tree
*/

#include <igrect.hpp>
#include <ipopmenu.hpp>

unsigned long _System DosBeep(unsigned long,unsigned long);
#define MAXCHILDREN 50
#define MAXPARTNERS 50

#include "NManTree.hpp"
#include "FTree.h"
#include "Main.hpp"
#include "NManList.hpp"
#include "NEditDialog.hpp"

#define PI 3.141592654

#define FLAGONLYONE   (1<<31)
#define FLAGALLHIDDEN (1<<30)
//onlyYoungest,onlyOldest

// class  : NManTree
// purpose: Project Mankind on a flat 2-dimensional field
NManTree*        NManTree::pNManTree       =NULL;

NManTree::Detail NManTree::detail          =NManTree::Detail(NManTree::firstName|NManTree::dates);
NManTree::Line   NManTree::line            =NManTree::orthogonal;
NManTree::Dir    NManTree::dir             =NManTree::vertical;

unsigned short   NManTree::depthAncestors  =-1;
unsigned short   NManTree::depthDescendants=-1;
unsigned short   NManTree::depthSiblings   = 0;
unsigned short   NManTree::totalAncestors  = 0;
unsigned short   NManTree::totalDescendants= 0;

IFont           NManTree::ifont            ("Times New Roman",10,False,True);
IColor::Color   NManTree::colorback        =IColor::white;
IColor::Color   NManTree::colorlines       =IColor::darkCyan;
IColor::Color   NManTree::colorboxes       =IColor::darkCyan;
IColor::Color   NManTree::coloremphasis    =IColor::red;
IColor::Color   NManTree::colortexts       =IColor::darkCyan;

NManTree::NManTree(unsigned long windowId,IWindow* pParentOwner,PNMankind pSetNMankind)
   : IViewPort(windowId,pParentOwner,pParentOwner
              ,IRectangle()
              ,IWindow::visible
               |IViewPort::alwaysHorizontalScrollBar
               |IViewPort::alwaysVerticalScrollBar
               |IViewPort::noViewWindowFill)
   , IPaintHandler()
   , IResizeHandler()
   , NMankindHandler()
   , IMenuHandler()
   , IMousePointerHandler()
   , NDragDropHandler()
   , drawingArea(1,this,this)
{
   // Initialize private data
   pNMankind       =pSetNMankind;
   pNManTree       =this;
   pManInstances   =NULL;
   pRelInstances   =NULL;
   xyZoom          =1;
   depthAncestors  =(unsigned short)-1;
   depthDescendants=(unsigned short)-1;
   depthSiblings   =(unsigned short)-1;
   userIsDragging  =False;
   bDisabled       =False;
   horizontalScrollBar()->setMinScrollIncrement(20);
   verticalScrollBar()  ->setMinScrollIncrement(20);
   pLockInstances=new IPrivateResource;

   // Do Handler
   IPaintHandler::handleEventsFor(&drawingArea);
   IMenuHandler::handleEventsFor(&drawingArea);
   IMousePointerHandler::handleEventsFor(&drawingArea);
   IResizeHandler::handleEventsFor(this);
   pNMankind->addMankindHandler(this);
   handleDragFor(&drawingArea,NDragDropHandler::Man);
   handleDropFor(&drawingArea,NDragDropHandler::File,IString(".fam"));
   // Done
}

NManTree::~NManTree()
{
   delete pLockInstances;
   IPaintHandler::stopHandlingEventsFor(&drawingArea);
   IMenuHandler::stopHandlingEventsFor(&drawingArea);
   IMousePointerHandler::stopHandlingEventsFor(&drawingArea);
   IResizeHandler::stopHandlingEventsFor(this);
   pNMankind->removeMankindHandler(this);
}

void NManTree::disableInput()
{
   bDisabled=True;
}
void NManTree::enableInput()
{
   bDisabled=False;
}

void NManTree::loadFromProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("NManTree");
   // Colors
   colorback    =IColor::Color(profile.integerWithKey("COLOR.BACK"    ));
   colorboxes   =IColor::Color(profile.integerWithKey("COLOR.BOXES"   ));
   colorlines   =IColor::Color(profile.integerWithKey("COLOR.LINES"   ));
   coloremphasis=IColor::Color(profile.integerWithKey("COLOR.EMPHASIS"));
   colortexts   =IColor::Color(profile.integerWithKey("COLOR.TEXTS"   ));
   // Font
   IFont tmp;
   tmp.useVectorOnly(profile.integerWithKey("FONTVECTOR"));
   tmp.useNonPropOnly(profile.integerWithKey("FONTNONPROP"));
   tmp.setName(profile.elementWithKey("FONTNAME"));
   tmp.setPointSize(profile.integerWithKey("FONTSIZE"));
   tmp.setBold(profile.integerWithKey("FONTBOLD"));
   tmp.setItalic(profile.integerWithKey("FONTITALIC"));
   ifont=tmp;
//oks   ifont.fontChanged();
   // Layout
   changeLine(Line(profile.integerWithKey("LINE")));
   changeDetail(Detail(profile.integerWithKey("DETAIL")),Detail(-1));
   // Done
}
void NManTree::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("NManTree");
   // Colors
   profile.addOrReplaceElementWithKey("COLOR.BACK"    ,colorback);
   profile.addOrReplaceElementWithKey("COLOR.LINES"   ,colorlines);
   profile.addOrReplaceElementWithKey("COLOR.BOXES"   ,colorboxes);
   profile.addOrReplaceElementWithKey("COLOR.EMPHASIS",coloremphasis);
   profile.addOrReplaceElementWithKey("COLOR.TEXTS"   ,colortexts);
   // Font
   profile.addOrReplaceElementWithKey("FONTVECTOR" ,ifont.isVectorOnly() );
   profile.addOrReplaceElementWithKey("FONTNONPROP",ifont.isNonPropOnly());
   profile.addOrReplaceElementWithKey("FONTNAME"   ,ifont.name()         );
   profile.addOrReplaceElementWithKey("FONTSIZE"   ,ifont.pointSize()    );
   profile.addOrReplaceElementWithKey("FONTBOLD"   ,ifont.isBold()       );
   profile.addOrReplaceElementWithKey("FONTITALIC" ,ifont.isItalic()     );
   // Layout
   profile.addOrReplaceElementWithKey("LINE"       ,line                       );
   profile.addOrReplaceElementWithKey("DETAIL"     ,detail                     );
   // Done
}

void NManTree::calcAncestorInfoOf(PNPerson pman
                                 ,unsigned short& neededManInstances
                                 ,unsigned short& neededRelInstances
                                 ,unsigned short depth)
{
   // This is one ancestor
   neededManInstances++;
   // Check if Parents ex
   if (!pman->getChildhood()) return;
   // Check if no Parents shown
   if (depth==0) {
      neededRelInstances+=2;  // parents (NULL) + child
      return;
      }
   // Dad & Mom ?
   neededRelInstances+=2+1+1; // father+mother+1+child
   calcAncestorInfoOf(pman->getChildhood()->getPartner(1)
                     ,neededManInstances
                     ,neededRelInstances
                     ,depth==0?0:depth-1);
   calcAncestorInfoOf(pman->getChildhood()->getPartner(2)
                     ,neededManInstances
                     ,neededRelInstances
                     ,depth==0?0:depth-1);
   // Done
}

void NManTree::calcDescendantInfoOf(PNPerson pman
                                   ,unsigned short& neededManInstances
                                   ,unsigned short& neededRelInstances
                                   ,unsigned short depth
                                   ,PNPerson pOnlyPartner)
{
   // This it one descendant
   neededManInstances++;
   if (pman->getNoOfPartners()==0) return;
   // Children
   PNPerson pChild;PREL pRel;
   for (unsigned short partner=1;partner<=pman->getNoOfPartners();partner++) {
      pRel=pman->getPartnership(partner);
      if ((!pOnlyPartner)||(pRel->getOther(pman)==pOnlyPartner)) {
         // Partnership
         neededManInstances++;
         neededRelInstances+=2+1; // father+mother+n
         // Childhood of Partner ?
         if ((!pOnlyPartner)&&(pRel->getOther(pman)->getChildhood()))
            neededRelInstances+=2;  // parents (NULL) + child
         // Children
         if (depth!=0)
         for (unsigned short child=1;child<=pRel->getNoOfChildren();child++) {
            neededRelInstances++; // child
            calcDescendantInfoOf(pRel->getChild(child)
                                ,neededManInstances
                                ,neededRelInstances
                                ,depth==0?0:depth-1);
            }
         // Done with Partner
         }
      }
   // Done
}

void NManTree::buildTree()
{
   pLockInstances->lock();
   // Free used Array + check for new
   if (pManInstances!=NULL) {
      free(pManInstances);
      pManInstances=NULL;
      }
   if (pRelInstances!=NULL) {
      free(pRelInstances);
      pRelInstances=NULL;
      }
   numberOfRelInstances=0;
   numberOfManInstances=0;
   if ( (pNMankind->getManCount()==0)
      ||(!pNMankind->getOrigin()    )) {
      isTree=ISize(0,0);
      pLockInstances->unlock();
      // Resize drawingArea to Size of view+tree
      disableUpdate();
      drawingArea.sizeTo(size()
                        -ISize(IScrollBar::systemScrollBarWidth(True)
                              ,IScrollBar::systemScrollBarWidth(False)));
      setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
      enableUpdate();
      return;
      }
   // Calculate information
   PNPerson pmanOrigin=pNMankind->getOrigin(),
        pmanActual=pNMankind->getActual();
   Boolean DoSiblings=(getDepth(ancestors)>0)
                    &&(getDepth(siblings)>0)
                    &&(pmanOrigin->getChildhood());
   totalAncestors  =pNMankind->numParentGenerations(pmanOrigin);
   totalDescendants=pNMankind-> numChildGenerations(pmanOrigin);;
   // ... ancestors
   unsigned short anceDepth=getDepth(ancestors);
   calcAncestorInfoOf  (pmanOrigin
                       ,numberOfManInstances
                       ,numberOfRelInstances
                       ,anceDepth);
   // ... descendants
   unsigned short descDepth=getDepth(descendants);
   if ((descDepth!=(unsigned short)-1)&&(DoSiblings)) descDepth++;
   calcDescendantInfoOf(DoSiblings?pmanOrigin->getChildhood()->getPartner(1):pmanOrigin
                       ,numberOfManInstances
                       ,numberOfRelInstances
                       ,descDepth
                       ,DoSiblings?pmanOrigin->getChildhood()->getPartner(2):0);
   // Get new Array
   pManInstances=(PNPersonINSTANCE)malloc(sizeof(NPersonINSTANCE)*numberOfManInstances);
   pRelInstances=(PRELINSTANCE)malloc(sizeof(RELINSTANCE)*numberOfRelInstances);
   // Calculate TreeParams
   manBoxWidth=4+4+NDate::drawSize(getFont()).width()*2+8;
   manBoxHeight=4+getFont().maxCharHeight()*3;
   if (manBoxWidth%2!=0) manBoxWidth++;
   if (manBoxHeight%2!=0) manBoxHeight++;
   if (getDir()==NManTree::horizontal) {
      distGenerations=manBoxWidth+40;
      distSiblings   =manBoxHeight+20;
      distPartners   =manBoxHeight+10;
      distAddPartners=manBoxHeight+6;
      }
   else {                   // Vertical
      distGenerations=manBoxHeight+40;
      distSiblings   =manBoxWidth+20;
      distPartners   =manBoxWidth+10;
      distAddPartners=manBoxWidth+6;
      }
   isHori=(getDir()==NManTree::horizontal);
   IPoint ipMove,ipOrigin;
   PNPersonINSTANCE porigin,ptmp,pmaninstance=pManInstances;
   PRELINSTANCE prelinstance=pRelInstances;
   // Calculate Ancestors Array[]={Opa,Oma,Mutter,Vater,Ursprung}
   IPointArray ancestorsMinMax(0);
   buildAncestorsOf(pmanOrigin
                   ,ancestorsMinMax
                   ,pmaninstance
                   ,prelinstance
                   ,getDepth(ancestors));

   ancestorsMinMax.remove(0);    // 1st Generation of Ancestors is double
   pmaninstance--;               // Origin can be dropped
   numberOfManInstances--;       // ... so we have one person less
   if (DoSiblings) {             // Are we doing siblings ?
      // Have to shift array
      long shift;
      if (isHori) shift=pmaninstance->pos.y()-(pmaninstance-1)->pos.y();
      else shift=pmaninstance->pos.x()-(pmaninstance-1)->pos.x();
      ISize isShift(shift,shift);
      for (int i=0;i<ancestorsMinMax.size();i++)
         ancestorsMinMax[i]+=isShift;
      // Manipulate stack
      ancestorsMinMax.remove(0); // 2nd Generation ist double, too
      pmaninstance--;            // ... Father can be dropped, too
      numberOfManInstances--;    // ... so we have one more less
      prelinstance-=4;           // We'll skip the last relation
      numberOfRelInstances-=4;   // ... so we have one less
      }
   // Save Remembered
   porigin=pmaninstance;
   ipOrigin=porigin->pos;
   // Calculate Descendants Array[]={Ursprung,Partner,Sohn,Enkel}
   IPointArray descendantsMinMax(0);
   buildDescendantsOf(DoSiblings?pmanOrigin->getChildhood()->getPartner(1):pmanOrigin
                     ,descendantsMinMax
                     ,pmaninstance
                     ,prelinstance
                     ,descDepth
                     ,DoSiblings?pmanOrigin->getChildhood()->getPartner(2):0);
   // Reposition all up to remembered
   ipMove=porigin->pos-ipOrigin;
   for (ptmp=pManInstances;ptmp<porigin;ptmp++)
      ptmp->pos+=ipMove;
   // Calculate Extent
   int minimum=0;
   int maximum=0;
   for (int i=0;i<ancestorsMinMax.size();i++) {
      minimum=min(minimum,ancestorsMinMax[i].coord1());
      maximum=max(maximum,ancestorsMinMax[i].coord2());
      }
   for (    i=0;i<descendantsMinMax.size();i++) {
      minimum=min(minimum,descendantsMinMax[i].coord1());
      maximum=max(maximum,descendantsMinMax[i].coord2());
      }
   // RePosition to lower/Left Corner of WorldSpace
   ipMove=IPoint(isHori ? manBoxWidth/2+ancestorsMinMax.size()*distGenerations
                        : manBoxWidth/2-minimum
                ,isHori ? manBoxHeight/2-minimum
                        : manBoxHeight/2+(descendantsMinMax.size()-1)*distGenerations )
                          -porigin->pos;
   for (ptmp=pManInstances;ptmp<pmaninstance;ptmp++)
      ptmp->pos+=ipMove;
   // Calculate Size
   if (isHori)
      isTree=ISize(manBoxWidth+(descendantsMinMax.size()-1+ancestorsMinMax.size())*distGenerations
                  ,maximum-minimum+manBoxHeight);
   else
      isTree=ISize(maximum-minimum+manBoxWidth
                  ,manBoxHeight+(descendantsMinMax.size()-1+ancestorsMinMax.size())*distGenerations);
   // Translate WorldTransformMatrix
   ipTranslate=size()/2;
   // Resize drawingArea to Size of view+tree + do recalc +scroll
   disableUpdate();
   drawingArea.sizeTo(size()+isTree.scaledBy(xyZoom,xyZoom));
   setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
   PNPersonINSTANCE pactual=pManInstances;
   for (;pactual<pManInstances+numberOfManInstances;pactual++)
      if (pactual->pman==pmanActual) break;
   if (pactual>=pManInstances+numberOfManInstances) pactual=porigin;
   scrollViewVerticallyTo( viewWindowSize().height()
           - (  pactual->pos.y()*xyZoom+ipTranslate.y()
               +viewWindowDrawRectangle().height()/2     ) );
   scrollViewHorizontallyTo( pactual->pos.x()*xyZoom+ipTranslate.x()
           - viewWindowDrawRectangle().width()/2 );
   enableUpdate();
   // Done
   pLockInstances->unlock();
}

void NManTree::buildAncestorsOf(PNPerson pman
                               ,IPointArray& ipaMinMax
                               ,PNPersonINSTANCE& pmaninstance
                               ,PRELINSTANCE& prelinstance
                               ,unsigned short depth)
{
   // Some Variables
   long distance=distPartners,temp;
   PREL prel=pman->getChildhood();
   PNPersonINSTANCE pdad,pmom,ptmp,pthis;
   IPoint ipMove;
   // Parents ?
   if ((!pman->getChildhood())||(depth==0)) {
      // Add This
      pmaninstance->pos =IPoint(0,0);
      pmaninstance->pman=pman;
      pmaninstance++;
      // new RelInstance if parents but depth==0
      if (pman->getChildhood()) {
         prelinstance->pmaninstance[0]=NULL;           // no parent
         prelinstance->pmaninstance[1]=pmaninstance-1; // ... only child (partner of this)
         prelinstance+=2;
         }
      // enlarge MinMax
      ipaMinMax.insert(0,IPoint(0,0));
      return;
      }
   // Add 2nd of Partners
   ptmp=pmaninstance;
   IPointArray ipaTemp(0);
   buildAncestorsOf(prel->getPartner(2)
                   ,ipaTemp
                   ,pmaninstance
                   ,prelinstance
                   ,depth==0?0:depth-1);
   pmom=pmaninstance-1;
   // Add 1st of Partners
   buildAncestorsOf(prel->getPartner(1)
                   ,ipaMinMax
                   ,pmaninstance
                   ,prelinstance
                   ,depth==0?0:depth-1);
   pdad=pmaninstance-1;
   // Add This
   pmaninstance->pos =IPoint(0,0);
   pmaninstance->pman=pman;
   pthis=pmaninstance;
   pmaninstance++;
   // new RelInstance
   prelinstance->pmaninstance[0]=pdad;
   prelinstance->pmaninstance[1]=pmom;
   prelinstance->pmaninstance[2]=(PNPersonINSTANCE)
                       (1|(prel->getNoOfChildren()==1?0:FLAGONLYONE));
   prelinstance->pmaninstance[3]=pthis;
   prelinstance+=4;
   // Check for minnimal Distance
   for (int i=0;(i<ipaMinMax.size())&&(i<ipaTemp.size());i++)
      // Dad.Right+(-1)*Mom.Left+100 is minimal Distance here
      distance=max(distance,ipaMinMax[i].coord2()-ipaTemp[i].coord1()+distPartners);
   // Move Dad+Mom around 0
   ipMove=IPoint( isHori ? -distGenerations : +distance/2
                , isHori ? +distance/2      : distGenerations )
          -pmom->pos;
   for (;ptmp<=pmom;ptmp++)
      ptmp->pos+=ipMove;

   ipMove=IPoint( isHori ? -distGenerations : -distance/2
                , isHori ? -distance/2      : distGenerations )
          -pdad->pos;
   for (;ptmp<=pdad;ptmp++)
      ptmp->pos+=ipMove;
   // Merge DadArray with MomArray around NewMiddle (Distance/2)
   for (i=0;(i<ipaTemp.size())&&(i<ipaMinMax.size());i++)
      // Mom&Dad
      ipaMinMax[i]=IPoint(ipaMinMax[i].coord1()-distance/2
                         ,ipaTemp  [i].coord2()+distance/2);
   for (;i<ipaTemp.size();i++)
      // Only Mom
      ipaMinMax.add(ipaTemp[i]+IPoint(distance/2,distance/2));
   for (;i<ipaMinMax.size();i++)
      // Only Dad
      ipaMinMax[i]-=IPoint(distance/2,distance/2);
   // Move direct parents around Base and change array[0] !
   pdad->pos=IPoint( (isHori?-distGenerations:-distPartners/2)
                   , (isHori?-distPartners/2:+distGenerations) );
   pmom->pos=IPoint( (isHori?-distGenerations:+distPartners/2)
                   , (isHori?+distPartners/2:+distGenerations) );
   ipaMinMax[0]=IPoint( -distPartners/2 , +distPartners/2 );
   // Large MinMax
   ipaMinMax.insert(0,IPoint(0,0));
}

void NManTree::buildDescendantsOf(PNPerson pman
                                 ,IPointArray& ipaMinMax
                                 ,PNPersonINSTANCE& pmaninstance
                                 ,PRELINSTANCE& prelinstance
                                 ,unsigned short depth
                                 ,PNPerson pOnlyPartner)
{
   // Partners ?
   if (pman->getNoOfPartners()==0) {
      // Self
      pmaninstance->pos =IPoint(0,0);  // <-- has to be 0
      pmaninstance->pman=pman;
      pmaninstance++;
      ipaMinMax.insert(0,IPoint(0,0));
      return;
      }
   // Some Variables
   IPoint        ipMove;
   NMarriage*     prel;
   PNPerson          pchild;
   int           childdist,partnerdist,i,partner;
   PNPersonINSTANCE  plastchild,ptmp,pparents;
   PRELINSTANCE  pchildships;
   // Add Parents
   pparents=pmaninstance;
   pmaninstance->pman=pman;
   pmaninstance++;
   for (partner=1;partner<=pman->getNoOfPartners();partner++) {
      prel=pman->getPartnership(partner,True);
      // Do all Partners (at least the wanted one)
      if ((!pOnlyPartner)||(pOnlyPartner==prel->getOther(pman))) {
         // new RelInstance if partner has parents
         if ((!pOnlyPartner)&&(prel->getOther(pman)->getChildhood())) {
            prelinstance->pmaninstance[0]=NULL;         // no parent
            prelinstance->pmaninstance[1]=pmaninstance; // ... only child (partner of this)
            prelinstance+=2;
            }
         // new ParentInstance
         pmaninstance->pman=prel->getOther(pman);
         pmaninstance++;
         // Do a Relation if childrun won't
         if (depth==0) {
            // new RelInstance between pman and partner
            prelinstance->pmaninstance[0]=&pparents[0];
            prelinstance->pmaninstance[1]=&pparents[pOnlyPartner?1:partner];
            prelinstance->pmaninstance[2]=(PNPersonINSTANCE)
                (0|(prel->getNoOfChildren()==0?0:FLAGALLHIDDEN));
            prelinstance+=3;
            }
         }
      // Next
      }
   // Go through Children of Partnerships
   if (depth>0) for (partner=1;partner<=pman->getNoOfPartners();partner++) {
      // Get Partnership with partner
      prel=pman->getPartnership(partner,True);
      // check if this partnership is wanted
      if ((!pOnlyPartner)||(pOnlyPartner==prel->getOther(pman))) {
         // new RelInstance between pman and partner
         prelinstance->pmaninstance[0]=&pparents[0];
         prelinstance->pmaninstance[1]=&pparents[pOnlyPartner?1:partner];
         prelinstance->pmaninstance[2]=(PNPersonINSTANCE)prel->getNoOfChildren();
         prelinstance+=3;
         pchildships=prelinstance;
         prelinstance+=prel->getNoOfChildren();
         // Add Children
         childdist=0;
         for (unsigned short child=1;child<=prel->getNoOfChildren();child++) {
            pchild=prel->getChild(child);
            // Add child to relinstances
            pchildships->pmaninstance[child-1]=pmaninstance;
            // Add Child and its decendants to Array and maninstances
            plastchild=pmaninstance;
            IPointArray ipaTemp(0);
            buildDescendantsOf(pchild,ipaTemp,pmaninstance,prelinstance
                              ,depth==0?0:depth-1);
            // Check for minimal Distance between children
            for (i=0;(i<ipaMinMax.size())&&(i<ipaTemp.size());i++)
                childdist=max(childdist,ipaMinMax[i].coord2()-ipaTemp[i].coord1()+distSiblings);
            // Move Child beside other Children
            ipMove=IPoint(isHori?+distGenerations:childdist,isHori?childdist:-distGenerations)
                  -plastchild->pos;
            for (ptmp=plastchild;ptmp<pmaninstance;ptmp++)
               ptmp->pos+=ipMove;
            // Merge Child with other Children
            for (i=0;i<ipaTemp.size();i++)
               if (i<ipaMinMax.size()) // Main still too ? -> Merge
                  ipaMinMax[i]=IPoint(ipaMinMax[i].coord1()
                                     ,ipaTemp[i].coord2()+childdist);
               else                    // Only this ! -> Add
                  ipaMinMax.add(ipaTemp[i]+IPoint(childdist,childdist));
            // Next Child
            childdist=distSiblings;
            }
         // Next Partnership
         }
      }
   // Calculate Center above Children
   int noPartners=pOnlyPartner?1:pman->getNoOfPartners();

   int center=0;
   if (ipaMinMax.size()!=0) {
      center=isHori ? plastchild->pos.y()/2-(distPartners+(noPartners-1)*distAddPartners)/2
                    : plastchild->pos.x()/2-(distPartners+(noPartners-1)*distAddPartners)/2;
      // Shift ipaArray relative to Base !
      ipMove=IPoint(-center,-center);
      for (i=0;i<ipaMinMax.size();i++) ipaMinMax[i]+=ipMove;
      }
   // Position Parents above children
   pparents[0].pos=IPoint(isHori?0:center,isHori?center:0);

   partnerdist=distPartners;
   for (partner=1;partner<=noPartners;partner++) {
      // update pos
      pparents[partner].pos=IPoint(isHori?0:center+partnerdist
                                  ,isHori?center+partnerdist:0);
      // Next
      partnerdist+=distAddPartners;
      }
   // Add Self/Partner to ipaArray!
   ipaMinMax.insert(0,IPoint(0,distPartners+(noPartners-1)*distAddPartners));
   // Done
}

PNPerson NManTree::findMan(const IPoint& point)
{
   // First change Point relative to GraphicContext
   IRectangle irTest((point-ipTranslate).scaledBy(1/xyZoom,1/xyZoom)
                     -IPoint(manBoxWidth/2,manBoxHeight/2)
                    ,ISize(manBoxWidth,manBoxHeight));
   // Try to find NPerson under Point
   PNPersonINSTANCE pinstance=pManInstances;
   for (unsigned short i=1;i<=numberOfManInstances;i++) {
      if (irTest.contains(pinstance->pos))
         return(pinstance->pman);
      pinstance++;
      }
   // Not found
   return(NULL);
}

// MouseHandler Methods
Boolean NManTree::makePopUpMenu(IMenuEvent& imeEvent)
{
   if (bDisabled) return(False);
   IPopUpMenu *ipmPopMenu;
   //  PopUp ?
   if (foundMan) {
      ipmPopMenu=new IPopUpMenu(IResourceId(MI_Man,*NApplication::pDLLResource),this);
      if (!ipmPopMenu) return(False);
      NMainWindow::pNMainWindow->fillManMenu(ipmPopMenu);
      }
   else {
      ipmPopMenu=new IPopUpMenu(IResourceId(MI_Mankind,*NApplication::pDLLResource),this);
      if (!ipmPopMenu) return(False);
      NMainWindow::pNMainWindow->fillViewMenu(ipmPopMenu);
      }
   // Show Menu
   ipmPopMenu->setAutoDeleteObject(true);
   ipmPopMenu->show(IWindow::mapPoint(imeEvent.mousePosition()
                                     ,imeEvent.window()->handle()
                                     ,handle()                  ));
   return(True);
}

Boolean NManTree::mousePointerChange(IMousePointerEvent& event)
{
   if (bDisabled)
      event.setMousePointer(ISystemPointerHandle(ISystemPointerHandle::wait));
   else {
      if (userIsDragging)
         event.setMousePointer(ISystemPointerHandle(ISystemPointerHandle::move));
      else
         event.setMousePointer(event.defaultMousePointer());
      }
   // I did everything
   return(True);
}

Boolean NManTree::mouseMoved(IMouseEvent& imeEvent)
{
   // Check for really new Position
   if (imeEvent.mousePosition()==ipStartDrag)
      return(False);
   // Check for Dragging
   static Boolean selfMoved=False;
   if (selfMoved) selfMoved=False;
   else if ((drawingArea.hasPointerCaptured())&&(ipStartDrag!=imeEvent.mousePosition())) {
      userIsDragging=True;
      // Calculate Movement
      int newx=viewWindowDrawRectangle().minX()
               +ipStartDrag.x()-imeEvent.mousePosition().x();
      if (newx<0) newx=0;
      scrollViewHorizontallyTo(newx);
      int newy=viewWindowSize().height()-viewWindowDrawRectangle().maxY()
               +imeEvent.mousePosition().y()-ipStartDrag.y();
      if (newy<0) newy=0;
      scrollViewVerticallyTo(newy);
      // scrolling results in moveEvent :-/
      selfMoved=True;
      }
   else userIsDragging=False;
   // Remember position for next move
   ipStartDrag=imeEvent.mousePosition();
   // Give other Handlers a chance
   return(True);
}

Boolean NManTree::mouseClicked(IMouseClickEvent& imceEvent)
{
   // Not for me
   foundMan=False;
   // Check for Mouse
   if (!bDisabled) {
   switch (imceEvent.mouseAction()) {
      case IMouseClickEvent::click :
      case IMouseClickEvent::doubleClick : {
         NPerson* pman=findMan(imceEvent.mousePosition());
         if (pman) {
            pNMankind->makeActual(pman);
            if (imceEvent.mouseAction()==IMouseClickEvent::doubleClick) {
               if (imceEvent.isShiftKeyDown())
                  NMainWindow::pNMainWindow->postEvent(IWindow::command,MI_ManEdit);
               else
                  pNMankind->makeOrigin();
               }
            foundMan=True;
            }
         } break;
      case IMouseClickEvent::down : {
         if (imceEvent.mouseButton()==IMouseClickEvent::button1) {
            userIsDragging=False;
            ipStartDrag=imceEvent.mousePosition();
            // Capture Mouse
            if (!drawingArea.hasPointerCaptured())
               drawingArea.capturePointer(True);
            }
         } break;
      case IMouseClickEvent::up : {
         if (imceEvent.mouseButton()==IMouseClickEvent::button1) {
            // End Dragging
            userIsDragging=False;
            // Release Pointer
            if (drawingArea.hasPointerCaptured())
               drawingArea.capturePointer(False);
            }
         } break;
      }
   }
   return(False);
}

const ISize& NManTree::getTreeSize()
{
   return(isTree);
}

void NManTree::printOn(IGraphicContext& igc
                     ,const IRectangle& irPrintRectangle
                     ,const double zoomX, const double zoomY)
{
   pLockInstances->lock();
   // prepare some params
   ITransformMatrix itm;
   itm.scaleBy(zoomX,zoomY)
      .translateBy(-irPrintRectangle.minXMinY());
   igc.setMixMode( IGraphicBundle::overPaint )
      .setPenColor( IColor(IColor::black) )
      .setFillColor( IColor(IColor::black) )
      .setDrawOperation( IGraphicBundle::frame )
      .setWorldTransformMatrix(itm)
      .setFont(getFont());
   PRELINSTANCE prelinstance,ptmp;PNPersonINSTANCE pmaninstance;
   IRectangle box=irPrintRectangle;
   box.scaleBy(1/zoomX,1/zoomY)
      .expandBy(IPair(manBoxWidth/2+4,manBoxHeight/2+4)); // Has to be after scale !
   // Draw ManInstances
   pmaninstance=pManInstances;
   while (pmaninstance<pManInstances+numberOfManInstances)
       pmaninstance=drawMan(pmaninstance,igc,box,True);
   // Draw RelInstances
   prelinstance=pRelInstances;
   while (prelinstance<pRelInstances+numberOfRelInstances) {
      prelinstance=drawRelation(prelinstance,igc,box,True);
      }
   // Done with printing
   pLockInstances->unlock();
}


// PaintHandler Methods
Boolean NManTree::paintWindow(IPaintEvent &event)
{
   if (!isShowing()) return(True);
   // Make Context
   IGraphicContext igc(event.presSpaceHandle());
   // Fill background
   IColor fillColor(IColor::white);
   if (getDetail()&NManTree::pageBorder)
      fillColor=IColor(IColor::darkGray);
   igc.setDrawOperation(IGraphicBundle::fillAndFrame)
      .setFillColor(fillColor)
      .setPenColor (fillColor);
   IGRectangle igrTemp=IRectangle(event.rect());
   igrTemp.drawOn(igc);
   // Check for valid data
   if (!pManInstances) return(True);
   pLockInstances->lock();
   // Calc Drawing Params
   fontLineHeight=getFont().maxCharHeight();
   fontBaseOffset=getFont().maxDescender();
   // prepare some params
   ITransformMatrix itm;
   itm.translateBy(ipTranslate/xyZoom);
   itm.scaleBy    (xyZoom,xyZoom);
   igc.setWorldTransformMatrix(itm)
      .setFont(getFont());

   unsigned short i;
   PRELINSTANCE prelinstance,ptmp;PNPersonINSTANCE pmaninstance;
   IRectangle box(event.rect());
   box.moveBy(-ipTranslate)
      .scaleBy(1/xyZoom,1/xyZoom)
      .expandBy(IPair(manBoxWidth/2+4,manBoxHeight/2+4));
   // Clear tree area (only if pageBorder visible)
   if (getDetail()&NManTree::pageBorder) {
      igc.setDrawOperation(IGraphicBundle::fillAndFrame)
         .setFillColor(IColor(getColor(back)))
         .setPenColor (IColor(IColor::paleGray));
      igrTemp=IRectangle(-manBoxHeight,-manBoxHeight
                        ,isTree.width()+manBoxHeight,isTree.height()+manBoxHeight);
      igrTemp.drawOn(igc);
      }
   // Draw ManInstances
   pmaninstance=pManInstances;
   for (i=0;i<numberOfManInstances;i++)
       pmaninstance=drawMan(pmaninstance,igc,box);
   // Draw RelInstances
   igc.setPenColor (IColor(getColor(NManTree::lines)));
   igc.setFillColor(IColor(getColor(NManTree::lines)));
   prelinstance=pRelInstances;
   while (prelinstance<pRelInstances+numberOfRelInstances) {
      prelinstance=drawRelation(prelinstance,igc,box);
      }
   // Done
   pLockInstances->unlock();
   return(True);
}


PRELINSTANCE NManTree::drawRelation(PRELINSTANCE pinstance
                                   ,IGraphicContext& igc
                                   ,IRectangle& visibleBox
                                   ,Boolean isPrint)
{
   // Static drawing params
   static IPoint ipsVertArrow[]={IPoint(),IPoint(-4,6),IPoint(4,6)};
   static IPoint ipsHoriArrow[]={IPoint(),IPoint(-6,-4),IPoint(-6,4)};
   static IPointArray ipaVertArrow(3,ipsVertArrow);
   static IPointArray ipaHoriArrow(3,ipsHoriArrow);

   static IPoint ipsVertDCont[]={IPoint(),IPoint(-4,-10),IPoint(5,-10)};
   static IPoint ipsHoriDCont[]={IPoint(),IPoint(10,-5),IPoint(10,5)};
   static IPointArray ipaVertDCont(3,ipsVertDCont);
   static IPointArray ipaHoriDCont(3,ipsHoriDCont);

   static IPoint ipsVertACont[]={IPoint(),IPoint(-4,10),IPoint(5,10)};
   static IPoint ipsHoriACont[]={IPoint(),IPoint(-10,-5),IPoint(-10,5)};
   static IPointArray ipaVertACont(3,ipsVertACont);
   static IPointArray ipaHoriACont(3,ipsHoriACont);

   static IGLine iglTemp(IPoint(0,0),IPoint(0,0));
   static IGEllipse igeTemp(IPoint(0,0),0);

   static IPoint ipParents,ipTemp;
   // Parents visible ? else only one child
   if (pinstance->pmaninstance[0]==NULL) {
      // Child is in first child field
      ipTemp=IPoint(pinstance->pmaninstance[1]->pos
                   +IPoint(isHori?-(manBoxWidth/2+4):0,isHori?0:manBoxHeight/2+4) );
      // Check visible
      if (visibleBox.contains(ipTemp)) {
         // ... then the ContinueTriangle
         igc.setPenType      (IGraphicBundle::dot  )
            .setDrawOperation(IGraphicBundle::frame);
         if (isHori) {
            IGPolygon igpCont(ipaHoriACont);
            igpCont.translateBy(ipTemp)
                   .setTransformMethod(IGraphic::leftMultiply)
                   .drawOn(igc);
            }
         else {
            IGPolygon igpCont(ipaVertACont);
            igpCont.translateBy(ipTemp)
                   .setTransformMethod(IGraphic::leftMultiply)
                   .drawOn(igc);
            }
         igc.setPenType      (IGraphicBundle::solid);
         }
      // done Continue
      return(pinstance+2);
      }
   // Calc Children count
   Boolean allChildrenAreHidden
           =(((unsigned long)pinstance->pmaninstance[2]&FLAGALLHIDDEN)!=0);
   Boolean hiddenSiblings
           =(((unsigned long)pinstance->pmaninstance[2]&FLAGONLYONE  )!=0);
   unsigned long nochildren=((unsigned long)pinstance->pmaninstance[2])
                            &~(FLAGALLHIDDEN|FLAGONLYONE);
   // Check visible
   IRectangle box(pinstance->pmaninstance[0]->pos
                 ,pinstance->pmaninstance[1]->pos);
   if (nochildren>0) box|=IRectangle(pinstance->pmaninstance[2+1]->pos
                                  ,pinstance->pmaninstance[2+nochildren]->pos);
   if (!box.intersects(visibleBox))
      return(pinstance+3+nochildren);
   // Family
   igc.setDrawOperation(IGraphicBundle::frame);
   // Do Rings
   int distance=isHori ? pinstance->pmaninstance[1]->pos.y()
                        -pinstance->pmaninstance[0]->pos.y()
                       : pinstance->pmaninstance[1]->pos.x()
                        -pinstance->pmaninstance[0]->pos.x();
   if (distance==distPartners) {
      igc.setDrawOperation(IGraphicBundle::fillAndFrame);
      igeTemp.setEnclosingRect(IRectangle(IPoint(pinstance->pmaninstance[0]->pos)
                                          +IPoint(isHori? -5 : distPartners/2-5
                                                 ,isHori? distPartners/2-5 : -5)
                                         ,ISize(10,10)))
             .drawOn(igc);
      igc.setDrawOperation(IGraphicBundle::frame);
      igeTemp.setEnclosingRect(IRectangle(IPoint(pinstance->pmaninstance[0]->pos)
                                          +IPoint(isHori? 0 : distPartners/2-5
                                                 ,isHori? distPartners/2-5 : 0)
                                         ,ISize(10,10)))
            .drawOn(igc);
      }
   // Calculate ParentBase
   ipParents=IPoint( isHori ? pinstance->pmaninstance[1]->pos.x()+manBoxWidth/2+5
                            : pinstance->pmaninstance[1]->pos.x()-distPartners/2
                   , isHori ? pinstance->pmaninstance[1]->pos.y()-distPartners/2
                            : pinstance->pmaninstance[1]->pos.y()-manBoxHeight/2-5
                   );
   // No children
   if (nochildren==0) {
      // Check if all children are just hidden
      if (allChildrenAreHidden) {
         // ... then the ContinueTriangle
         igc.setPenType(IGraphicBundle::dot);
         if (isHori) {
            IGPolygon igpCont(ipaHoriDCont);
            igpCont.translateBy(ipParents)
                   .setTransformMethod(IGraphic::leftMultiply)
                   .drawOn(igc);
            }
         else {
            IGPolygon igpCont(ipaVertDCont);
            igpCont.translateBy(ipParents)
                   .setTransformMethod(IGraphic::leftMultiply)
                   .drawOn(igc);
            }
         igc.setPenType(IGraphicBundle::solid);
         }
      // done Continue
      return(pinstance+3+nochildren);
      }
   // Check for orthogonal
   if (getLine()==NManTree::orthogonal) {
      // Line from parents to point above children
      ipTemp=ipParents;
      long offset=((pinstance->pmaninstance[1]-pinstance->pmaninstance[0]-1)&3)*4;
      ipParents+=IPoint( isHori? +(distGenerations-manBoxWidth-5-5)/2-offset:0
                       , isHori? 0:-(distGenerations-manBoxHeight-5-5)/2+offset );
      iglTemp.setStartingPoint(ipTemp).setEndingPoint(ipParents).drawOn(igc);
      // Line above children
      if (isHori) {
         iglTemp.setStartingPoint(IPoint(ipParents.x()
                            ,min(pinstance->pmaninstance[3]->pos.y(),ipParents.y())))
                .setEndingPoint  (IPoint(ipParents.x()
                            ,max(pinstance->pmaninstance[nochildren+2]->pos.y(),ipParents.y())))
                .drawOn(igc);
         }
      else {
         iglTemp.setStartingPoint(IPoint(min(pinstance->pmaninstance[3]->pos.x(),ipParents.x())
                                         ,ipParents.y()) )
                .setEndingPoint  (IPoint(max(pinstance->pmaninstance[nochildren+2]->pos.x(),ipParents.x())
                                         ,ipParents.y()) )
                .drawOn(igc);
         }
      // Do indication if only one of several siblings is shown
      if (hiddenSiblings) {
         igc.setPenType(IGraphicBundle::dot);
         iglTemp.setStartingPoint(iglTemp.startingPoint()+IPoint(isHori?0:-10,isHori?-10:0))
                .setEndingPoint  (iglTemp.endingPoint()  +IPoint(isHori?0: 10,isHori? 10:0))
                .drawOn(igc);
         igc.setPenType(IGraphicBundle::solid);
         }
      // Direct connections of every child to base line
      igc.setDrawOperation(IGraphicBundle::fillAndFrame);
      for (unsigned short i=1;i<=nochildren;i++) {
         ipTemp=pinstance->pmaninstance[2+i]->pos
                 +IPoint( isHori? -(manBoxWidth/2+4) : 0
                        , isHori? 0 : +(manBoxHeight/2+4) );
         iglTemp.setStartingPoint(IPoint(isHori?ipParents.x():ipTemp.x()
                                        ,isHori?ipTemp.y():ipParents.y()) )
                .setEndingPoint(ipTemp).drawOn(igc);
         // ... then the Arrow
         if (isHori) {
            IGPolygon igpArrow(ipaHoriArrow);
            igpArrow.translateBy(ipTemp)
                    .setTransformMethod(IGraphic::leftMultiply)
                    .drawOn(igc);
            }
         else {
            IGPolygon igpArrow(ipaVertArrow);
            igpArrow.translateBy(ipTemp)
                    .setTransformMethod(IGraphic::leftMultiply)
                    .drawOn(igc);
            }
         // Done w painting
         }
      }
   // Diagonal
   else {
      // Check for only ONE child visible
      if (hiddenSiblings) {
         igc.setPenType(IGraphicBundle::dot);
         iglTemp.setStartingPoint(ipParents+IPoint(isHori?0:-4,isHori?-4:0))
                .setEndingPoint  (ipParents+IPoint(isHori?+(distGenerations-manBoxWidth )/2:-8
                                                  ,isHori?-8:-(distGenerations-manBoxHeight)/2) )
                .drawOn(igc);
         iglTemp.setStartingPoint(ipParents+IPoint(isHori?0:4,isHori?4:0))
                .setEndingPoint  (ipParents+IPoint(isHori?+(distGenerations-manBoxWidth )/2:8
                                                  ,isHori?8:-(distGenerations-manBoxHeight)/2) )
                .drawOn(igc);
         igc.setPenType(IGraphicBundle::solid);
         // Continue
         }
      // Check for ChildConnections
      igc.setDrawOperation(IGraphicBundle::fillAndFrame);
      for (unsigned short i=1;i<=nochildren;i++) {
         ipTemp=pinstance->pmaninstance[2+i]->pos
               +IPoint(isHori?-(manBoxWidth/2+4):0
                      ,isHori?0:+(manBoxHeight/2+4));
         long offset=0;
         if (nochildren>1)
            offset = max(min(-4*((long)nochildren+1)+8*i
                            ,(isHori?manBoxHeight/2:manBoxWidth/2))
                        ,-(isHori?manBoxHeight/2:manBoxWidth/2));
         iglTemp.setStartingPoint(ipParents
                                 +IPoint(isHori?0:offset,isHori?offset:0))
                .setEndingPoint(ipTemp)
                .drawOn(igc);
         // ... then the Arrow
         double gkatete=isHori?(ipTemp-iglTemp.startingPoint()).coord2() : (ipTemp-iglTemp.startingPoint()).coord1(),
                akatete=isHori?(iglTemp.startingPoint()-ipTemp).coord1() : (iglTemp.startingPoint()-ipTemp).coord2(),
                grad=360*atan(gkatete/akatete)/(2*PI);
         if (isHori) {
            IGPolygon igpArrow(ipaHoriArrow);
            igpArrow.translateBy(ipTemp).setTransformMethod(IGraphic::leftMultiply)
                    .rotateBy(-grad,ipTemp)
                    .drawOn(igc);
            }
         else {
            IGPolygon igpArrow(ipaVertArrow);
            igpArrow.translateBy(ipTemp).setTransformMethod(IGraphic::leftMultiply)
                    .rotateBy(grad,ipTemp)
                    .drawOn(igc);
            }
         // Next
         }
      }
   // Done
   return(pinstance+3+nochildren);
}

PNPersonINSTANCE NManTree::drawMan(PNPersonINSTANCE pinstance
                              ,IGraphicContext& igc
                              ,IRectangle& visibleBox
                              ,Boolean isPrint)
{
   // Statics
   static IGRectangle igrTemp;
   static IGString igsTemp("");

   // Visiblility
   if ( (pinstance->pos.x()<visibleBox.minX())
      ||(pinstance->pos.x()>visibleBox.maxX())
      ||(pinstance->pos.y()<visibleBox.minY())
      ||(pinstance->pos.y()>visibleBox.maxY()) )
      return(pinstance+1);
   IRectangle box(pinstance->pos+IPoint(-(manBoxWidth/2),-(manBoxHeight/2))
                 ,ISize(manBoxWidth,manBoxHeight));
   // Check if actual
   if ((!isPrint)&&(pNMankind->getActual()==pinstance->pman)) {
      // Add Red Outer Box
      igc.setDrawOperation(IGraphicBundle::frame)
         .setPenColor(IColor(getColor(NManTree::emphasis)));
      igrTemp.setEnclosingRect(IRectangle(pinstance->pos+IPoint(-(manBoxWidth/2+4),-(manBoxHeight/2+4))
                              ,ISize(manBoxWidth+2*4,manBoxHeight+2*4)))
             .drawOn(igc);
      // Add RedShaded Outer Box (Filling S3-bug -> width -1)
      igc.setDrawOperation(IGraphicBundle::fill)
         .setFillColor(IColor(getColor(NManTree::emphasis)))
         .setFillPattern(IGraphicBundle::dense5);
      igrTemp.setEnclosingRect(IRectangle(pinstance->pos+IPoint(-(manBoxWidth/2+4),-(manBoxHeight/2+4)),
                               ISize(manBoxWidth+2*4-1,manBoxHeight+2*4)))
             .drawOn(igc);
      // Add White Inner Box
      igc.setDrawOperation(IGraphicBundle::fill)
         .setFillColor(IColor(getColor(NManTree::back)))
         .setFillPattern(IGraphicBundle::solid);
      igrTemp.setEnclosingRect(box)
             .drawOn(igc);
      }
   // First surrounding Box
   igc.setDrawOperation(IGraphicBundle::frame);
   if (!isPrint) {
      if (pNMankind->getOrigin()==pinstance->pman)
         igc.setPenColor(IColor(getColor(NManTree::emphasis)));
      else
         igc.setPenColor(IColor(getColor(NManTree::boxes)));
      }
   if (pinstance->pman->getSex()==2) igrTemp.setRounding(IPair(7,7));
   igrTemp.setEnclosingRect(box)
          .drawOn(igc);
   igrTemp.setRounding();
   // Add Text : Name
   if ((!isPrint)&&(pNMankind->getOrigin()!=pinstance->pman))
      igc.setPenColor(IColor(getColor(NManTree::texts)));
   igsTemp.setText(pinstance->pman->getName())
          .moveTo(pinstance->pos+IPoint(-manBoxWidth/2+4,fontLineHeight/2+fontBaseOffset))
          .setClippingRect(box)
          .drawOn(igc);
   // FirstName
   if ((getDetail()&NManTree::firstName)&&
       (pinstance->pman->getFirstName().length()>0)) {
      igsTemp.setText(pinstance->pman->getCallFirstName())
             .moveTo(pinstance->pos+IPoint(-manBoxWidth/2+4,-fontLineHeight/2+fontBaseOffset))
             .setClippingRect(box)
             .drawOn(igc);
      }
   // Birth+Death-Date
   if (getDetail()&NManTree::dates) {
      IPoint pos=pinstance->pos+IPoint(-manBoxWidth/2+4,-fontLineHeight*3/2+fontBaseOffset);
      if (pinstance->pman->getBirthDate().hasInfo())
         pinstance->pman->getBirthDate().drawOn(igc,pos,ifont,igsTemp);
      if (pinstance->pman->getDeathDate().hasInfo())
         pinstance->pman->getDeathDate().drawOn(igc,pos+IPoint(NDate::drawSize(ifont).width()+8,0),ifont,igsTemp);
      // Done with Dates
      }
   // Done
   return(pinstance+1);
}


// Mankind Handler Methods
void NManTree::collectParm(IString& isParm)
{
   isParm+=IString(":TREE")
          +" ZOOM"+IString(xyZoom)+
          +" DIR"+IString(getDir()==horizontal?'H':'V');
   if (getDepth(ancestors)!=(unsigned short)-1)
      isParm+=" #A"+IString(getDepth(ancestors));
   if (getDepth(descendants)!=(unsigned short)-1)
      isParm+=" #D"+IString(getDepth(descendants));
   if (getDepth(siblings)!=(unsigned short)-1)
      isParm+=" #S"+IString(getDepth(siblings));
}
void NManTree::spreadParm (const IString& isParm)
{
   // Calc my part
   unsigned idx=isParm.indexOf(":TREE");
   if (idx==0) return;
   IString isForMe(isParm.subString(idx+5));
   idx=isForMe.indexOf(':');
   if (idx!=0) isForMe.remove(idx);
   // Check for Dir
   idx=isForMe.indexOf("DIR");
   dir=horizontal;
   if (idx!=0) {
      switch (isForMe[idx+3]) {
        case 'V': dir=vertical;break;
        case 'H': dir=horizontal;break;
        }
      }
   // Check for Zoom
   xyZoom=1;
   idx=isForMe.indexOf("ZOOM");
   if (idx!=0) {
      xyZoom=isForMe.subString(idx+4).asDouble();
      if ((xyZoom>1)||(xyZoom<=0)) xyZoom=1;
      }
   // Check for number of Ancestors
   idx=isForMe.indexOf("#A");
   if (idx!=0) depthAncestors=(unsigned short)isForMe.subString(idx+2).asInt();
   else        depthAncestors=(unsigned short)-1;
   // Check for number of Descendants
   idx=isForMe.indexOf("#D");
   if (idx!=0) depthDescendants=(unsigned short)isForMe.subString(idx+2).asInt();
   else        depthDescendants=(unsigned short)-1;
   // Check for number of Descendants
   idx=isForMe.indexOf("#S");
   if (idx!=0) depthSiblings=(unsigned short)isForMe.subString(idx+2).asInt();
   else        depthSiblings=(unsigned short)-1;
   // Done
}
void NManTree::structureChanged()
{
   // do Tree
   buildTree();
}
void NManTree::manAdded(PNPerson which,Boolean more)
{
   if (more) return;
   // do Tree
   buildTree();
}
void NManTree::manAdded(SEQOFNPerson& which)
{
   // do Tree
   buildTree();
}
void NManTree::disconnentMen()
{
   // do Tree
   buildTree();
}
void NManTree::manChanged(NMankindHandler* pInitiator)
{
   if (!isShowing()) return;
   // Get a Context
   IGraphicContext igc(drawingArea.handle());
   ITransformMatrix itm;
   itm.translateBy(ipTranslate/xyZoom);
   itm.scaleBy    (xyZoom,xyZoom);
   igc.setWorldTransformMatrix(itm)
      .setFont(getFont());
   // Do GraphicBundle
   IGRectangle igrTemp;
   IRectangle irSmall(-IPoint(manBoxWidth/2,manBoxHeight/2)
                     ,ISize(manBoxWidth,manBoxHeight));
   IRectangle irBig  (-IPoint(manBoxWidth/2+4,manBoxHeight/2+4)
                     ,ISize(manBoxWidth+8,manBoxHeight+8));
   // Calculate drawingbox
   IRectangle box(viewWindowDrawRectangle());
   box.moveBy(-ipTranslate)
      .scaleBy(1/xyZoom,1/xyZoom)
      .expandBy(IPair(manBoxWidth/2+4,manBoxHeight/2+4));
   // Search for changed MEN
   PNPersonINSTANCE pinstance=pManInstances;
   for (unsigned short i=1;i<=numberOfManInstances;i++) {
      // ... found
      if (pinstance->pman->changedThisTime()) {
         // Do a white fill
         igc.setDrawOperation(IGraphicBundle::fillAndFrame)
            .setFillColor    (IColor(getColor(back)))
            .setPenColor     (IColor(getColor(back)));
         igrTemp.setEnclosingRect(irBig.movedBy(pinstance->pos))
                .drawOn(igc);
         // Do man drawing
         drawMan(pinstance,igc,box);
         }
      // Next
      pinstance++;
      }
   // Check RELs
   igc.setPenColor (IColor(getColor(NManTree::lines)));
   igc.setFillColor(IColor(getColor(NManTree::lines)));
   PRELINSTANCE prelinstance=pRelInstances;
   while (prelinstance!=pRelInstances+numberOfRelInstances) {
      // Family with only one child
      if (prelinstance->pmaninstance[0]==NULL) {
         if (prelinstance->pmaninstance[1]->pman->changedThisTime())
            drawRelation(prelinstance,igc,box);
         prelinstance+=2;
         }
      // Normal Family
      else {
         Boolean draw=False;
         // Father or Mother
         if ( (prelinstance->pmaninstance[0]->pman->changedThisTime())
            ||(prelinstance->pmaninstance[1]->pman->changedThisTime()))
            draw=True;
         // Children
         unsigned long nochildren
            =((unsigned long)prelinstance->pmaninstance[2])&~(FLAGALLHIDDEN|FLAGONLYONE);
         for (int l=0;l<nochildren;l++)
            if (prelinstance->pmaninstance[3+l]->pman->changedThisTime()) draw=True;
         // Draw ?
         if (draw) drawRelation(prelinstance,igc,box);
         prelinstance+=2+1+nochildren;
         }
      // Next
      }
   // Done
}
void NManTree::actualChanged(PNPerson pOld,PNPerson pNew)
{
   if (!isShowing()) return;
   // Get a Context
   IGraphicContext igc(drawingArea.handle());
   ITransformMatrix itm;
   itm.translateBy(ipTranslate/xyZoom);
   itm.scaleBy    (xyZoom,xyZoom);
   igc.setWorldTransformMatrix(itm)
      .setFont(getFont());
   // Do GraphicBundle
   IGRectangle igrTemp;
   IRectangle irSmall(-IPoint(manBoxWidth/2,manBoxHeight/2)
                     ,ISize(manBoxWidth,manBoxHeight));
   IRectangle irBig  (-IPoint(manBoxWidth/2+4,manBoxHeight/2+4)
                     ,ISize(manBoxWidth+8,manBoxHeight+8));
   // Calculate drawingbox
   IRectangle box(viewWindowDrawRectangle());
   box.moveBy(-ipTranslate)
      .scaleBy(1/xyZoom,1/xyZoom)
      .expandBy(IPair(manBoxWidth/2+4,manBoxHeight/2+4));
   // Search for MEN (first old then new)
   PNPerson pFind=pOld;
   for (short i=1;i<=2;i++) {
      PNPersonINSTANCE pinstance=pManInstances;
      for (unsigned short man=1;man<=numberOfManInstances;man++) {
         // found OLD
         if (pinstance->pman==pFind) {
            // Do a white fill
            igc.setDrawOperation(IGraphicBundle::fillAndFrame)
               .setFillColor    (IColor(getColor(back)))
               .setPenColor     (IColor(getColor(back)));
            igrTemp.setEnclosingRect(irBig.movedBy(pinstance->pos))
                   .drawOn(igc);
            // Do man drawing
            drawMan(pinstance,igc,box);
            //  Done with this
            break;
            }
         // Next
         pinstance++;
         }
      // Search for new MEN
      pFind=pNew;
      }
   // Check RELs
   igc.setPenColor (IColor(getColor(NManTree::lines)));
   igc.setFillColor(IColor(getColor(NManTree::lines)));
   PRELINSTANCE prelinstance=pRelInstances;
   while (prelinstance!=pRelInstances+numberOfRelInstances) {
      // Family with only one child
      if (prelinstance->pmaninstance[0]==NULL) {
         if (prelinstance->pmaninstance[1]->pman==pOld)
            drawRelation(prelinstance,igc,box);
         prelinstance+=2;
         }
      // Normal Family
      else {
         Boolean draw=False;
         // Father or Mother
         if ( (prelinstance->pmaninstance[0]->pman==pOld)
            ||(prelinstance->pmaninstance[1]->pman==pOld))
            draw=True;
         // Children
         unsigned long nochildren
            =((unsigned long)prelinstance->pmaninstance[2])&~(FLAGALLHIDDEN|FLAGONLYONE);
         for (int l=0;l<nochildren;l++)
            if (prelinstance->pmaninstance[3+l]->pman==pOld) draw=True;
         // Draw ?
         if (draw) drawRelation(prelinstance,igc,box);
         prelinstance+=2+1+nochildren;
         }
      // Next
      }
   // Done
}
void NManTree::originChanged(PNPerson pOld,PNPerson pNew)
{
   // do Tree
   buildTree();
}

// SizeHandler Methods
Boolean NManTree::windowResize(IResizeEvent& ireEvent)
{
   // Resize drawingArea if not minimized
   if ((ireEvent.newSize().height()>0)||(ireEvent.newSize().width()>0)) {
      // Resize drawingArea to Size of view+tree
      drawingArea.sizeTo(ireEvent.newSize()+isTree.scaledBy(xyZoom,xyZoom));
      // Translate WorldTransformMatrix
      ipTranslate=size()/2;
      }
   // Let system handler do the rest
   return(False);
}

// Options Methods
void NManTree::changeZoom(const double xySetZoom)
{
   // Set Param
   if ((xyZoom<=0)||(xyZoom>1)) { xyZoom=1; }
   xyZoom=xySetZoom;
   // Stop update
   disableUpdate();
   // CenterPosition of ViewPort - left/lower border = Position in mantree
   IPoint ipCenter=viewWindowDrawRectangle().centerXCenterY()
                   -IPoint(size().width()/2,size().height()/2);
   double xratio=((double)ipCenter.x())
                  / (viewWindowSize().width()-size().width()) ;
   xratio=max(0,min(1,xratio));
   double yratio=((double)ipCenter.y())
                 / (viewWindowSize().height()-size().height()) ;
   yratio=max(0,min(1,yratio));
   // Recalc ViewPort + Redraw
   drawingArea.sizeTo(size()+isTree.scaledBy(xyZoom,xyZoom));
   setLayoutDistorted(IWindow::layoutChanged|IWindow::immediateUpdate,0);
   // Scroll Back: ViewSize/2 + MankindSize*ratio - ViewSize/2
   scrollViewVerticallyTo
           ( viewWindowSize().height()
           - size().height()
           - yratio * (viewWindowSize().height()-size().height()) );
   scrollViewHorizontallyTo
           ( xratio * (viewWindowSize().width()-size().width()) );
   // Done so re-update
   enableUpdate();
}
double NManTree::getZoom()
{
   return(xyZoom);
}

Boolean NManTree::changeDepth(Depth which,unsigned short depth)
{
   // Check for new Value
   switch (which) {
      case ancestors       : depthAncestors  =depth;break;
      case descendants     : depthDescendants=depth;break;
      case siblings        : depthSiblings   =depth;break;
      }
   // Recalc ViewPort + Redraw
   if (pNManTree) pNManTree->buildTree();
   // Done
   return(True);
}
unsigned short NManTree::getDepth(Depth which)
{
   switch (which) {
      case ancestors       : return(depthAncestors  ); break;
      case descendants     : return(depthDescendants); break;
      case siblings        : return(depthSiblings   ); break;
      case totalancestors  : return(totalAncestors  ); break;
      case totaldescendants: return(totalDescendants); break;
      }
   return(0);
}

Boolean NManTree::changeLine(Line setLine)
{
   // Check for new value
   switch (setLine) {
      case diagonal   : break;
      case orthogonal : break;
      default         : setLine=diagonal;
      }
   line=setLine;
   // Redraw
   if (pNManTree) pNManTree->refresh();
   // Done
   return(True);
}
NManTree::Line NManTree::getLine()
{
   return(line);
}
Boolean NManTree::changeFont(const IFont& setFont)
{
   ifont=setFont;
   // Recalc ViewPort + Redraw
   if (pNManTree) pNManTree->buildTree();
   return(True);
}
IFont& NManTree::getFont  ()
{
   return(ifont);
}
Boolean NManTree::changeColor(Color which,IColor::Color setColor)
{
   switch (which) {
      case lines   : colorlines   =setColor;break;
      case boxes   : colorboxes   =setColor;break;
      case emphasis: coloremphasis=setColor;break;
      case texts   : colortexts   =setColor;break;
      case back    : colorback    =setColor;break;
      default      : return(False);
      }
   // Redraw
   if (pNManTree) pNManTree->refresh();
   // Done
   return(True);
}
Boolean NManTree::resetColors()
{
   changeColor(back    ,IColor::white   );
   changeColor(lines   ,IColor::darkCyan);
   changeColor(boxes   ,IColor::darkCyan);
   changeColor(emphasis,IColor::red     );
   changeColor(texts   ,IColor::darkCyan);
   return(True);
}
IColor::Color NManTree::getColor (Color which)
{
   switch (which) {
      case lines   : return(colorlines);
      case boxes   : return(colorboxes);
      case emphasis: return(coloremphasis);
      case texts   : return(colortexts);
      }
   //case back:
   return(colorback);
}
Boolean NManTree::changeDetail(Detail setDetail,Detail delDetail)
{
   setDetail=Detail(setDetail&Detail(firstName+dates+pageBorder));
   delDetail=Detail(delDetail&Detail(firstName+dates+pageBorder));
   detail=NManTree::Detail(detail&(-1^delDetail));
   detail=NManTree::Detail(detail|setDetail);
   // Redraw
   if (pNManTree) pNManTree->refresh();
   // Done
   return(True);
}
NManTree::Detail NManTree::getDetail()
{
   return(detail);
}
Boolean NManTree::changeDir(Dir setDir)
{
   // Check for new Value
   if (setDir==dir) return(False);
   switch (setDir) {
      case horizontal : break;
      case vertical   : break;
      default         : setDir=horizontal;
      }
   dir=setDir;
   // Recalc ViewPort + Redraw
   if (pNManTree) pNManTree->buildTree();
   // Done
   return(True);
}
NManTree::Dir NManTree::getDir()
{
   return(dir);
}

PNPerson NManTree::calcManToDrag(IPoint where)
{
   PNPerson found=findMan(where);
   if (found) pNMankind->makeActual(found);
   return(found);
}

const IString& NManTree::getLastFileDrop()
{
   return(isLastFileDrop);
}

void NManTree::fileDropped(IString isFile)
{
   isLastFileDrop=isFile;
   owner()->postEvent(IWindow::command,MI_FileOpenDropped);

}


