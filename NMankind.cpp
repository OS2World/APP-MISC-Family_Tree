/*
   File   : mankind.cpp
   Purpose: Define Classes for Handling of people
*/

#include <imsgbox.hpp>
#include <math.h>

#include "FTree.h"
#include "Main.hpp"
#include "NMankind.hpp"

unsigned long _System DosBeep(unsigned long,unsigned long);

// #define NOTRANSLATE // Mankind stays in in lowerleft Edge

// Auxilary functions

short readShort(istream& input) {
   short sTemp; input.read((char*)&sTemp,sizeof(short)); return(sTemp);
}
void writeShort(ostream& output,short sValue) {
   output.write((char*)&sValue,sizeof(short));
}
long readLong(istream& input) {
   long  lTemp; input.read((char*)&lTemp,sizeof( long)); return(lTemp);
}
void writeLong(ostream& output,long lValue) {
   output.write((char*)&lValue,sizeof(long));
}
Boolean readHeader(istream& input,char* pC4Header) {
   // read 4 bytes header
   static char test[4];
   input.read(test,4);
   // compare it and return true if equal
   if (strncmp(test,pC4Header,4)==0)
      return(True);
   // if not correct reposition & return false
   input.sync(); // .. seekg with relative offset doesn't seem to work reliably without this
   input.seekg(-4,ios::cur);
   return(False);
}

// Class  : NEvent
// Purpse : Encapsulate an event
Boolean NEvent::changed;

NEvent::NEvent()
{
   changeStamp=0;
}
NEvent::~NEvent()
{
   // Free Space for ManAndRoles
   for (short i=1;i<=nptManAndRole.num();i++) {
      nptManAndRole.get(i)->pMan->delEvent(this);
      delete nptManAndRole.get(i);
      }
   // Done
}

NEvent& NEvent::setName       (const IString& isSetName       )
{
   // Remember data
   isName=isSetName;
   markChanged();
   // Done
   return(*this);
}

NEvent& NEvent::setFrom       (const NDate&   ndSetFrom       )
{
   // Remember data
   ndFrom=ndSetFrom;
   markChanged();
   // Done
   return(*this);
}
NEvent& NEvent::setTo         (const NDate&   ndSetTo         )
{
   // Remember data
   ndTo=ndSetTo;
   markChanged();
   // Done
   return(*this);
}
NEvent& NEvent::setPlace      (const IString& isSetPlace      )
{
   // Remember data
   isPlace=isSetPlace;
   markChanged();
   // Done
   return(*this);
}
NEvent& NEvent::setDescription(const IString& isSetDescription)
{
   // Remember data
   isDescription=isSetDescription;
   markChanged();
   // Done
   return(*this);
}

const IString& NEvent::getName       ()
{
   return(isName);
}
const NDate&   NEvent::getFrom       ()
{
   return(ndFrom);
}
const NDate&   NEvent::getTo         ()
{
   return(ndTo);
}
const IString& NEvent::getPlace      ()
{
   return(isPlace);
}
const IString& NEvent::getDescription()
{
   return(isDescription);
}

NEvent& NEvent::addMan(PNPerson pmanWhich,const IString isRole)
{
   markChanged();
   ManAndRole* pTmp=new ManAndRole;
   pTmp->pMan=pmanWhich;
   pTmp->isRole=isRole;
   nptManAndRole.add(pTmp);
   pmanWhich->addEvent(this);
   return(*this);
}
NEvent& NEvent::changeRoleOfMan(PNPerson pmanWhich,const IString isRole)
{
   // Look for man
   ManAndRole* pTmp;
   for (short i=1;i<=nptManAndRole.num();i++) {
      pTmp=nptManAndRole.get(i);
      if (pTmp->pMan==pmanWhich) break;
      }
   // change it
   if (i>nptManAndRole.num()) return(*this);
   return(changeRoleOfMan(i,isRole));
}

NEvent& NEvent::changeRoleOfMan(short sWhich,const IString isRole)
{
   markChanged();
   nptManAndRole.get(sWhich)->isRole=isRole;
   return(*this);
}

NEvent& NEvent::delMan(PNPerson pmanWhich)
{
   // Look for man
   for (short i=1;i<=nptManAndRole.num();i++) {
      if (nptManAndRole.get(i)->pMan==pmanWhich)
         return(delMan(i));
      }
   return(*this);
}

NEvent& NEvent::delMan(short sWhich)
{
   markChanged();
   if (sWhich>nptManAndRole.num()) return(*this);
   nptManAndRole.get(sWhich)->pMan->delEvent(this);
   delete nptManAndRole.get(sWhich);
   nptManAndRole.del(sWhich);

   return(*this);
}
NEvent& NEvent::swapMan(short sOne,short sTwo)
{
   markChanged();
   nptManAndRole.swap(sOne,sTwo);
   return(*this);
}
short NEvent::numMan()
{
   return(nptManAndRole.num());
}

PNPerson NEvent::getMan(short sWhich)
{
   return(nptManAndRole.get(sWhich)->pMan);
}
IString NEvent::getRoleOfMan(PNPerson pmanWhich)
{
   // Look for man
   ManAndRole* pTmp;
   for (short i=1;i<=nptManAndRole.num();i++) {
      pTmp=nptManAndRole.get(i);
      if (pTmp->pMan==pmanWhich)
         return(IString(pTmp->isRole));
      }
   return(IString());
}


IString NEvent::getRoleOfMan(short sWhich)
{
   ManAndRole* pTmp=nptManAndRole.get(sWhich);
   if (!pTmp) return(IString());
   return(IString(pTmp->isRole));
}
NEvent& NEvent::setRoleOfMan(short sWhich,const IString& isRole)
{
   markChanged();
   ManAndRole* pTmp=nptManAndRole.get(sWhich);
   if (!pTmp) return(*this);
   pTmp->isRole=isRole;
   return(*this);
}


void NEvent::markChanged() {
   NEvent::changed=True;
   changeStamp=NMankind::timeStamp;
}

Boolean NEvent::changedThisTime()
{
   return(changeStamp==NMankind::timeStamp);
}


void NEvent::saveTo(ostream& oStream)
{
   NDate dTemp;short sTemp;
   // Name information ?
   if (getName().length()>0) {
      oStream << EDAT_NAME;
      oStream << getName();
      oStream.put(EDAT_END);
      }
   // From information
   dTemp=getFrom();
   if (dTemp.hasInfo()) {
      oStream << EDAT_FROM;
      oStream << dTemp;
      }
   // To information
   dTemp=getTo();
   if (dTemp.hasInfo()) {
      oStream << EDAT_TO;
      oStream << dTemp;
      }
   // Place information ?
   if (getPlace().length()>0) {
      oStream << EDAT_PLACE;
      oStream << getPlace();
      oStream.put(EDAT_END);
      }
   // Description information ?
   if (getDescription().length()>0) {
      oStream << EDAT_DESCRIPTION;
      oStream << getDescription();
      oStream.put(EDAT_END);
      }
   // Involved information ?
   if (numMan()>0) {
      oStream << EDAT_INVOLVED;
      writeShort(oStream,numMan());
      for (short i=1;i<=numMan();i++) {
         writeShort(oStream,getMan(i)->getShort());
         oStream << getRoleOfMan(i);
         oStream.put(EDAT_END);
         }
      // .. done with involved
      }
   // End !
   oStream.put(EDAT_END);
}
NEvent* NEvent::loadFrom(istream& iStream,PNPerson* ppPersons,short sCountPersons)
{
   // Create new event
   NEvent* pnew=new NEvent();
   // Fill with data
   char dat;NDate dTemp;short sTemp;IString isTemp;
   do {
      iStream >> dat;  // Check for data
      switch (dat) {
         case EDAT_NAME:
            pnew->setName(IString::lineFrom(iStream,EDAT_END));
            break;
         case EDAT_FROM:
            iStream >> dTemp;
            pnew->setFrom(dTemp);
            break;
         case EDAT_TO  :
            iStream >> dTemp;
            pnew->setTo(dTemp);
            break;
         case EDAT_PLACE:
            pnew->setPlace(IString::lineFrom(iStream,EDAT_END));
            break;
         case EDAT_DESCRIPTION:
            pnew->setDescription(IString::lineFrom(iStream,EDAT_END));
            break;
         case EDAT_INVOLVED: {
            short numInvolved=readShort(iStream);
            for (short i=1;i<=numInvolved;i++) {
               // .. Get idx of involved person
               sTemp=readShort(iStream);
               if ((sTemp<1)||(sTemp>sCountPersons)) return(NULL);
               // .. get role of involved person
               isTemp=IString::lineFrom(iStream,EDAT_END);
               // .. add person
               pnew->addMan(ppPersons[sTemp-1],isTemp);
               // next
               }
            } break;
         case EDAT_END:
            break;
         }
      }
   while ((dat!=EDAT_END)&&(iStream.good()));
   // Done
   return(pnew);
}


// Class  : NPerson
// Purpose: Handle a NPerson
Boolean        NPerson::changed;

// Special constructor for REXX-NPerson import

NPerson::NPerson()
   : isName(),
     isFirstName(),
     isBirthPlace(),
     dBirthDate(),
     isDeathPlace(),
     dDeathDate(),
     isOccupation(),
     isAddress(),
     isMemo(),
     isBitmap()
{
   // Set NPerson::Data
   changeStamp=0;
   mid=0;
   cSex=0;
   pChildhood=NULL;
   iShort=0;
}

// Normal constructor
NPerson::NPerson(const IString &isSet)
   : isName(isSet),
     isFirstName(),
     isBirthPlace(),
     dBirthDate(),
     isDeathPlace(),
     dDeathDate(),
     isOccupation(),
     isAddress(),
     isMemo(),
     isBitmap()
{
   // Set NPerson::Data
   changeStamp=0;
   mid=0;
   cSex=0;
   pChildhood=NULL;
   iShort=0;
}
NPerson::~NPerson()
{
   // Files Array
   delAllFiles();
   // UserFields
   delAllUserFields();
   // Done
}
ostream& operator<< (ostream& oStream, NPerson& person)
{
   // Put static data
   oStream << PDAT_ID;
   writeLong(oStream,person.mid);
   // Name
   if (person.getName().size()>0) {
      oStream << PDAT_NAME << person.getName();
      oStream.put(PDAT_END);
      }
   // First name
   if (person.isFirstName.size()>0) {
      oStream << PDAT_FIRSTNAME << person.isFirstName;
      oStream.put(PDAT_END);
      }
   // Sex
   if (person.cSex!=0) {
      oStream << PDAT_SEX << person.cSex;
      }
   // Birth place
   if (person.isBirthPlace.size()>0) {
      oStream << PDAT_BIRTHPLACE << person.isBirthPlace;
      oStream.put(PDAT_END);
      }
   // Death place
   if (person.isDeathPlace.size()>0) {
      oStream << PDAT_DEATHPLACE << person.isDeathPlace;
      oStream.put(PDAT_END);
      }
   // Occupation
   if (person.isOccupation.size()>0) {
      oStream << PDAT_OCCUPATION << person.isOccupation;
      oStream.put(PDAT_END);
      }
   // Address
   if (person.isAddress.size()>0) {
      oStream << PDAT_ADDRESS << person.isAddress;
      oStream.put(PDAT_END);
      }
   // Memo
   if (person.isMemo.size()>0) {
      oStream << PDAT_MEMO << person.isMemo;
      oStream.put(PDAT_END);
      }
   // Bitmap
   if (person.isBitmap.size()>0) {
      oStream << PDAT_BITMAP << person.isBitmap;
      oStream.put(PDAT_END);
      }
   // Files
   if (person.getNoOfFiles()>0) {
      oStream << PDAT_FILES;
      for (unsigned short i=1;i<=person.getNoOfFiles();i++) {
         oStream << *person.getFile(i);
         oStream.put(PDAT_END);
         }
      oStream.put(PDAT_END);
      }
   // Birth date
   if (person.dBirthDate.hasInfo()) {
      oStream << PDAT_BIRTHDATE << person.dBirthDate;
      }
   // Death date
   if (person.dDeathDate.hasInfo()) {
      oStream << PDAT_DEATHDATE << person.dDeathDate;
      }
   // User fields
   short sNumFields=0;
   for (short i=1;i<=person.nptUserFields.num();i++)
      if (person.nptUserFields.get(i)) sNumFields++;
   if (sNumFields) {
      // .. fields header
      oStream << PDAT_USERFIELDS;
      writeShort(oStream,sNumFields);
      // .. save field informations
      NUserField* pfield;
      for (short field=1;field<=person.nptUserFields.num();field++) {
         // .. calc field
         pfield=person.getUserField(field);
         if (!pfield) continue;
         // .. save field idx
         writeShort(oStream,field);
         // .. save field
         pfield->save(field,oStream);
         // .. next
         }
      // .. done
      }
   // End !
   oStream.put(PDAT_END);
   // Done
   return(oStream);
}
istream& operator>> (istream& iStream, NPerson& person)
{
   char dat;NDate d;unsigned long ulTemp;
   do {
      iStream >> dat;  // Check for data
      switch (dat) {
         // .. ID
         case PDAT_ID:
            person.setMid(readLong(iStream));
            break;
         // .. Name
         case PDAT_NAME:
            person.setName(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. First name
         case PDAT_FIRSTNAME:
            person.setFirstName(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. Sex
         case PDAT_SEX:
            dat=iStream.get();
            // Compatibility to older version
            switch (dat) {
               case '-': dat=0;break;
               case 'M': dat=1;break;
               case 'W': dat=2;break;
               case 'F': dat=2;break;
               case  1 :       break;
               case  2 :       break;
               default : dat=0;break;
               }
            person.setSex(dat);
            break;
         // .. Birth Place
         case PDAT_BIRTHPLACE:
            person.setBirthPlace(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. Death Place
         case PDAT_DEATHPLACE: // DeathPlace
            person.setDeathPlace(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. Occupation
         case PDAT_OCCUPATION:
            person.setOccupation(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. Address
         case PDAT_ADDRESS:
            person.setAddress(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. Memo
         case PDAT_MEMO:
            person.setMemo(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. Birth date
         case PDAT_BIRTHDATE: {
            iStream >> d;
            person.setBirthDate(d);
            } break;
         // .. Death date
         case PDAT_DEATHDATE: {
            iStream >> d;
            person.setDeathDate(d);
            } break;
         // .. Bitmap
         case PDAT_BITMAP:
            person.setBitmap(IString::lineFrom(iStream,PDAT_END));
            break;
         // .. Files
         case PDAT_FILES:
            while (True) {
               const IString& isTemp=IString::lineFrom(iStream,PDAT_END);
               if (isTemp.length()==0) break;
               person.addFile(isTemp);
               }
            break;
         // .. User fields
         case PDAT_USERFIELDS: {
            NUserField* pField;
            short sFields=readShort(iStream),
                  sFieldIdx;
            for (;sFields>0;sFields--) {
               // .. get field idx
               sFieldIdx=readShort(iStream);
               // Bug from 19.06. caused a long field count to be written
               // ... so this could be high-word of field-count
               if (sFieldIdx==0) sFieldIdx=readShort(iStream);
               //..  get field
               pField=NUserField::load(sFieldIdx,iStream);
               if (!pField) return(iStream);
               person.setUserField(pField,sFieldIdx);
               // .. next
               }
            } break;
         // .. End
         case PDAT_END:
            break;
         }
      }
   while ((dat!=PDAT_END)&&(iStream.good()));
   // Done
   return(iStream);
}
NPerson& NPerson::setShort(short int iSet)
{
   iShort=iSet;
   return(*this);
}
short int NPerson::getShort()
{
   return(iShort);
}

void NPerson::addChildhood(NMarriage* pSet)
{
   pChildhood=pSet;
}
void NPerson::removeChildhood()
{
   pChildhood=NULL;
}
void NPerson::addPartnership(NMarriage* pSet)
{
   nptPartnerships.add(pSet);
   // Done
}
void NPerson::removePartnership(NMarriage* pDel)
{
   // Search for partner
   for (short i=1;i<=nptPartnerships.num();i++)
      if (nptPartnerships.get(i)==pDel) break;
   // If not found -> return
   nptPartnerships.del(i);
   // Done
}
Boolean NPerson::hasPartners()
{
   return(nptPartnerships.num()>0);
}
Boolean NPerson::hasPartner(PNPerson pmanCheck)
{
   for (short int i=1;i<=nptPartnerships.num();i++)
      if (getPartnership(i)->getOther(this)==pmanCheck) return(True);
   return(False);
}

PREL NPerson::getPartnership(unsigned short int iPartner,Boolean reverse)
{
   if (reverse) return(nptPartnerships.get(nptPartnerships.num()+1-iPartner));
   return(nptPartnerships.get(iPartner));
}
short int NPerson::getNoOfPartners()
{
   return(nptPartnerships.num());
}
Boolean NPerson::hasParents()
{
   return(pChildhood!=NULL);
}
PREL NPerson::getChildhood()
{
   return(pChildhood);
}
short int NPerson::getNoOfChildrenWith(unsigned short int iPartner)
{
   PREL pTemp=getPartnership(iPartner);
   if (!pTemp) return(0);
   return(pTemp->getNoOfChildren());
}

Boolean NPerson::hasChild(PNPerson pchild,unsigned short marriage)
{
   if (getNoOfPartners()<marriage) return(False);
   if (getPartnership(marriage)->hasChild(pchild))
      return(True);
   return(False);
}
Boolean NPerson::hasChildren()
{
   if (getNoOfPartners()==0)
      return(False);
   for (short int i=1;i<=nptPartnerships.num();i++)
      if (getNoOfChildrenWith(i)>0) return(True);
   return(False);
}
short NPerson::getGenerationsOfAncestors  ()
{
   if (!pChildhood) return(0);
   // Dad & Mom ?
   return(max( pChildhood->getPartner(1)->getGenerationsOfAncestors()
             , pChildhood->getPartner(2)->getGenerationsOfAncestors() )+1);
}
short NPerson::getGenerationsOfDescendants()
{
   if (getNoOfPartners()==0) return(0);
   NPerson* pChild;PREL pRel;short temp=0;
   for (short int partner=1;partner<=getNoOfPartners();partner++) {
      // Partnership
      pRel=getPartnership(partner);
      // Children
      for (short int child=1;child<=pRel->getNoOfChildren();child++)
         temp=max(temp,pRel->getChild(child)->getGenerationsOfDescendants()+1);
      }
   return(temp);
}

void NPerson::setMid(GEDCOMID newMid)
{
   mid=newMid;
}
GEDCOMID NPerson::getMid() const
{
   return(mid);
}

void NPerson::setSex(const char cSetSex) {
   markChanged();
   cSex=cSetSex;
}
void NPerson::setName(const IString &isSetName) {
   markChanged();
   isName=isSetName;
}
void NPerson::setFirstName(const IString &isSetFirstName) {
   markChanged();
   isFirstName=isSetFirstName;
}
void NPerson::setBirthDate(const NDate& dSetBirthDate) {
   markChanged();
   dBirthDate=dSetBirthDate;
}
void NPerson::setBirthPlace(const IString &isSetBirthPlace) {
   markChanged();
   isBirthPlace=isSetBirthPlace;
}
void NPerson::setDeathDate(const NDate& dSetDeathDate) {
   markChanged();
   dDeathDate=dSetDeathDate;
}
void NPerson::setDeathPlace(const IString &isSetDeathPlace) {
   markChanged();
   isDeathPlace=isSetDeathPlace;
}
void NPerson::setOccupation(const IString& isSetOccupation) {
   markChanged();
   isOccupation=isSetOccupation;
}
void NPerson::setAddress(const IString& isSetAddress) {
   markChanged();
   isAddress=isSetAddress;
}
void NPerson::setMemo(const IString& isSetMemo) {
   markChanged();
   isMemo=isSetMemo;
}
void NPerson::setMarriageDate(unsigned short int iNo,const NDate& dSetMarriageDate)
{
   if (getNoOfPartners()<iNo) return;
   getPartnership(iNo)->setMarriageDate(dSetMarriageDate);
   markChanged();
   getPartnership(iNo)->getOther(this)->markChanged();
}
void NPerson::setMarriagePlace(unsigned short int iNo,const IString& isSetPlace)
{
   if (getNoOfPartners()<iNo) return;
   getPartnership(iNo)->setMarriagePlace(isSetPlace);
   markChanged();
   getPartnership(iNo)->getOther(this)->markChanged();
}
void NPerson::setDivorceDate(unsigned short int iNo,const NDate& dSetDivorceDate)
{
   if (getNoOfPartners()<iNo) return;
   getPartnership(iNo)->setDivorceDate(dSetDivorceDate);
   markChanged();
   getPartnership(iNo)->getOther(this)->markChanged();
}
const NDate& NPerson::getMarriageDate(unsigned short int iNo)
{
   return(getPartnership(iNo)->getMarriageDate());
}
const IString& NPerson::getMarriagePlace(unsigned short int iNo)
{
   return(getPartnership(iNo)->getMarriagePlace());
}
const NDate& NPerson::getDivorceDate(unsigned short int iNo)
{
   return(getPartnership(iNo)->getDivorceDate());
}
IString NPerson::getPartnerName(unsigned short int iNo)
{
   if (getNoOfPartners()<iNo) return(IString());
   NPerson* pPartner=getPartnership(iNo)->getOther(this);
   const IString& isTemp=pPartner->getCallFirstName();
   if (isTemp.size()==0)
      return(pPartner->getName());
   return(isTemp+" "+pPartner->getName());
}
const IString& NPerson::getName() {
   return(isName);
}
const IString& NPerson::getFirstName() {
   return(isFirstName);
}
const char NPerson::getSex() {
   return(cSex);
}
const IString& NPerson::getBirthPlace() {
   return(isBirthPlace);
}
const NDate& NPerson::getBirthDate() {
   return(dBirthDate);
}
const IString& NPerson::getDeathPlace() {
   return(isDeathPlace);
}
const NDate& NPerson::getDeathDate() {
   return(dDeathDate);
}
const IString& NPerson::getOccupation() {
   return(isOccupation);
}
const IString& NPerson::getAddress() {
   return(isAddress);
}
const IString& NPerson::getMemo() {
   return(isMemo);
}
IString NPerson::getCallFirstName() {
   // Calc CallName
   unsigned i;
   if ((i=isFirstName.indexOf('~'))==0)
      return(IString(isFirstName));
   IString isTemp=isFirstName;
   isTemp.remove(1,i);
   if ((i=isTemp.indexOf(' '))!=0) {
      isTemp.remove(i);
      }
   return(IString(isTemp));
   // Done
}
IString NPerson::getNameDotFirstName() {
   // Calc FirstName
   IString first=getCallFirstName();
   if (first.length()==0) return(IString(isName));
   return(IString(isName)+","+first);
}
void NPerson::setBitmap(const IString isSetBitmap) {
   markChanged();
   isBitmap=isSetBitmap;
}
IString& NPerson::getBitmap() {
   return(isBitmap);
}

void NPerson::addFile(const IString& isFile)
{
   markChanged();
   IString* pfile=new IString(isFile);
   nptFiles.add(pfile);
   // Done
}
void NPerson::delAllFiles()
{
   markChanged();
   for (short i=1;i<=nptFiles.num();i++) {
      delete nptFiles.get(i);
      }
   nptFiles.delAll();
}
void NPerson::delFile(unsigned short iFile)
{
   markChanged();
   delete nptFiles.get(iFile);
   nptFiles.del(iFile);
   // Done
}
unsigned short NPerson::getNoOfFiles()
{
   return(nptFiles.num());
}
IString* NPerson::getFile(unsigned short iFile)
{
   if (iFile>nptFiles.num()) return(NULL);
   return(nptFiles.get(iFile));
}

// Called after person has been changed
Boolean NPerson::positionInFamily()
{
   Boolean change=False;
   // ... position between siblings
   if (pChildhood)
      change|=pChildhood->positionChildren();
   // ... position as a partner of others
   for (short i=1;i<=getNoOfPartners();i++)
      change|=getPartnership(i)->getOther(this)->positionPartners();
   // Done
   return(change);
}

Boolean NPerson::positionPartners()
{
   // One partner -> done
   if (nptPartnerships.num()<=1) return(False);
   // Go through Partners
   Boolean change=False;
   for (short i=1;i<nptPartnerships.num();i++) {
      if (nptPartnerships.get(i+1)->getMarriageDate()
          < nptPartnerships.get(i)->getMarriageDate()) {
         change=True;
         nptPartnerships.swap(i,i+1);
         i=0;
         }
      }
   // Done
   return(change);
}

void NPerson::addEvent(NEvent* pEvent)
{
   markChanged();
   nptEvents.add(pEvent);
}
PNEvent NPerson::getEvent(short which)
{
   return(nptEvents.get(which));
}
void NPerson::delEvent(NEvent* pEvent)
{
   for (short i=1;i<=nptEvents.num();i++) {
      if (nptEvents.get(i)==pEvent) {
         markChanged();
         nptEvents.del(i);
         break;
         }
      }
}
short NPerson::numEvents()
{
   return(nptEvents.num());
}

void NPerson::markChanged()
{
   changeStamp=NMankind::timeStamp;
   NPerson::changed=True;
}


Boolean NPerson::changedThisTime()
{
   return(changeStamp==NMankind::timeStamp);
}

int NPerson::getNoOfUserFields()
{
  return(nptUserFields.num());
}

NUserField* NPerson::getUserField(short idx)
{
   return(nptUserFields.get(idx));
}

void NPerson::setUserField(NUserField* set,short idx)
{
   // fill gap between last and this user field
   for (short i=nptUserFields.num();i<=idx;i++)
      nptUserFields.add(NULL);
   // delete old user field at that position
   if (nptUserFields.get(idx))
      delete nptUserFields.get(idx);
   // set new user field
   nptUserFields.change(idx,set);
   // done
   markChanged();
}

void NPerson::delUserField(short idx)
{
   // Check
   if (idx>nptUserFields.num()) return;
   // Free space
   delete nptUserFields.get(idx);
   // kick pointer
   nptUserFields.del(idx);
   // done
   markChanged();
}

void NPerson::delAllUserFields()
{
   // Free storage
   for (short i=1;i<=nptUserFields.num();i++)
      if (nptUserFields.get(i)) delete nptUserFields.get(i);
   // kill pointers
   nptUserFields.delAll();
   // done
   markChanged();
}



// Classs : NUserField
// Purpose: encapsulate multiple field types (text,number,yesno,date)

NUserField::~NUserField() {}

void NUserField::save(short fieldIdx,ostream& oStream)
{
   // .. find out type and save data
   switch (NMankind::pNMankind->getUserFieldType(fieldIdx)) {
      case NMankind::YesNo  : {
         // .. calc special this
         NUserFieldYesNo* pThis=(NUserFieldYesNo*)this;
         // .. save data
         if (pThis->getYesNo()==Yes) { oStream << 'Y'; return; }
         if (pThis->getYesNo()==No ) { oStream << 'N'; return; }
         oStream << 'U'; return;
         }
      case NMankind::Text   : {
         // .. calc special this
         NUserFieldText* pThis=(NUserFieldText*)this;
         // .. save data
         oStream << pThis->getText();
         oStream.put(UDAT_END);
         return;
         }
      case NMankind::Number : {
         // .. calc special this
         NUserFieldNumber* pThis=(NUserFieldNumber*)this;
         // .. save data
         writeLong(oStream,pThis->getNumber());
         return;
         }
      case NMankind::Date   : {
         // .. calc special this
         NUserFieldDate* pThis=(NUserFieldDate*)this;
         // .. save data
         NDate dTemp=pThis->getDate();
         oStream << dTemp;
         return;
         }
      }
   // Done
}

IString NUserField::getValue(short fieldIdx)
{
   // .. find out type and return
   switch (NMankind::pNMankind->getUserFieldType(fieldIdx)) {
      case NMankind::YesNo  : {
         // .. calc special this
         NUserFieldYesNo* pThis=(NUserFieldYesNo*)this;
         // .. data
         if (pThis->getYesNo()==Yes) { return IString("YES"); }
         if (pThis->getYesNo()==No ) { return IString("NO"); }
         return IString("-");
         }
      case NMankind::Text   : {
         // .. calc special this
         NUserFieldText* pThis=(NUserFieldText*)this;
         // .. data
         return IString(pThis->getText());
         }
      case NMankind::Number : {
         // .. calc special this
         NUserFieldNumber* pThis=(NUserFieldNumber*)this;
         // .. data
         return IString(pThis->getNumber());
         }
      case NMankind::Date   : {
         // .. calc special this
         NUserFieldDate* pThis=(NUserFieldDate*)this;
         // .. save data
         return IString(pThis->getDate().asString());
         }
      }
   // Done
   return NULL;
}

NUserField* NUserField::load(short fieldIdx,istream& iStream)
{
   if ( (fieldIdx<1                                       )
      ||(fieldIdx>NMankind::pNMankind->getNoOfUserFields()) )
      return(NULL);
   // .. find out type and save data
   switch (NMankind::pNMankind->getUserFieldType(fieldIdx)) {
      case NMankind::YesNo  : {
         // .. read value
         YesNo set=Unknown;
         switch (iStream.get()) {
            case 'Y' : set=Yes;break;
            case 'N' : set=No ;break;
            }
         // return userfield
         return(new NUserFieldYesNo(set));
         }
      case NMankind::Text   : {
         // .. read value & return userfield
         return(new NUserFieldText(IString::lineFrom(iStream,UDAT_END)));
         }
      case NMankind::Number :
         // .. read value & return userfield
         return(new NUserFieldNumber(readLong(iStream)));
      case NMankind::Date   :
         // .. read value
         NDate dTemp;
         iStream >> dTemp;
         // return userfield
         return(new NUserFieldDate(dTemp));
      }
   // Hmmm
   return(NULL);
}

NUserFieldText  ::NUserFieldText  (const IString& isSet) { isStat=isSet; }
NUserFieldYesNo ::NUserFieldYesNo (      YesNo    ynSet) { ynStat=ynSet; }
NUserFieldNumber::NUserFieldNumber(      long     lSet ) { lStat=lSet  ; }
NUserFieldDate  ::NUserFieldDate  (const NDate&   ndSet) { ndStat=ndSet; }

IString           NUserFieldText  ::getText  () { return(IString(isStat)); }
NUserField::YesNo NUserFieldYesNo ::getYesNo () { return(ynStat         ); }
long              NUserFieldNumber::getNumber() { return(lStat          ); }
NDate             NUserFieldDate  ::getDate  () { return(NDate(ndStat)  ); }


// Class  : NMarriage
// Purpose: Handle NMarriage between PARTNERS and their CHILDs

NMarriage::NMarriage() // Special constructor for REXX-NMarriage import
   : dMarriageDate()
   , isMarriagePlace()
   , dDivorceDate()
{
   // Set SaveData
   fid=0;
   iShort=0;
   pPartners[1-1]=(PNPerson)0; // This is no pointer but a reference
   pPartners[2-1]=(PNPerson)0; // This is no pointer but a reference
}

NMarriage::NMarriage(NPerson* pmanSetPartner1,NPerson* pmanSetPartner2)
   : dMarriageDate()
   , isMarriagePlace()
   , dDivorceDate()
{
   // Set SaveData
   iShort=0;
   fid=0;
   pPartners[1-1]=pmanSetPartner1;
   pPartners[2-1]=pmanSetPartner2;
   // Attach this to NPersons
   pPartners[1-1]->addPartnership(this);
   pPartners[2-1]->addPartnership(this);
}
NMarriage::NMarriage(NPerson* pmanChild,NPerson* pmanSetParent1,NPerson* pmanSetParent2)
   : dMarriageDate()
   , isMarriagePlace()
   , dDivorceDate()
{
   // Set SaveData
   iShort=0;
   fid=0;
   // Attach this to NPersons
   pmanSetParent1->addPartnership(this);
   pPartners[1-1]=pmanSetParent1;
   pmanSetParent2->addPartnership(this);
   pPartners[2-1]=pmanSetParent2;
   // Add Child to this
   addChild(pmanChild);
}
NMarriage::~NMarriage()
{
}

void NMarriage::changePartner(unsigned short iPos,PNPerson pSetPartner,Boolean notifyOld,Boolean notifyNew)
{
   if (notifyOld) pPartners[iPos-1]->removePartnership(this);
   pPartners[iPos-1]=pSetPartner;
   if (notifyNew) pSetPartner->addPartnership(this);
}

void NMarriage::changePartner(PNPerson pPartner,PNPerson pSetPartner,Boolean notifyOld,Boolean notifyNew)
{
   for (unsigned short int i=1;i<=2;i++)
      if (pPartners[i-1]==pPartner) {
         if (notifyOld) pPartners[i-1]->removePartnership(this);
         pPartners[i-1]=pSetPartner;
         if (notifyNew) pSetPartner->addPartnership(this);
         return;
         }
}

void NMarriage::setFid(unsigned long newFid)
{
   fid=newFid;
}
GEDCOMID NMarriage::getFid() const
{
   return(fid);
}
void NMarriage::setMarriageDate(const NDate& dSetMarriageDate)
{
   dMarriageDate=dSetMarriageDate;
}
void NMarriage::setMarriagePlace(const IString& isSetPlace)
{
   isMarriagePlace=isSetPlace;
}
void NMarriage::setDivorceDate(const NDate& dSetDivorceDate)
{
   dDivorceDate=dSetDivorceDate;
}
const NDate& NMarriage::getMarriageDate()
{
   return(dMarriageDate);
}
const IString& NMarriage::getMarriagePlace()
{
   return(isMarriagePlace);
}
const NDate& NMarriage::getDivorceDate()
{
   return(dDivorceDate);
}

ostream& operator<< (ostream& oStream, NMarriage& rel)
{
   // Give Partners
   writeShort(oStream,rel.getPartner(1)->getShort());
   writeShort(oStream,rel.getPartner(2)->getShort());
   // And Children
   writeShort(oStream,rel.getNoOfChildren()        );
   for (short int iLoop=1;iLoop<=rel.getNoOfChildren();iLoop++)
      writeShort(oStream,rel.getChild(iLoop)->getShort());
   // Put static data
   oStream << MDAT_ID;
   writeLong(oStream,rel.fid);
   // Additional Information
   if (rel.dMarriageDate.hasInfo()) {
      oStream << MDAT_MARRIAGEDATE;
      oStream << rel.dMarriageDate;
      }
   if (rel.dDivorceDate.hasInfo()) {
      oStream << MDAT_DIVORCEDATE;
      oStream << rel.dDivorceDate;
      }
   if (rel.isMarriagePlace.size()>0) {
      oStream << MDAT_MARRIAGEPLACE << rel.isMarriagePlace; oStream.put(PDAT_END);
      }
   // End !
   oStream.put(MDAT_END);
   // Done
   return(oStream);
}
istream& operator>> (istream& iStream, NMarriage& rel)
{
   char dat;NDate d;
   do {
      iStream >> dat;  // Check for data
      switch (dat) {
         case MDAT_ID: // ID
            rel.setFid(readLong(iStream));
            break;
         case MDAT_MARRIAGEDATE: {  // MarriageDate
            iStream >> d;
            rel.setMarriageDate(d);
            } break;
         case MDAT_DIVORCEDATE: {  // DivorceDate
            iStream >> d;
            rel.setDivorceDate(d);
            } break;
         case MDAT_MARRIAGEPLACE: // Place
            rel.setMarriagePlace(IString::lineFrom(iStream,MDAT_END));
            break;
         case MDAT_END:
            break;
         }
      }
   while ((dat!=PDAT_END)&&(iStream.good()));
   // Done
   return(iStream);
}

NMarriage& NMarriage::setShort(short int iSet)
{
   iShort=iSet;
   return(*this);
}
short int NMarriage::getShort()
{
   return(iShort);
}

PNPerson NMarriage::getPartner(unsigned short int iGetNumber)
{
   return(pPartners[iGetNumber-1]);
}
NPerson* NMarriage::getOther(NPerson* pOfThis)
{
   if (pOfThis==getPartner(1))
      return(getPartner(2));
   return(getPartner(1));
}
NPerson* NMarriage::getChild(unsigned short int iGetNumber)
{
   return(nptChildren.get(iGetNumber));
}
void NMarriage::addChildrenFrom(PREL pfrom)
{
   if (pfrom->getNoOfChildren()==0) return;
   // Add and update new children here
   for (short j=1;j<=pfrom->getNoOfChildren();j++) {
      nptChildren.add(pfrom->nptChildren.get(j));
      pfrom->nptChildren.get(j)->addChildhood(this);
      }
   pfrom->nptChildren.delAll();
   // Done
}

Boolean NMarriage::hasChild(PNPerson pchild)
{
   for (unsigned short i=1;i<=getNoOfChildren();i++)
      if (getChild(i)==pchild) return(True);
   return(False);
}

void NMarriage::addChild(NPerson* pNewChild)
{
   pNewChild->addChildhood(this);
   nptChildren.add(pNewChild);
   positionChildren();
   // Done
}
void NMarriage::removeChildren()
{
   for (short i=1;i<=nptChildren.num();i++)
      nptChildren.get(i)->removeChildhood();
   nptChildren.delAll();
}
void NMarriage::removeChild(NPerson* pChild)
{
   // Search for child
   for (short iPos=1;iPos<=nptChildren.num();iPos++)
      if (nptChildren.get(iPos)==pChild) break;
   // If found
   if (iPos>nptChildren.num()) return;
   // Empty child ParentPtr and remove child
   pChild->removeChildhood();
   nptChildren.del(iPos);
   // Done
}

void NMarriage::changeChild(unsigned short int iChange,PNPerson pSetChild,Boolean notifyOld,Boolean notifyNew)
{
   if (nptChildren.num()<iChange) return;
   if (notifyOld)
      nptChildren.get(iChange)->removeChildhood();
   nptChildren.change(iChange,pSetChild);
   if (notifyNew)
      pSetChild->addChildhood(this);
}

void NMarriage::removePartner(NPerson* pPartner)
{
   for (unsigned short int i=1;i<=2;i++)
      if (pPartners[i-1]==pPartner) {
         pPartners[i-1]->removePartnership(this);
         pPartners[i-1]=NULL;
         return;
         }
}

Boolean NMarriage::importAddPartner(unsigned long id)
{
   for (unsigned short int i=1;i<=2;i++)
      if (getPartner(i)==0) {
         changePartner(i,(PNPerson)id,False,False);
         return(True);
         }
   return(False);
}
Boolean NMarriage::importAddChild(unsigned long id)
{
   nptChildren.add((PNPerson)id);
   return(True);
}

short int NMarriage::getNoOfChildren()
{
   return(nptChildren.num());
}
Boolean NMarriage::positionChildren()
{
   // One child NO changes
   if (getNoOfChildren()<=1) return(False);
   // Go through children
   Boolean changed=False;
   PNPerson pTemp;
   for (short int i=2;i<=getNoOfChildren();i++)
      if (getChild(i)->getBirthDate()<getChild(i-1)->getBirthDate()) {
         changed=True;
         PNPerson ptemp=getChild(i);
         changeChild(i,getChild(i-1),False,False);
         changeChild(i-1,ptemp,False,False);
         i=2;
         }
   // Done
   return(changed);
}

// Class  : NMankind
// Purpose: Handle all NPerson in one space
NMankind::Assist NMankind::assist   =NMankind::noAssistance;
NMankind*        NMankind::pNMankind=NULL;
Boolean          NMankind::changed  =False;
long             NMankind::timeStamp=0;

void NMankind::loadFromProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("NMankind");
   // Assitance
   int temp=profile.integerWithKey("ASSIST");
   changeAssist(Assist(temp),noAssistance);
   // Done
}
void NMankind::saveToProfile(IProfile& profile)
{
   profile.setDefaultApplicationName("NMankind");
   // Assistance
   profile.addOrReplaceElementWithKey("ASSIST",assist);
   // Done
}

NMankind::NMankind()
{
   // Preset NMankind::values
   pNMankind=this;
   // Initial Mankind
   newMankind();
   // Done
}
NMankind::~NMankind ()
{
   delAllEvents();
   delAllMen();
   delAllUserFields();
}

void NMankind::addMankindHandler(PNMankindHandler handler)
{
   seqHandler.add(handler);
   handler->disconnentMen();
   handler->disconnentEvents();
   handler->manAdded(seqMan);
   handler->eventAdded(seqEvent);
}
void NMankind::removeMankindHandler(PNMankindHandler handler)
{
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor)
      if (seqHandler.elementAt(cursor)==handler) {
         seqHandler.removeAt(cursor);
         return;
         }
}

void NMankind::newMankind()
{
   // Clear NMankind and enter first NPerson
   delAllEvents();
   delAllMen();
   delAllUserFields();
   // Insert ADAM
   pmanOrigin=new NPerson(NApplication::pDLLResource->loadString(SI_Adam));
   pmanOrigin->setMid(++maxMid);
   pmanActual=pmanOrigin;
   seqMan.add(pmanOrigin);
   // Notify Handlers
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor) seqHandler.elementAt(cursor)->manAdded(pmanOrigin,False);
   // Done
   changed=False;
   timeStamp=0;
}


IBoolean deleteManFromSeq(const PNPerson& pman,void*) {
   delete pman;
   return(True);
}
IBoolean deleteRelFromSeq(const PREL& prel,void*) {
   delete prel;
   return(True);
}
IBoolean deleteEventFromSeq(const PNEvent& pevent,void*) {
   delete pevent;
   return(True);
}
void NMankind::delAllEvents()
{
   // Reset Origin/Actual
   peventActual=NULL;
   // Notify Handlers
   SEQOFHANDLER::Cursor handlercursor(seqHandler);
   forCursor(handlercursor)
       seqHandler.elementAt(handlercursor)->disconnentEvents();
   // Remove all NEvents
   seqEvent.removeAll(deleteEventFromSeq);
   // Done
}

void NMankind::delAllMen()
{
   // Reset Origin/Actual
   maxFid=0;
   maxMid=0;
   pmanOrigin=NULL;
   pmanActual=NULL;
   pmanRemembered=NULL;
   // Notify Handlers
   SEQOFHANDLER::Cursor handlercursor(seqHandler);
   forCursor(handlercursor)
       seqHandler.elementAt(handlercursor)->disconnentMen();
   // Remove all NPersons and RELs
   seqMan      .removeAll(deleteManFromSeq);
   seqRel      .removeAll(deleteRelFromSeq);
   // Done
}

Boolean NMankind::save(fstream& fsFile)
{
   IString isSavePerson  (NApplication::pDLLResource->loadString(SI_MsgSavePerson  )),
           isSaveMarriage(NApplication::pDLLResource->loadString(SI_MsgSaveMarriage)),
           isSaveEvent   (NApplication::pDLLResource->loadString(SI_MsgSaveEvent   ));
   short int iCountMan=seqMan.numberOfElements();
   short int iShort;
   // Notify Handlers and gather data
   IString isParm;
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor) {
      seqHandler.elementAt(cursor)->doChangesNow();
      seqHandler.elementAt(cursor)->collectParm(isParm);
      }
   // UserFields ?
   if (getNoOfUserFields()>0) {
      // .. insert USER-Header
      fsFile << "USER";
      writeShort(fsFile,getNoOfUserFields());
      for (short i=1;i<=getNoOfUserFields();i++) {
         // .. type
         switch (getUserFieldType(i)) {
            case YesNo : fsFile << UDAT_YESNO ;break;
            case Text  : fsFile << UDAT_TEXT  ;break;
            case Number: fsFile << UDAT_NUMBER;break;
            case Date  : fsFile << UDAT_DATE  ;break;
            }
         // .. name
         fsFile << getUserFieldName(i) << (char)0;
         // .. next
         }
      // .. done
      }
   // Insert MANS-Header
   fsFile << "MANS";
   writeShort(fsFile,iCountMan);
   // .. store NPerson's
   iShort=1;
   short int iMod=iCountMan/64+1;
   SEQOFNPerson::Cursor manCursor(seqMan);
   forCursor(manCursor) {
      // Report action to handlers
      if (iShort%iMod==0) forCursor(cursor)
         seqHandler.elementAt(cursor)->progressReport(isSavePerson+IString(iShort)+" / "+IString(iCountMan));
      // Do it
      fsFile << seqMan.elementAt(manCursor)->setShort(iShort++);
      }
   // Insert RELS-HEADER
   short int iCountRelation=seqRel.numberOfElements();
   fsFile << "REL!";    // New one !
   writeShort(fsFile,iCountRelation);
   // .. store NMarriage's
   short int iRel=1;
             iMod=iCountRelation/64+1;
   SEQOFNMarriage::Cursor relCursor(seqRel);
   forCursor (relCursor) {
      // Report action to handlers
      if (iRel%iMod==0) forCursor(cursor)
         seqHandler.elementAt(cursor)->progressReport(isSaveMarriage+IString(iRel)+" / "+IString(iCountRelation));
      // Do it
      fsFile << *seqRel.elementAt(relCursor);
      iRel++;
      }
   // Insert EVEN-Header
   short sEvent=1,
         sNumEvents=getNoOfEvents(),
         sMod=sNumEvents/64+1;
   fsFile << "EVEN";
   writeShort(fsFile,sNumEvents);
   // .. store NEvents
   SEQOFNEvent::Cursor ecursor(seqEvent);
   forCursor (ecursor) {
      // Report action to handlers
      if (sEvent%sMod==0) forCursor(cursor)
         seqHandler.elementAt(cursor)->progressReport(isSaveEvent+IString(sEvent)+" / "+IString(sNumEvents));
      // Do it
      seqEvent.elementAt(ecursor)->saveTo(fsFile);;
      sEvent++;
      }
   // Insert PARM-Header;
   fsFile << "PARM";
      // Actual
      fsFile.put(PARM_ACTUAL);
      writeShort(fsFile,pmanActual->getShort());
      // Origin
      fsFile.put(PARM_ORIGIN);
      writeShort(fsFile,pmanOrigin->getShort());
      // Handler Parameters
      if (isParm.length()>0) {
         fsFile.put(PARM_HANDLER);
         writeShort(fsFile,isParm.length());
         fsFile.write((char*)isParm,isParm.length());
         }
      // No more params
   fsFile.put(PARM_END);
   // Done
   if (fsFile.good()) changed=False;
   return(True);
}

void NMankind::checkIDs()
{
   // Check for NPerson IDs
   HASHOFID ids;
   Boolean warning=True,alright=True;
   PNPerson pman;
   SEQOFNPerson::Cursor manCursor(seqMan);
   forCursor(manCursor) {
      pman=seqMan.elementAt(manCursor);
      if ((pman->getMid()==0)||(ids.locateOrAdd(pman->getMid()))) {
         if (warning) {
            IMessageBox mbox(NULL);
            if (mbox.show(NApplication::pDLLResource->loadString(SI_MsgDuplicateIDs)
                         ,IMessageBox::queryIcon
                         |IMessageBox::moveable
                         |IMessageBox::yesNoButton)
                ==IMessageBox::no) return;
            warning=False;
            }
         alright=False;
         pman->setMid(0);
         }
      }
   // Correct Duplicate/Missing ID(s)
   if (!alright) {
      forCursor(manCursor) {
         pman=seqMan.elementAt(manCursor);
         if (pman->getMid()==0) pman->setMid(++maxMid);
         }
      }
   // Check for Family IDs
   ids.removeAll();
   alright=True;
   PREL prel;
   SEQOFNMarriage::Cursor relCursor(seqRel);
   forCursor(relCursor) {
      prel=seqRel.elementAt(relCursor);
      if ((prel->getFid()==0)||(ids.locateOrAdd(prel->getFid()))) {
         if (warning) {
            IMessageBox mbox(NULL);
            if (mbox.show(NApplication::pDLLResource->loadString(SI_MsgDuplicateIDs)
                         ,IMessageBox::informationIcon|IMessageBox::yesNoButton)
                ==IMessageBox::no) return;
            warning=False;
            }
         alright=False;
         prel->setFid(0);
         }
      }
   // Correct Duplicate/Missing ID(s)
   if (alright) return;
   forCursor(relCursor) {
      prel=seqRel.elementAt(relCursor);
      if (prel->getFid()==0) prel->setFid(++maxFid);
      }
   // Done
}

Boolean NMankind::checkHeader(fstream& fsFile)
{
   // Check for TREE !?
   if (!readHeader(fsFile,"TREE"))
      return(False);
   // O.K. Can load it
   return(True);
}

Boolean NMankind::load(fstream& fsFile)
{
   IString isLoadPerson  (NApplication::pDLLResource->loadString(SI_MsgLoadPerson  )),
           isLoadMarriage(NApplication::pDLLResource->loadString(SI_MsgLoadMarriage)),
           isLoadEvent   (NApplication::pDLLResource->loadString(SI_MsgLoadEvent   )),
           isLoadCheck   (NApplication::pDLLResource->loadString(SI_MsgLoadCheck   ));
   // Clear Mankind
   delAllEvents();
   delAllMen();
   delAllUserFields();
   // CHECK FOR >USER< HEADER (CAN BE)
   if (readHeader(fsFile,"USER")) {
      // .. get no of field
      short iCountUser=0;
      fsFile.read((char*)&iCountUser,sizeof(short));
      // .. read fields
      char cType;IString isName;
      for (short i=1;i<=iCountUser;i++) {
         // .. get type & name
         cType=fsFile.get();
         isName=IString::lineFrom(fsFile,UDAT_END);
         // .. new field
         switch (cType) {
            case UDAT_YESNO : addUserField(IString(isName),YesNo );break;
            case UDAT_TEXT  : addUserField(IString(isName),Text  );break;
            case UDAT_NUMBER: addUserField(IString(isName),Number);break;
            case UDAT_DATE  : addUserField(IString(isName),Date  );break;
            }
         // .. next
         }
      // .. done
      }
   // CHECK FOR >MANS< HEADER (MUST BE)
   if (!readHeader(fsFile,"MANS")) {
      newMankind();
      return(False);
      }
   // .. Import NPersons
   SEQOFHANDLER::Cursor cursor(seqHandler);
   short int iCountMan=0;  // First No NPerson
   short int iLoopMan=readShort(fsFile); // # of MEN
   NPerson* pMan;
   short int iModMan=iLoopMan/64+1;
   PNPerson* pmanArray=(PNPerson*)malloc(sizeof(PNPerson)*iLoopMan);
   for (;iLoopMan>0;iLoopMan--) {
      // Report action to handlers
      if (iCountMan%iModMan==0) forCursor(cursor)
         seqHandler.elementAt(cursor)->progressReport(isLoadPerson+IString(iCountMan+1)+" / "+IString(iCountMan+iLoopMan));
      // Do it
      pMan=new NPerson(IString());    // Make new NPerson
      seqMan.add(pMan);    // ... add to ObjList
      pmanArray[iCountMan]=pMan;  // Remember in Array
      fsFile >> *pMan;
      maxMid=max(pMan->getMid(),maxMid);
      // Next
      iCountMan++;                // inc # of NPerson
      }
   // CHECK FOR >RELS< HEADER (MUST BE)
   Boolean newVersion=False;
   if (!readHeader(fsFile,"RELS")) {
      if (!readHeader(fsFile,"REL!")) {
         free(pmanArray);
         newMankind();
         return(False);
      }
      newVersion=True;
   }
   // .. Import NMarriages
   short int iCountRelation=0;  // First No NMarriage
   NMarriage* pRel;short int iLoopRel=readShort(fsFile);
   short int iLoopChild;short int iTemp1;short int iTemp2;
   short int iModRel=iLoopRel/64+1;
   for (;iLoopRel>0;iLoopRel--) {
      // Do it
      iTemp1=readShort(fsFile);  // Parent1
      iTemp2=readShort(fsFile);  // Parent2
      // Report action to handlers
      if (iCountRelation%iModRel==0) forCursor(cursor)
         seqHandler.elementAt(cursor)->progressReport(isLoadMarriage+IString(iCountRelation+1)+" / "+IString(iCountRelation+iLoopRel));
      // Check for consistency
      if ((iTemp1==0)||(iTemp2==0)||(iTemp1>iCountMan)||(iTemp2>iCountMan)) {
         free(pmanArray);
         newMankind();
         return(False);
         }
      // Add Family
      pRel=new NMarriage(pmanArray[iTemp1-1]
                       ,pmanArray[iTemp2-1]);
      seqRel.add(pRel);                     // add to ObjList
      iLoopChild=readShort(fsFile);         // #Childs
      for (;iLoopChild>0;iLoopChild--) {
         iTemp1=readShort(fsFile);          // Child
         // Check for consistency
         if ((iTemp1<1)||(iTemp1>iCountMan)) {
            free(pmanArray);
            newMankind();
            return(False);
            }
         // .. add it
         pRel->addChild(pmanArray[iTemp1-1]);
         }
      if (newVersion) fsFile >> *pRel;
      maxFid=max(pRel->getFid(),maxFid);
      // Next
      iCountRelation++;  // inc # of NMarriages
      }
   // CHECK FOR >EVEN< HEADER
   if (readHeader(fsFile,"EVEN")) {
      // .. calc no of events
      short sEvents=readShort(fsFile);
      // .. read em in
      NEvent* pevent;
      for (short event=1;event<=sEvents;event++) {
         pevent=NEvent::loadFrom(fsFile,pmanArray,iCountMan);
         if (!pevent) {
            free(pmanArray);
            newMankind();
            return(False);
            }
         // Add the new event
         seqEvent.add(pevent);
         }
      // .. done
      }
   // Structural consistency ?
   SEQOFNPerson::Cursor manCursor(seqMan);
   forCursor(manCursor)
      seqMan.elementAt(manCursor)->positionInFamily();
   // Check IDs
   forCursor(cursor)
      seqHandler.elementAt(cursor)->progressReport(isLoadCheck);
   checkIDs();
   forCursor(cursor)
      seqHandler.elementAt(cursor)->progressReport(IString());
   // CHECK FOR >PARM< HEADER
   short int iActual=1,iOrigin=1,iTemp;
   IString isParm;
   if (readHeader(fsFile,"PARM")) {
      char parm;
      do {
         fsFile >> parm;
         switch (parm) {
            case PARM_ACTUAL: { // Actual
               iActual=readShort(fsFile);
               } break;
            case PARM_ORIGIN: { // Origin
               iOrigin=readShort(fsFile);
               } break;
            case PARM_HANDLER: { // Handler Settings
               iTemp=readShort(fsFile);
               isParm.leftJustify(iTemp);
               fsFile.read((char*)isParm,iTemp);
               } break;
            default : {      // Unknown old setting
               fsFile.read((char*)&iTemp,sizeof(short int));
               } break;
            }
         }
      while ((parm!=PARM_END)&&(fsFile.good()));
      }
   // Update Actual/Origin/Parms
   if ( (iActual<1)||(iActual>iCountMan)
      ||(iOrigin<1)||(iOrigin>iCountMan)) {
      free(pmanArray);
      newMankind();
      return(False);
      }
   pmanActual=pmanArray[iActual-1];
   pmanOrigin=pmanArray[iOrigin-1];
   if (seqEvent.numberOfElements()>0) peventActual=seqEvent.firstElement();
   // Notify Handlers
   SEQOFHANDLER::Cursor handlercursor(seqHandler);
   forCursor(handlercursor) {
      seqHandler.elementAt(handlercursor)->spreadParm(isParm);
      seqHandler.elementAt(handlercursor)->manAdded(seqMan);
      seqHandler.elementAt(handlercursor)->eventAdded(seqEvent);
      }
   // Done
   free(pmanArray);
   changed=False;
   return(True);
}

IBoolean setShortTo0(PNPerson& pman,void*) {
   pman->setShort(0);
   return(True);
}

Boolean NMankind::addManPartner(PNPerson pmanPartner)
{
   // Create Partner and Insert into NMankind
   NPerson* pman;
   if (pmanPartner) pman=pmanPartner;
   else {
      pman=new NPerson(NApplication::pDLLResource->loadString(SI_Partner));
      pman->setMid(++maxMid);
      }
   // Create Relation between Partners
   NMarriage* pRelation=new NMarriage(pmanActual,pman);
   pRelation->setFid(++maxFid);
   seqRel.add(pRelation);
   // Update data
   if (!pmanPartner) {
      // New Man Insert
      seqMan.add(pman);
      // Assistance
      if (assist&addPartnerSameAddress)
         pman->setAddress(pmanActual->getAddress());
      if ((assist&addPartnerOppositeSex)&&(pmanActual->getSex()!=0))
         pman->setSex(3-pmanActual->getSex());
      // Notify Handlers
      SEQOFHANDLER::Cursor cursor(seqHandler);
      forCursor(cursor) seqHandler.elementAt(cursor)->manAdded(pman,False);
      }
   else {
      // Notify Handlers
      SEQOFHANDLER::Cursor cursor(seqHandler);
      forCursor(cursor) seqHandler.elementAt(cursor)->structureChanged();
      }
   // Done
   changed=True;
   return(True);
}

Boolean isDescendant(PNPerson pthis,PNPerson pancestor)
{
   PREL prel;PNPerson pparent;
   // Check if is himself
   if (pthis==pancestor) return(True);
   // Check Ancestors of this
   prel=pthis->getChildhood();
   if (prel) {
      // Check if parents are descendants of pancestor
      for (unsigned short parent=1;parent<=2;parent++) {
         pparent=prel->getPartner(parent);
         // Parent is descendant of pancestor
         if (isDescendant(pparent,pancestor)) return(True);
         // Next Parent
         }
      }
   // Doesn't seem to be descendant
   return(False);
}

void setAllRelativesTo1(PNPerson from)
{
   PREL prel;PNPerson pman;
   // mark this as reached
   from->setShort(1);
   // Go for parents
   prel=from->getChildhood();
   if (prel) {
      prel->setShort(1);
      pman=prel->getPartner(1);
      if (pman->getShort()==0) setAllRelativesTo1(pman);
      pman=prel->getPartner(2);
      if (pman->getShort()==0) setAllRelativesTo1(pman);
      }
   // Go for Partners
   for (unsigned short partner=1;partner<=from->getNoOfPartners();partner++) {
      prel=from->getPartnership(partner);
      prel->setShort(1);
      pman=prel->getOther(from);
      if (pman->getShort()==0) setAllRelativesTo1(pman);
      // Go for Children
      for (unsigned short child=1;child<=prel->getNoOfChildren();child++) {
         pman=prel->getChild(child);
         if (pman->getShort()==0) setAllRelativesTo1(pman);
         // Next Child
         }
      // Next Partner
      }
   // Done
}

IBoolean deleteMan0(const PNPerson& pman,void*) {
   if (pman->getShort()==0) {
      // Delete storage
      delete pman;
      return(True);
      }
   return(False);
}
IBoolean deleteRel0(const PREL& prel,void*) {
   if (prel->getShort()==0) {
      delete prel;
      return(True);
      }
   return(False);
}
IBoolean setMan0(PNPerson& pman,void*)
{
   pman->setShort(0);
   return(True);
}
IBoolean setRel0(PREL& prel,void*)
{
   prel->setShort(0);
   return(True);
}



Boolean NMankind::addManChild(unsigned short int iPartner,PNPerson pmanChild)
{
   if (pmanActual->getNoOfPartners()<iPartner)
      return(False);
   // got to distinguish between new child and existing child
   if (pmanChild) {
      // Existing child, so check relationship first !
      if ( (isDescendant(pmanActual,pmanChild))
         ||(isDescendant(pmanActual->getPartnership(iPartner)->getOther(pmanActual),pmanChild)))
         return(False);
      // Notify Handlers to save their data now
      SEQOFHANDLER::Cursor handlerCursor(seqHandler);
      forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
      // Save the actual and do a no go
      PNPerson pactual=pmanActual,
           porigin=pmanOrigin;
      pmanActual=NULL;
      pmanOrigin=NULL;
      // Notify Handlers
      forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->disconnentMen();
      // Cut of old parents
      if (pmanChild->getChildhood())
         pmanChild->getChildhood()->removeChild(pmanChild);
      // Let Man add Child to its corresponding NMarriage
      pactual->getPartnership(iPartner)->addChild(pmanChild);
      // set shorts of NPerson/REL to 0
      seqMan.allElementsDo(setMan0,NULL);
      seqRel.allElementsDo(setRel0,NULL);
      // Set All Relatives of actual to 1 and make sure origin and remembered is 1
      setAllRelativesTo1(pactual);
      if (porigin->getShort()==0) porigin=pactual;
      if ((pmanRemembered)&&(pmanRemembered->getShort()==0))
         pmanRemembered=NULL;
      // Delete not used
      seqMan.removeAll(deleteMan0,(void*)NULL);
      seqRel.removeAll(deleteRel0,(void*)NULL);
      // Done
      pmanOrigin=porigin;
      pmanActual=pactual;
      // Notify Handlers
      forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->manAdded(seqMan);
      }
   else {
      // Create New Child and Insert into NMankind
      NPerson* pman=new NPerson(NApplication::pDLLResource->loadString(SI_Child));
      pman->setMid(++maxMid);
      seqMan.add(pman);
      // Assitance
      if (assist&addChildSameAddress)
         pman->setAddress(pmanActual->getAddress());
      if (assist&addChildSameName)
         pman->setName(pmanActual->getName());
      // Let Man add Child to its corresponding NMarriage
      pmanActual->getPartnership(iPartner)->addChild(pman);
      // Notify Handlers
      SEQOFHANDLER::Cursor cursor(seqHandler);
      forCursor(cursor) seqHandler.elementAt(cursor)->manAdded(pman,False);
      }
   // Done
   changed=True;
   return(True);
}
Boolean NMankind::addManParents(const IString& isMom,const IString& isDad)
{
   // Create Parents and Insert into NMankind
   NPerson* pmanDad=new NPerson(isDad);
   pmanDad->setMid(++maxMid);
   NPerson* pmanMom=new NPerson(isMom);
   pmanMom->setMid(++maxMid);
   NMarriage* pRelation=new NMarriage(pmanActual,pmanDad,pmanMom);
   pRelation->setFid(++maxFid);
   seqMan.add(pmanMom);
   seqMan.add(pmanDad);
   seqRel.add(pRelation);
   // Data Assistance
   if (assist&addParentsMaleFemale) {
      pmanDad->setSex(1);
      pmanMom->setSex(2);
      }
   if (assist&addParentsSameAddress) {
      pmanDad->setAddress(pmanActual->getAddress());
      pmanMom->setAddress(pmanActual->getAddress());
      }
   if (assist&addParentsSameName) {
      pmanDad->setName(pmanActual->getName());
      pmanMom->setName(pmanActual->getName());
      }
   // Notify Handlers
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor) {
       seqHandler.elementAt(cursor)->manAdded(pmanDad,True);
       seqHandler.elementAt(cursor)->manAdded(pmanMom,False);
       }
   // Done
   changed=True;
   return(True);
}

Boolean NMankind::deletePartner(unsigned short int iPartner)
{
   // Children there ?
   PREL pRel=pmanActual->getPartnership(iPartner);
   if (!pRel) return(False);
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Save the actual and do a no go
   PNPerson pactual=pmanActual,
        porigin=pmanOrigin;
   pmanActual=NULL;
   pmanOrigin=NULL;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->disconnentMen();
   // remove all Partnerships
   pRel->removePartner(pRel->getPartner(1));
   pRel->removePartner(pRel->getPartner(2));
   // set shorts of NPerson/REL to 0
   seqMan.allElementsDo(setMan0,NULL);
   seqRel.allElementsDo(setRel0,NULL);
   // Set All Relatives of actual to 1 and make sure origin and remembered is 1
   setAllRelativesTo1(pactual);
   if (porigin->getShort()==0) porigin=pactual;
   if ((pmanRemembered)&&(pmanRemembered->getShort()==0))
      pmanRemembered=NULL;
   // Delete not used
   seqMan.removeAll(deleteMan0,(void*)NULL);
   seqRel.removeAll(deleteRel0,(void*)NULL);
   // Done
   pmanOrigin=porigin;
   pmanActual=pactual;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->manAdded(seqMan);
   // Done
   changed=True;
   return(True);
}

Boolean NMankind::deleteChilds(unsigned short int iPartner)
{
   // Children there ?
   PREL pRel=pmanActual->getPartnership(iPartner);
   if (!pRel) return(False);
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Save the actual and do a no go
   PNPerson pactual=pmanActual,
        porigin=pmanOrigin;
   pmanActual=NULL;
   pmanOrigin=NULL;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->disconnentMen();
   // remove Children
   PNPerson pChild;
   while (pRel->getNoOfChildren()>0) {
      pChild=pRel->getChild(1);
      pRel->removeChild(pChild);
      }
   // set shorts of NPerson/REL to 0
   seqMan.allElementsDo(setMan0,NULL);
   seqRel.allElementsDo(setRel0,NULL);
   // Set All Relatives of actual to 1 and make sure origin and rememberedis 1
   setAllRelativesTo1(pactual);
   if (porigin->getShort()==0) porigin=pactual;
   if ((pmanRemembered)&&(pmanRemembered->getShort()==0))
      pmanRemembered=NULL;
   // Delete not used
   seqMan.removeAll(deleteMan0,(void*)NULL);
   seqRel.removeAll(deleteRel0,(void*)NULL);
   // Done
   pmanOrigin=porigin;
   pmanActual=pactual;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->manAdded(seqMan);
   // Done
   changed=True;
   return(True);
}
Boolean NMankind::deleteParents()
{
   // Parents there ?
   PREL pRel=pmanActual->getChildhood();
   if (!pRel) return(False);
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Save the actual and do a no go
   PNPerson pactual=pmanActual,
        porigin=pmanOrigin;
   pmanOrigin=NULL;
   pmanActual=NULL;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->disconnentMen();
   // remove Childhood
   pRel->removeChild(pactual);
   // set shorts of NPerson/REL to 0
   seqMan.allElementsDo(setMan0,NULL);
   seqRel.allElementsDo(setRel0,NULL);
   // Set All Relatives of actual to 1 and make sure origin and remembered is 1
   setAllRelativesTo1(pactual);
   if (porigin->getShort()==0) porigin=pactual;
   if ((pmanRemembered)&&(pmanRemembered->getShort()==0))
      pmanRemembered=NULL;
   // Delete not used
   seqMan.removeAll(deleteMan0,(void*)NULL);
   seqRel.removeAll(deleteRel0,(void*)NULL);
   // Done
   pmanOrigin=porigin;
   pmanActual=pactual;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->manAdded(seqMan);
   // Done
   changed=True;
   return(True);
}

Boolean NMankind::deleteMan()
{
   // Is the Actual deletable ?
   if (pmanActual==pmanOrigin) return(False);
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Save origin and do a no go
   PNPerson pold=pmanOrigin,
        pdel=pmanActual;
   pmanActual=NULL;
   pmanOrigin=NULL;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->disconnentMen();
   // set shorts of NPerson/REL to 0
   SEQOFNPerson::Cursor manCursor(seqMan);
   forCursor(manCursor) seqMan.elementAt(manCursor)->setShort(0);
   SEQOFNMarriage::Cursor relCursor(seqRel);
   forCursor(relCursor) seqRel.elementAt(relCursor)->setShort(0);
   // remove all Partnerships
   PREL prel;
   for (short int partner=1;partner<=pdel->getNoOfPartners();partner++) {
      prel=pdel->getPartnership(partner);
      // Separate everything from Relations
      prel->removePartner(prel->getPartner(1));
      prel->removePartner(prel->getPartner(2));
      prel->removeChildren();
      }
   // remove Childhood
   if (pdel->getChildhood()) {
      // Separate my parents from me
      pdel->getChildhood()->removeChild(pdel);
      }
   // Set All Relatives of actual to 1 and make sure remembered is 1
   setAllRelativesTo1(pold);
   if ((pmanRemembered)&&(pmanRemembered->getShort()==0))
      pmanRemembered=NULL;
   // Delete not used
   seqMan.removeAll(deleteMan0,(void*)NULL);
   seqRel.removeAll(deleteRel0,(void*)NULL);
   // Done
   pmanOrigin=pold;
   pmanActual=pold;
   // Notify Handlers
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->manAdded(seqMan);
   // Done
   changed=True;
   return(True);
}


short NMankind::getNoOfEvents()
{
   return(seqEvent.numberOfElements());
}

void NMankind::addEvent(const IString& isName,const NDate& ndFrom,const NDate& ndTo,const IString& isPlace,const IString& isDescription)
{
   // Create a new event and fill it with data
   PNEvent pevent=new NEvent();
   pevent->setName       (isName       )
          .setFrom       (ndFrom       )
          .setTo         (ndTo         )
          .setPlace      (isPlace      )
          .setDescription(isDescription);
   // Add
   peventActual=pevent;
   seqEvent.add(pevent);
   // Notify Handlers
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor) {
       seqHandler.elementAt(cursor)->eventAdded(pevent,False);
       }
   // Done
   changed=True;
}

void NMankind::delEvent(PNEvent pwhich)
{
   // Look for event in sequence
   SEQOFNEvent::Cursor eventCursor(seqEvent);
   forCursor(eventCursor)
      if (seqEvent.elementAt(eventCursor)==peventActual) break;
   if (!eventCursor.isValid()) return;
   // Save origin and do a no go
   PNEvent pold=peventActual;
   peventActual=NULL;
   // Notify Handlers
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor)
      seqHandler.elementAt(handlerCursor)->disconnentEvents();
   // start change watch
   beginChange();
   // remove event
   delete seqEvent.elementAt(eventCursor);
   seqEvent.removeAt(eventCursor);
   // Restore usable events
   if (seqEvent.numberOfElements()>0) {
      // calc actual
      if (pold==pwhich) peventActual=seqEvent.firstElement();
      else peventActual=pold;
      // Notify Handlers
      forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->eventAdded(seqEvent);
      }
   // end change watch
   endChange();
   // Done
   changed=True;
}

PNEvent NMankind::getActualEvent()
{
   return(peventActual);
}
PNEvent NMankind::makeActualEvent(PNEvent peventSetActual)
{
   // Update Date
   if (!peventSetActual) return(peventActual);
   // Remember new actual
   PNEvent poldActual=peventActual;
   peventActual=peventSetActual;
   // Notify Handlers
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor)
      seqHandler.elementAt(cursor)->actualEventChanged(poldActual,peventActual);
   // Done
   return(peventActual);
}

void NMankind::beginChange()
{
   NPerson::changed=False;
   NEvent::changed=False;
   timeStamp++;
}
void NMankind::endChange(NMankindHandler* pInitiator)
{
   // Grafik change in DrawingCanvas ?
   if (NPerson::changed) {
      changed=True;
      // Notify Handlers
      SEQOFHANDLER::Cursor cursor(seqHandler);
         forCursor(cursor) seqHandler.elementAt(cursor)->manChanged(pInitiator);
      // Structural change ?
      Boolean change=False;
      SEQOFNPerson::Cursor manCursor(seqMan);
      forCursor(manCursor) {
         PNPerson pman=seqMan.elementAt(manCursor);
         if (pman->changedThisTime())
            change|=pman->positionInFamily();
         }
      if (change) {
         // Notify Handlers
         SEQOFHANDLER::Cursor cursor(seqHandler);
            forCursor(cursor) seqHandler.elementAt(cursor)->structureChanged();
         }
      // o.k.
      }
   // Check for changes
   if (NEvent::changed) {
      changed=True;
      // Notify Handlers
      SEQOFHANDLER::Cursor cursor(seqHandler);
      forCursor(cursor) seqHandler.elementAt(cursor)->eventChanged(pInitiator);
      }
   // Done
}

unsigned short NMankind::numChildGenerations (PNPerson pwhich)
{
   unsigned short maximum=0;
   // Go through partners
   for (short partner=1;partner<=pwhich->getNoOfPartners();partner++) {
      // Calc relation
      PREL prel=pwhich->getPartnership(partner);
      // Go through children
      for (short child=1;child<=prel->getNoOfChildren();child++) {
         maximum=max(maximum,numChildGenerations(prel->getChild(child))+1);
         // ... next child
         }
      // ... next partner
      }
   // Done
   return(maximum);
}
unsigned short NMankind::numParentGenerations(PNPerson pwhich)
{
   if (!pwhich->getChildhood()) return(0);
   // Changed to 2 separate steps rather than max on 10.06.97 because of hangs
   // .. with certain trees
   unsigned short one=numParentGenerations(pwhich->getChildhood()->getPartner(1)),
                  two=numParentGenerations(pwhich->getChildhood()->getPartner(2));
   return(max(one,two)+1);
}

Boolean NMankind::makeActual(NPerson* pmanSetActual)
{
   // If same as before -> no change
   if (pmanSetActual==pmanActual)
      return(True);
   // Remember new actual
   PNPerson poldActual=pmanActual;
   pmanActual=pmanSetActual;
   // Notify Handlers
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor)
      seqHandler.elementAt(cursor)->actualChanged(poldActual,pmanActual);
   // Done
   return(True);
}
NPerson* NMankind::getActual()
{
   return(pmanActual);
}
PNPerson NMankind::getOrigin()
{
   return(pmanOrigin);
}

Boolean NMankind::makeOrigin()
{
   // Remember NewOrigin
   PNPerson pold=pmanOrigin;
   pmanOrigin=pmanActual;
   // Notify Handlers
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor)
      seqHandler.elementAt(cursor)->originChanged(pold,pmanOrigin);
   return(True);
}
Boolean NMankind::isOrigin(NPerson* pmanTest)
{
   return(pmanOrigin==pmanTest);
}
void NMankind::remember()
{
   pmanRemembered=pmanActual;
}
PNPerson NMankind::getRemembered()
{
   return(pmanRemembered);
}

Boolean NMankind::changeAssist(Assist setAssist,Assist delAssist)
{
   assist=NMankind::Assist((assist^delAssist)|setAssist);
   // Done
   return(True);
}
NMankind::Assist NMankind::getAssist()
{
   return(assist);
}

Boolean NMankind::hasChanged()
{
   // Let Handlers do some working before
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor)
      seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Done
   return(changed);
}
short int NMankind::getManCount()
{
   return(seqMan.numberOfElements());
}
short int NMankind::getRelCount()
{
   return(seqRel.numberOfElements());
}

PNPerson* NMankind::mallocManArray(short int& idxActual,short int& lenArray)
{
   // Allocate memory for array
   PNPerson* pArray=(PNPerson*)malloc(seqMan.numberOfElements()*sizeof(PNPerson));
   // Fill array
   if (pArray) {
      short int pos=0;
      PNPerson pman;
      SEQOFNPerson::Cursor visible(seqMan);
      forCursor (visible) {
         pman=seqMan.elementAt(visible);
         if (pman==pmanActual) idxActual=pos;
         pArray[pos++]=pman;
         }
      }
   lenArray=seqMan.numberOfElements();
   // Done
   return(pArray);
}

PREL* NMankind::mallocRelationArray(short int& idxActual,short int& lenArray)
{
   if (seqRel.isEmpty()) {
      lenArray=0;
      idxActual=0;
      return(NULL);
      }
   // Allocate memory for array
   PREL* pArray=(PREL*)malloc(seqRel.numberOfElements()*sizeof(PREL));
   // Fill array
   if (pArray) {
      short int pos=0;
      SEQOFNMarriage::Cursor visible(seqRel);
      forCursor (visible)
         pArray[pos++]=seqRel.elementAt(visible);
      }
   idxActual=0;
   lenArray=seqRel.numberOfElements();
   // Done
   return(pArray);
}

PNPerson NMankind::importMan()
{
   // Create ImportMan and Remember
   PNPerson pman=new NPerson();
   seqManImport.add(pman);
   return(pman);
}
PREL NMankind::importRelation()
{
   // Create ImportRelation and Remember
   PREL prel=new NMarriage();
   seqRelImport.add(prel);
   return(prel);
}

IBoolean delMan0MoveOthersTo(const PNPerson& pman,void* seqToMove) {
   if (pman->getShort()!=0) {
      ((SEQOFNPerson*)seqToMove)->add(pman);
      }
   else delete pman;
   return(True);
}

IBoolean delRel0MoveOthersTo(const PREL& prel,void* seqToMove) {
   if (prel->getShort()!=0) ((SEQOFNMarriage*)seqToMove)->add(prel);
   else delete prel;
   return(True);
}


// Connect Families to Persons
// make sure no person appears twice in a family
unsigned short countManAdded;

void NMankind::importConnect()
{
   // Some vars
   PREL prel;PNPerson pman;
   GEDCOMID mid,tid;
   unsigned short resolvedChildren,resolvedPartners;
   // go through Relations
   SEQOFNPerson::Cursor seqManImportCursor(seqManImport);
   SEQOFNMarriage::Cursor seqRelImportCursor(seqRelImport);
   forCursor(seqRelImportCursor) {
      prel=seqRelImport.elementAt(seqRelImportCursor);
      // SearchPartner1+2 + Children
      resolvedChildren=0;resolvedPartners=0;
      forCursor(seqManImportCursor) {
         pman=seqManImport.elementAt(seqManImportCursor);
         // Calc Man ID
         mid=pman->getMid();
         // This Man could be in there
         while (mid>0) {
            // Look if pman is one of the Partner
            for (unsigned short i=resolvedPartners+1;i<=2;i++) {
                if (mid==(GEDCOMID)prel->getPartner(i)) {
                   resolvedPartners++;
                   if (i!=resolvedPartners) {
                      tid=(GEDCOMID)prel->getPartner(resolvedPartners);
                      prel->changePartner(resolvedPartners,pman,False,True);
                      prel->changePartner(i,(PNPerson)tid,False,False);
                      }
                   else prel->changePartner(resolvedPartners,pman,False,True);
                   break;
                   }
                }
            if ((i==1)||(i==2)) break;
            // Look if pman is one of the children (if not already child)
            if (!pman->getChildhood())
            for (i=resolvedChildren+1;i<=prel->getNoOfChildren();i++) {
                if (mid==(GEDCOMID)prel->getChild(i)) {
                   resolvedChildren++;
                   if (i!=resolvedChildren) {
                      tid=(GEDCOMID)prel->getChild(resolvedChildren);
                      prel->changeChild(resolvedChildren,pman,False,True);
                      prel->changeChild(i,(PNPerson)tid,False,False);
                      }
                   else prel->changeChild(resolvedChildren,pman,False,True);
                   break;
                   }
                }
            // Not found in family
            break;
            }
         // Next Man
         }
      // When partners not found -> create
      for (resolvedPartners++;resolvedPartners<=2;resolvedPartners++) {
         countManAdded++;
         pman=importMan();
         pman->setName(IString(" ERROR"));
         pman->setMemo(IString("This partner #")
                      +IString((unsigned long)prel->getPartner(resolvedPartners))
                      +"\nin family #"
                      +IString(prel->getFid())
                      +"\nhas not been found or\nappears twice in there.");
         prel->changePartner(resolvedPartners,pman,False,True);
         }
      // When child not found -> create
      for (resolvedChildren++;resolvedChildren<=prel->getNoOfChildren();resolvedChildren++) {
         countManAdded++;
         pman=importMan();
         pman->setName(IString(" ERROR"));
         pman->setMemo(IString("This child #")
                      +IString((unsigned long)prel->getChild(resolvedChildren))
                      +"\nin family #"
                      +IString(prel->getFid())
                      +"\nhas not been found or\nappears twice in the family\nor is child in another family.");
         prel->changeChild(resolvedChildren,pman,False,True);
         }
      // Next Relation
      }
   // Done
}

// Check the imported tree and mark all o.k. persons/families
// make sure - no one's married twice w/same
//           - no one's his own ancestor
//           - no one's his own descendant

void searchLoopWithPartners(PNPerson pman)
{
   PREL prel,pdouble;PNPerson pother;
   // Check Partners (married twice w/same)
   for (unsigned short partner=1;partner<=pman->getNoOfPartners();partner++) {
      // Calc Partnership i
      prel=pman->getPartnership(partner);
      pother=prel->getOther(pman);
      // go through left partners and check if we are married twice
      for (unsigned search=partner+1;search<=pman->getNoOfPartners();search++) {
         pdouble=pman->getPartnership(search);
         if (pdouble->getOther(pman)==pother) {
            // remove Partnership and add all childen from it
            pdouble->removePartner(pother);
            pdouble->removePartner(pman);
            prel->addChildrenFrom(pdouble);
            // search continues one down
            search--;
            // Search for next double marriage
            }
         }
      // Next Partner
      }
   // Done
}

#define NPerson_UNKNOWN    0
#define NPerson_CHECKLATER 1
#define NPerson_CHECKED    2
#define NPerson_KNOWN      3

#define REL_UNKOWN 0
#define REL_KNOWN  1

void searchLoopInAncestors(PNPerson pman)
{
   PREL prel;PNPerson pparent,pnew,pchild,ppartner;
   // Mark this as being checked
   pman->setShort(NPerson_CHECKED);
   // Check Ancestors of me
   prel=pman->getChildhood();
   if (prel) {
      prel->setShort(REL_KNOWN);
      // Check if parents are known
      for (unsigned short parent=1;parent<=2;parent++) {
         pparent=prel->getPartner(parent);
         switch (pparent->getShort()) {
         case NPerson_UNKNOWN :                  // Unkown
         case NPerson_CHECKLATER:                // or check later
            searchLoopInAncestors(pparent);  // ... continue with parent
            break;
         case NPerson_CHECKED:                         // Known Ancestor
            countManAdded++;
            pnew=NMankind::pNMankind->importMan(); // ... Break it up here !
            pnew->setShort(NPerson_CHECKED);
            pnew->setName(IString(" ERROR"));
            pnew->setMemo(IString("Here should be person #")
                         +IString(pparent->getMid())
                         +"\nbut then it would be an\nancestor of itself.");
            prel->changePartner(parent,pnew,True,True);
            break;
         case NPerson_KNOWN :                    // Man with no loop in ancestors
            break;                           // .. we're ready
            }
         // Next Parent
         pparent->setShort(NPerson_KNOWN);      // Now he is known
         }
      // Childhood checked
      }
   // See if we can mark some direct relatives
   for (unsigned short partner=1;partner<=pman->getNoOfPartners();partner++) {
      prel=pman->getPartnership(partner);
      prel->setShort(REL_KNOWN);
      ppartner=prel->getOther(pman);
      if (ppartner->getShort()==NPerson_UNKNOWN) ppartner->setShort(NPerson_CHECKLATER);
      for (unsigned short child=1;child<=prel->getNoOfChildren();child++) {
         pchild=prel->getChild(child);
         if (pchild->getShort()==NPerson_UNKNOWN) pchild->setShort(NPerson_CHECKLATER);
         }
      }
   // Done
}

IBoolean moveCheckedOrKnownManAsKnownTo(const PNPerson& pman,void* seqToMove) {
   if ( (pman->getShort()==NPerson_CHECKED)
      ||(pman->getShort()==NPerson_KNOWN)) {
      pman->setShort(NPerson_KNOWN);
      ((SEQOFNPerson*)seqToMove)->add(pman);
      return(True);
      }
   return(False);
}
IBoolean moveKnownRelTo(const PREL& prel,void* seqToMove) {
   if (prel->getShort()==REL_KNOWN) {
      ((SEQOFNMarriage*)seqToMove)->add(prel);
      return(True);
      }
   return(False);
}
IBoolean delMan(const PNPerson& pman,void*) {
   delete pman; return(True);
}
IBoolean delRel(const PREL& prel,void*) {
   delete prel; return(True);
}

void NMankind::importCheck()
{
   PNPerson pcheck; SEQOFNPerson::Cursor manCursor(seqManImport);
   // Make sure no one's married twice w/same
   pcheck=seqManImport.firstElement();
   while (pcheck!=NULL) {
      // Check Man
      searchLoopWithPartners(pcheck);
      searchLoopInAncestors(pcheck);
      // Import Man and his descendants
      seqManImport.removeAll(moveCheckedOrKnownManAsKnownTo,&seqMan);
      seqRelImport.removeAll(moveKnownRelTo,&seqRel);
      // Seach next who's NPerson_CHECKLATER
      pcheck=NULL;
      forCursor(manCursor) {
         pcheck=seqManImport.elementAt(manCursor);
         if (pcheck->getShort()==NPerson_CHECKLATER) break;
         else pcheck=NULL;
         }
      }
   // Done
   pmanOrigin=seqMan.firstElement();
   pmanActual=pmanOrigin;
}

Boolean NMankind::importDone(IString* pisReport,Boolean canContinue,Boolean isContinue)
{
   if ((!canContinue)||(seqManImport.numberOfElements()==0)) {
      // Clean up
      if (pisReport)
         *pisReport+=NApplication::pDLLResource->loadString(SI_ImportNoData);
      seqManImport.removeAll(delMan);
      seqRelImport.removeAll(delRel);
      // Done with cleanup
      return(False);
      }
   changed=True;
   // Prepare for Import
   delAllEvents();
   delAllMen();
   unsigned short countManImported=seqManImport.numberOfElements();
   unsigned short countRelImported=seqRelImport.numberOfElements();
   // Connect Families to Persons
   countManAdded=0;
   if (!isContinue) importConnect();
   // Check for consistency, move to seqMan+seqRel
   importCheck();
   // Report
   unsigned short countManRemoved=countManImported+countManAdded-seqMan.numberOfElements();
   unsigned short countRelRemoved=countRelImported-seqRel.numberOfElements();
   *pisReport+=NApplication::pDLLResource->loadString(SI_ImportNumPersons)
                              +IString(countManImported)+"\r\n"
              +NApplication::pDLLResource->loadString(SI_ImportOrigin)
                              +pmanOrigin->getFirstName()+" "+pmanOrigin->getName()
                              +" (PID="+IString(pmanOrigin->getMid())+")\r\n"
              +NApplication::pDLLResource->loadString(SI_ImportPAdded)
                              +IString(countManAdded)+"\r\n"
              +NApplication::pDLLResource->loadString(SI_ImportPDropped)
                              +IString(countManRemoved)+"\r\n"
              +NApplication::pDLLResource->loadString(SI_ImportFDropped)
                              +IString(countRelRemoved)+"\r\n";
   // Notify Handlers
   SEQOFHANDLER::Cursor cursor(seqHandler);
   forCursor(cursor) {
      seqHandler.elementAt(cursor)->manAdded(seqMan);
      }
   // Done
   return(seqManImport.numberOfElements()>0);
}

IString NMankind::getUserFieldName(short which)
{
   if (which>nptUserFields.num()) return(IString());
   return(IString(nptUserFields.get(which)->isName));
}
NMankind::FieldType NMankind::getUserFieldType(short which)
{
   if (which>nptUserFields.num()) return(Unknown);
   return(nptUserFields.get(which)->type);
}
void NMankind::addUserField(const IString& isName,FieldType type)
{
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Add it
   UserField* pTmp=new UserField;
   pTmp->isName=isName;
   pTmp->type=type;
   nptUserFields.add(pTmp);
   // Notify Handlers
   forCursor(handlerCursor)
      seqHandler.elementAt(handlerCursor)->userFieldsModified();
   // Done
}
void NMankind::changeUserField(short which,const IString& isName)
{
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Change it (only name)
   if (which>nptUserFields.num()) return;
   nptUserFields.get(which)->isName=isName;
   // Notify Handlers
   forCursor(handlerCursor)
      seqHandler.elementAt(handlerCursor)->userFieldsModified();
   // Done
}

void NMankind::delUserField(short which)
{
   if (which>nptUserFields.num()) return;
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Delete it here and in users
   delete nptUserFields.get(which);
   nptUserFields.del(which);
   SEQOFNPerson::Cursor manCursor(seqMan);
   forCursor(manCursor) seqMan.elementAt(manCursor)->delUserField(which);
   // Notify Handlers
   forCursor(handlerCursor)
      seqHandler.elementAt(handlerCursor)->userFieldsModified();
   // Done
}
short NMankind::getNoOfUserFields()
{
   return(nptUserFields.num());
}
void NMankind::delAllUserFields()
{
   // Notify Handlers to save their data now
   SEQOFHANDLER::Cursor handlerCursor(seqHandler);
   forCursor(handlerCursor) seqHandler.elementAt(handlerCursor)->doChangesNow();
   // Delete it in persons
   SEQOFNPerson::Cursor manCursor(seqMan);
   forCursor(manCursor) seqMan.elementAt(manCursor)->delAllUserFields();
   // Free Space of UserFields here
   for (short i=1;i<=nptUserFields.num();i++)
      delete nptUserFields.get(i);
   nptUserFields.delAll();
   // Notify Handlers
   forCursor(handlerCursor)
      seqHandler.elementAt(handlerCursor)->userFieldsModified();
   // Done
}

// Class   : NMankindHandler
// Purpose : Handler for Mankind that reacts to events
NMankindHandler::NMankindHandler() {}
NMankindHandler::~NMankindHandler() {}
void NMankindHandler::collectParm(IString& isParm) {}
void NMankindHandler::spreadParm (const IString& isParm) {}
void NMankindHandler::doChangesNow() {}
void NMankindHandler::structureChanged() {}
void NMankindHandler::manChanged(NMankindHandler* pInitiator) {}
void NMankindHandler::eventChanged(NMankindHandler* pInitiator) {}
void NMankindHandler::manAdded(PNPerson which,Boolean more) {}
void NMankindHandler::manAdded(SEQOFNPerson& which) {}
void NMankindHandler::eventAdded(PNEvent which,Boolean more) {}
void NMankindHandler::eventAdded(SEQOFNEvent& which) {}
void NMankindHandler::disconnentMen() {}
void NMankindHandler::disconnentEvents() {}
void NMankindHandler::actualChanged(PNPerson pOld,PNPerson pNew) {}
void NMankindHandler::originChanged(PNPerson pOld,PNPerson pNew) {}
void NMankindHandler::progressReport(const IString& isReport) {}
void NMankindHandler::actualEventChanged(PNEvent pOld,PNEvent pNew) {}
void NMankindHandler::userFieldsModified() {}



