:userdoc.
:title.Family Tree (Online Help)
:docprof ctrlarea=coverpage.
:ctrldef.
:ctrl ctrlid=test coverpage controls='ESC SEARCH PRINT INDEX CONTENTS BACK FORWARD'.
:ectrldef.

.* -------------- Symbol definitions ----------------------.
.nameit symbol=ID_NMainWindow        text=100.
.nameit symbol=ID_NEditWindow        text=150.
.nameit symbol=ID_NListWindow        text=151.
.nameit symbol=ID_NPrintDialog       text=152.
.nameit symbol=ID_NAboutDialog       text=153.
.nameit symbol=ID_NRexxDialog        text=154.
.nameit symbol=ID_NRexxOutput        text=155.
.nameit symbol=ID_NOptionsDialog     text=156.
.nameit symbol=ID_NSearchDialog      text=157.
.nameit symbol=ID_NEventsWindow      text=158.

.nameit symbol=MI_Main               text=1000.
.nameit symbol=MI_MainFile           text=1002.
.nameit symbol=MI_MainView           text=1004.
.nameit symbol=MI_MainHelp           text=1007.

.nameit symbol=MI_FileNew            text=1020.
.nameit symbol=MI_FileOpen           text=1022.
.nameit symbol=MI_FileSave           text=1024.
.nameit symbol=MI_FileSaveAs         text=1026.
.nameit symbol=MI_FileClose          text=1028.
.nameit symbol=MI_FilePrint          text=1030.

.nameit symbol=MI_Mankind            text=1040.
.nameit symbol=MI_MankindAncestors   text=1042.
.nameit symbol=MI_MankindDescendants text=1043.
.nameit symbol=MI_MankindSiblings    text=1044.
.nameit symbol=MI_MankindHori        text=1054.
.nameit symbol=MI_MankindVert        text=1055.
.nameit symbol=MI_MankindList        text=1058.
.nameit symbol=MI_MankindZoomIn      text=1060.
.nameit symbol=MI_MankindZoomOut     text=1062.

.nameit symbol=MI_RexxStart            text=1400.

.nameit symbol=MI_OptionsLayout        text=1450.
.nameit symbol=MI_OptionsDataAssistant text=1451.
.nameit symbol=MI_OptionsMiscellaneous text=1452.
.nameit symbol=MI_OptionsUserFields    text=1453.

.nameit symbol=MI_Man                  text=1070.
.nameit symbol=MI_ManCenter            text=1072.
.nameit symbol=MI_ManEdit              text=1074.
.nameit symbol=MI_ManOrigin            text=1076.
.nameit symbol=MI_ManDel               text=1077.
.nameit symbol=MI_ManDelPartner        text=1078.
.nameit symbol=MI_ManDelChilds         text=1129.
.nameit symbol=MI_ManDelParents        text=1179.
.nameit symbol=MI_ManDelThis           text=1180.
.nameit symbol=MI_ManChild             text=1181.
.nameit symbol=MI_ManChildNew          text=1232.
.nameit symbol=MI_ManChildRemembered   text=1282.
.nameit symbol=MI_ManParents           text=1390.
.nameit symbol=MI_ManPartner           text=1391.
.nameit symbol=MI_ManPartnerNew        text=1392.
.nameit symbol=MI_ManPartnerRemembered text=1393.
.nameit symbol=MI_ManRemember          text=1394.


.* ------------------- winMain --------------------------------.
:h1 id=FamilyTree.Family Tree
:i1 id=FamilyTree.Family Tree
:p.A family tree consists of persons, who are related to each
other. The initial family tree contains only one person who
is the origin of the tree and the actual one:
:artwork name='sample1.bmp'.
.*.
Choose :link reftype=hd refid=menuManPartner.Add Partner:elink.
from the context menu of this :link reftype=hd refid=menuMan.person:elink.
and it gets a partner:
:artwork name='sample2.bmp'.
.*.
Now you can add children by choosing
:link reftype=hd refid=menuManChild.Add Child with:elink. from the
context menu of one of them.
By selecting a person with a mouse click, it gets the
red-shaded border - it becomes the actual.
:artwork name='sample3.bmp'.
.*.
Parents for "Adam" are no problem. Select "Adam" by clicking on it and
choose :link reftype=hd refid=menuManParents.Add Parents:elink.
from the :link reftype=hd refid=menuMan.person:elink.'s context menu.
:artwork name='sample4.bmp'.
.*.
Not all persons in the family tree are always shown. It depends
on the origin, which relatives are shown.
All ancestors (direct line) and descendants (with partner) of the
origin are shown.
(Exception: You can change the view of the family tree via the
:link reftype=hd refid=menuMankind.Viewmenu:elink.).
.*.
:p.Try to make "Partner" the new origin by double-clicking on it or
choosing :link reftype=hd refid=menuManOrigin.Origin:elink. from its context menu.
The color of "Partner" changes to :color fc=red.red:color fc=default.
- now it ist the origin of the family tree.
:artwork name='sample5.bmp'.
.*.
Did you notice, that the parents of "Adam" have vanished and only a dotted line
shows they're still there ? But now you could see all ancestors of person "Partner".
.*.
:p.Now let "Child" become the origin (its color changes to :color fc=red.red:color fc=default.)
and add a few siblings via the context-menu from "Adam":
:artwork name='sample6.bmp'.
.*.
The siblings of "Child" are not shown, because they are neither ancestors nor
descendants of it.

:p.Try to build a family tree and change the origin. Change the view by choosing
items from the :link reftype=hd refid=menuMankind.View Menu:elink..
:p.Open the :link reftype=hd refid=winList.List Window:elink., where all
persons are shown in a list.
:p.Open the :link reftype=hd refid=winEdit.Edit Window:elink. and change properties
like name or birth date.

.* ------------------- winMain --------------------------------.
:h1 res=&ID_NMainWindow..Main Window
:i1 id=winMain.Main Window
:p.:hp3.Family Tree:ehp3.
:p.  ... Graphical Managing of Family Trees :artwork runin name='FTree.bmp'.
:p.Copyright 1996, 1997 by :link reftype=hd refid=People.Nils Meier:elink.
:note text='Important :'.Please read :hp1.readme.txt:ehp1. for release information !
:p.This is the initial help for Family Tree. If you start the program, you'll
see the main window and get to this help by pressing F1.
:p.The main window consists of a "white piece of paper" with one person
"Adam", a :link reftype=hd refid=toolbar.toolbar:elink. and a window menu.
The principle of a family tree is explained :link reftype=hd refid=FamilyTree.here:elink..
.*.
:p.You can immediately start entering a family tree by selecting a person
(simple mouse-click), activating the context menu of the
:link reftype=hd refid=menuMan.person:elink. (click with right mouse button)
and choosing actions.
Click with the right mouse-button on a free space in the family
and the :link reftype=hd refid=menuMankind.view Menu:elink. appears.
:p.By clicking on the tree with the left mouse-button and keeping
it pressed down you can move the tree around.
.*.
:p.You can save, load and print a family tree via the
:link reftype=hd refid=menuFile.file menu:elink..
The :link reftype=hd refid=menuMankind.view menu:elink.
offers selections to change the family tree view.
Start Rexx scripts that do various reports and im/export
via the :link reftype=hd refid=menuRexxStart.Rexx Menu:elink.
and :link reftype=hd refid=rexxing.program your own Rexx Routines:elink..
:p.You can change the zoom-rate by using the horizontal slider in the
toolbar or by pressing +/-.
:p.If you want to change program or tree settings just open a page
from the :link reftype=hd refid=winOptions.Options Dialog:elink. by
choosing an item from the
:link reftype=hd refid=winOptions.Options Menu:elink..
:p.If you want a default tree to be loaded on startup, just specify its name
on the commandline or the parameter field of the program object.
:p.You can also drag&amp.drop a .fam-file to the main window.

.* ------------------- toolbar    ------------------------------.
:h1 id=toolbar.Toolbar
:i1.Toolbar
In the main window, atop of the tree, is the toolbar:
:artwork name='Toolbar.bmp'.
It shows several buttons. Their meaning is:
:ol.
:li.:link reftype=hd refid=treeLoad.Load data:elink.
:li.:link reftype=hd refid=treeSave.Save data:elink.
:li.:link reftype=hd refid=treePrint.Print data:elink.
:li.:link reftype=hd refid=winOptions.Set options:elink.
:li.:link reftype=hd refid=winSearch.Search:elink.
:lp.
:li.Set :link reftype=hd refid=menuMankindVert.vertical:elink. /
:link reftype=hd refid=menuMankindHori.horizontal:elink. view
:li.:link reftype=hd refid=menuMankindAncestors.Set number of viewed generations of ancestors:elink.
:li.:link reftype=hd refid=menuMankindDescendants.Set number of viewed generations of descendants:elink.
:li.:link reftype=hd refid=menuMankindSiblings.Enable view of Siblings:elink.
:li.Zoom :link reftype=hd refid=menuMankindZoomIn.in:elink. /
:link reftype=hd refid=menuMankindZoomOut.out:elink.
:lp.
:li.Place toolbar at top/bottom
:eol.

.* ------------------- winSearch ------------------------------.
:h1 res=&ID_NSearchDialog. id=winSearch.Search Dialog

This function is not implemented yet

.* ------------------- winOptions ------------------------------.
:h1 res=&ID_NOptionsDialog. id=winOptions.Options Dialog
:i1.Options Dialog
:p.You can change properties of FamilyTree here. There are four
notebook pages, to change
:ol compact.
:li.layout options
:li.data assistant options
:li.miscellaneous options
:li.user field options
:eol.

:dl compact.
:dt.:hp2.Layout:ehp2.
:dd.Here you can specify, how the lines between persons in the
tree look like. You can change the detail of information that
is shown in the tree and change the used font and color of visible
items.
:dt.:hp2.Data Assistant:ehp2.
:dd.You can enable assistance here. Depending on an action you take,
like "Add Partner", you can enable the action "gets same address",
so the new partner get the address from the "Actual".
:dt.:hp2.Miscellaneous:ehp2.
:dd.If you're not satified with the date format, FamilyTree has detected
from your system, you can change it here. You can also enable the automatic
focus change to the edit window after a click on a person. That way,
you can click through the tree and change data on the fly, without
moving the mouse to the edit dialog again.
:p.One other thing is the "Try to store relative paths" option.
If it is enabled, FamilyTree tries to store relative paths to all
attached data, like bitmaps. That means, if you have dropped a picture,
that resides in a subdirectory of your tree, FamilyTree cuts off all absolute
path information. FamilyTree will warn you, that it can't store
a relative path, when an attachment is not in a subdirectory.
In that case FamilyTree will store the whole path information.
:p.By enabling "Always preserve non imported Persons", you can specify,
that all imported data, that doesn't make it in a tree, will be preserved
for a later import. When enabled, you can import one tree after the other
and won't be asked every time, what to do with dropped persons.
Keep in mind, that the data is kept in memory until a new import is done!
:dt.:hp2.User Fields:ehp2.
:dd.Here you can define your own data fields for persons
(a maximum of 3 fields is possible in the unregistred version).
There are four types of fields:
:ol compact.
:li.Yes/No
:li.Text
:li.Number
:li.Date
:eol.
:p.The field definitions belong to the opened tree and are saved together
with the tree data.
:edl.


.* ------------------- winEdit ------------------------------.
:h1 res=&ID_NEditWindow. id=winEdit.Edit Window
:i1.Edit Window
:p.Open the Edit Window via the context-menu of a
:link reftype=hd refid=menuMan.person:elink..
It contains a notebook with five pages:

:artwork name='Edit.bmp'.

:ol.
:li.Page
:p.The first page contains several input fields for various
information about the actual person. Just fill them with your
personal data.

:artwork name='edit1.bmp'.

:p.If you place a tilde (~) in the first-name field, the name
directly following it becomes the given name and is the
only one that is shown in the family tree.
:p.You can delete any date information by pressing
:hp1.ESC:ehp1., when the cursor is in a date field.
A date can also include a date modifier choosable by right-clicking
on it.
:p.Change the sex of a person, by clicking on the "Sex"-button with
the symbol of a person (gray: not specified, blue: male,
red: female).
:p.If the "Actual" has several partners, you can cycle
through them by clicking at the button beside the marriage field.
:p.Drag&amp.Drop any GIF, BMP or PCX to the
picture field or click on the picture to enter a path
manually.
:warning text='Attention:'.FTree only remembers
where the picture resides (Its data is not saved).
If the picture gets moved or deleted later, FTree
can't find it and shows a questionmark.

:li.Page
:p.By defining user fields in the :link reftype=hd refid=winOptions.Options Dialog:elink.,
you add fields to all persons in the tree. You can enter the information
that belongs to the actual person on this page like shown here:

:artwork name='edit2.bmp'.

:li.Page
:p.Documented sources are not implemented yet.

:li.Page
:p.This page shows all events, the actual person has been involved
in. Define events in the :link reftype=hd refid=winEvents.Events Window:elink.,
and drag&amp.drop them to this window to make a connection or
drag&amp.drop a person to the event. You are then asked about the
role that person played in the event.
By right-clicking on an event here, you can choose to remove the
involvement or change how that person was involved in the event.
This sample shows that the actual person has been at two birthday
parties.

:artwork name='edit4.bmp'.

:li.Page
:p. The last page accepts dropped files and stores a reference to
them. When you have a picture or document, that belongs to a certain
person, just drag&amp.drop it here.
You can later modify the file-reference, by right-clicking on it.

:artwork name='edit5.bmp'.

:eol.

.* ------------------- winList -----------------------------------.
:h1 res=&ID_NListWindow. id=winList.List Window
:i1.List Window
:p.This window contains a list of all persons in the family tree.
You can select a person and open its context-menu via right mouse-click.
:artwork name='list.bmp'.
:p.Open the List Window, by choosing List from the
:link reftype=hd refid=menuMankind.View Menu:elink..
:p.By right-clicking on an empty space or the columns header a menu
with one entry "Sort by" appears. Here you can set sorting
criterias. If you'd like to sort by just one criteria (e.g. Name)
simply click on the header of the corresponding column.

.* ------------------- winEvents -----------------------------------.
:h1 res=&ID_NEventsWindow. id=winEvents.Events Window
This window contains a list of all events belonging to the tree.
:artwork name='events.bmp'.
:p.You can change the actual event via the notebook on the right
side of the window. An event consists of three pages of data:

:ol.
:li.Page
:p.The first page contains several input fields for various
information about the actual event. Just enter the event's data:

:artwork name='events1.bmp'.

:li.Page
:p.Drag&amp.drop any person from the tree or list window here.
You are then asked, how this person was involved in that event.
By right-clicking on any person in the list, you can modify
it's involvement.

:artwork name='events2.bmp'.

:li.Page
:p.The last page contains references to documented sources for
that event (e.g. register of a ship that proofs emigration).
This function is not implemented yet.

:eol.

.* ------------------- winPrint ------------------------------.
:h1 res=&ID_NPrintDialog. id=winPrint.Print dialog
:i1.Print dialog
:p.This dialog shows up, when you select :hp1.Print:ehp1. from the
File Menu.
:p.You can choose a :hp1.Printer:ehp1. here and change print options.
:artwork name='print1.bmp'.
:p.Click on :hp1.Fit:ehp1. to change the number of pages to a specific
number. The family tree will be shrunk or grown to the specified
number of pages.
:artwork name='print2.bmp'.
:p.When :hp1.Fit:ehp1. isn't choosen, you can choose a magnify factor
by using the slider :hp1.Zoom by:ehp1..
:artwork name='print3.bmp'.
:p.Click on :hp1.Print:ehp1. to start the printing, :hp1.Properties:ehp1.
to change printer options and :hp1.Cancel:ehp1. to go back to the
family tree.
:artwork name='print4.bmp'.

.* ------------------- winRexx ------------------------------.
:h1 res=&ID_NRexxDialog. id=winRexx.Rexx dialog
:i1.Rexx dialog
:p.You can choose a Rexx-Script in this dialog.
:artwork name='rexx1.bmp'.
:p.All scripts in the current directory are shown (*.ftx)
with author and a short description.
:artwork name='rexx2.bmp'.
You can :hp1.start:ehp1. the selected script or :hp1.cancel:ehp1.
rexx starting.
After pressing the button :hp1.Remember:ehp1. the selected
script appears in the rexxmenu and can be started directly there.
:p.FamilyTree delivers the following scripts right out of the box:
:ol compact.
:li.Alphabetic List (ABC-List.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script shows all persons in the tree in alphabetical order.
:li.Anniversaries of Death (AnnDeath.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script shows aniversaries of death, ordered by month + day.
:li.List Birthdays (Birthday.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script shows birthdays, ordered by month + day.
:li.Check Consistency (Check.ftx) by :link reftype=hd refid=People.Peter Gervai:elink.
.br
This script checks the tree consistency. It investigates the data and
reports any ambiguous information.
:li.List Descendants (Descends.ftx) by :link reftype=hd refid=People.Peter Gervai:elink.
.br
This script shows the descendants of the actual person.
:li.Export Gedcom (ExGedcom.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script exports the current tree to GEDCOM format. GEDCOM is THE standard
for genealogical data and widely used.
:li.Full Dump (FullDump.ftx) by :link reftype=hd refid=People.Peter Gervai:elink.
.br
This script dumps all information in the tree color-separated. This way
you can export your data to a calculation program.
:li.Import Gedcom (ImGedcom.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script imports trees from a GEDCOM file (see export above).
:li.Continue Import (ImContue.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script continues an import, when the last import has left dropped Persons (and
families). This can happen, when imported data isn't a connected tree. In that case
you are asked, wether you want to keep those persons, that are not related to the
origin. This script can be used in such cases to do a separate tree for them.
:li.Longevity Report (Lngevity.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script shows a statistic about the ages at death.
:li.Report of a Person (Person.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script displays informations about the actual person.
:li.List Weddings (Weddings.ftx) by :link reftype=hd refid=People.Nils Meier:elink.
.br
This script shows all weddings in the tree in order month + day.
:eol.

.* ------------------- winRexxOutput --------------------------.
:h1 res=&ID_NRexxOutput. id=winRexxOut.Rexx output
:i1.Rexx output
:p.The output of a Rexx-script is shown in this window.
You can :hp1.Cancel:ehp1. the rexx execution, close the
window by choosing :hp1.OK:ehp1. and :hp1.Save:ehp1.
the output to a file.

.* ------------------- Rexxing --------------------------.
:h1 id=rexxing.Programming Rexx
:i1.Programming Rexx
:p.FamilyTree can run Rexx scripts in its context for various
purposes. A script can do different reports, manipulate data
and import or export FamilyTree information to/from a file.
:p.You have to create a Rexx file (*.ftx) if you want
to run your own routines. The header should look like this:

:font facename=VIO size=9x6.:xmp.
/*
   Family Tree Rexx Script FTX
   Copyright (C) 1996 by <Author's name>

   <
   English&colon.    Description in english   &colon.English
   Deutsch&colon.    Description in german    &colon.Deutsch
   Nederlands&colon. Description in dutch     &colon.Nederlands
   Francais&colon.   Description in french    &colon.Francais
   >

   Long name is <
                 English&colon.    Long name in english &colon.English
                 Deutsch&colon.    Long name in german  &colon.Deutsch
                 Nederlands&colon. Long name in dutch   &colon.Nederlands
                 Francais&colon.   Long name in french  &colon.Francais
                >
*/
:exmp.:font facename=default size=0x0.

:p.The author's name (between <>) and the description (between <>) is shown in
the Rexx selection dialog.
:p.A Rexx script can access persons and families from the
actual family tree with several functions. There is always
one person and one family (if exists) selected.
You can either change the person or family selection by moving
from one to another or query/change data with data functions.
Please use the provided scripts as examples when you write
your own and send a copy of your new script to the author
if it can be of interest for other users.

:p.:hp2.The following functions let you query program settings: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 旼컴컴컴컴컴컴컴컴컴쩡컴컴컴컴컴컴컴컴컴컴컴컫컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
 � FUNCTION NAME     � ACTION                 � PARAMETERS                  �
 쳐컴컴컴컴컴컴컴컴컴탠컴컴컴컴컴컴컴컴컴컴컴컵컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑
 � getLanguage()     � returns language       � none                        �
 �                   �    'deutsch'/'english' �                             �
 � selectFilename()  � returns filename       � 'Dialogtitle','*.*'         �
 �                   �    '' for cancel       �                             �
 읕컴컴컴컴컴컴컴컴컴좔컴컴컴컴컴컴컴컴컴컴컴컨컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
:ecgraphic. :font facename=default size=0x0.


:p.:hp2.The following functions let you move through persons and families: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 旼컴컴컴컴컴컴컴컴컴쩡컴컴컴컴컴컴컴컴컴컴컴컫컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
 � FUNCTION NAME     � ACTION                 � PARAMETERS                  �
 쳐컴컴컴컴컴컴컴컴컴탠컴컴컴컴컴컴컴컴컴컴컴컵컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑
 � sortPersons()     � sort persons by        � 'N|n'ame                    �
 �                   �                        � 'F|f'irst name              �
 �                   �                        � 'B|b[D|M|Y]'irth date       �
 �                   �                        � 'D|d[D|M|Y]'eath date       �
 �                   �                        � 'L|l[D|M|Y]'ifetime         �
 �                   �                        � 'P|p'ersons ID              �
 �                   �                        � - Repeat via ','            �
 �                   �                        � - 'A'scending/'d'escending  �
 �                   �                        � - [Day,Month,Year]          �
 � sortFamilies()    � sort families by       �   'M|m[D|M|Y]'arriage date  �
 �                   �                        � - see above                 �
 쳐컴컴컴컴컴컴컴컴컴탠컴컴컴컴컴컴컴컴컴컴컴컵컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑
 � selectPerson()    � selects a person       � 'F'irst                     �
 �                   � returns '1' if done    � 'L'ast                      �
 �                   �         '0' otherwise  � 'P'revious                  �
 �                   �                        � 'N'ext                      �
 �                   �                        � 'R'emembered                �
 �                   �                        � 'O'rigin                    �
 �                   �                        �*'f'ather of current family  �
 �                   �                        �*'m'other of current family  �
 �                   �                        �*'p'artner in current family �
 �                   �                        �*'1|2|3..' child in family   �
 �                   �                        � - * results in error, when  �
 �                   �                        � actual family was imported  �
 � selectFamily()    � selects a family       � 'F'irst                     �
 �                   � returns '1' if done    � 'L'ast                      �
 �                   �         '0' otherwise  � 'P'revious                  �
 �                   �                        � 'N'ext                      �
 �                   �                        �*'p'arental fam of person    �
 �                   �                        �*'1|2|..' fam of act. person �
 �                   �                        � - * results in error, when  �
 �                   �                        � actual person was imported  �
 쳐컴컴컴컴컴컴컴컴컴탠컴컴컴컴컴컴컴컴컴컴컴컵컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑
 � doStack()         � stack operation        �*'P(P|F)'ush on stack        �
 �                   �                        � 'p(P|F)'op  from stack      �
 �                   �                        � 'D(P|F)'rop from stack      �
 �                   �                        �*'S(P|F)'wap with stack      �
 �                   �                        � - [P]erson|[F]amily         �
 �                   �                        � - * results in error, when  �
 �                   �                        � you try to push or swap an  �
 �                   �                        � imported Person or Family   �
 읕컴컴컴컴컴컴컴컴컴좔컴컴컴컴컴컴컴컴컴컴컴컨컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
:ecgraphic. :font facename=default size=0x0.
:p.Example: Sort all persons by birth date (ascending)
:xmp. rc=sortPersons('B') :exmp.
:p.Example: Sort all persons by their birth month,day (descending)
:xmp. rc=sortPersons('bm,bd') :exmp.
:p.Example: Sort all families by marriage date (ascending)
:xmp. rc=sortFamilies('M') :exmp.
:p.Example: Select first man in list
:xmp. rc=selectPerson('F') :exmp.
:p.Example: Select 2nd child of current family
:xmp. rc=selectPerson('2') :exmp.
:p.Example: Select 1st family of current person
:xmp. rc=selectFamily('1') :exmp.

:p.:hp2.The following functions refer to the data of the actual person: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 旼컴컴컴컴컴컴컴컴컴쩡컴컴컴컴컴컴컴컴컴컴컴컫컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
 � FUNCTION NAME     � ACTION                 � PARAMETERS                  �
 쳐컴컴컴컴컴컴컴컴컴탠컴컴컴컴컴컴컴컴컴컴컴컵컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑
 � getPID()          � returns ID             � none                        �
 � getName()         � returns name           � none                        �
 � getFirstName()    � returns first name     � none                        �
 � getSex()          � returns sex 0|1|2      � none                        �
 � getBirthDate()    � returns birth date     � none (full)                 �
 �                   �                        � |'D'ay                      �
 �                   �                        � |'M'onth                    �
 �                   �                        � |'m'onth name               �
 �                   �                        � |'Y'ear                     �
 �                   �                        � |'d'ays (since 00.00.0000)  �
 �                   �                        � |'c'omplete ? (1/0)         �
 �                   �                        � | m'O'difier (see setDate)  �
 � getBirthPlace()   � returns birth place    � none                        �
 � getDeathDate()    � returns death date     � none (full date)            �
 �                   �                        � |'D'ay                      �
 �                   �                        � |'M'onth                    �
 �                   �                        � |'m'onth name               �
 �                   �                        � |'Y'ear                     �
 �                   �                        � |'d'ays (since 00.00.0000)  �
 �                   �                        � |'c'omplete ? (1/0)         �
 �                   �                        � | m'O'difier (see setDate)  �
 � getDeathPlace()   � returns death place    � none                        �
 � getOccupation()   � returns occupation     � none                        �
 � getAddress()      � returns address        � none                        �
 � getMemo()         � ret. memo or '' if n/a � none (all)                  �
 �                   �                        � |line '1','2','3',...       �
 � getPicture()      � returns picture path   � none                        �
 � getFile()         � ret. file attachment   � '1','2','3',...             �
 � getUserField()    � ret. user field        � '1','2','3',...             �
 � getUserFieldName()� ret. user field name   � '1','2','3',...             �
 � setPID()          � sets ID                � 'id'                        �
 �                   �                        � - results in error, when    �
 �                   �                        � actual is not an imported   �
 �                   �                        � or id=0 or id is not num    �
 � setName()         � sets name              � 'name'                      �
 � setFirstName()    � sets first name        � 'first name'                �
 � setSex()          � sets sex               � '0'None|'1'Male|'2'Female   �
 � setBirthDate()    � sets birth date        � '[m,]dd.mm.yyyy'            �
 �                   �                        � Modifier m=0 None           �
 �                   �                        �          m=1 About          �
 �                   �                        �          m=2 After          �
 �                   �                        �          m=3 Before         �
 �                   �                        �          m=4 Estimated      �
 � setBirthPlace()   � sets birth place       � 'place'                     �
 � setDeathDate()    � sets death date        � '[m,]dd.mm.yyyy'            �
 �                   �                        � Modifier see above          �
 � setDeathPlace()   � sets death place       � 'place'                     �
 � setOccupation()   � sets occupation        � 'occupation'                �
 � setAddress()      � sets address           � 'address'                   �
 � setMemo()         � sets memo              � 'memo'                      �
 � setPicture()      � sets picture path      � 'path'                      �
 � addFile()         � adds file attachment   � 'file'                      �
 읕컴컴컴컴컴컴컴컴컴좔컴컴컴컴컴컴컴컴컴컴컴컨컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
:ecgraphic. :font facename=default size=0x0.
:p.Example: Get name of actual person
:xmp. name=getName() :exmp.
:p.Example: Get birth month of actual person
:xmp. date=getBirthDate('M') :exmp.
:p.Example: Get 2nd line of the memo-field of the actual person
:xmp. memo=getMemo('2') :exmp.

:p.:hp2.The following functions refer to the data of the actual family: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 旼컴컴컴컴컴컴컴컴컴쩡컴컴컴컴컴컴컴컴컴컴컴컫컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
 � FUNCTION NAME     � ACTION                 � PARAMETERS                  �
 쳐컴컴컴컴컴컴컴컴컴탠컴컴컴컴컴컴컴컴컴컴컴컵컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑
 � getFID()          � returns ID             � none                        �
 � getMarriageDate() � returns marriage date  � none (full date)            �
 �                   �                        � |'D'ay                      �
 �                   �                        � |'M'onth                    �
 �                   �                        � |'m'onth name               �
 �                   �                        � |'Y'ear                     �
 �                   �                        � |'d'ays (since 00.00.0000)  �
 �                   �                        � |'c'omplete ? (1/0)         �
 �                   �                        � | m'O'difier (see setDate)  �
 � getMarriagePlace()� returns marriage place � none                        �
 � getDivorceDate()  � returns divorce date   � none (full date)            �
 �                   �                        � |'D'ay                      �
 �                   �                        � |'M'onth                    �
 �                   �                        � |'m'onth name               �
 �                   �                        � |'Y'ear                     �
 �                   �                        � |'d'ays (since 00.00.0000)  �
 �                   �                        � |'c'omplete ? (1/0)         �
 �                   �                        � | m'O'difier (see setDate)  �
 � setFID()          � sets ID                � 'id'                        �
 �                   �                        � - results in error, when    �
 �                   �                        � actual is not an imported   �
 �                   �                        � or id=0 or id is not num    �
 � setMarriageDate() � sets marriage date     � '[m],dd.mm.yyyy'            �
 �                   �                        � Modifier m=0 None           �
 �                   �                        �          m=1 About          �
 �                   �                        �          m=2 After          �
 �                   �                        �          m=3 Before         �
 �                   �                        �          m=4 Estimated      �
 � setMarriagePlace()� sets marriage place    � 'place'                     �
 � setDivorceDate()  � sets divorce date      � '[m,]dd.mm.yyyy.'           �
 �                   �                        � Modifier see above          �
 읕컴컴컴컴컴컴컴컴컴좔컴컴컴컴컴컴컴컴컴컴컴컨컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
:ecgraphic. :font facename=default size=0x0.
:p.Example: Get divorce date of current family
:xmp. date=getDivorceDate() :exmp.
:p.Example: Get marriage year of current family
:xmp. date=getMarriageDate('Y') :exmp.

:p.:hp2.The following functions let you import new persons and families: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 旼컴컴컴컴컴컴컴컴컴쩡컴컴컴컴컴컴컴컴컴컴컴컫컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
 � FUNCTION NAME     � ACTION                 � PARAMETERS                  �
 쳐컴컴컴컴컴컴컴컴컴탠컴컴컴컴컴컴컴컴컴컴컴컵컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑
 � importPerson()    � create a new Person    � none                        �
 � importFamily()    � create a new Family    � none                        �
 � importAddPartner()� add ID of Partner to   � 'id' of one Partner of the  �
 �                   � imported family        � last imported! Family       �
 � importAddChild()  � add ID of Child to     � 'id' of one Child of the    �
 �                   � imported family        � last imported! Family       �
 � importDone()      � trigger import routine � none                        �
 �                   � tree is resetted and   � objects with lost ID's and  �
 �                   � as much information as � Families that build loops   �
 �                   � possible imported      � are not imported            �
 읕컴컴컴컴컴컴컴컴컴좔컴컴컴컴컴컴컴컴컴컴컴컨컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
:ecgraphic. :font facename=default size=0x0.
:p.Example: Import a Person
:xmp. ok=importPerson() :exmp.
:p.Example: Set ID of one Partners of the last imported family to 3
:xmp. ok=importAddPartner(3) :exmp.


.* ----------------- menuFile -------------------------------------.
:h1 id=menuFile.Filemenu
:i1 id=menuFile.Filemenu
:p.The Filemenu enables you to do the following actions on the family tree.
You can
:ol compact.
:li.:link reftype=hd refid=treeNew.Make a new:elink.,
:li.:link reftype=hd refid=treeLoad.load:elink.,
:li.:link reftype=hd refid=treeSave.save:elink.,
:li.:link reftype=hd refid=treeSaveAs.save with name:elink.,
:li.:link reftype=hd refid=treePrint.print:elink.,
:li.:link reftype=hd refid=treeClose.close:elink. a family tree and
quit the program.
:eol.

:h2 res=&MI_FileNew. id=treeNew.New family tree
:i2 refid=menuFile.New
:p.By choosing :hp1.File|New:ehp1. you can start with a new family tree.

:h2 res=&MI_FileOpen. id=treeLoad.Load family tree
:i2 refid=menuFile.Load
:p.By choosing :hp1.File|Load:ehp1. you can load an existing family tree.

:h2 res=&MI_FileSave. id=treeSave.Save family tree
:i2 refid=menuFile.Save
:p.By choosing :hp1.File|Save:ehp1. you can save a modified family tree
3which was previously loaded or saved with
:link reftype=hd refid=treeSaveAs.save as:elink..

:h2 res=&MI_FileSaveAs. id=treeSaveAs.Save family tree as
:i2 refid=menuFile.Save As
:p.By choosing :hp1.File|SaveAs:ehp1. you can save a modified family tree.

:h2 res=&MI_FileClose. id=treeClose.Close family tree
:i2 refid=menuFile.Close
:p.By choosing :hp1.File|Close:ehp1. you can close the family tree and
quit the program.

:h2 res=&MI_FilePrint. id=treePrint.Print family tree
:i2 refid=menuPrint.Printing
:p.by choosing :hp1.File|Print:ehp1. you can print
out the family tree.

.* ----------------- menuMankind -------------------------------------.
:h1 id=menuMankind.Viewmenu
:i1 id=menuMankind.Viewmenu
:p.The :hp1.Viewmenu:ehp1. is shown via the mainmenu at the top of the
main window or by right-clicking on a free space in the family tree.
:artwork name='menuMankind.bmp'.
Following options are available:
:ol compact.
:li.:link reftype=hd refid=menuMankindAncestors.Ancestors:elink.,
:li.:link reftype=hd refid=menuMankindDescendants.Descendants:elink.,
:li.:link reftype=hd refid=menuMankindSiblings.Siblings:elink.
:lp.
:li.:link reftype=hd refid=menuMankindZoomIn.ZoomIn:elink.
:li.:link reftype=hd refid=menuMankindZoomOut.ZoomOut:elink.
:lp.
:li.:link reftype=hd refid=menuMankindHori.Horizontal:elink.
:li.:link reftype=hd refid=menuMankindVert.Vertical:elink.
:lp.
:li.:link reftype=hd refid=menuMankindList.List:elink.
:eol.

:h2 res=&MI_MankindAncestors. id=menuMankindAncestors.Ancestors
:i2 refid=menuMankind.Ancestors
:p.By choosing :hp1.View|Ancestors:ehp1., a dialog pops up, where
you can specify how many generations of ancestors of the "Origin"
are shown. So if you enter a 0 you won't see any ancestors, and if
you leave the enty field empty, you'll see all ancestors of the "Origin".

:h2 res=&MI_MankindDescendants. id=menuMankindDescendants.Descendants
:i2 refid=menuMankind.Descendants
:p.By choosing :hp1.View|Descendants:ehp1., a dialog pops up, where
you can specify how many generations of descendants of the "Origin"
are shown. So if you enter a 0 you won't see any descendants, and if
you leave the enty field empty, you'll see all descendants of the "Origin".

:h2 res=&MI_MankindSiblings. id=menuMankindSiblings.Siblings
:i2 refid=menuMankind.Siblings
:p.By choosing :hp1.View|Siblings:ehp1. all siblings of the "Origin"
and their descendants are shown (if the parents of the "Origin" are
visible). By choosing it a second time, only the descendants of the
"Origin" are shown.

:h2 res=&MI_MankindZoomIn. id=menuMankindZoomIn.Zoom in
:i2 refid=menuMankind.Zoom in
:p.By choosing :hp1.View|Zoom in:ehp1., you can increase the
zoom rate to a maximum of 1.

:h2 res=&MI_MankindZoomOut. id=menuMankindZoomOut.Zoom out
:i2 refid=menuMankind.Zoom out
:p.By choosing :hp1.View|Zoom out:ehp1., you can decrease the
zoom rate to a minimum of 0.1.

:h2 res=&MI_MankindVert. id=menuMankindVert.Vertical
:i2 refid=menuMankind.Vertical
:p.By choosing :hp1.View|Vertical:ehp1. the family tree is shown vertical.

:h2 res=&MI_MankindHori. id=menuMankindHori.Horizontal
:i2 refid=menuMankind.Horizontal
:p.By choosing :hp1.View|Horizontal:ehp1. the family tree is shown horizontal.

:h2 res=&MI_MankindList. id=menuMankindList.List
:i2 refid=menuMankind.List
:p.By choosing :hp1.View|List:ehp1. a :link reftype=hd refid=winList.List Window:elink.
with an alphabetic list of all persons in the family tree is shown.
:p.An additional selection closes the List Window, when it is already open
(the item is checked).

.* ----------------- Man -----------------------------------------.
:h1 res=&MI_ManDel. id=menuMan.Person
:i1 id=menuMan.Person
:p.The context-menu of a person enables you to do the following actions
(A right mouse-click on a person in the family tree opens it):
:artwork name='menuMan.bmp'.
:ol compact.
:li.:link reftype=hd refid=menuManCenter.Center:elink.,
:li.:link reftype=hd refid=menuManEdit.Edit:elink.,
:li.:link reftype=hd refid=menuManOrigin.Make Origin:elink.
:lp.
:li.:link reftype=hd refid=menuManDelPartner.Delete Partner:elink.
:li.:link reftype=hd refid=menuManDelChildren.Delete Children:elink.
:li.:link reftype=hd refid=menuManDelParents.Delete Parents:elink.
:li.:link reftype=hd refid=menuManDelThis.Delete "Actual":elink.
:lp.
:li.:link reftype=hd refid=menuManChild.Add Child:elink.
:li.:link reftype=hd refid=menuManParents.Add Parents:elink.
:li.:link reftype=hd refid=menuManPartner.Add Partner:elink.
:eol.

:h2 res=&MI_ManCenter. id=menuManCenter.Center
:i2 refid=menuMan.Center
:p.By choosing :hp1.Center:ehp1. the family tree is so positioned that
the "Actual" is in the middle of the window.

:h2 res=&MI_ManEdit. id=menuManEdit.Edit
:i2 refid=menuMan.Edit
:p.By choosing :hp1.Edit:ehp1. a :link reftype=hd refid=winEdit.Edit Window:elink.
is opened, where you can change the properties of the "Actual".
:p.The item is checked, if the edit window is already open, and a new selection
closes it.

:h2 res=&MI_ManOrigin. id=menuManOrigin.Origin
:i2 refid=menuMan.Origin
:p.By choosing :hp1.Origin:ehp1. the "Actual" becomes the "Origin" of the
family tree.

:h2 res=&MI_ManDelPartner. id=menuManDelPartner.Delete Partner
:i2 refid=menuMan.Delete Partner
:p.By choosing :hp1.Del|Partner:ehp1. the choosen partner
and all children with him are deleted.

:h2 res=&MI_ManDelChilds. id=menuManDelChildren.Delete Children
:i2 refid=menuMan.Delete Children
:p.By choosing :hp1.Del|Children with:ehp1. all children of the
"Actual" are deleted.
If the "Actual" has more than one partner you have to specify
the other one, whose children should be deleted.
:warning text='Warning:'.
All children, their descendants and ancestors (without the "Actual") are deleted !
:ewarning.

:h2 res=&MI_ManDelParents. id=menuManDelParents.Delete Parents
:i2 refid=menuMan.Delete Parents
:p.By choosing :hp1.Del|Parents:ehp1. the parents of the "Actual" are deleted.
:warning text='Warning:'.
The parents, their ancestors and descendants (without the "Actual") are deleted !
:ewarning.

:h2 res=&MI_ManDelThis. id=menuManDelThis.Delete Person
:i2 refid=menuMan.Delete Person
:p.By choosing :hp1.Del|This:ehp1. the "Actual" is deleted.
:p.All persons, who are not related to the "Origin" anymore
are deleted, too.

:h2 res=&MI_ManChild. id=menuManChild.Add Child
:i2 refid=menuMan.Add Child
:p.By choosing :hp1.Add child with:ehp1. the "Actual" is added a child.
If the "Actual" has more than one partner you have to specify
the other one, who is parent of the new child.
You can
:link reftype=hd refid=menuManChildNew.add anew child:elink.,
or
:link reftype=hd refid=menuManChildRemembered.add a remembered person:elink..
:p.This item is not selectable, if the "Actual" has no partner or has
the maximum allowable number of children.

:h2 res=&MI_ManChildNew. id=menuManChildNew.Add Child (New)
:i2 refid=menuMan.Add Child (New)
:p.By choosing :hp1.Add Child with|Partner|New:ehp1. a new
person is added as a child.
You can also
:link reftype=hd refid=menuManChildRemembered.add a remembered person:elink.
as a child.

:h2 res=&MI_ManChildRemembered. id=menuManChildRemembered.Add Child (Remembered)
:i2 refid=menuMan.Add child (Remembered)
:p.By choosing :hp1.Add Child with|Partner|Remembered:ehp1. the
:link reftype=hd refid=menuManRemember.remembered:elink.
person becomes a child of the "Actual". You can also
:link reftype=hd refid=menuManChildNew.add a new child:elink..

:h2 res=&MI_ManParents. id=menuManParents.Add Parents
:i2 refid=menuMan.Add Parents
:p.By choosing :hp1.Add Parents:ehp1. the "Actual" is added parents.
:p.This item is not selectable, if the "Actual" already has parents.

:h2 res=&MI_ManPartner. id=menuManPartner.Add Partner
:i2 refid=menuMan.Add Partner
:p.By choosing :hp1.Add Partner:ehp1. the "Actual" is added an additional partner.
:p.This menu is a conditional cascade menu. If you have used the action
:link reftype=hd refid=menuManRemember.Remember:elink. before, you can
choose if you want to give the actual a new partner or the remembered
person as partner.

:h2 res=&MI_ManPartnerNew. id=menuManPartnerNew.Add Partner (New)
:i2 refid=menuMan.Add Partner (New)
:p.By choosing :hp1.Add Partner|New:ehp1. the "Actual" is added an
additional new partner.
If you want to give him a partner, who is in your tree already, choose
:link reftype=hd refid=menuManPartnerRemembered.Add Partner|Name:elink..

:h2 res=&MI_ManPartnerRemembered. id=menuManPartnerRemembered.Add Partner (Remembered)
:i2 refid=menuMan.Add Partner (Remembered)
:p.By choosing :hp1.Add Partner|Remembered:ehp1. the "Actual" is added the
previously :link reftype=hd refid=menuManRemember.remembered:elink..
You can also
:link reftype=hd refid=menuManPartnerNew.add a new partner:elink..
:p.You can only add a remembered person to someone, when he is
not married with him already and is not the same as the remembered.

:h2 res=&MI_ManRemember. id=menuManRemember.Remember
:i2 refid=menuMan.Remember
:p.By choosing :hp1.Remember:ehp1. the "Actual" is remembered, so you
can add him as a partner to someone, who already is in the tree.

.* ----------------- menuOptions -------------------------------------.
:h1 id=menuOptions.Options Menu
:i1 id=menuOptions.Options Menu
:p.The :hp1.Options Menu:ehp1. resides in the main menu at the top of
the main window. By choosing one of its item, you can directly jump to
an options page of the :link reftype=hd refid=winOptions.Options Dialog:elink..

The following option pages are available:
:ol compact.
:li.:link reftype=hd refid=menuOptionsLayout.Layout:elink.,
:li.:link reftype=hd refid=menuOptionsDataAssistant.Data Assistant:elink.,
:li.:link reftype=hd refid=menuOptionsMiscellaneous.Miscellaneous:elink.
:li.:link reftype=hd refid=menuOptionsUserFields.User Fields:elink.
:eol.

:h2 res=&MI_OptionsLayout. id=menuOptionsLayout.Layout
:i2 refid=menuOptions.Layout
:p.By choosing :hp1.Options|Layout:ehp1., you can jump to the settings
page of the :link reftype=hd refid=winOptions.Options Dialog:elink.,
where you can change layout parameters.

:h2 res=&MI_OptionsDataAssistant. id=menuOptionsDataAssistant.Data Assistant
:i2 refid=menuOptions.Data Assistant
:p.By choosing :hp1.Options|Data Assistant:ehp1., you can jump to the settings
page of the :link reftype=hd refid=winOptions.Options Dialog:elink.,
where you can change data assistant parameters.

:h2 res=&MI_OptionsMiscellaneous. id=menuOptionsMiscellaneous.Miscellaneous
:i2 refid=menuOptions.Miscellaneous
:p.By choosing :hp1.Options|Miscellaneous:ehp1., you can jump to the settings
page of the :link reftype=hd refid=winOptions.Options Dialog:elink.,
where you can change miscellaneous parameters like date format.

:h2 res=&MI_OptionsUserFields. id=menuOptionsUserFields.User Fields
:i2 refid=menuOptions.User Fields
:p.By choosing :hp1.Options|User Fields:ehp1., you can jump to the settings
page of the :link reftype=hd refid=winOptions.Options Dialog:elink.,
where you can define user fields for the actual tree.


.* ----------------- menuRexx -------------------------------------.
:h1 res=&MI_RexxStart. id=menuRexxStart.Start Rexx
:i1 id=menuRexxStart.Start Rexx
:p.You can start a Rexx-script by choosing :hp1.Rexx|Start:ehp1.
from the mainmenu at the top of the main window.
The :link reftype=hd refid=winRexx.Rexx dialog:elink.
appears for a detailed selection.
:p.A remembered script can be started directly.

.* ----------------- People ---------------------------------------.
:h1 id=people.People involved in this
:i1.People

These persons have something to do with FamilyTree:

:sl.
:li.:lines.
:hp1.Responsible for starting this whole thing:ehp1.
Nils Meier (meier2@cs.uni-bonn.de)
Birkenweg 27
53797 Lohmar
Germany / Europe
:elines.
:li.:lines.
:hp1.Mister Rexx:ehp1.
Peter "Grin" Gervai (grin@exabit.hu)
:elines.
:li.:lines.
:hp1.Dutch translation:ehp1.
Tjalco P. van der Mey (T.P.vd.Mey@net.hcc.nl)
Burg. Bloemersstraat 8
7271 DC Borculo
The Netherlands
Tel +31-545-273522
Fax +31-545-275108
:elines.
:li.:lines.
:hp1.French translation:ehp1.
Francois Massonneau (island@worldnet.fr)
Operator of Island's BBS (use "FTREE" to get the latest version there):
Node 1 : +33-(0)251.397.478
Node 2 : +33-(0)251.395.251
Fidonet 2:326/2.0
V34+ (Up to 33600 bauds).
:elines.
:esl.

.* ----------------- Registration ---------------------------------.
:h1 res=&ID_NAboutDialog. id=register.Registration
:i1 id=register.Registration
:p.Family Tree is :hp1.Shareware:ehp1.. You are allowed to test and use
this program for x days (:color fc=red.x depends on you:color fc=default.).
If you want to support the author in developing this software you
are invited to register.

:p.Registration fee is 20 German Marks or US $ 15.
You will get a registration code that you have to enter in the about dialog
(reachable by the menu :hp1.Help|About:ehp1.).
The registration is valid for all versions 1.x and 2.x.
Beside that you will get an e-mail, whenever a new version of Family Tree
is released.
:p.Add 5 German Marks or US $ 5, if you want to receive the actual version
on disk. Please send me a note which format I should put it on (3 1/2 or 5 1/4).

:p.You can pay by
:ol compact.
:li.Money transfer order to
.br
    Nils Meier
.br
    Account number 0516 075 130
.br
    at Allgemeine Deutsche Direktbank AG, Germany
.br
    bank code 500 105 17
.br
Please specify "FTree Registration" and your primary name.
Note: Please use a method where you pay the bank fee !
Because banks tend to charge immense ammounts of money
for their service, this way is not advised when living
outside germany.
:li.Cash by postal mail to :link reftype=hd refid=People.Nils Meier:elink.
:li.Eurocheque payable to :link reftype=hd refid=People.Nils Meier:elink. in German Marks
.br
Please don't send other cheques than Eurocheques. Foreign cheques are
charged with special rates from german banks.
:li.Registration through BMT Micro in Wilmington, NC 28408, USA
.br
Please refer to the text file Order.BMT for information on how to register this way.
(Registration fee is US $ 20)
:eol.

:p.Please send always the following information :
:ol compact.
:li.Name
:li.Address
:li.E-Mail address
:li.Way of payment
:ul compact.
:li.Money transfer order
:li.Cheque (Included in postal mail)
:li.Cash (Included in postal mail)
:eul.
:li.Where you want the registration code to be sent.
:ul compact.
:li.Postal mail to above address.
:li.E-mail to above address.
:eul.
:eol.

:p.:font facename=Helv size=24x24.Thanks for registering !:font facename=default size=0x0.



:euserdoc.
