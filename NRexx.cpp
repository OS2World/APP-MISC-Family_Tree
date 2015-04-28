// File    : Rexx.cpp
// Purpose : Rexx Support

//#define DONOTSTDERR

#define STDIN 0
#define STDOUT 1
#define STDERR 2

//Include OS/2
#define  INCL_DOSMISC                  /* Miscellaneous doscalls              */
#define  INCL_DOSERRORS                /* OS/2 errors                         */
#define  INCL_DOSQUEUES                /* Queues                              */
#define  INCL_RXSYSEXIT                /* REXX system exits                   */
#define  INCL_REXXSAA
#define  INCL_DOSFILEMGR
#define  INCL_DOSPROCESS
#define  INCL_DOSMEMMGR
#include <os2.h>
#include <rexxsaa.h>

// Include Classlibs
#include <fstream.h>
#include <imsgbox.hpp>
#include <ifont.hpp>
#include <iclipbrd.hpp>
#include <stdio.h>

// Include Obj-specifiy

#include "FTree.h"
#include "Main.hpp"
#define INCL_INTERNAL
#include "NRexx.hpp"
#include "NMainWindow.hpp"


// REXX External Functions
PNPerson pMan;
PNPerson* pManArray;
short int idxManArray;
short int lenManArray;

PREL pRelation;
PREL* pRelationArray;
short int idxRelationArray;
short int lenRelationArray;

PNPerson* pManStack;
short int topManStack;
short int lenManStack;

PREL* pRelationStack;
short int topRelationStack;
short int lenRelationStack;

char* pParm;
short int lenParm;

Boolean bImporting=False;
Boolean bImportedMan=False;
Boolean bImportedRelation=False;

NRexxOutput* pNRexxOutput;

// --- getUserFieldName(n)
LONG APIENTRY getUserFieldName(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   // Calc UserField #
   argv->strptr[argv->strlength]='\0';
   int no=atoi(argv->strptr);
   if (no==0) return(1);
   // Check if field is there
   if (no>NMankind::pNMankind->getNoOfUserFields()) {
      retstr->strlength=0;
      return(0);
      }
   // Return Field
   strcpy(retstr->strptr,(PCHAR)NMankind::pNMankind->getUserFieldName(no));
   retstr->strlength=strlen(retstr->strptr);
   return(0);
}

// --- getUserField(n)
LONG APIENTRY getUserField(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   // Calc UserField #
   argv->strptr[argv->strlength]='\0';
   int no=atoi(argv->strptr);
   if (no==0) return(1);
   // Check if field is there
   if (no>NMankind::pNMankind->getNoOfUserFields()) {
      retstr->strlength=0;
      return(0);
      }
   // Return Field
   NUserField* nptField = pMan->getUserField(no);
   if (nptField==NULL) {
      retstr->strlength=0;
      return(0);
      }
   strcpy(retstr->strptr,(PCHAR)nptField->getValue(no));
   retstr->strlength=strlen(retstr->strptr);
   return(0);
}

// --- getPID()
LONG APIENTRY getPID(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   IString isTemp(pMan->getMid());
   strcpy(retstr->strptr,(PCHAR)isTemp);
   retstr->strlength=isTemp.size();
   return(0);
}
// --- getName()
LONG APIENTRY getName(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   ULONG len=pMan->getName().size();
   memcpy(retstr->strptr,(PCHAR)pMan->getName(),len);
   retstr->strlength=len;
   return(0);
}
// --- getFirstName()
LONG APIENTRY getFirstName(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=0) return(1);
   ULONG len=pMan->getFirstName().size();
   memcpy(retstr->strptr,(PCHAR)pMan->getFirstName(),len);
   retstr->strlength=len;
   return(0);
}
// --- getSex()
LONG APIENTRY getSex(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=0) return(1);
   strcpy(retstr->strptr,IString((int)pMan->getSex()));
   retstr->strlength=1;
   return(0);
}
LONG APIENTRY getDate(const NDate& d,LONG argc,PRXSTRING argv,PRXSTRING retstr) {
   if (argc==0) {
      if (d.hasInfo()) strcpy(retstr->strptr,d.asString());
      else strcpy(retstr->strptr,"");
      }
   else switch (*argv->strptr) {   // Part of NDate
      case 'D': strcpy(retstr->strptr,IString(d.day())      );break;
      case 'M': strcpy(retstr->strptr,IString(d.month())    );break;
      case 'm': strcpy(retstr->strptr,NApplication::pDLLResource->loadString(SI_MonthNames+d.month()));break;
      case 'Y': strcpy(retstr->strptr,IString(d.year())     );break;
      case 'd': strcpy(retstr->strptr,IString(d.days())     );break;
      case 'O': strcpy(retstr->strptr,IString(d.modifier()) );break;
      case 'c': if ((d.day()!=0)&&(d.month()!=0)&&(d.year()!=0))
                     strcpy(retstr->strptr,"1");
                else strcpy(retstr->strptr,"0");
                break;
      default : return(1);
      }
   retstr->strlength=strlen(retstr->strptr);
   return(0);
}
// --- getBirthDate(D|M|m|Y|d|c)
LONG APIENTRY getBirthDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc>1) return(1);
   return(getDate(pMan->getBirthDate(),argc,argv,retstr));
}
// --- getBirthPlace()
LONG APIENTRY getBirthPlace(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   ULONG len=pMan->getBirthPlace().size();
   memcpy(retstr->strptr,(PCHAR)pMan->getBirthPlace(),len);
   retstr->strlength=len;
   return(0);
}
// --- getDeathDate(D|M|m|Y|d|c)
LONG APIENTRY getDeathDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc>1) return(1);
   return(getDate(pMan->getDeathDate(),argc,argv,retstr));
}
// --- getDeathPlace()
LONG APIENTRY getDeathPlace(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   ULONG len=pMan->getDeathPlace().size();
   memcpy(retstr->strptr,(PCHAR)pMan->getDeathPlace(),len);
   retstr->strlength=len;
   return(0);
}
// --- getOccupation()
LONG APIENTRY getOccupation(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   ULONG len=pMan->getOccupation().size();
   memcpy(retstr->strptr,(PCHAR)pMan->getOccupation(),len);
   retstr->strlength=len;
   return(0);
}
// --- getAddress()
LONG APIENTRY getAddress(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   ULONG len=pMan->getAddress().size();
   memcpy(retstr->strptr,(PCHAR)pMan->getAddress(),len);
   retstr->strlength=len;
   return(0);
}
// --- getMemo(n)
LONG APIENTRY getMemo(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc>1) return(1);
   char* result=pMan->getMemo();
   char* eol;
   ULONG len=pMan->getMemo().size();
   // Calc result
   if ((argc==1)&&(len>0)) {
      argv->strptr[argv->strlength]='\0';
      int line=atoi(argv->strptr);
      // Check if no line parameter
      if (line==0) return(1);
      // Calc correct line
      eol=result-1;
      while (line>=1) {
         // Check for still line there
         if (!eol) {
            retstr->strlength=0;
            return(0);
            }
         result=eol+1;
         if (*result=='\n') result++;
         // search for line
         eol=strchr(result,'\r');
         line--;
         }
      if (eol) len=eol-result;
      else len=strlen(result);
      // Empty Line gets one SPACE
      if (len==0) {
         *retstr->strptr=' ';
         retstr->strlength=1;
         return(0);
         }
      }
   // Standard line data
   if (len>=retstr->strlength)
      DosAllocMem((PPVOID)&retstr->strptr,len,PAG_COMMIT|PAG_READ|PAG_WRITE);
   memcpy(retstr->strptr,result,len);
   retstr->strlength=len;
   return(0);
}
// --- getPicture()
LONG APIENTRY getPicture(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   ULONG len=pMan->getBitmap().size();
   memcpy(retstr->strptr,(PCHAR)pMan->getBitmap(),len);
   retstr->strlength=len;
   return(0);
}
// --- getFile(n)
LONG APIENTRY getFile(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   // Calc File #
   argv->strptr[argv->strlength]='\0';
   int no=atoi(argv->strptr);
   if (no==0) return(1);
   // Check if line is there
   if (no>pMan->getNoOfFiles()) {
      retstr->strlength=0;
      return(0);
      }
   // Return Filename
   strcpy(retstr->strptr,*pMan->getFile(no));
   retstr->strlength=strlen(retstr->strptr);
   return(0);
}
// --- setPID(i)
LONG APIENTRY setPID(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=1)||(!bImportedMan)) return(1);
   unsigned long id=atol(argv[0].strptr);
   if (id==0) return(1);
   pMan->setMid(id);
   *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}
// --- setName(n)
LONG APIENTRY setName(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   if (argv->strlength>256)
        { pMan->setName( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pMan->setName( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setFirstName(f)
LONG APIENTRY setFirstName(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=1) return(1);
   if (argv->strlength>256)
        { pMan->setFirstName( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pMan->setFirstName( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setSex(s)
LONG APIENTRY setSex(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=1) return(1);
   unsigned short sex=atoi(argv->strptr);
   if (sex>2) { pMan->setSex(0);   *retstr->strptr='0'; }
   else       { pMan->setSex(sex); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setXxxxDate(0|1|2|3|4,'d.m.y')  0 NONE 1 ABT 2 AFT 3 BEF 4 EST
NDate calcDate(char* date) {
   unsigned short  mod=0;
   if (*(date+1)==',') {
      mod=atoi(date);
      date+=2;
      }
   unsigned short year=0;
   unsigned char month=0;
   unsigned char   day=atoi(date);
   if (day>31) day=31;
   date=strchr(date,'.');
   if (date) {
      month=atoi(++date);
      if (month>12) month=12;
      date=strchr(date,'.');
      if (date) {
         year=atoi(++date);
         if (year>9999) year=9999;
         }
      }
   return(NDate(day,month,year,mod));
}
LONG APIENTRY setBirthDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=1) return(1);
   pMan->setBirthDate(calcDate(argv->strptr));
   *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}
// --- setBirthPlace(p)
LONG APIENTRY setBirthPlace(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   if (argv->strlength>256)
        { pMan->setBirthPlace( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pMan->setBirthPlace( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setDeathDate('d.m.y')
LONG APIENTRY setDeathDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=1) return(1);
   pMan->setDeathDate(calcDate(argv->strptr));
   *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}
// --- setDeathPlace(p)
LONG APIENTRY setDeathPlace(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   if (argv->strlength>256)
        { pMan->setDeathPlace( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pMan->setDeathPlace( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setOccupation(o)
LONG APIENTRY setOccupation(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   if (argv->strlength>256)
        { pMan->setOccupation( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pMan->setOccupation( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setAddress(a)
LONG APIENTRY setAddress(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   if (argv->strlength>256)
        { pMan->setAddress( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pMan->setAddress( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setMemo(m)
LONG APIENTRY setMemo(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   if (argv->strlength>4096)
        { pMan->setMemo( IString(argv->strptr,4096) ); *retstr->strptr='0'; }
   else { pMan->setMemo( IString(argv->strptr     ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setPicture()
LONG APIENTRY setPicture(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   if (argv->strlength>256)
        { pMan->setBitmap( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pMan->setBitmap( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- addFile(f)
LONG APIENTRY addFile(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=1) return(1);
   *(argv->strptr+argv->strlength)='\0';
   pMan->addFile(argv->strptr);
   *retstr->strptr='0';
   retstr->strlength=1;
   return(0);
}
// --- getFid()
LONG APIENTRY getFid(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=0)||(!pRelation)) return(1);
   IString isTemp(pRelation->getFid());
   strcpy(retstr->strptr,(PCHAR)isTemp);
   retstr->strlength=isTemp.size();
   return(0);
}
// --- getMarriageDate(D|M|m|Y|d|c)
LONG APIENTRY getMarriageDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc>1)||(!pRelation)) return(1);
   return(getDate(pRelation->getMarriageDate(),argc,argv,retstr));
}
// --- getMarriagePlace()
LONG APIENTRY getMarriagePlace(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc>1)||(!pRelation)) return(1);
   ULONG len=pRelation->getMarriagePlace().size();
   memcpy(retstr->strptr,(PCHAR)pRelation->getMarriagePlace(),len);
   retstr->strlength=len;
   return(0);
}
// --- getDivorceDate(D|M|m|Y|d|c)
LONG APIENTRY getDivorceDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc>1)||(!pRelation)) return(1);
   return(getDate(pRelation->getDivorceDate(),argc,argv,retstr));
}
// --- setFid(i)
LONG APIENTRY setFid(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=1)||(!bImportedRelation)) return(1);
   unsigned long id=atol(argv[0].strptr);
   if (id==0) return(1);
   if (pRelation) pRelation->setFid(id);
   *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}
// --- setMarriageDate('d.m.y')
LONG APIENTRY setMarriageDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=1)||(!pRelation)) return(1);
   pRelation->setMarriageDate(calcDate(argv->strptr));
   *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}
// --- setMarriagePlace(p)
LONG APIENTRY setMarriagePlace(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=1)||(!pRelation)) return(1);
   if (argv->strlength>256)
        { pRelation->setMarriagePlace( IString(argv->strptr,256) ); *retstr->strptr='0'; }
   else { pRelation->setMarriagePlace( IString(argv->strptr    ) ); *retstr->strptr='1'; }
   retstr->strlength=1;
   return(0);
}
// --- setDivorceDate('d.m.y')
LONG APIENTRY setDivorceDate(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=1)||(!pRelation)) return(1);
   pRelation->setDivorceDate(calcDate(argv->strptr));
   *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}

// --- doStack(([P]ush|[p]op|[S]wap)([P]erson|[F]amily))
LONG APIENTRY doStack(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   // exactly 1 parameter / length==2
   if ((argc!=1)||(argv->strlength!=2))
      return(1);
   // Check for stack operation
   switch (*argv->strptr) {
      // [D]rop([P]erson|[F]amily)
      case 'D': if (*(argv->strptr+1)=='F') {
                   if (topRelationStack==0) return(1);
                   --topRelationStack;
                   }
                else {
                   if (topManStack==0) return(1);
                   --topManStack;
                   }
                break;
      // [P]ush([P]erson|[F]amily) (if enough room)
      case 'P': if (*(argv->strptr+1)=='F') {
                   if ((topRelationStack>=lenRelationStack)||(bImportedRelation)) return(1);
                   pRelationStack[topRelationStack++]=pRelation;
                   bImportedRelation=False;
                   }
                else {
                   if ((topManStack>=lenManStack)||((bImportedMan))) return(1);
                   pManStack[topManStack++]=pMan;
                   bImportedMan=False;
                   }
                break;
      // [p]op([P]erson|[F]amily) (if not empty)
      case 'p': if (*(argv->strptr+1)=='F') {
                   if (topRelationStack==0) return(1);
                   pRelation=pRelationStack[--topRelationStack];
                   idxRelationArray=pRelation->getShort();
                   bImportedRelation=False;
                   }
                else {
                   if (topManStack==0) return(1);
                   pMan=pManStack[--topManStack];
                   idxManArray=pMan->getShort();
                   bImportedMan=False;
                   }
                break;
      // [S]wap([P]erson|[F]amily) (if not empty)
      case 'S': if (*(argv->strptr+1)=='F') {
                   if ((topRelationStack==0)||(bImportedRelation)) return(1);
                   PREL pTemp=pRelationStack[topRelationStack-1];
                   pRelationStack[topRelationStack-1]=pRelation;
                   pRelation=pTemp;
                   idxRelationArray=pTemp->getShort();
                   bImportedRelation=False;
                   }
                else {
                   if ((topManStack==0)||(bImportedMan)) return(1);
                   PNPerson pTemp=pManStack[topManStack-1];
                   pManStack[topManStack-1]=pMan;
                   pMan=pTemp;
                   idxManArray=pMan->getShort();
                   bImportedMan=False;
                   }
                break;
      // No Hit !
      default :    return(1);
      }
   // Done
   return(0);
}
// --- selectPerson(O|R|F|L|P|N|f|m|p|1|2|3|...)
LONG APIENTRY selectPerson(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=1) return(1);
   *(retstr->strptr)='1'; // Default Signal done
   unsigned short c;
   switch (*argv->strptr) {
      // [R]emembered
      case 'R' : {
                 PNPerson pRem=NMankind::pNMankind->getRemembered();
                 if (pRem==NULL)
                    *(retstr->strptr)='0';
                 else pMan=pRem;
                 }
                 break;
      // [O]rigin
      case 'O' : pMan=NMankind::pNMankind->getOrigin();
                 break;
      // [F]irst
      case 'F' : pMan=pManArray[0];break;
      // [L]ast
      case 'L' : pMan=pManArray[lenManArray-1];;break;
      // [N]ext (if not last)
      case 'N' : if (idxManArray==lenManArray-1) *(retstr->strptr)='0'; // Signal not moved
                 else pMan=pManArray[idxManArray+1]; // Move It
                 break;
      // [P]revious (if not first)
      case 'P' : if (idxManArray==0) *(retstr->strptr)='0';
                 else  pMan=pManArray[idxManArray-1]; // Move It
                 break;
      // [f]ather of family (if family)
      case 'f' : if (bImportedRelation) return(1);
                 if (!pRelation) *(retstr->strptr)='0';
                 else pMan=pRelation->getPartner(1);
                 break;
      // [m]other of family (if family)
      case 'm' : if (bImportedRelation) return(1);
                 if (!pRelation) *(retstr->strptr)='0';
                 else pMan=pRelation->getPartner(2);
                 break;
      // [p]artner of actual in family (if family)
      case 'p' : if (bImportedRelation) return(1);
                 if (!pRelationArray) *(retstr->strptr)='0';
                 else pMan=pRelation->getOther(pMan);
                 break;
      // [1]st [2]nd ... child of actual family (if exists)
      default  : c=atoi(argv->strptr);
                 if ((bImportedRelation)||(c<=0)) return(1);
                 if ( (!pRelation)
                    ||(!pRelation->getChild(c)) )
                    *(retstr->strptr)='0'; // Signal not moved
                 else pMan=pRelation->getChild(c);
                 break;
      // Done
      }
   bImportedMan=False;
   idxManArray=pMan->getShort();
   retstr->strlength=1;
   return(0);
}
// --- selectFamily(F|L|P|N|p|1|2|3|...)
LONG APIENTRY selectFamily(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if (argc!=1) return(1);
   unsigned short f;
   if (!pRelation) *(retstr->strptr)='0'; // Signal nothing done
   else {
      *(retstr->strptr)='1'; // Default Signal done
      switch (*argv->strptr) {
         // [F]irst
         case 'F' : pRelation=pRelationArray[0];break;
         // [L]ast
         case 'L' : pRelation=pRelationArray[lenRelationArray-1];break;
         // [N]ext (if not last)
         case 'N' : if (idxRelationArray==lenRelationArray-1)
                       *(retstr->strptr)='0';                          // Signal not moved
                    else pRelation=pRelationArray[idxRelationArray+1]; // Move It
                    break;
         // [P]revious (if not first)
         case 'P' : if (idxRelationArray==0) *(retstr->strptr)='0';
                    else pRelation=pRelationArray[idxRelationArray-1]; // Move It
                    break;
         // [p]arent family (if exists)
         case 'p' : if (bImportedMan) return(1);
                    if (!pMan->hasParents())
                       *(retstr->strptr)='0'; // Signal not moved
                    else pRelation=pMan->getChildhood();
                    break;
         // [1]st [2]nd ... family of actual (if exists)
         default  : f=atoi(argv->strptr);
                    if ((bImportedMan)||(f<=0)) return(1);
                    if (!pMan->getPartnership(f))
                       *(retstr->strptr)='0'; // Signal not moved
                    else pRelation=pMan->getPartnership(f);
                    break;
         // Done
         }
      idxRelationArray=pRelation->getShort();
      }
   bImportedMan=False;
   retstr->strlength=1;
   return(0);
}

int _Optlink compareMan(const void* key,const void* element){
   short int idx=0;
   short int rc;
   short int lt;
   short int gt;
   Boolean ascending;

   while (idx<lenParm) {
      ascending=True;lt=-1;gt=1;
      switch (pParm[idx]) {
         // Name ,n(descending) ,N(ascending)
         case 'n':
            ascending=False;
         case 'N':
            idx+=2;
            rc=strcmpi( (*((PNPerson*)key))->getName() , (*((PNPerson*)element))->getName() );
            if (rc!=0) return(ascending ? rc : 0-rc);
            break;
         // FirstName ,f(descending) ,F(ascending)
         case 'f' :
            ascending=False;
         case 'F' :
            idx+=2;
            rc=strcmpi( (*((PNPerson*)key))->getFirstName() , (*((PNPerson*)element))->getFirstName() );
            if (rc!=0) return(ascending ? rc : 0-rc);
            break;
         // DeathDate ,d(descending) ,D(ascending) [DMY]
         // BirthDate ,b(descending) ,B(ascending) [DMY]
         case 'b' :
         case 'd' :
            ascending=False;lt=1;gt=-1;
         case 'B' :
         case 'D' :  {
            NDate dKey,dElement;
            if ( (pParm[idx]=='b') || (pParm[idx]=='B') ) {
               dKey=(*((PNPerson*)key))->getBirthDate();
               dElement=(*((PNPerson*)element))->getBirthDate();
               }
            else {
               dKey=(*((PNPerson*)key))->getDeathDate();
               dElement=(*((PNPerson*)element))->getDeathDate();
               }
            idx++;
            // Check for special Parm
            switch (pParm[idx]) {
               case 'Y':
                  idx+=2;
                  if (dKey.year() < dElement.year()) return(lt);
                  if (dKey.year() > dElement.year()) return(gt);
                  break;
               case 'M':
                  idx+=2;
                  if (dKey.month() < dElement.month()) return(lt);
                  if (dKey.month() > dElement.month()) return(gt);
                  break;
               case 'D':
                  idx+=2;
                  if (dKey.day() < dElement.day()) return(lt);
                  if (dKey.day() > dElement.day()) return(gt);
                  break;
               default:
                  idx++;
                  if (dKey < dElement) return(lt);
                  if (dElement < dKey) return(gt);
                  break;
               }
            } break;
         // Longevity ,l(descending) ,L(ascending)
         case 'l' :
            ascending=False;lt=1;gt=-1;
         case 'L' :  {
            idx++;
            NDate birthKey=(*((PNPerson*)key))->getBirthDate();
            NDate birthElement=(*((PNPerson*)element))->getBirthDate();
            NDate deathKey=(*((PNPerson*)key))->getDeathDate();
            NDate deathElement=(*((PNPerson*)element))->getDeathDate();
            // Check for Age
            if (deathKey.days()-birthKey.days() < deathElement.days()-birthElement.days())
               return(lt);
            if (deathElement.days()-birthElement.days() < deathKey.days()-birthKey.days())
               return(gt);
            } break;
         // PID ,p(descending) ,P(ascending)
         case 'p':
            ascending=False;lt=1;gt=-1;
         case 'P': {
            idx++;
            // Check for Age
            if ( (*((PNPerson*)key))->getMid() < (*((PNPerson*)element))->getMid() )
               return(lt);
            if ( (*((PNPerson*)key))->getMid() > (*((PNPerson*)element))->getMid() )
               return(gt);
            } break;
         // Default unknown sort criteria
         default :
            return(lt);
         }
      }
   return(lt);
}

int _Optlink compareRelation(const void* key,const void* element){
   short int idx=0;
   short int rc;
   short int lt;
   short int gt;
   Boolean ascending;

   while (idx<lenParm) {
      ascending=True;lt=-1;gt=1;
      switch (pParm[idx]) {
         // MarriageDate ,m(descending) ,M(ascending) [DMY]
         // DivorceDate ,d(descending) ,D(ascending) [DMY]
         case 'm' :
         case 'd' :
            ascending=False;lt=1;gt=-1;
         case 'M' :
         case 'D' : {
            NDate dKey,dElement;
            if ( (pParm[idx]=='m') || (pParm[idx]=='M') ) {
               dKey=(*((PREL*)key))->getMarriageDate();
               dElement=(*((PREL*)element))->getMarriageDate();
               }
            else {
               dKey=(*((PREL*)key))->getDivorceDate();
               dElement=(*((PREL*)element))->getDivorceDate();
               }
            idx++;
            // Check for special Parm
            switch (pParm[idx]) {
               case 'Y':
                  idx+=2;
                  if (dKey.year() < dElement.year()) return(lt);
                  if (dKey.year() > dElement.year()) return(gt);
                  break;
               case 'M':
                  idx+=2;
                  if (dKey.month() < dElement.month()) return(lt);
                  if (dKey.month() > dElement.month()) return(gt);
                  break;
               case 'D':
                  idx+=2;
                  if (dKey.day() < dElement.day()) return(lt);
                  if (dKey.day() > dElement.day()) return(gt);
                  break;
               default:
                  idx++;
                  if (dKey < dElement) return(lt);
                  if (dElement < dKey) return(gt);
                  break;
               }
            } break;
         // No more
         default :
            return(lt);
         }
      }
   return(lt);
}


// --- sortPersons({ [ N|n | F|f | (B|b)[D|M|Y] | (D|d)[D|M|Y] | L|l | P|p ] ,})
LONG APIENTRY sortPersons(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if ((argc!=1)||(argv->strlength==0)) return(1);
   // Calc criteria
   pParm=argv->strptr;
   lenParm=argv->strlength;
   // sort array
   qsort(pManArray,lenManArray,sizeof(PNPerson),compareMan);
   // Set Shorts
   for (short int i=0;i<lenManArray;i++)
       pManArray[i]->setShort(i);
   // Done
   return(0);
}
// --- sortFamilies({ [ (M|m)[D|M|Y] ] ,})
LONG APIENTRY sortFamilies(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr){
   if ((argc!=1)||(argv->strlength==0)) return(1);
   // Empty relations don't need to be sorted
   if (!pRelation) return(0);
   // Calc criteria
   pParm=argv->strptr;
   lenParm=argv->strlength;
   // sort array
   qsort(pRelationArray,lenRelationArray,sizeof(PREL),compareRelation);
   // Set Shorts
   for (short int i=0;i<lenRelationArray;i++)
       pRelationArray[i]->setShort(i);
   // Done
   return(0);
}
// --- importPerson()
LONG APIENTRY importPerson(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   // Check Args
   if (argc!=0) return(1);
   // Start Importing if not already done
   if (!bImporting) {
      NMankind::pNMankind->importDone(NULL,False);
      bImporting=True;
      }
   // New ImportMan
   PNPerson pNewMan=NMankind::pNMankind->importMan();
   if (!pNewMan) return(1);
   bImportedMan=True; // Actual is imported
   pMan=pNewMan;
   // Done
   return(0);
}
// --- importFamily()
LONG APIENTRY importFamily(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   // Check Args
   if (argc!=0) return(1);
   // Start Importing if not already done
   if (!bImporting) {
      NMankind::pNMankind->importDone(NULL,False);
      bImporting=True;
      }
   // New ImportMan
   PREL pNewRelation=NMankind::pNMankind->importRelation();
   if (!pNewRelation) return(1);
   bImportedRelation=True;  // Actual is imported
   pRelation=pNewRelation;
   // Done
   return(0);
}
// --- importAddPartner(n)
LONG APIENTRY importAddPartner(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=1)||(!pRelation)||(!bImportedRelation)) return(1);
   // Calc Id
   unsigned long id=atol(argv->strptr);
   if ((id==0)||(!pRelation->importAddPartner(id))) *retstr->strptr='0';
   else *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}
// --- importAddChild(n)
LONG APIENTRY importAddChild(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if ((argc!=1)||(!pRelation)||(!bImportedRelation)) return(1);
   // Calc Id
   unsigned long id=atol(argv->strptr);
   if ((id==0)||(!pRelation->importAddChild(id))) *retstr->strptr='0';
   else *retstr->strptr='1';
   retstr->strlength=1;
   return(0);
}
// --- importDone()
LONG APIENTRY importDone(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   // Finish Import
   IString report;
   if ( (NMankind::pNMankind->importDone(&report,True,bImporting==False))
      &&(!NMainWindow::pNMainWindow->isImportPreserve()                 )) {
      // Have to ask user about droppable persons
      PNPerson porigin=NMankind::pNMankind->getOrigin();
      IMessageBox msgBox(pNRexxOutput);
      Boolean rc=msgBox.show(NApplication::pDLLResource->loadString(SI_RexxUnfinishedImport)
                            ,IMessageBox::yesNoButton|IMessageBox::queryIcon|IMessageBox::moveable);
      // Check user wish and kill remaining (if wanted)
      if (rc==IMessageBox::no)
         NMankind::pNMankind->importDone(NULL,False);
      // Done with import
      }
   bImporting=False;
   pMan=pManArray[idxManArray];
   if (pRelationArray) pRelation=pRelationArray[idxRelationArray];
   else pRelation=NULL;
   // Done
   strcpy(retstr->strptr,report);
   retstr->strlength=report.length();
   return(0);
}
// --- getLanguage()
LONG APIENTRY getLanguage(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   strcpy(retstr->strptr,NApplication::pDLLResource->loadString(SI_Language));
   retstr->strlength=strlen(retstr->strptr);
   return(0);
}
// --- getFilename()
LONG APIENTRY getFilename(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=2) return(1);
   IFileDialog::Settings fsettings;
   fsettings.setTitle(argv[0].strptr);
   fsettings.setFileName(argv[1].strptr);
   IFileDialog filedialog(IWindow::desktopWindow(),pNRexxOutput
                         ,fsettings,IFileDialog::classDefaultStyle|IFileDialog::selectableListbox);
   if (filedialog.pressedOK())
      strcpy(retstr->strptr,(char*)filedialog.fileName());
   else *retstr->strptr=0;
   retstr->strlength=strlen(retstr->strptr);
   return(0);
}
// --- getNameOfTree()
LONG APIENTRY getNameOfTree(PSZ name,LONG argc,PRXSTRING argv,PSZ queue,PRXSTRING retstr) {
   if (argc!=0) return(1);
   strcpy(retstr->strptr,NApplication::isTree);
   retstr->strlength=strlen(retstr->strptr);
   return(0);
}


// Class   : NRexxOutput
// Purpose : Output of Rexx Run
NRexxOutput::NRexxOutput(IWindow* pOwner,IString& isSetScript,IString& isSetName)
   : IFrameWindow(IResourceId(ID_NRexxOutput)
                 ,IWindow::desktopWindow()
                 ,pOwner
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::sizingBorder
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::windowList
                  |IFrameWindow::titleBar
                  |IFrameWindow::noMoveWithOwner
                  |IFrameWindow::shellPosition)
   , ICommandHandler()
   , IKeyboardHandler()
   , IFrameHandler()
   , itTitle(this)
   , imccCanvas(ID_NRexxOutput+1,this,this)
   , ilbOutput(ID_NRexxOutput+10,&imccCanvas,&imccCanvas,IRectangle()
               ,IBaseListBox::horizontalScroll
                |IWindow::visible|IBaseListBox::extendedSelect)
   , ipbDone(ID_NRexxOutput+20,&imccCanvas,&imccCanvas,IRectangle()
           ,IWindow::visible|IPushButton::defaultButton)
   , ipbCancel(ID_NRexxOutput+21,&imccCanvas,&imccCanvas)
   , ipbSave(ID_NRexxOutput+22,&imccCanvas,&imccCanvas)
   , ipbPrint(ID_NRexxOutput+23,&imccCanvas,&imccCanvas)
   , ipbCopy(ID_NRexxOutput+24,&imccCanvas,&imccCanvas)

   , threadRexx()
   , threadRead()
{
   // Data
   setIcon(RES_IconFTree);
   isScript=isSetScript;
   itTitle.setObjectText(NApplication::pDLLResource->loadString(SI_RexxTitle)
                        +isSetName+" ("+isSetScript+")");
   itTitle.setViewText(IString());
   // 2th Row
   ipbDone.setText(NApplication::pDLLResource->loadString(SI_RexxDone))
          .enableTabStop()
          .disable();
   ipbCancel.setText(NApplication::pDLLResource->loadString(SI_RexxCancel));
   ipbSave.setText(NApplication::pDLLResource->loadString(SI_RexxSave))
          .enableTabStop()
          .disable();
   ipbPrint.setText(NApplication::pDLLResource->loadString(SI_RexxPrint))
           .enableTabStop()
           .disable();
   ipbCopy.setText("Copy")
          .enableTabStop()
          .disable();
   // Setup Layout
   setClient(&imccCanvas);
   imccCanvas.addToCell(&ilbOutput ,1,1,5,1);
   imccCanvas.addToCell(&ipbDone,   1,2,1,1);
   imccCanvas.addToCell(&ipbCancel, 2,2,1,1);
   imccCanvas.addToCell(&ipbSave,   3,2,1,1);
   imccCanvas.addToCell(&ipbPrint,  4,2,1,1);
   imccCanvas.addToCell(&ipbCopy,   5,2,1,1);
   imccCanvas.setColumnWidth(1,imccCanvas.columnWidth(1),True);
   imccCanvas.setRowHeight(1,imccCanvas.rowHeight(1),True);
   // Handler
   ICommandHandler::handleEventsFor(this);
   IKeyboardHandler::handleEventsFor(this);
   IFrameHandler::handleEventsFor(this);
   // SetUp Position & Size & Font
   if (  (NRexx::irProfileRectangle.width()>0)
      && (NRexx::irProfileRectangle.height()>0) )
      moveSizeTo(NRexx::irProfileRectangle);
   else
      moveSizeTo(IRectangle(pOwner->position(),pOwner->size()));
   ilbOutput.setFont(NRexx::ifProfileFont);
   // Start to work
   startRexxing();
   // Done
}
NRexxOutput::~NRexxOutput()
{
}

Boolean NRexxOutput::closed(IFrameEvent& frameEvent)
{
   if (threadRead.isStarted()) {
      DosBeep(200,200);
      return(True);
      }
   // Save Frame Status
   NRexx::irProfileRectangle=rect();
   NRexx::ifProfileFont=ilbOutput.font();
   // Done
   return(False);
} /* end NListDialog::closed() */


Boolean NRexxOutput::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      // Done Button
      case ID_NRexxOutput+20 :
         close();
         break;
      // Cancel Button
      case ID_NRexxOutput+21 :
         ipbCancel.disable();
         RexxSetHalt(IApplication::currentPID(),threadRexx.id());
         break;
      // Save Button
      case ID_NRexxOutput+22 : {
         IFileDialog::Settings fsettings;
         fsettings.setTitle(NApplication::pDLLResource->loadString(SI_RexxOutputSave));
         fsettings.setFileName("*.out");
         fsettings.setSaveAsDialog();
         IFileDialog filedialog(IWindow::desktopWindow(),this
                               ,fsettings,IFileDialog::classDefaultStyle|IFileDialog::selectableListbox);
         if (filedialog.pressedOK()) {
            // Saving
            fstream fs;
            fs.open(filedialog.fileName(),ios::binary|ios::out);
            if (!fs.good()) {
               IMessageBox msgBox( this );
               msgBox.show(NApplication::pDLLResource->loadString(SI_RexxOutputSaveError)
                          ,IMessageBox::informationIcon|IMessageBox::okButton|IMessageBox::moveable );
               }
            else {
               IBaseListBox::Cursor cursor(ilbOutput,IBaseListBox::Cursor::allItems);
               for (cursor.setToFirst();cursor.isValid();cursor.setToNext())
                  fs << ilbOutput.elementAt(cursor)<< "\r\n";
               fs.close();
               if (!fs.good()) {
                  IMessageBox msgBox( this );
                  msgBox.show(NApplication::pDLLResource->loadString(SI_RexxOutputSaveError)
                             ,IMessageBox::informationIcon|IMessageBox::okButton|IMessageBox::moveable );
                  }
               }
            }
         } break;
      // Copy Button
      case ID_NRexxOutput+24 : {
         IString isTemp;
         if (ilbOutput.selection()==IListBox::notFound) {
            IListBox::Cursor cursor(ilbOutput,IListBox::Cursor::allItems);
            forCursor (cursor) isTemp+=ilbOutput.elementAt(cursor)+"\r\n";
            }
         else {
            IListBox::Cursor cursor(ilbOutput,IListBox::Cursor::selectedItems);
            forCursor (cursor) isTemp+=ilbOutput.elementAt(cursor)+"\n";
            }
         if (isTemp.length()>0) {
            IClipboard cb(handle());
            cb.setText(isTemp);
            }
         } break;
      }
   // Done everything
   return(True);
}

Boolean NRexxOutput::virtualKeyPress(IKeyboardEvent& event)
{
   if (event.virtualKey()==IKeyboardEvent::esc)
      ipbDone.click();
   // I'm not the last handler
   return(False);
}

void NRexxOutput::startRexxing()
{
   // Setup Pipes
   HFILE hfAlias;
   hfSTDOUT=-1;
   hfSTDERR=-1;

   DosDupHandle(STDOUT,&hfSTDOUT);            // Save STDOUT
   #ifndef DONOTSTDERR
   DosDupHandle(STDERR,&hfSTDERR);            // Save STDERR
   #endif
   DosCreatePipe(&hfRead,&hfWrite,256);       // Create New Pipe
   hfAlias=STDOUT;
   DosDupHandle(hfWrite,&hfAlias);            // Duplicate new Out to STDOUT
   #ifndef DONOTSTDERR
   hfAlias=STDERR;
   DosDupHandle(hfWrite,&hfAlias);            // Duplicate new Out to STDERR
   #endif
   DosClose(hfWrite);                         // Close New Pipe
   // Start Threads
   threadRexx.setPriority(IApplication::regular,00);
   threadRead.setPriority(IApplication::regular,01);
   IThread::current().setPriority(IApplication::regular,02);
   threadRexx.start(new IThreadMemberFn<NRexxOutput>(*this,NRexxOutput::rexxing),True);
   threadRead.start(new IThreadMemberFn<NRexxOutput>(*this,NRexxOutput::reading),True);
   // Done for now
}

void NRexxOutput::reading() //  \r\n = '0d0a'x
{
   char buffer[256]="";ULONG ulRead; // >237 stops output ??
   char *newline;ULONG pos=0;Boolean kill=False;
   // READ
   do {
      // Read from Pipe
      if (DosRead(hfRead,buffer+pos,sizeof(buffer)-1-pos,&ulRead)>0)
         ulRead=0;
      // if something read
      if (ulRead) {
         // Mark end with 0
         buffer[pos+ulRead]='\0';
         // Debug Out ilbOutput.addAsLast(IString("<")+buffer+IString(">"));
         // Check for newline from last line
         if (*buffer=='\n') strcpy(buffer,buffer+1);
         // Calc lines
         do {
            // check for newline
            newline=strpbrk(buffer,"\r\n");
            if (newline) {
               // ... calc line
               if (*newline=='\n')
                  *newline++='\0';
               else {
                  while (*newline=='\r')
                     *newline++='\0';
                  if (*newline=='\n') *newline++='\0';
                  }
               // ... check for overfull line
               if (kill) kill=False; // if kill rest of line -> done
               else ilbOutput.addAsLast(buffer);
               // ... jump over written/discarded
               strcpy(buffer,newline);
               }
            // Next line
            }
         while (newline);
         // Calc next start
         pos=strlen(buffer);
         // 255 chars read and no newline ?
         if (pos==255) {
            pos=0;          // Start at 1st of buffer
            if (!kill) {    // not yet kill rest of line ?
               ilbOutput.addAsLast(buffer); // last output from this line
               kill=True;                   // rest is killed
               }
            }
         }
      // Next if something was read
      }
   while (ulRead);
   DosClose(hfRead);
   // Enable End
   NMainWindow::pNMainWindow->doneWithRexx();
   ipbDone.enable();
   ipbSave.enable();
   ipbCopy.enable();
   enableUpdate();
   IThread::current().setPriority(IApplication::regular,00);
}

void NRexxOutput::rexxing()
{
   // Initialize RunEnvironment + Set Shorts
   pManArray=NMankind::pNMankind->mallocManArray(idxManArray,lenManArray);
   for (short int i=0;i<lenManArray;i++)
       pManArray[i]->setShort(i);
   pMan=pManArray[idxManArray];
   pRelationArray=NMankind::pNMankind->mallocRelationArray(idxRelationArray,lenRelationArray);
   if (pRelationArray) {
      for (i=0;i<lenRelationArray;i++)
          pRelationArray[i]->setShort(i);
      pRelation=pRelationArray[idxRelationArray];
      }
   else pRelation=NULL;
   pNRexxOutput=this;
   bImporting=False;
   bImportedMan=False;
   bImportedRelation=False;
   // Initialize Stacks
   lenManStack=1000;topManStack=0;
   pManStack=(PNPerson*)malloc(lenManStack*sizeof(PNPerson));
   lenRelationStack=500;topRelationStack=0;
   pRelationStack=(PREL*)malloc(lenRelationStack*sizeof(PREL));
   // START REXX
   RXSTRING arg;                       /* argument string for REXX  */
   RXSTRING rexxretval;                /* return value from REXX    */
   long     rc;                        /* return code from REXX     */
   SHORT    rexxrc = 0;                /* return code from function */
   rexxretval.strlength = 0L;          /* initialize return to empty*/
   MAKERXSTRING(arg, "", strlen(""));  /* create input argument     */
   rc=RexxStart((LONG)      1,             /* number of arguments   */
               (PRXSTRING)  &arg,          /* array of arguments    */
               (PSZ)        IString("FTXs\\")+isScript,      /* name of REXX file     */
               (PRXSTRING)  0,             /* No INSTORE used       */
               (PSZ)        "FTX",         /* Command env. name     */
               (LONG)       RXSUBROUTINE,  /* Code for how invoked  */
               (PRXSYSEXIT) 0,             /* No EXITs on this call */
               (PSHORT)     &rexxrc,       /* Rexx program output   */
               (PRXSTRING)  &rexxretval ); /* Rexx program output   */
   DosFreeMem(rexxretval.strptr);       /* Release storage from REXX*/
   // Free RunEnvironment
   if (bImporting) NMankind::pNMankind->importDone(NULL,False);
   free(pManStack);
   free(pRelationStack);
   free(pManArray);
   free(pRelationArray);
   // End reading
   HFILE hfAlias;

   DosClose(STDOUT);
   hfAlias=STDOUT;
   DosDupHandle(hfSTDOUT,&hfAlias);   // Restore original STDOUT
   DosClose(hfSTDOUT);
   #ifndef DONOTSTDERR
   DosClose(STDERR);
   hfAlias=STDERR;
   DosDupHandle(hfSTDERR,&hfAlias);   // Restore original STDERR
   DosClose(hfSTDERR);
   #endif
   // Report End
   itTitle.setViewText(NApplication::pDLLResource->loadString(SI_RexxFinished)
                      +"(rc="
                      +IString(rc)+"/"
                      +IString(rexxrc)+")");
   ipbCancel.disable();
   // Done
}

// Class   : NRexxDialog
// Purpose : User Interface for NRexx
NRexxDialog::NRexxDialog(IWindow* pOwner)
   : IFrameWindow(IResourceId(ID_NRexxDialog)
                 ,IWindow::desktopWindow()
                 ,pOwner
                 ,IRectangle()
                 ,IFrameWindow::systemMenu
                  |IFrameWindow::dialogBorder
                  |IFrameWindow::dialogBackground
                  |IFrameWindow::titleBar
                  |IFrameWindow::noMoveWithOwner
                  |IFrameWindow::shellPosition
                 ,NApplication::pDLLResource->loadString(SI_RexxDialog))
   , ICommandHandler()
   , ISelectHandler()
   , IKeyboardHandler()
   , imccCanvas(ID_NRexxDialog+1,this,this)
   , istScript(ID_NRexxDialog+20,&imccCanvas,&imccCanvas)
   , icbScript(ID_NRexxDialog+21,&imccCanvas,&imccCanvas,IRectangle()
              ,IWindow::visible|IBaseComboBox::readOnlyDropDownType)
   , istName(ID_NRexxDialog+20,&imccCanvas,&imccCanvas)
   , iefName(ID_NRexxDialog+21,&imccCanvas,&imccCanvas,IRectangle()
            ,IWindow::visible|IEntryField::readOnly)
   , istAuthor(ID_NRexxDialog+30,&imccCanvas,&imccCanvas)
   , iefAuthor(ID_NRexxDialog+31,&imccCanvas,&imccCanvas,IRectangle()
              ,IWindow::visible|IEntryField::readOnly)
   , istNote(ID_NRexxDialog+40,&imccCanvas,&imccCanvas)
   , imleNote(ID_NRexxDialog+41,&imccCanvas,&imccCanvas,IRectangle()
             ,IMultiLineEdit::verticalScroll
              |IMultiLineEdit::wordWrap
              |IMultiLineEdit::ignoreTab
              |IMultiLineEdit::readOnly
              |IWindow::visible)
   , ipbRun(ID_NRexxDialog+50,&imccCanvas,&imccCanvas,IRectangle()
           ,IWindow::visible|IPushButton::defaultButton)
   , ipbRemember(ID_NRexxDialog+51,&imccCanvas,&imccCanvas)
   , ipbCancel(ID_NRexxDialog+52,&imccCanvas,&imccCanvas)
{
   setFont(IFont("Helv",8));
   imccCanvas.setDefaultCell(ISize(3,2));
   // 1st Row
   istScript.setText(NApplication::pDLLResource->loadString(SI_RexxScript))
            .setAlignment(IStaticText::centerLeft);
   icbScript.setMinimumRows(10)
            .enableTabStop();
   // 2nd Row
   istName.setText(NApplication::pDLLResource->loadString(SI_RexxLongname))
          .setAlignment(IStaticText::centerLeft);
   iefName.setBackgroundColor(IColor(223,223,223));
   // 3rd Row
   istAuthor.setText(NApplication::pDLLResource->loadString(SI_RexxAuthor))
            .setAlignment(IStaticText::centerLeft);
   iefAuthor.setBackgroundColor(IColor(223,223,223));
   // 4th Row
   istNote.setText(NApplication::pDLLResource->loadString(SI_RexxComment))
          .setAlignment(IStaticText::centerLeft);
   imleNote.setBackgroundColor(IColor(223,223,223))
           .setMinimumSize(imleNote.minimumSize()*IPair(2,2));
   // 5th Row
   ipbRun.setText(NApplication::pDLLResource->loadString(SI_RexxStart))
         .enableTabStop();
   ipbRemember.disableMouseClickFocus()
              .setText(NApplication::pDLLResource->loadString(SI_RexxRemember))
              .enableTabStop();
   ipbCancel.setText(NApplication::pDLLResource->loadString(SI_RexxCancel))
            .enableTabStop();
   // Setup Layout
   setClient(&imccCanvas);

   imccCanvas.addToCell(&istScript,2,2,1,1);
   imccCanvas.addToCell(&icbScript,3,2,2,1);

   imccCanvas.addToCell(&istName  ,2,4,1,1);
   imccCanvas.addToCell(&iefName  ,3,4,2,1);

   imccCanvas.addToCell(&istAuthor,2,6,1,1);
   imccCanvas.addToCell(&iefAuthor,3,6,2,1);

   imccCanvas.addToCell(&istNote  ,2,8,1,1);
   imccCanvas.addToCell(&imleNote ,3,8,2,1);

   imccCanvas.addToCell(&ipbRun,     2,10,1,1);
   imccCanvas.addToCell(&ipbRemember,3,10,1,1);
   imccCanvas.addToCell(&ipbCancel,  4,10,1,1);
   // Done
   moveSizeToClient(IRectangle(IPoint(0,0),imccCanvas.minimumSize()));
   moveTo(pOwner->position()+pOwner->size()/2-size()/2);
   // Find Scripts
   findScripts();
   if (icbScript.count()==0) {
      ipbRun.disable();
      ipbRemember.disable();
      icbScript.addAscending(NApplication::pDLLResource->loadString(SI_RexxNotFound));
      icbScript.select(0);
      }
   else {
      ISelectHandler::handleEventsFor(&icbScript);
      icbScript.select(0);
      }
   // Handler
   ICommandHandler::handleEventsFor(this);
   IKeyboardHandler::handleEventsFor(this);
   // Done
}

NRexxDialog::~NRexxDialog() {}

void NRexxDialog::findScripts()
{
   // Check for Rexx-Scripts
   HDIR hdir=HDIR_SYSTEM;FILEFINDBUF3 buffer;ULONG count=1;
   ULONG rc=DosFindFirst("FTXs\\*.ftx",&hdir,FILE_NORMAL
                        ,&buffer,sizeof(FILEFINDBUF3),&count,FIL_STANDARD);
   IString isFile;
   while ((rc!=ERROR_NO_MORE_FILES)&&(rc==NO_ERROR)) {
      // Enter script in comboBox
      isFile=buffer.achName;
      isFile.remove(1,isFile.lastIndexOf('\\'));
      icbScript.addAscending(isFile);
      // Get Next
      count=1;
      rc = DosFindNext(hdir,&buffer,sizeof(FILEFINDBUF3),&count);
      if (rc!=NO_ERROR)
         return;
      }
   // Close find
   DosFindClose(hdir);
   // Done
}

Boolean NRexxDialog::command(ICommandEvent& event)
{
   switch (event.commandId()) {
      // Run Button
      case ID_NRexxDialog+50 :
         NRexx::pNRexx->setScript(icbScript.itemText(icbScript.selection()),iefName.text());
         dismiss(DID_OK);
         break;
      // Remember Button
      case ID_NRexxDialog+51 :
         NRexx::pNRexx->remember(icbScript.itemText(icbScript.selection()),iefName.text());
         ipbRemember.disable();
         break;
      // Cancel Button
      case ID_NRexxDialog+52 :
         dismiss(DID_CANCEL);
         break;
      }
   // Done everything
   return(True);
}

Boolean analyseScript(const IString& isFile,IString& isName,IString& isAuthor,IString& isNote)
{
   fstream fsFile;
   fsFile.open(IString("FTXs\\")+isFile,ios::binary|ios::in|ios::nocreate);
   if (!fsFile.good()) return(False);
   // Read Author
   char cTemp[2048+1];char c;
   fsFile.get(cTemp,2048,'<');
   fsFile.get(c);
   if (c=='<') {
      fsFile.get(cTemp,2048,'>');
      fsFile.get(c);
      if (c=='>') isAuthor=IString(cTemp);
      }
   // Read Note
   fsFile.get(cTemp,2048,'<');
   fsFile.get(c);
   if (c=='<') {
      fsFile.get(cTemp,2048,'>');
      fsFile.get(c);
      if (c=='>') isNote=IString(cTemp);
      }
   // Read Name
   fsFile.get(cTemp,2048,'<');
   fsFile.get(c);
   if (c=='<') {
      fsFile.get(cTemp,2048,'>');
      fsFile.get(c);
      if (c=='>') isName=IString(cTemp);
      }
   // Language parsing
   IString isLang(NApplication::pDLLResource->loadString(SI_Language));

   unsigned pos=isName.indexOf(isLang+":");
   if (pos>0) {
      isName.remove(1,pos+isLang.length());
      pos=isName.indexOf(":"+isLang);
      if (pos>0) isName.remove(pos);
      }
   else { // No Language found use English (Suomi)
      isLang="English";
      pos=isName.indexOf(isLang+":");
      if (pos>0) {
         isName.remove(1,pos+isLang.length());
         pos=isName.indexOf(":"+isLang);
         if (pos>0) isName.remove(pos);
         }
      else { // No Name description at all
         isName=isFile;
         }
      }
   isName.remove(40);
   // Same for Notes
   pos=isNote.indexOf(isLang+":");
   if (pos>0) {
      isNote.remove(1,pos+isLang.length());
      pos=isNote.indexOf(":"+isLang);
      if (pos>0) isNote.remove(pos);
      }
   // Done
   fsFile.close();
   return(True);
}

Boolean NRexxDialog::selected(IControlEvent& event)
{
   ISelectHandler::disable(); // Bug in ClassLib: when iefName.setText(longText) -> selected()
   IString isFile(icbScript.itemText(icbScript.selection()));
   IString isAuthor;
   IString isNote;
   IString isName;
   // Get script infos
   if (!analyseScript(isFile,isName,isAuthor,isNote))
      isAuthor=NApplication::pDLLResource->loadString(SI_RexxUnknown);
   if (isName.length()==0) isName=isFile;
   iefName.setText(isName.space());
   iefAuthor.setText(isAuthor.space());
   imleNote.setText(isNote.space().translate("^","\n"));
   // Remember
   if (NRexx::pNRexx->doYouRemember(isFile))
      ipbRemember.disable();
   else
      ipbRemember.enable();
   // Done
   ISelectHandler::enable();
   return(False);
}
Boolean NRexxDialog::virtualKeyPress(IKeyboardEvent& event)
{
   if (event.virtualKey()==IKeyboardEvent::esc)
      ipbCancel.click();
   // I'm not the last handler
   return(False);
}


// Class   : NRexx
// Purpose : Enable Rexx Support
NRexx*     NRexx::pNRexx;
IFont      NRexx::ifProfileFont("System VIO",10);
IRectangle NRexx::irProfileRectangle;

NRexx::NRexx()
{
   rememberCount=0;
   pNRexx=this;
   // NPerson Functions
   RexxRegisterFunctionExe("getPID"          , (PFN)getPID          );
   RexxRegisterFunctionExe("getName"         , (PFN)getName         );
   RexxRegisterFunctionExe("getFirstName"    , (PFN)getFirstName    );
   RexxRegisterFunctionExe("getSex"          , (PFN)getSex          );
   RexxRegisterFunctionExe("getBirthDate"    , (PFN)getBirthDate    );
   RexxRegisterFunctionExe("getBirthPlace"   , (PFN)getBirthPlace   );
   RexxRegisterFunctionExe("getDeathDate"    , (PFN)getDeathDate    );
   RexxRegisterFunctionExe("getDeathPlace"   , (PFN)getDeathPlace   );
   RexxRegisterFunctionExe("getOccupation"   , (PFN)getOccupation   );
   RexxRegisterFunctionExe("getAddress"      , (PFN)getAddress      );
   RexxRegisterFunctionExe("getMemo"         , (PFN)getMemo         );
   RexxRegisterFunctionExe("getPicture"      , (PFN)getPicture      );
   RexxRegisterFunctionExe("getFile"         , (PFN)getFile         );
   RexxRegisterFunctionExe("getUserField"    , (PFN)getUserField    );
   RexxRegisterFunctionExe("getUserFieldName", (PFN)getUserFieldName);
   RexxRegisterFunctionExe("setPID"          , (PFN)setPID          );
   RexxRegisterFunctionExe("setName"         , (PFN)setName         );
   RexxRegisterFunctionExe("setFirstName"    , (PFN)setFirstName    );
   RexxRegisterFunctionExe("setSex"          , (PFN)setSex          );
   RexxRegisterFunctionExe("setBirthDate"    , (PFN)setBirthDate    );
   RexxRegisterFunctionExe("setBirthPlace"   , (PFN)setBirthPlace   );
   RexxRegisterFunctionExe("setDeathDate"    , (PFN)setDeathDate    );
   RexxRegisterFunctionExe("setDeathPlace"   , (PFN)setDeathPlace   );
   RexxRegisterFunctionExe("setOccupation"   , (PFN)setOccupation   );
   RexxRegisterFunctionExe("setAddress"      , (PFN)setAddress      );
   RexxRegisterFunctionExe("setMemo"         , (PFN)setMemo         );
   RexxRegisterFunctionExe("setPicture"      , (PFN)setPicture      );
   RexxRegisterFunctionExe("addFile"         , (PFN)addFile         );
   // NMarriage Functions
   RexxRegisterFunctionExe("getFid"          , (PFN)getFid          );
   RexxRegisterFunctionExe("getMarriageDate" , (PFN)getMarriageDate );
   RexxRegisterFunctionExe("getMarriagePlace", (PFN)getMarriagePlace);
   RexxRegisterFunctionExe("getDivorceDate"  , (PFN)getDivorceDate  );
   RexxRegisterFunctionExe("setFid"          , (PFN)setFid          );
   RexxRegisterFunctionExe("setMarriageDate" , (PFN)setMarriageDate );
   RexxRegisterFunctionExe("setMarriagePlace", (PFN)setMarriagePlace);
   RexxRegisterFunctionExe("setDivorceDate"  , (PFN)setDivorceDate  );
   RexxRegisterFunctionExe("doStack"         , (PFN)doStack         );
   RexxRegisterFunctionExe("selectPerson"    , (PFN)selectPerson    );
   RexxRegisterFunctionExe("selectFamily"    , (PFN)selectFamily    );
   RexxRegisterFunctionExe("sortPersons"     , (PFN)sortPersons     );
   RexxRegisterFunctionExe("sortFamilies"    , (PFN)sortFamilies    );
   // Import Functions
   RexxRegisterFunctionExe("importPerson"    , (PFN)importPerson    );
   RexxRegisterFunctionExe("importFamily"    , (PFN)importFamily    );
   RexxRegisterFunctionExe("importAddPartner", (PFN)importAddPartner);
   RexxRegisterFunctionExe("importAddChild"  , (PFN)importAddChild  );
   RexxRegisterFunctionExe("importDone"      , (PFN)importDone      );
   // Misc Functions
   RexxRegisterFunctionExe("getLanguage"     , (PFN)getLanguage     );
   RexxRegisterFunctionExe("getFilename"     , (PFN)getFilename     );
   RexxRegisterFunctionExe("getNameOfTree"   , (PFN)getNameOfTree   );
   // Done
}
NRexx::~NRexx()
{
   // Remove Strings
   for (unsigned i=0;i<rememberCount;i++) {
      delete pisRememberFtx[i];
      delete pisRememberName[i];
      }
   // NPerson Functions
   RexxDeregisterFunction("getPID"          );
   RexxDeregisterFunction("getName"         );
   RexxDeregisterFunction("getFirstName"    );
   RexxDeregisterFunction("getSex"          );
   RexxDeregisterFunction("getBirthDate"    );
   RexxDeregisterFunction("getBirthPlace"   );
   RexxDeregisterFunction("getDeathDate"    );
   RexxDeregisterFunction("getDeathPlace"   );
   RexxDeregisterFunction("getOccupation"   );
   RexxDeregisterFunction("getAddress"      );
   RexxDeregisterFunction("getMemo"         );
   RexxDeregisterFunction("getPicture"      );
   RexxDeregisterFunction("getFile"         );
   RexxDeregisterFunction("setPID"          );
   RexxDeregisterFunction("setName"         );
   RexxDeregisterFunction("setFirstName"    );
   RexxDeregisterFunction("setSex"          );
   RexxDeregisterFunction("setBirthDate"    );
   RexxDeregisterFunction("setBirthPlace"   );
   RexxDeregisterFunction("setDeathDate"    );
   RexxDeregisterFunction("setDeathPlace"   );
   RexxDeregisterFunction("setOccupation"   );
   RexxDeregisterFunction("setAddress"      );
   RexxDeregisterFunction("setMemo"         );
   RexxDeregisterFunction("setPicture"      );
   RexxDeregisterFunction("addFile"         );
   RexxDeregisterFunction("getUserField"    );
   RexxDeregisterFunction("getUserFieldName");
   // NMarriage Functions
   RexxDeregisterFunction("getFid"          );
   RexxDeregisterFunction("getMarriageDate" );
   RexxDeregisterFunction("getMarriagePlace");
   RexxDeregisterFunction("getDivorceDate"  );
   RexxDeregisterFunction("setFid"          );
   RexxDeregisterFunction("setMarriageDate" );
   RexxDeregisterFunction("setMarriagePlace");
   RexxDeregisterFunction("setDivorceDate"  );
   RexxDeregisterFunction("doStack"         );
   RexxDeregisterFunction("selectPerson"    );
   RexxDeregisterFunction("selectFamily"    );
   RexxDeregisterFunction("sortPersons"     );
   RexxDeregisterFunction("sortFamilies"    );
   // Import Functions
   RexxDeregisterFunction("importPerson"    );
   RexxDeregisterFunction("importFamily"    );
   RexxDeregisterFunction("importAddPartner");
   RexxDeregisterFunction("importAddChild"  );
   RexxDeregisterFunction("importDone"      );
   // Misc Functions
   RexxDeregisterFunction("getLanguage"     );
   RexxDeregisterFunction("getFilename"     );
   RexxDeregisterFunction("getNameOfTree"   );
   // Done
}

void NRexx::loadFromProfile(IProfile& profile)
{
   // Check for REXX-shortcuts
   profile.setDefaultApplicationName("REXXSCRIPTS");
   IProfile::Cursor cursor(profile,"REXXSCRIPTS");
   IString no,file,name,author,note;
   for (cursor.setToFirst();cursor.isValid();cursor.setToNext()) {
      no=profile.applicationOrKeyAt(cursor);
      file=profile.elementWithKey(no);
      analyseScript(file,name,author,note);
      remember(file,name);
      }
   // Check for Output Params
   profile.setDefaultApplicationName("REXX");
   irProfileRectangle=IRectangle(profile.integerWithKey("MINX")
                                ,profile.integerWithKey("MINY")
                                ,profile.integerWithKey("MAXX")
                                ,profile.integerWithKey("MAXY"));
   ifProfileFont=IFont(profile.elementWithKey("FONTNAME")
                      ,profile.integerWithKey("FONTSIZE"));
   // Done
}
void NRexx::saveToProfile(IProfile& profile)
{
   // Set Rexx-shortcuts
   profile.setDefaultApplicationName("REXXSCRIPTS");
   for (unsigned i=1;i<=rememberCount;i++)
      profile.addOrReplaceElementWithKey(IString(i),getRemembered(i,True));
   // Set Output Params
   profile.setDefaultApplicationName("REXX");
   profile.addOrReplaceElementWithKey("MINX",irProfileRectangle.minX());
   profile.addOrReplaceElementWithKey("MINY",irProfileRectangle.minY());
   profile.addOrReplaceElementWithKey("MAXX",irProfileRectangle.maxX());
   profile.addOrReplaceElementWithKey("MAXY",irProfileRectangle.maxY());
   profile.addOrReplaceElementWithKey("FONTNAME",ifProfileFont.name());
   profile.addOrReplaceElementWithKey("FONTSIZE",ifProfileFont.pointSize());
}
void NRexx::setScript(const IString& isSetFtx,const IString& isSetName)
{
   isFtx=isSetFtx;
   isName=isSetName;
}

unsigned NRexx::getRememberCount()
{
   return(rememberCount);
}

const IString& NRexx::getRemembered(unsigned number,Boolean truename)
{
   if (truename)
      return(*pisRememberFtx[number-1]);
   else
      return(*pisRememberName[number-1]);
}

void NRexx::remember(const IString& isFtx,const IString& isName)
{
   // Check for forget one
   if (rememberCount==MAX_REMEMBER) {
      // Kill First
      delete pisRememberFtx[0];
      delete pisRememberName[0];
      // Shift StringArray
      for (unsigned i=0;i<MAX_REMEMBER-1;i++) {
         pisRememberFtx[i]=pisRememberFtx[i+1];
         pisRememberName[i]=pisRememberName[i+1];
         }
      // Decrease Count
      rememberCount--;
      }
   // Add this one
   pisRememberFtx[rememberCount]=new IString(isFtx);
   pisRememberName[rememberCount]=new IString(isName);
   rememberCount++;
   // Done
}

Boolean NRexx::doYouRemember(const IString& isFtx)
{
   // Look for Test in rememberList
   for (unsigned i=0;i<rememberCount;i++)
      if (isFtx==*pisRememberFtx[i]) return(True);
   // Not found
   return(False);
}

Boolean NRexx::startRexx(IWindow *pOwner,unsigned number)
{
   if (number==0) {
      // Setup NRexxDialog
      NRexxDialog dialog(pOwner);
      dialog.setFocus();
      dialog.showModally();
      if (dialog.result()!=DID_OK)
         return(False);
      }
   else {
      setScript(getRemembered(number,True),getRemembered(number,False));
      }
   // Prepare for Rexx-Output
   NRexxOutput* pOutput=new NRexxOutput(pOwner,isFtx,isName);
   pOutput->setFocus();
   pOutput->show();
   // Done
   return(True);
}


