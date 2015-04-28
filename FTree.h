#define ID_NMainWindow            100
#define ID_nbOpen                 104
#define ID_nbSave                 105
#define ID_nbPrint                106
#define ID_nbOptions              107
#define ID_nbLocate               108
#define ID_nbHoriVert             109
#define ID_nbAncestors            110
#define ID_nbDescendants          111
#define ID_nbSiblings             112
#define ID_isZoom                 113
#define ID_nslMessage             114
#define ID_nbTopBottom            115

#define ID_NEditWindow            150
#define ID_NListWindow            151
#define ID_NPrintDialog           152
#define ID_NAboutDialog           153
#define ID_NRexxDialog            154
#define ID_NRexxOutput            155
#define ID_NOptionsDialog         156
#define ID_NSearchDialog          157
#define ID_NEventsWindow          158

#define MI_Main                  1000
#define MI_MainFile              1002
#define MI_MainView              1004
#define MI_MainOptions           1005
#define MI_MainRexx              1006
#define MI_MainHelp              1007
#define MI_About                 1008

#define MI_FileNew               1020
#define MI_FileOpen              1022
#define MI_FileMerge             1023
#define MI_FileSave              1024
#define MI_FileSaveAs            1026
#define MI_FileClose             1028
#define MI_FilePrint             1030
#define MI_FileOpenDropped       1031 // Special internal command id
#define MI_FileOpenStartup       1032  // Special internal command id

#define MI_Mankind               1040
#define MI_MankindAncestors      1042 // 0
#define MI_MankindDescendants    1043 // 1
#define MI_MankindSiblings       1044 // 2
#define MI_MankindHori           1054 // 0
#define MI_MankindVert           1055 // 1
#define MI_MankindList           1058
#define MI_MankindEvents         1059
#define MI_MankindToolbar        1060
#define MI_MankindZoomIn         1061
#define MI_MankindZoomOut        1062

#define MI_Man                   1070
#define MI_ManCenter             1072
#define MI_ManEdit               1074
#define MI_ManOrigin             1076
#define MI_ManDel                1077
#define MI_ManDelPartner         1078 // Reserve +50
#define MI_ManDelChilds          1129 // Reserve +50
#define MI_ManDelParents         1179
#define MI_ManDelThis            1180
#define MI_ManChild              1181
#define MI_ManChildWith          1182 // Reserve 50
#define MI_ManChildNew           1232 // Reserve 50
#define MI_ManChildRemembered    1282 // Reserve 50
#define MI_ManParents            1390
#define MI_ManPartner            1391
#define MI_ManPartnerNew         1392
#define MI_ManPartnerRemembered  1393
#define MI_ManRemember           1394

#define MI_Manlist               1400
#define MI_ManlistSort           1401

#define MI_EventList             1402
#define MI_EventListNew          1403
#define MI_EventListDel          1404

#define MI_EventEdit             1405
#define MI_EventEditJump         1406
#define MI_EventEditChange       1407
#define MI_EventEditUp           1408
#define MI_EventEditDown         1409
#define MI_EventEditDel          1410

#define MI_ManEditEventList       1411
#define MI_ManEditEventListJump   1412
#define MI_ManEditEventListChange 1413
#define MI_ManEditEventListDel    1414

#define MI_RexxStart             1420 // Reserve 20

#define MI_OptionsLayout         1450 //  0
#define MI_OptionsDataAssistant  1451 // +1
#define MI_OptionsMiscellaneous  1452 // +2
#define MI_OptionsUserFields     1453 // +3

#define MI_FileBox               1460
#define MI_FileBoxOpen           1461
#define MI_FileBoxChange         1462
#define MI_FileBoxFirst          1463
#define MI_FileBoxDel            1464
#define MI_FileBoxNew            1465
#define MI_FileBoxClear          1466
#define MI_FileBoxDrop           1467

#define MI_Date                  1480
#define MI_DateAt                1481
#define MI_DateAbout             1482
#define MI_DateAfter             1483
#define MI_DateBefore            1484
#define MI_DateEstimated         1485

#define SI_MainTitle             2000
#define SI_MainTitleUnregistered 2001
#define SI_EditTitle             2002
#define SI_ListTitle             2004
#define SI_EventTitle            2005
#define SI_OptionsTitle          2006

#define SI_NotImplemented        2080
#define SI_TabStandard           2081
#define SI_TabUser               2082
#define SI_TabSources            2083
#define SI_TabEvents             2084
#define SI_TabAttachments        2085

#define SI_Name                  2100
#define SI_FirstName             2102
#define SI_Sex                   2104
#define SI_MaleFemale            2106
#define SI_BirthDate             2108
#define SI_BirthPlace            2110
#define SI_DeathDate             2112
#define SI_DeathPlace            2114
#define SI_MarriageDate          2116
#define SI_MarriagePlace         2118
#define SI_DivorceDate           2120
#define SI_Occupation            2122
#define SI_Address               2124
#define SI_Memo                  2126
#define SI_Bitmap                2128

#define SI_Adam                  2200
#define SI_Child                 2202
#define SI_Mom                   2204
#define SI_Dad                   2206
#define SI_Partner               2208
#define SI_NoneRemembered        2210
#define SI_NewPerson             2212
#define SI_ReallyDelChilds1      2214
#define SI_ReallyDelChilds2      2216
#define SI_ReallyDelParents1     2218
#define SI_ReallyDelParents2     2220
#define SI_ReallyDelThis1        2222
#define SI_ReallyDelThis2        2224
#define SI_ReallyDelPartner1     2226
#define SI_ReallyDelPartner2     2228
#define SI_ReallyStealChild1     2230
#define SI_ReallyStealChild2     2232
#define SI_AncestorLoop1         2234
#define SI_AncestorLoop2         2236
#define SI_ImportNoData          2238
#define SI_ImportNumPersons      2240
#define SI_ImportOrigin          2242
#define SI_ImportPAdded          2244
#define SI_ImportPDropped        2246
#define SI_ImportFDropped        2248

#define SI_DlgFileError          2300
#define SI_DlgFileFalse          2302
#define SI_DlgFileBadWrite       2304
#define SI_DlgFileLoad           2306
#define SI_DlgFileSave           2308

#define SI_BmpOrGifPlease        2350
#define SI_BmpGiveName           2352
#define SI_BmpDoRelative         2354

#define SI_NoHelp                2380
#define SI_SaveNow               2382

#define SI_NoProfile             2390
#define SI_NoSaveProfile         2392

#define SI_PrintTitle              2400
#define SI_PrintPrinter            2402
#define SI_PrintPageSize           2404
#define SI_PrintDPI                2406
#define SI_PrintPages              2408
#define SI_PrintFit                2410
#define SI_PrintZoom               2412
#define SI_PrintDoPrint            2414
#define SI_PrintDoProps            2416
#define SI_PrintDoCancel           2418
#define SI_PrintErrNoQueuesDef     2420
#define SI_PrintErrDevOpenFailed   2422
#define SI_PrintErrQueryCapsFailed 2424
#define SI_PrintErrCreatePSFailed  2426

#define SI_QueryGenerationsPlease  2450
#define SI_QueryGenerationsParent  2451
#define SI_QueryGenerationsChild   2452
#define SI_QueryGenerationsMax     2453
#define SI_QueryGenerationsUpTo    2454

#define SI_DropFileHere            2460
#define SI_DropPersonHere          2461
#define SI_DropEventHere           2462
#define SI_ClickForEvent           2463
#define SI_Person                  2464
#define SI_Event                   2465
#define SI_InvolvedPerson          2466
#define SI_InvolvedAs              2467

#define SI_MsgGenAncestors        2480
#define SI_MsgGenDescendants      2482
#define SI_MsgNumberOfMan         2484
#define SI_MsgNumberOfRel         2486
#define SI_MsgPrinting            2488
#define SI_MsgLoadPerson          2490
#define SI_MsgLoadMarriage        2492
#define SI_MsgLoadEvent           2493
#define SI_MsgSavePerson          2494
#define SI_MsgSaveMarriage        2495
#define SI_MsgSaveEvent           2496
#define SI_MsgLoadCheck           2497
#define SI_MsgDuplicateIDs        2498
#define SI_MsgDoingBackup         2499

#define SI_AboutTitle            2500
#define SI_AboutHead             2502
#define SI_AboutCopyright        2504
#define SI_AboutRegisterHere     2506
#define SI_AboutPlease1          2508
#define SI_AboutPlease2          2510
#define SI_AboutDoRegister       2512
#define SI_AboutLater            2514
#define SI_AboutIsRegTo          2516
#define SI_AboutThanks           2518
#define SI_AboutVerify           2520

#define SI_MonthNames            2550 // Reserve 13
#define SI_Window                2570
#define SI_Language              2571
#define SI_EnterPath             2572

#define SI_RexxDialog            2600
#define SI_RexxScript            2601
#define SI_RexxLongname          2602
#define SI_RexxAuthor            2603
#define SI_RexxComment           2604
#define SI_RexxStart             2605
#define SI_RexxRemember          2606
#define SI_RexxNotFound          2607
#define SI_RexxUnknown           2608
#define SI_RexxTitle             2609
#define SI_RexxFinished          2610
#define SI_RexxDone              2611
#define SI_RexxCancel            2612
#define SI_RexxPrint             2613
#define SI_RexxSave              2614
#define SI_RexxOutputSave        2615
#define SI_RexxOutputSaveError   2616
#define SI_RexxUnfinishedImport  2618

#define SI_ListId                2650 // +1
#define SI_ListSex               2651 // +1
#define SI_ListName              2652 // +1
#define SI_ListFirst             2653 // +1
#define SI_ListBirth             2654 // +1
#define SI_ListBPlace            2655 // +1
#define SI_ListDeath             2656 // +1
#define SI_ListDPlace            2657 // +1

#define SI_ListSortBy            2660
#define SI_ListSortFields        2661
#define SI_ListSortSequence      2662

#define SI_EventsTab1            2670
#define SI_EventsTab2            2671
#define SI_EventsTab3            2672
#define SI_EventsName            2673 // +1
#define SI_EventsFrom            2674 // +1
#define SI_EventsTo              2675 // +1
#define SI_EventsPlace           2676 // +1
#define SI_EventsDescription     2677 // +1

#define SI_OptionsLayout            2700
#define SI_OptionsDataAssistant     2702
#define SI_OptionsMiscellaneous     2704
#define SI_OptionsDateFormat        2703
#define SI_OptionsUserFields        2706
#define SI_OptionsProgram           2708
#define SI_OptionsTree              2710
#define SI_OptionsLines             2712
#define SI_OptionsDiagonal          2714
#define SI_OptionsOrthogonal        2716
#define SI_OptionsDetail            2718
#define SI_OptionsFirstNames        2720
#define SI_OptionsDates             2722
#define SI_OptionsColor             2724
#define SI_OptionsCBackground       2726
#define SI_OptionsCLines            2728
#define SI_OptionsCBoxes            2730
#define SI_OptionsCEmphasis         2732
#define SI_OptionsCTexts            2734
#define SI_OptionsCDefault          2736
#define SI_OptionsFont              2738
#define SI_OptionsAddPartSA         2740
#define SI_OptionsAddPartOS         2742
#define SI_OptionsAddChilSN         2744
#define SI_OptionsAddChilSA         2746
#define SI_OptionsAddPareMF         2748
#define SI_OptionsAddPareSN         2750
#define SI_OptionsAddPareSA         2752
#define SI_OptionsFocusToEdit       2754
#define SI_OptionsRelPath           2756
#define SI_OptionsDoBackup          2757
#define SI_OptionsImportPreserve    2758
#define SI_OptionsFloatEdit         2759
#define SI_OptionsFloatList         2760
#define SI_OptionsFloatEvents       2761
#define SI_OptionsMemoWordWrap      2762
#define SI_OptionsDateFmt           2763
#define SI_OptionsChooseFont        2764
#define SI_OptionsBorderlessButtons 2765

#define SI_FeatureNotAvailable1  2800
#define SI_FeatureNotAvailable2  2801

#define SI_UserFieldField        2820
#define SI_UserFieldValue        2821
#define SI_UserFieldFieldName    2822
#define SI_UserFieldFieldType    2823
#define SI_UserFieldYesNo        2824
#define SI_UserFieldText         2825
#define SI_UserFieldNumber       2826
#define SI_UserFieldDate         2827
#define SI_UserFieldNone         2828
#define SI_UserFieldMax3         2829
#define SI_UserFieldDelete       2830

#define SI_OK                    2850
#define SI_Cancel                2851
#define SI_New                   2852
#define SI_Change                2853
#define SI_Delete                2854
#define SI_Yes                   2855
#define SI_No                    2856
#define SI_Unknown               2857

#define RES_BmpDefault           3000
#define RES_BmpNotFound          3001
#define RES_IconFTree            3002

#define RES_BmpSexMale           3003
#define RES_BmpSexFemale         3004
#define RES_BmpSexUnknown        3005

#define RES_BmpOpen              3006
#define RES_BmpSave              3007
#define RES_BmpPrint             3008
#define RES_BmpOptions           3009
#define RES_BmpLocate            3010
#define RES_BmpVertical          3011
#define RES_BmpHorizontal        3012
#define RES_BmpTop               3013
#define RES_BmpBottom            3014
#define RES_BmpNoSiblings        3015
#define RES_BmpSiblings          3016
#define RES_BmpAncestors         3017
#define RES_BmpDescendants       3018

#define RES_BmpPageMan           3020
#define RES_BmpPageManPlus       3021
#define RES_BmpPageDocsA         3022
#define RES_BmpPageDocsB         3023
#define RES_BmpPageEventsA       3024
#define RES_BmpPageEventsB       3025
#define RES_BmpPageFilesA        3026
#define RES_BmpPageFilesB        3027
#define RES_BmpPageEvent         3028
#define RES_BmpPagePersons       3029

#define RES_BmpYes               3030
#define RES_BmpNo                3031
#define RES_BmpUnknown           3032

#define RES_BmpUP                3033
#define RES_BmpDOWN              3034
#define RES_BmpLEFT              3035
#define RES_BmpRIGHT             3036

#define RES_BmpLinkMan           3040
#define RES_BmpLinkEvent         3041

#define RES_IconDragMan          3050
#define RES_IconDragEvent        3051


#define PROFILE "FTREE.INI"
#define HELPFILE "FTREE.HLP"
#define TREEFILE "*.FAM"
#define DLLFILE "FTREE"
