// Class  : NPointerArray
// Purpose: Provide a dynamic array of pointers

#include "NPointersTo.h"
#include "stdlib.h"

template<class ITEM>
NPointersTo<ITEM>::NPointersTo()
{
   pPointers=NULL;
   sNumPointers=0;
}

template<class ITEM>
NPointersTo<ITEM>::~NPointersTo()
{
   if (pPointers) free(pPointers);
}

template<class ITEM>
NPointersTo<ITEM>& NPointersTo<ITEM>::add(ITEM* which)
{
   // Allocate space for new array of pointers
   ITEM** pNew=(ITEM**)malloc(sizeof(ITEM*)*(sNumPointers+1));
   // ... copy old array to new
   for (short i=1;i<=sNumPointers;i++)
      pNew[i-1]=pPointers[i-1];
   // ... add new pointer
   sNumPointers++;
   pNew[i-1]=which;
   // ... switch to new array
   if (pPointers) free(pPointers);
   pPointers=pNew;
   // Done
   return(*this);
}

template<class ITEM>
ITEM* NPointersTo<ITEM>::get(short which)
{
   if ((which<1)||(which>sNumPointers)) return(NULL);
   return(pPointers[which-1]);
}

template<class ITEM>
NPointersTo<ITEM>& NPointersTo<ITEM>::swap(short one,short two)
{
   if ((one<1)||(one>sNumPointers)||(two<1)||(two>sNumPointers))
      return(*this);
   ITEM* pTemp=pPointers[one-1];
   pPointers[one-1]=pPointers[two-1];
   pPointers[two-1]=pTemp;
   return(*this);
}

template<class ITEM>
NPointersTo<ITEM>& NPointersTo<ITEM>::del(short which)
{
   // Check input
   if ((which<1)||(which>sNumPointers))
      return(*this);
   // ... remove by shift
   sNumPointers--;
   for (short i=which;i<=sNumPointers;i++)
      pPointers[i-1]=pPointers[i];
   // Done
   return(*this);
}

template<class ITEM>
NPointersTo<ITEM>& NPointersTo<ITEM>::delAll()
{
   if (pPointers) free(pPointers);
   sNumPointers=0;
   pPointers=NULL;
   // Done
   return(*this);
}

template<class ITEM>
short NPointersTo<ITEM>::num()
{
   return(sNumPointers);
}

template<class ITEM>
NPointersTo<ITEM>& NPointersTo<ITEM>::change(short which,ITEM* with)
{
   // Check input
   if ((which<1)||(which>sNumPointers))
      return(*this);
   // ... change it
   pPointers[which-1]=with;
   // Done
   return(*this);

}

