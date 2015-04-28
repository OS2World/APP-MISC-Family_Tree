FamilyTree Version 1.2f
=======================

Copyright (C) 1996 Nils Meier

Permission to copy and distribute this software and its documentation
for private purpose and without charging more than a nominal fee for
copying is hereby granted, provided that the above copyright notice
appears in all copies.
This software is provided "as is" without express or implied warranty.
The author does not guarantee the correctness of the program. The
author may not be held responsible for any loss of data or other
damages.

From version 1.0d on, FamilyTree is Shareware. Please test and review
this program and register if you want to support the authors efforts
in developing this program further. Thanks.
Please refer to the online-help for more information about registering.

SHORT DESCRIPTION:
==================

FamilyTree is a PM-32bit-multithreaded program for OS/2 3.x.

With FamilyTree you can edit your ancestral data as a visual family
tree. You can add persons to a family tree and enter their properties
have a look at their ancestors and descendants and investigate
relationships.
Supported properties are: Name, Birth, Death, Marriage, Address,
Comment, Picture, Files, *UserFields , *Sources, *Events (*=planned).
Working is done by pointing with the mouse and opening context-menus
via a right-mouse-click.
FamilyTree supports Rexx-scripts for various (extendable) reports and
im/export (GEDCOM is supported).

I'm providing my own family tree NILS.FAM as an example.

Refer to online help (F1) for more information about this program.

FamilyTree was written and tested on OS/2 3.0 (4) with VAC++ 3.0.

INSTALLATION:
=============

Unpack the file FTREE*.ZIP into the wanted directory on your hard disk.
Use the program INSTALL.CMD to create a FamilyTree program object on the
desktop and initialize a language specific DLL+HLP-file.
Make sure your CONFIG.SYS contains something similar like this:
      LIBPATH=.;remaining pathinstruction

RUNNING FTREE:
==============

Go to the command-line and type ftree.exe or double-klick on
the Family Tree program object on you desktop (if installation was
successfull).
You can start with a default tree by specifying its name as a parameter
on the command-line (ftree.exe .\nils.fam), or entering this information
in the parameters field in the program object.

RELEASE HISTORY:
================

version (NEW feature, BUG fixed, CHAnge)

1.0 , Mar 11 1996: NEW:initial release

1.0a, Mar 16 1996: BUG:Forgot /Gd- with DLLs

1.0b, Mar 24 1996: NEW:Choose given name with tilde (~)
                   NEW:Drag&Drop of pictures to properties
                   NEW:max. childs = 12
                   NEW:DeathDate

1.0c, Apr 14 1996: NEW:FTree now writes a new format that can't be read by older versions !!!
                       (reading old format is still supported)
                   NEW:HELP buttonpanel now contains Contents/Previous/Search/Print/Index
                   CHA:Changed initial Position of list/main/edit window
                   CHA:Memosize is now 4k
                   CHA:Loading is done in a separate Thread
                   CHA:Date in english is MM/DD/YYYY - in german DD.MM.YYYY
                   NEW:FTree asks for save of modifications when closing a tree
                   NEW:MainWindow remembers that it was maximized
                   BUG:Workaround for S3-Video Driver Bug in API GpiBox
                       (red line remains when Actual changes)
                   NEW:Rings between partners
                   NEW:Multiple marriages (maximum is 4)
                   NEW:New fixed fields: Sex + Death location + MarriageInfos (marriage,location,divorce)
                   NEW:Siblings are sorting according to their Birth date / Partners by Marriage date
                   NEW:Choose treeview information (first name,dates)
                   NEW:Printing
                       Attention  : Printing on FaxWorks only works if FaxWorks is already running
                       Information: I'll try to put printing in a 2nd thread, but an ugly bug
                                    makes this impossible in this version :-(

1.0d, Apr 22 1996: BUG:Printing is now done in a second thread and as I expected FaxWorks works, too
                       Information: You have to open the DeviceContext in the second thread, not
                       (as in 1.0c) in the first thread :-/
                   NEW:InfoBar gives information about Printing/Loading/Saving
                   BUG:Red border of "Actual" isn't printed
                   CHA:Improved scrolling performance
                   NEW:Click and Drag Tree by pressing button1 and moving the mouse
                   NEW:"About" dialog (with version info) where one can register FamilyTree
                   NEW:Menus accessible by accelerator keys and zoom via +/-
                   CHA:Improved view calculation after user action (New,Load,MakeOrigin,Add,Del)
                   BUG:When printing with only Ancestors on a fixed number of pages
                       the size isn't calculated correctly ("Origin" is missing)
                   NEW:Vertical view of the tree
                   BUG:Sex is now saved language independent (0/1/2 instead of -/M/W because W!=F)

1.0e, May 01 1996: NEW:Date Format (DD.MM.YYYY or MM/DD/YYYY) is not decided by language but
                       OS/2 system setting
                   NEW:Rexx (beta), saving no printing
                   NEW:ABC-List.ftx (List of persons sorted by name)
                   NEW:AnnDeath.ftx (Anniversary of death)
                   NEW:Birthday.ftx (Birthdays this year)
                   NEW:Lngevity.ftx (Longevity analysis)
                   BUG:Removed display-bug in vertical mode with A+D+S
                   NEW:AddChild -> gets name of parent (the actual one)
                   NEW:Tabbing between dates -> number is selected and overwritten by next key press
                   NEW:Dialogs appear in window list
                   NEW:Drag&Drop of Font and Color to MainWindow

1.0f, May 25 1996: BUG:Dragging was still active though no mouse button pressed
                   BUG:Birthday.ftx didn't work correctly because sorting by birthdate was wrong
                   CHA:List dialog now is a container (DetailsView) with/sorted by dates
                   CHA:Printing of several pages is done in reversed order
                   NEW:Partial Dates
                   NEW:Sex of "parents" preselected
                   CHA:n/n after selection of person with several partners
                   BUG:Delete operation can result in an inconsistent Tree
                       when using multiple Partnerships !!
                   BUG:Display was corrupt when using A+D+S and multiple Partnerships
                   CHA:List window as container (with sorting)
                   NEW:Person.ftx   (Personal Data)
                   NEW:Weddings.ftx (Wedding Anniversaries)
                   NEW:Registration is possible through BMT Micro
                   NEW:Rexx scripts for GEDCOM Im/Export (beta)
                   NEW:Rexx Documentation (beta)
                   CHA:Changed Class MAN -> no IGList anymore

1.0g, Jun 25 1996: BUG:3175 if not printer is installed
                   NEW:Actual zoom rate is saved in .fam file
                   BUG:Edit dialog didn't highlight address field when editing
                   NEW:Females are drawn with rounded borders
                   CHA:No initial "missing profile"
                   CHA:List dialog is no longer autoforeground
                   NEW:List dialog can hide/show columns
                   CHA:List dialog has better performance
                   CHA:List dialog can sort by multiple fields (e.g. Name,First,Birth)
                   CHA:EditDialog has new layout
                   CHA:Memo Field and Picture get equal space
                   CHA:Improved performance during Load
                   BUG:Bug after several Rexx starts: didn't close all pipe handles
                   NEW:Date Modifiers '+-'ABT '>'AFT '<'BEF '?'EST
                   NEW:TITL supported
                   CHA:??.??.1950 -> --.--.1950 in main window
                   NEW:Rexx Output can be copied to Clipboard
                   NEW:More Rexx scripts
                   BUG:Hungarian date is not correct YYYY-MM-YYYY (Sorry Grin :)
                   CHA:Technically unlimited number (65535) of children and partners
                       (restricted to 50 because otherwise too many menu items)
                   CHA:Picture Drag&Drop shows link operation to visualize linking and no data copying.
                   BUG:GEDCOM Export did a 'W' for female -> changed to 'F'
                   BUG:Started with wrong "max ID" after loading -> two persons with same ID
                      (bad for GEDCOM export)

1.0h, Aug 18 1996: NEW:ImGedcom.ftx now understands CONC like CONT of NOTE
                   NEW:Rexx scripts *.ftx can have a long name that appears in menu
                   NEW:Family relations can be shown with diagonal or orthogonal lines
                   BUG:Still 3175 when no printer installed
                   BUG:3175 when changing printer properties under MERLIN
                   BUG:Invalid cursor during printing/saving/rearranging
                   NEW:ListDialog scrolls to pointed person + cursor key strokes
                   BUG:New familiy-IDs start too low after loading of tree
                   BUG:IDs are checked and corrected after loading
                   CHA:Rexx output is saved/copied to clipboard with CR/LF instead of LF
                   NEW:Configuration tab notebook:
                       Layout (Program settings) :
                       X Font
                       X Colors
                       X Detail: firstName,dates
                       X Lines: diagonal/orthogonal
                       Data Assistant (Program settings) :
                       X Partner -> same address
                       X Partner -> opposite Sex
                       X Child   -> same address
                       X Child   -> same name
                       X Parents -> Male+Female
                       X Parents -> same address
                       X Parents -> same name
                       Miscellaneous (Program settings) :
                       X Focus to edit dialog ofter click on person
                       X Try to store relative paths
                       X Date format MM-DD-YYYY/DD-MM-YYYY/YYYY-MM-DD Separator .-x/
                       User Fields (Tree settings) :
                       - 10 programmable fields (not yet)
                   NEW:selectable depth of ancestor/descendants generations (Siblings)
                   CHA:edit dialog has new layout
                   CHA:main window has new layout
                   NEW:Included FILE_ID.DIZ
                   CHA:ExGedcom.ftx did swapped HUSB+WIFE (depending on tree)
                   NEW:"Add Partner" allows existing person to be added
                   NEW:New ManTree which has no problems with circles in data
                   CHA:Revised and updated docs to explain new features
                       incl. info about REXX-scripts, dialogs, etc.
                   NEW:Dutch language dll
                   NEW:When deleting, the person's name is shown in the safety dialog
                   NEW:new consistency check after import (certain loops allowed)

1.0i, Aug 26 1996: BUG:Import Routine stumbles over cousins (or great-cousins,...) who are married
                   BUG:Delete|This results in incorrect tree (total crash during save)
                   NEW:You can now delete a persons partner via context menu

1.1 , Oct 17 1996: NEW:Backup last version before save to *.BAK
                   BUG:Rexx menu fast access doesn't work
                   CHA:Drawing of persons and families is faster
                   BUG:During actions is buttonbar not disabled
                   BUG:System isn't notified when mouse click on list window happens
                   BUG:After bringing ManList to forground, a row that is half hidden isn't redrawn
                   BUG:Diagonal lines to two or more children are not drawn correctly
                   CHA:Changed edit dialog (because of notebook)
                   BUG:After changing data in the edit window and Loading/Creating a new tree,
                       there's no warning to save the tree first
                   BUG:Warning before Rexx execution to save data first
                   NEW:Edit dialog with a drag&drop-field for additional information
                       (Gifs,Texts,AVIs,..) that can be opened by a double-click
                       (registered version only)
                   CHA:ImGedcom.ftx accepts dates non-uppercase months like 'Jan 1900'
                   NEW:Notebooks under Merlin have new look
                   BUG:ID check method doesn't check family IDs when person IDs are o.k.
                   NEW:Add existing Person as Child to family (old Parents loose it)
                       (Together with 'Add Partner|Existing Person', a tree can be corrected
                        without reentering data in case of previously wrong inserted data)
                   CHA:GEDCOM import that doesn't drop persons unconditionally
                   CHA:Dates with no information (day/month/year) can have modifiers
                   CHA:Rexx Selection Dialog shows only language specific info
                   NEW:French language dll

1.1a, Oct 30 1996: NEW:Double-click with shift-key on a person opens the edit-dialog
                   BUG:Main window minimized and selecting person from list window -> Exception
                   BUG:French language support says Englais instead of Francais (French.rc+FTXs)
                   BUG:Sys3175 with S3 drivers (under 800x600x65k colors)
                       [ISlider::moveArmToTick(9) results in exception when window is visible]
                   BUG:Edit dialog scroll bars are always both visible or none
                   NEW:Long name of REXX-script is language dependend
                   CHA:Documentation explains date modifiers
                   NEW:Rexx can sort persons by PID
                   NEW:Edit & List Window can be floating/non-floating

1.1b, Nov 16 1996: BUG:Still Sys3175 under certain resolutions/color depths - RTFM !!

1.2 , May 25 1997: BUG:Gedcom export writes 05 25 1970 instead of 05 MAY 1970
                   NEW:RexxFunction get/setAddress is mentioned in the docs
                   NEW:Notes word wrapping can be switched on/off
                   NEW:Rexx Funktion selectPerson('R|emembered') + selectPerson('O|rigin')
                   NEW:Toggle show/hide toolbar
                   NEW:List of persons is rewritten to reuse code in event lists
                   NEW:Control for entering dates is rewritten to reuse code from list/tree of persons
                   NEW:Dates are drawn day/month/year aligned
                   NEW:Redesign of several window elements for preserving screen space / more useability / better look
                   BUG:Printing over several pages can lead to missing parts of persons at paper edge (Reported by E.Neubauer)
                   NEW:Drag&Drop of tree-file to main window
                   CHA:Query dialog for visible generations of ancestors/descendants
                   NEW:Events
                   NEW:User defineable fields (Maximum of 3 in unregistered version)

1.2a, Jun 02 1997: NEW:Added fly over text for marriage button - shows name of partner
                   BUG:No automatic focus switch to edit window after click on person
                   CHA:Revised and updated online-help
                   BUG:Change File-attachment via menu -> catched exception
                   BUG:Partnerships in edit window are indexed in reversed order

1.2b, Jun 10 1997: CHA:Date format settings has an own page in the options dialog
                   BUG:Tree calculation hangs on certain trees (e.g. Heuler+Caesar)
                   CHA:Status area in button bar doesn't use all available space
                   NEW:Installation sets FTree object as default app for .fam-files

1.2c, Jun 19 1997: BUG:Serious bug writing user-fields -> results in crash during load
                   BUG:Invalid Cursor while working with the tree and having a minimized list-window

1.2d, Jul 06 1997: BUG:Selecting a menu item from the notebook pop-up under Warp4 can crash FTree
                   BUG:Crash when starting FTree with a tree as parameter (mostly only first time after boot)
                   NEW:Setting that chooses border around new looking buttons
                   BUG:Error in IBM class library can cause in unloadable tree (extra sync before seekg)

1.2e, Oct 16 1997: NEW:Width of columns in lists of edit window are remembered
                   BUG:Running under Warp 3.0 or Warp 4.0 + Fix#3 results in corrupted event list

1.2f, Aug 05 1998: NEW:UserFields can be used by REXX-Scripts. Please look at person.ftx


CONTACTING THE AUTHOR:
======================

My mail address is
    Nils Meier
    Birkenweg 27
    53797 Lohmar
    GERMANY / EUROPE

My Internet address is
    meier2@athene.informatik.uni-bonn.de

Please send any suggestions, comments or bug reports to one of the above
addresses.
