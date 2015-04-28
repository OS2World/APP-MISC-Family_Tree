// Class  : NPointerArray
// Purpose: Provide a dynamic array of pointers

#ifndef _NPOINTERSTO_

#define _NPOINTERSTO_

template <class ITEM>
class NPointersTo
{
public:
   NPointersTo();
   ~NPointersTo();
   NPointersTo<class ITEM>& add   (ITEM* which);
   NPointersTo<class ITEM>& del   (short which);
   NPointersTo<class ITEM>& delAll();
   NPointersTo<class ITEM>& swap  (short one,short two);
   NPointersTo<class ITEM>& change(short which,ITEM* with);
   ITEM*                    get   (short which);
   short                    num   ();
private:
   ITEM**   pPointers;
   short sNumPointers;
};

#endif
