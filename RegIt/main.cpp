#include <istring.hpp>
#include <iostream.h>

int main (int argc,char* argv[])
{
  cout << "FamilyTree Registration \n";

   // Check for user name as argument
   if (argc!=2) {
      cout << "Usage : regit \"firstname name\" \n";
      return(1);
   }
   char* user=argv[1];

   if ( (strlen(user)==0)
      ||(strlen(user)>16)) {
      cout << "*** User name length is between 1 and 16 !\n";
      return(1);
   }
   // Calc Code
   unsigned long check1=1234567;  // 7 Ziffern
   unsigned long check2=87654321; // 8 Ziffern

   char mod1[]="FAMILY";
   char mod2[]="TREE";

   for (int i=1;i<=strlen(user);i++)
      check1-=user[i-1]*mod1[i%strlen(mod1)];
   for (i=1;i<=strlen(user);i++)
      check2-=user[i-1]*mod2[i%strlen(mod2)];

   // Done
   cout << "User <" << user << "> has registration code <"
        << check1 << "/" << check2 << ">\n";
   return(0);
}
