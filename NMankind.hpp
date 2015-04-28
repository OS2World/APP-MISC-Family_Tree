/*
   File   : Mankind.hpp
   Purpose: Define Classes for Handling of people
*/

#ifndef _NNPersonKIND_HPP
#define _NNPersonKIND_HPP

#include <iostream.h>
#include <iseq.h>
#include <ibaghks.h>

#include <igrect.hpp>
#include <igline.hpp>
#include <igstring.hpp>
#include <icolor.hpp>
#include <igrafctx.hpp>
#include <iptarray.hpp>
#include <igpyline.hpp>
#include <igelipse.hpp>
#include <ifont.hpp>
#include <ireslib.hpp>
#include <fstream.h>
#include <ipainhdr.hpp>
#include <ikeyhdr.hpp>
#include <iobservr.hpp>
#include <inotify.hpp>
#include <inotifev.hpp>
#include <ihandle.hpp>
#include <itextctl.hpp>
#include <iprofile.hpp>

#include <math.h>

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

class NEvent;
class NPerson;
class NMarriage;

class NMankind;
class NMankindHandler;

typedef NMankind*        PNMankind;
typedef NMankindHandler* PNMankindHandler;
typedef NPerson*             PNPerson;
typedef PNPerson*            PPNPerson;
typedef NMarriage*        PREL;
typedef PREL*            PPREL;
typedef NEvent*           PNEvent;
typedef PNEvent*          PPNEvent;

typedef unsigned long GEDCOMID;

#include "NMisc.hpp"
#include "NPointersTo.h"

#define MAXDEPTH 30
#define MAXVIEWMODE 3

#define ZOOMSTEPS 8

// DAT for SaveData
#define PDAT_NAME       'A'
#define PDAT_FIRSTNAME  'B'
#define PDAT_BIRTHPLACE 'C'
#define PDAT_BIRTHDATE  'D'
#define PDAT_MEMO       'E'
#define PDAT_BITMAP     'F'
#define PDAT_DEATHDATE  'G'
#define PDAT_DEATHPLACE 'H'
#define PDAT_SEX        'I'
#define PDAT_ID         'J'
#define PDAT_OCCUPATION 'K'
#define PDAT_ADDRESS    'L'
#define PDAT_FILES      'M'
#define PDAT_USERFIELDS 'N'
#define PDAT_END         0

#define MDAT_MARRIAGEDATE  'A'
#define MDAT_DIVORCEDATE   'B'
#define MDAT_MARRIAGEPLACE 'C'
#define MDAT_ID            'D'
#define MDAT_END            0

#define UDAT_YESNO       'A'
#define UDAT_TEXT        'B'
#define UDAT_NUMBER      'C'
#define UDAT_DATE        'D'
#define UDAT_END          0

#define EDAT_NAME        'A'
#define EDAT_FROM        'B'
#define EDAT_TO          'C'
#define EDAT_PLACE       'D'
#define EDAT_DESCRIPTION 'E'
#define EDAT_INVOLVED    'F'
#define EDAT_END          0

//#define PARM_VIEW      'A'
//#define PARM_DETAIL    'B'
#define PARM_ACTUAL      'C'
#define PARM_ORIGIN      'D'
//#define PARM_DIRECTION   'E'
//#define PARM_ZOOM        'F'
//#define PARM_ANCESTORS   'G'
//#define PARM_DESCENDANTS 'H'
//#define PARM_SIBLINGS    'I'
#define PARM_HANDLER     'J'
#define PARM_END          0


// Class  : NEvent
// Purpse : Encapsulate an event
class NEvent
{
   friend class NMankind;

   class ManAndRole {
     public:
       PNPerson    pMan;
       IString isRole;
     };
public:
   NEvent();
   ~NEvent();

          void    saveTo  (ostream& oStream);
   static NEvent* loadFrom(istream& iStream,PNPerson* ppPersons,short sCountPersons);


   NEvent& setName       (const IString& isSetName       );
   NEvent& setFrom       (const NDate&   ndSetFrom       );
   NEvent& setTo         (const NDate&   ndSetTo         );
   NEvent& setPlace      (const IString& isSetPlace      );
   NEvent& setDescription(const IString& isSetDescription);

   const IString& getName       ();
   const NDate&   getFrom       ();
   const NDate&   getTo         ();
   const IString& getPlace      ();
   const IString& getDescription();

   NEvent&  addMan(PNPerson pmanWhich,const IString isRole=IString());
   NEvent&  delMan(short sWhich);
   NEvent&  delMan(PNPerson pmanWhich);
   NEvent& swapMan(short sOne,short STwo);
   short   numMan();
   PNPerson    getMan(short sWhich);
   IString getRoleOfMan   (PNPerson pmanWhich);
   IString getRoleOfMan   (short sWhich);
   NEvent&  setRoleOfMan   (short sWhich,const IString& isRole);
   NEvent&  changeRoleOfMan(PNPerson pmanWhich,const IString isRole=IString());
   NEvent&  changeRoleOfMan(short sWhich,const IString isRole=IString());

   void    markChanged();
   Boolean changedThisTime();
private:
   IString isName;
   NDate   ndFrom;
   NDate   ndTo;
   IString isPlace;
   IString isDescription;
   long    changeStamp;

   NPointersTo<ManAndRole> nptManAndRole;

   static Boolean changed;
};


// Classs : NUserField
// Purpose: encapsulate multiple field types (text,number,yesno,date)
class NUserField {
public:
   enum YesNo { Unknown, Yes, No};
   virtual ~NUserField();
   void save(short fieldIdx,ostream& oStream);
   static NUserField* load(short fieldIdx,istream& iStream);
   IString getValue(short fieldIdx);
};

class NUserFieldText : public NUserField {
public:
   NUserFieldText(const IString& isSet);
   IString getText();
private:
   IString isStat;
};

class NUserFieldYesNo : public NUserField {
public:
   NUserFieldYesNo(YesNo ynSet);
   YesNo getYesNo();
private:
   YesNo ynStat;
};

class NUserFieldNumber : public NUserField {
public:
   NUserFieldNumber(long lSet);
   long getNumber();
private:
   long lStat;
};

class NUserFieldDate : public NUserField {
public:
   NUserFieldDate(const NDate& ndSet);
   NDate getDate();
private:
   NDate ndStat;
};

// Class  : NPerson
// Purpose: Handle a NPerson
class NPerson
{
   friend ostream& operator<< (ostream& oStream, NPerson& person);
   friend istream& operator>> (istream& iStream, NPerson& person);
   friend class NMankind;
   friend class NMarriage;
public:
   NPerson();
   NPerson(const IString &isSet);
   virtual ~NPerson();

   void setMid(GEDCOMID newMid);
   void setName      (const IString& isSetName);
   void setFirstName (const IString& isSetFirstName);
   void setSex       (const char cSetSex);
   void setBirthDate (const NDate&    dSetBirthDate);
   void setBirthPlace(const IString& isSetBirthPlace);
   void setDeathDate (const NDate&    dSetDeathDate);
   void setDeathPlace(const IString& isSetDeathPlace);
   void setMarriageDate(unsigned short int iNo,const NDate& dSetMarriageDate);
   void setMarriagePlace(unsigned short int iNo,const IString& isSetPlace);
   void setDivorceDate(unsigned short int iNo,const NDate& dSetDivorceDate);
   void setOccupation(const IString& isSetOccupation);
   void setAddress   (const IString& isSetAddress);
   void setMemo      (const IString& isSetMemo);
   void setBitmap    (const IString isSetBitmap);
   void addFile      (const IString& isFile);
   void delFile      (unsigned short iFile);
   void delAllFiles  ();
   NPerson& setShort (short int iSet);
   void setVisible   (Boolean bSet);

   void   addEvent   (NEvent* pEvent);
   PNEvent getEvent  (short which);
   void   delEvent   (NEvent* pEvent);
   short  numEvents  ();

   GEDCOMID       getMid() const;
   IString        getPartnerName(unsigned short int iNo);
   const IString& getName();
   const IString& getFirstName();
   const char     getSex();
   const IString& getBirthPlace();
   const NDate&    getBirthDate();
   const IString& getDeathPlace();
   const NDate&    getDeathDate();
   const IString& getOccupation();
   const IString& getAddress();
   const IString& getMemo();
   const NDate&    getMarriageDate(unsigned short int iNo);
   const IString& getMarriagePlace(unsigned short int iNo);
   const NDate&    getDivorceDate(unsigned short int iNo);
   short int      getNoOfPartners();
   PREL           getPartnership(unsigned short int iPartner,Boolean reverse=False);
   IString&       getBitmap();
   unsigned short getNoOfFiles();
   IString*       getFile(unsigned short iFile);
   short int      getShort();
   IString        getCallFirstName();
   IString        getNameDotFirstName();
   short int      getNoOfChildrenWith(unsigned short int iPartner);
   Boolean        visible() const;

   Boolean hasPartners();
   Boolean hasPartner(PNPerson pmanCheck);
   Boolean hasParents();
   Boolean hasChildren();
   Boolean hasChild(PNPerson pchild,unsigned short marriage);

   Boolean positionInFamily();
   Boolean positionPartners();

   void addChildhood(NMarriage* pSet);
   PREL getChildhood();
   void removeChildhood();
   void addPartnership(NMarriage* pSet);
   void removePartnership(NMarriage* pDel);

   short getGenerationsOfAncestors  ();
   short getGenerationsOfDescendants();

   void markChanged();
   Boolean changedThisTime();

   int         getNoOfUserFields();
   NUserField* getUserField(short idx);
   void        setUserField(NUserField* set,short idx);
   void        delUserField(short idx);
   void        delAllUserFields();

private:
   NPointersTo<NMarriage>   nptPartnerships;
   PREL                    pChildhood;
   char                    cSex;
   IString                 isName;
   IString                 isFirstName;
   IString                 isBirthPlace;
   IString                 isDeathPlace;
   NDate                   dBirthDate;
   NDate                   dDeathDate;
   IString                 isOccupation;
   IString                 isAddress;
   IString                 isMemo;
   short int               iShort;
   GEDCOMID                mid;
   IString                 isBitmap;
   NPointersTo<IString>    nptFiles;
   NPointersTo<NEvent>      nptEvents;
   long                    changeStamp;
   NPointersTo<NUserField> nptUserFields;

   static Boolean changed;
};

ostream& operator<< (ostream& oStream, NPerson& man);
istream& operator>> (istream& iStream, NPerson& man);

// Class  : NMarriage
// Purpose: Handle NMarriage between PARTNERS and their CHILDs
class NMarriage
{
   friend ostream& operator<< (ostream& oStream, NMarriage& rel);
   friend istream& operator>> (istream& iStream, NMarriage& rel);
   friend class NMankind;
public:
   NMarriage();
   NMarriage(PNPerson pmanSetPartner1,PNPerson pmanSetPartner2);
   NMarriage(PNPerson pmanChild,PNPerson pmanSetParent1,PNPerson pmanSetParent2);
   ~NMarriage();

   short int getNoOfChildren();
   void      changePartner(unsigned short iPos,NPerson* pSetPartner,Boolean notifyOld=True,Boolean notifyNew=True);
   void      changePartner(PNPerson pPartner,PNPerson pSetPartner,Boolean notifyOld=True,Boolean notifyNew=True);
   PNPerson      getPartner(unsigned short int iGetNumber);
   PNPerson      getOther(PNPerson pOfThis);
   PNPerson      getChild(unsigned short int iGetNumber);
   void      addChildrenFrom(PREL pfrom);
   void      addChild(PNPerson pNewChild);
   void      removeChild(PNPerson pChild);
   void      removeChildren();
   void      changeChild(unsigned short int iChange,PNPerson pSetChild,Boolean notifyOld=True,Boolean notifyNew=True);
   void      removePartner(PNPerson pPartner);
   Boolean   hasChild(PNPerson pchild);

   Boolean   importAddPartner(unsigned long id);
   Boolean   importAddChild(unsigned long id);

   void           setMarriageDate(const NDate& dSetMarriageDate);
   void           setMarriagePlace(const IString& isSetPlace);
   void           setDivorceDate(const NDate& dSetDivorceDate);
   const NDate&    getMarriageDate();
   const IString& getMarriagePlace();
   const NDate&    getDivorceDate();

   NMarriage&         setShort(short int iSet);
   short int         getShort();
   void              setFid(GEDCOMID newFid);
   unsigned long     getFid() const;

   Boolean   positionChildren();
private:
   PNPerson             pPartners[2];
   NPointersTo<NPerson> nptChildren;
   NDate            dMarriageDate;
   IString          isMarriagePlace;
   NDate            dDivorceDate;
   short int        iShort;
   GEDCOMID         fid;
};
ostream& operator<< (ostream& oStream, NMarriage& rel);
istream& operator>> (istream& iStream, NMarriage& rel);

// Class  : NMankind
// Purpose: Handle all NPerson in one space

typedef ISequence <PNPerson> SEQOFNPerson;
typedef ISequence <PREL> SEQOFNMarriage;
typedef ISequence <PNEvent> SEQOFNEvent;
typedef ISequence <PNMankindHandler> SEQOFHANDLER;
typedef IBagOnHashKeySet <GEDCOMID> HASHOFID;

class NMankind
{
   friend class NPerson;
   friend class NMarriage;
   friend class NEvent;
public:
   enum Assist { noAssistance         =0
               , addPartnerSameAddress=1
               , addPartnerOppositeSex=2
               , addChildSameAddress  =4
               , addChildSameName     =8
               , addParentsMaleFemale =16
               , addParentsSameName   =32
               , addParentsSameAddress=64};

   enum FieldType { Unknown, YesNo, Text, Number, Date };

   class UserField {
     public:
       IString   isName;
       FieldType type;
     };

   static NMankind* pNMankind;

   static void loadFromProfile(IProfile& profile);
   static void saveToProfile(IProfile& profile);

   NMankind();
   virtual ~NMankind();

   void    addMankindHandler   (NMankindHandler* handler);
   void    removeMankindHandler(NMankindHandler* handler);

   Boolean addManChild(unsigned short int iPartner,PNPerson pmanChild=NULL);
   Boolean addManPartner(PNPerson pmanPartner=NULL);
   Boolean addManParents(const IString& isMom,const IString& isDad);
   Boolean deleteMan();
   Boolean deletePartner(unsigned short int iPartner);
   Boolean deleteChilds(unsigned short int iPartner);
   Boolean deleteParents();

   unsigned short numChildGenerations (PNPerson pwhich);
   unsigned short numParentGenerations(PNPerson pwhich);

   IString   getUserFieldName (short which);
   FieldType getUserFieldType (short which);
   void      addUserField     (const IString& isName,FieldType type);
   void      changeUserField  (short which,const IString& isName);
   void      delUserField     (short which);
   short     getNoOfUserFields();

   void    beginChange();
   void    endChange(NMankindHandler* pInitiator=NULL);

   void    newMankind();

   Boolean makeActual(PNPerson pmanSetActual);
   void    remember();
   PNPerson    getRemembered();
   PNPerson    getActual();
   PNPerson    getOrigin();
   Boolean makeOrigin();
   Boolean isOrigin(PNPerson pmanTest);

   static Boolean          changeAssist(Assist setAssist,Assist delAssist  );
   static NMankind::Assist getAssist();

   void      checkIDs();
   Boolean   save(fstream& fsFile);
   Boolean   load(fstream& fsFile);
   Boolean   checkHeader(fstream& fsFile);
   Boolean   hasChanged();
   short int getManCount();
   short int getRelCount();

   PNPerson*   mallocManArray(short int& idxActual,short int& lenArray);
   PREL*   mallocRelationArray(short int& idxActual,short int& lenArray);
   PNPerson    importMan();
   PREL    importRelation();
   Boolean importDone(IString* pisReport,Boolean canContinue,Boolean isContinue=False);
   void    importConnect();
   void    importCheck();

   void    addEvent(const IString& isName=IString(),const NDate& ndFrom=NDate(),const NDate& ndTo=NDate(),const IString& isPlace=IString(),const IString& isDescription=IString());
   void    delEvent(PNEvent pWhich);
   short   getNoOfEvents();
   PNEvent  getActualEvent();
   PNEvent  makeActualEvent(PNEvent peventSetActual);
private:
   SEQOFHANDLER seqHandler;

   SEQOFNPerson seqMan;
   SEQOFNMarriage seqRel;
   SEQOFNPerson seqManImport;
   SEQOFNMarriage seqRelImport;

   SEQOFNEvent seqEvent;

   NPointersTo<UserField> nptUserFields;

   void    delAllMen();
   void    delAllEvents();
   void    delAllUserFields();

   static  Boolean changed;
   static  Assist  assist;
   static  long    timeStamp;

   PNEvent   peventActual;
   PNPerson     pmanActual;
   PNPerson     pmanOrigin;
   PNPerson     pmanRemembered;
   GEDCOMID maxMid;
   GEDCOMID maxFid;
};


// Class   : NMankindHandler
// Purpose : Handler for Mankind that reacts to events
class NMankindHandler {
public:
   NMankindHandler();
   ~NMankindHandler();
   virtual void collectParm(IString& isParm);
   virtual void spreadParm(const IString& isParm);

   virtual void manChanged(NMankindHandler* pInitiator);
   virtual void eventChanged(NMankindHandler* pInitiator);

   virtual void doChangesNow();
   virtual void structureChanged();
   virtual void actualChanged(PNPerson pOld,PNPerson pNew);
   virtual void originChanged(PNPerson pOld,PNPerson pNew);
   virtual void actualEventChanged(PNEvent pOld,PNEvent pNew);

   virtual void disconnentMen();
   virtual void manAdded(PNPerson which,Boolean more);
   virtual void manAdded(SEQOFNPerson& which);
   virtual void disconnentEvents();
   virtual void eventAdded(PNEvent which,Boolean more);
   virtual void eventAdded(SEQOFNEvent& which);
   virtual void userFieldsModified();

   virtual void progressReport(const IString& isReport);
};

#endif
