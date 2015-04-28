/* FamilyTree installation */
SAY 'FamilyTree Installation'
SAY
SAY '    (d)eutsch (e)nglish d(u)tch (f)rench f(i)nnish  (r)ussian?'

CALL RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
CALL SysLoadFuncs

name='Family Tree 1.2e'
language=SysGetKey(NOECHO)

PARSE UPPER VAR language language
SAY

/* Do a german install */
IF language='D' THEN DO
   SAY 'Erzeuge Onlinehilfe !'
   '@copy German.hlp FTree.hlp >nul'
   SAY 'Erzeuge Sprachresource !'
   '@copy German.dll FTree.dll >nul'
   SAY 'Erzeuge Programmobjekt !'
   IF SubCreateObject(name)=0
      THEN SAY '*** Fehler beim Erzeugen des Programmobjektes!'
   EXIT
   END

/* Do a dutch install */
IF language='U' THEN DO
   SAY 'Creating english online-help !'
   '@copy English.hlp FTree.hlp >nul'
   SAY 'Creating dutch language-resource !'
   '@copy Dutch.dll FTree.dll >nul'
   SAY 'Creating program object !'
   IF SubCreateObject(name)=0
      THEN SAY '*** Error creating program object !'
   EXIT
   END

/* Do a french install */
IF language='F' THEN DO
   SAY 'Creating english online-help !'
   '@copy English.hlp FTree.hlp >nul'
   SAY 'Creating french language-resource !'
   '@copy French.dll FTree.dll >nul'
   SAY 'Creating program object !'
   IF SubCreateObject(name)=0
      THEN SAY '*** Error creating program object !'
   EXIT
   END

/* Do a finnish install */
IF language='I' THEN DO
   SAY 'Creating english online-help !'
   '@copy English.hlp FTree.hlp >nul'
   SAY 'Creating finnish language-resource !'
   '@copy Finnish.dll FTree.dll >nul'
   SAY 'Creating program object !'
   IF SubCreateObject(name)=0
      THEN SAY '*** Error creating program object !'
   EXIT
   END

/* Do a french install */
IF language='F' THEN DO
   SAY 'Creating english online-help !'
   '@copy English.hlp FTree.hlp >nul'
   SAY 'Creating french language-resource !'
   '@copy French.dll FTree.dll >nul'
   SAY 'Creating program object !'
   IF SubCreateObject(name)=0
      THEN SAY '*** Error creating program object !'
   EXIT
   END

/* Do a russian */
   SAY 'Creating online-help !'
   '@copy English.hlp FTree.hlp >nul'
   SAY 'Creating language-resource !'
   '@copy Russian.dll FTree.dll >nul'
   SAY 'Creating program object !'
   IF SubCreateObject(name)=0
      THEN SAY '*** Error creating program object !'
   EXIT

/* Creation subroutine */
SubCreateObject:
   PARSE ARG objname
   return(SysCreateObject("WPProgram",objname,"<WP_DESKTOP>","EXENAME="||DIRECTORY()||"\ftree.exe;OBJECTID=<FTREE>;ASSOCFILTER=*.fam",replace))
