:userdoc.
:title.Family Tree (Online Hilfe)
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
:h1 id=FamilyTree.Stammbaum
:i1 id=FamilyTree.Stammbaum
:p.Ein Stammbaum besteht aus Menschen, die miteinander verwandt
sind. Im "leeren" Stammbaum befindet sich nur ein Mensch welcher
Ursprung und der Aktuelle ist:
:artwork name='sample1.bmp'.
.*.
W„hlen Sie vom Kontextmen des :link reftype=hd refid=menuMan.Mensch:elink.en
:link reftype=hd refid=menuManPartner.Neuer Partner:elink. und er
bekommt einen Partner:
:artwork name='sample2.bmp'.
.*.
Jetzt k”nnen Kinder hinzugefgt werden, indem aus dem Kontextmen
einer der beiden Menschen
:link reftype=hd refid=menuManChild.Kind hinzufgen:elink. ausgew„hlt wird.
Wenn Sie ein Menschen ausw„hlen, werden Sie bemerken, daá er
den rot-schraffierten Rahmen bekommt - dieser dann der Aktuelle.
:artwork name='sample3.bmp'.
.*.
Eltern fr "Adam" sind kein Problem. Selektieren Sie "Adam", indem Sie
auf ihn klicken, und w„hlen
Sie aus dem Kontextmen dieses :link reftype=hd refid=menuMan.Mensch:elink.en
:link reftype=hd refid=menuManParents.Eltern hinzufgen:elink..
:artwork name='sample4.bmp'.
.*.
Nicht alle im Stammbaumes werden gleichzeitig gezeigt.
Welche Verwandte gezeigt werden h„ngt davon ab, wer der "Ursprung" ist.
Alle Vorfahren (direkte Linie) und Nachfahren (mit Partner) des "Ursprungs"
werden gezeigt.
(Ausnahme: Sie k”nnen die Darstellung des Stammbaumes durch das
:link reftype=hd refid=menuMankind.Ansichtmen:elink. ver„ndern).
.*.
:p.Versuchen Sie, "Partner" zum "Ursprung" zu machen, indem Sie auf ihn
doppelklicken oder :link reftype=hd refid=menuManOrigin.Ursprung:elink. aus
seinem Kontextmen w„hlen. Dadurch „ndert sich die
Farbe von "Partner" zu :color fc=red.rot:color fc=default.
- nun ist er der "Ursprung" des Stammbaumes.
:artwork name='sample5.bmp'.
.*.
Habe Sie gemerkt, daá die Eltern von "Adam" verschwunden sind und nur eine
gepunktete Linie darauf hinweiát, daá sie vorhanden sind ? Aber nun k”nnen
Sie alle Vorfahren von "Partner" sehen, wenn Sie diese hinzufgen.
.*.
:p.Machen Sie nun "Kind" zum "Ursprung"
(seine Farbe wechselt zu :color fc=red.rot:color fc=default.)
und fgen Sie ein paar Brder ber das Kontextmen von "Adam" hinzu:
:artwork name='sample6.bmp'.
.*.
Die Brder von "Kind" werden nicht gezeigt, da diese weder Vorfahren
noch Nachfahren von ihm sind.

:p.Versuchen Sie, einen Stammbaum aufzubauen und den "Ursprung"
zu ver„ndern. Verstellen Sie die Darstellung
ber das :link reftype=hd refid=menuMankind.Ansichtmen:elink..
:p.™ffnen sie das :link reftype=hd refid=winList.Listenfenster:elink.,
indem alle Menschen alphabetisch sortiert dargestellt werden.
:p.™ffnen Sie das :link reftype=hd refid=winEdit.Editierfenster:elink.
und „ndern Sie Eigenschaften, wie Name oder Vorname.

.* ------------------- winMain --------------------------------.
:h1 res=&ID_NMainWindow..Hauptfenster
:i1 id=winMain.Hauptfenster
:p.:hp3.Family Tree:ehp3.
:p.  ... grafische Verwaltung eines Stammbaumes :artwork runin name='FTree.bmp'.
:p.Copyright 1996, 1997 by :link reftype=hd refid=People.Nils Meier:elink.
:note text='Wichtig :'.Bitte lesen Sie :hp1.liesmich.txt:ehp1. fr
Ver”ffentlichungsinformationen !
:p.Dies ist die Starthilfe fr Family Tree. Wenn Sie das Programm starten, sehen
sie das Hauptfenster vor sich und gelangen ber F1 zu dieser Hilfe.
:p.Das Hauptfenster besteht aus einem "weiáen Blatt" mit einem Menschen
"Adam", einer :link reftype=hd refid=toolbar.Buttonleiste:elink. und einem Fenstermen.
Das Prinzip eines Stammbaumes wird :link reftype=hd refid=FamilyTree.hier:elink.
erkl„rt.
.*.
:p.Sie k”nnen direkt damit beginnen einen Stammbaum einzugeben, indem Sie einen
Menschen ausw„hlen (einfacher Mausklick), das Kontextmen des
:link reftype=hd refid=menuMan.Mensch:elink.en aktivieren
(rechter Mausklick) und Aktionen daraus ausw„hlen.
Klicken Sie mit der rechten Maustaste in den Stammbaum auf einen freien Bereich
und das :link reftype=hd refid=menuMankind.Ansichtmen:elink. erscheint.
:p.Wenn Sie mit der linken Maustaste auf den Stammbaum klicken und diese gedrckt
halten, k”nnen Sie durch Bewegung des Mauszeigers den Stammbaum verschieben.
.*.
:p.šber das :link reftype=hd refid=menuFile.Dateimen:elink. k”nnen Sie einen
Stammbaum speichern, einladen und drucken.
Das :link reftype=hd refid=menuMankind.Ansichtmen:elink.
erm”glicht es, die Darstellung des Stammbaumes zu ver„ndern.
Rexx Skripte, die Berichte erstellen und Im/Exportieren, starten Sie ber das
:link reftype=hd refid=menuRexxStart.Rexxmenu:elink..
Sie k”nnen auch :link reftype=hd refid=rexxing.eigene Rexx-Skripte erstellen:elink..
:p.Sie k”nnen den Zoomfaktor des Baumes mit dem Schieberegler in der Buttonleiste
oder durch drcken der Tasten +/- ver„ndern.
:p.Wenn Sie Programm- oder Baumeinstellungen „ndern wollen, erreichen Sie
ber das :link reftype=hd refid=winOptions.Optionsmen:elink.
den :link reftype=hd refid=winOptions.Optionsdialog:elink..
:p.Falls Sie immer den gleichen Stammbaum bearbeiten wollen, geben Sie den Namen auf der
Kommandozeile oder im Parameterfeld des Programmobjektes an, und er wird beim Starten
automatisch eingeladen.
:p.Durch Drag&amp.Drop einer .fam-Datei auf das Hauptfenster k”nnen Sie ein
Laden veranlassen.

.* ------------------- toolbar    ------------------------------.
:h1 id=toolbar.Toolbar
:i1.Toolbar
Im Hauptfenster oberhalb des Baumes liegt die Buttonleiste:
:artwork name='Toolbar.bmp'.
Darauf sind mehrere Buttons, die folgendes bewirken:
:ol.
:li.:link reftype=hd refid=treeLoad.Daten laden:elink.
:li.:link reftype=hd refid=treeSave.Daten speichern:elink.
:li.:link reftype=hd refid=treePrint.Daten drucken:elink.
:li.:link reftype=hd refid=winOptions.Optionen ver„ndern:elink.
:li.:link reftype=hd refid=winSearch.Suchen:elink.
:lp.
:li.:link reftype=hd refid=menuMankindVert.Vertikale:elink. /
:link reftype=hd refid=menuMankindHori.horizontale:elink. Ansicht
:li.:link reftype=hd refid=menuMankindAncestors.Anzahl der sichtbaren Generationen von Vorfahren einstellen:elink.
:li.:link reftype=hd refid=menuMankindDescendants.Anzahl der sichtbaren Generationen von Nachfahren einstellen:elink.
:li.:link reftype=hd refid=menuMankindSiblings.Ansicht der Geschwister anschalten:elink.
:li.:link reftype=hd refid=menuMankindZoomIn.Rein:elink. /
:link reftype=hd refid=menuMankindZoomOut.raus:elink.zoomen
:lp.
:li.Ort der Buttonleiste (oben/unten) einstellen
:eol.

.* ------------------- winSearch ------------------------------.
:h1 res=&ID_NSearchDialog. id=winSearch.Suchfenster

Diese Funktion ist noch nicht implementiert.

.* ------------------- winOptions ------------------------------.
:h1 res=&ID_NOptionsDialog. id=winOptions.Optionsdialog
:i1.Optionsdialog
:p.Hier k”nnen Sie die Optionen von FamilyTree „ndern. Dazu gibt
es vier Notizbuchseiten mit Optionen fr
:ol compact.
:li.Layout
:li.Datenassistent
:li.Verschiedenes
:li.Benutzerfelder
:eol.

:dl compact.
:dt.:hp2.Layout:ehp2.
:dd.Hier k”nnen Sie angeben, wie Linien zwischen Menschen aussehen.
Sie k”nnen die Detailstufe einstellen, die Schriftart und Farben der
sichtbaren Elemente einstellen.
:dt.:hp2.Datenassistent:ehp2.
:dd.Den Datenassistent beeinflussen Sie hier. Abh„ngig von der
Aktion die Sie durchfhren, wie "Neuer Partner", k”nnen Sie die
Option "bekommt gleiche Adresse" ausw„hlen, so daá ein neuer
Partner die Adresse des "Aktuellen" bekommt.
:dt.:hp2.Verschiedenes:ehp2.
:dd.Wenn Sie das Datumsformat „ndern wollen, geben Sie hier einfach
das Format und das Trennzeichen an. Auáerdem k”nnen Sie den
automatischen Eingabefocuswechsel aktivieren, so daá nach einem
Mausklick auf eine Person das Editierfenster aktive wird. So k”nnen
Sie durch den Baum wandern und brauchen nicht mit der Maus
zum Editierfenster gehen, um Daten zu „ndern.
:p.Eine andere Sache ist die Option "Versuche relativen Pfad zu speichern".
Wenn diese aktiviert ist, versucht FamilyTree fr alle Attachments
einen relative Pfad zu merken. Das bedeutet, daá nach einem Bild-Drop
FamilyTree alle absoluten Pfadinformationen l”scht, wenn dieses
in einem Unterverzeichnes des aktuellen Baumes ist.
FamilyTree warnt dann auch, wenn es keinen relative Pfad berechnen
kann und speichert dann den absoluten Pfad.
:p.Mit "Immer die nicht importierten Personen aufheben" k”nnen Sie bestimmen,
daá eventuell briggebliebene Daten nach dem Import immer aufgehoben werden.
So k”nnen Sie diese Daten schnell Baum fr Baum importieren und werden nicht
jedesmal gefragt. Groáe Datenmengen brauchen dann allerdings auch viel
Speicherplatz, der bis zum n„chsten Import belegt ist!
:dt.:hp2.Benutzerfelder:ehp2.
:dd.Hier k”nnen Sie eigene Benutzerfelder fr Personen definieren
(In der unregistrierten Version k”nnen Sie nur 3 eigene Felder erzeugen).
Es gibt momentan vier verschiedene Typen von Feldern:
:ol compact.
:li.Ja/Nein
:li.Text
:li.Zahl
:li.Datum
:eol.
:p.Die Felddefinitionen geh”ren zum jeweiligen Stammbaum und werden beim
Speichern mit ihm gesichert.
:edl.

.* ------------------- winEdit ------------------------------.
:h1 res=&ID_NEditWindow. id=winEdit.Editierfenster
:i1.Editierfenster
:p.In diesem Fenster k”nnen sie die Eigenschaften des aktuellen Menschen ver„ndern.
Es besteht aus fnf Seiten:

:artwork name='edit.bmp'.

:ol.
:li.Seite
:p. Die erste Seite enth„lt mehrere Eingabefelder fr verschiedene
personenbezogene Daten. Fllen Sie sie einfach fr die aktuelle
Person aus.

:artwork name='edit1.bmp'.

:p.Wenn Sie eine Tilde (~) in das Vornamenfeld eingeben, wird
der Vorname direkt dahinter der Rufname und im Stammbaum gezeigt.
:p.Sie k”nnen Datumsinformationen l”schen, indem Sie :hp1.ESC:ehp1.
drcken, wenn der Cursor in einem Datumsfeld ist.
Auáerdem kann ein Datum noch einen Modifikator beinhalten, der
nach einen rechten Mausklick auf das Datum ausgew„hlt werden kann.
 :p.ndern Sie das Geschlecht einen Menschen, indem Sie auf
den Button mit dem Personensymbol klicken (grau: nicht angegeben, blau: m„nnlich,
rot: weiblich).
:p.Wenn der Mensch mehrere Partner hat k”nnen die Partnerschaften
durchgehen, wenn Sie auf den Knopf neben dem Heiratsfeld klicken.
:p.Ziehen Sie per Drag&amp.Drop ein GIF, BMP oder PCX auf das
Bild oder klicken Sie auf das Bild um einen Pfad zum gewnschten
Bild manuell einzugeben.
:warning text='Vorsicht:'.FTree merkt sich nur, wo das Bild
liegt (Die Bilddaten werden nicht gespeichert).
Wird das Bild also sp„ter verschoben oder gel”scht, kann
FTree es nicht mehr finden und zeigt ein Fragezeichen.

:li.Seite
:p.Nachdem Sie im :link reftype=hd refid=winOptions.Optionsdialog:elink.
Benutzerfelder fr den Baum definiert haben, k”nnen Sie hier personenbezogene
Daten, wie hier gezeigt, eingeben:

:artwork name='edit2.bmp'.

:li.Seite
:p.Quellenangaben sind noch nicht implementiert.

:li.Seite
:p.Diese Seite zeigt alle Ereignisse, in denen die aktuelle Person involviert
ist. Erzeugen Sie Ereignisse im :link reftype=hd refid=winEvents.Ereignisfenster:elink.
und fgen Sie dies mittels Drag&amp.Drop der aktuellen Person hinzu.
Sie werden anschlieáen nach der Rolle der Person beim Ereignis befragt.
Wenn Sie einen rechten Mausklick auf ein Ereignis durchfhren, k”nnen Sie die
Beteiligung entfernen, oder die Rolle „ndern.
Dieses Beispiel zeigt, daá die aktuelle Person auf zwei Geburtstagsfeiern anwesend
war:

:artwork name='edit4.bmp'.

:li.Seite
:p.Auf der letzten Seite k”nnen Referenzen zu Dateien gespeichert werden, die im
Zusammenhang zur aktuellen Person stehen.
Wenn Sie zum Beispiel Bilder oder Dokumente haben, ziehen Sie diese einfach per
Drag&amp.Drop hierher.
Sp„ter k”nnen Sie die Referenzen durch einen rechten Mausklick ver„ndern.

:artwork name='edit5.bmp'.

:eol.

.* ------------------- winList ------------------------------.
:h1 res=&ID_NListWindow. id=winList.Listenfenster
:i1.Listenfenster
:p.In diesem Fenster wird eine Liste aller Menschen im Stammbaum angezeigt.
Sie k”nnen einen Menschen ausw„hlen und sein Kontextmen per rechtem Mausklick
aufklappen.
:artwork name='list.bmp'.
:p.™ffnen Sie das Listenfenster, indem sie Liste aus dem
:link reftype=hd refid=menuMankind.Ansichtmen:elink. w„hlen.
:p.Durch einen rechten Mausklick auf einen freien Bereich oder
die Spaltenk”pfe erscheint ein Men mit einer Auswahl.
Mit dieser k”nnen Sie die Sortierkriterien ver„ndern.
Wenn Sie nur nach einem Kriterium (z.B. Name) sortieren
m”chten, klicken Sie einfach auf den entsprechenden Spaltenkopf.

.* ------------------- winEvents -----------------------------------.
:h1 res=&ID_NEventsWindow. id=winEvents.Ereignisfenster
Dieses Fenster enth„lt eine Liste aller Ereignisse des Baumes.
:artwork name='events.bmp'.
:p.Sie k”nnen das aktuelle Ereignis mittels der Seiten auf der
rechten Seite „ndern. Ein Ereignis besteht aus drei Seiten von Daten:

:ol.
:li.Seite
:p.Die erste Seite enth„lt mehrere Eingabefelder fr verschiedene
Informationen ber das aktuelle Ereignis. Geben Sie einfach die
Ereignisdaten hier ein:

:artwork name='events1.bmp'.

:li.Seite
:p.Ziehen Sie eine Person aus dem Baum oder der Liste mittels Drag&amp.Drop
hierher. Sie werden dann nach der Rolle dieser Person bei dem Ereignis
befragt.
Wenn Sie mit der rechten Maus auf eine Person in der Liste klicken, k”nnen
Sie dessen Beteiligung „ndern.

:artwork name='events2.bmp'.

:li.Seite
:p.Die letzte Seite speichert Quellenverweise, die das Ereignis belegen
(z.B. eine Referenz auf ein Schiffsregister bzgl. einer Auswanderung).
Diese Funktion ist noch nicht implementiert.

:eol.


.* ------------------- winPrint ------------------------------.
:h1 res=&ID_NPrintDialog. id=winPrint.Druckdialog
:i1.Druckdialog
:p.Zu diesem Dialog gelangen Sie, indem Sie aus dem Men den
Punkt :hp1.Drucken:ehp1. w„hlen.
:p.Hier k”nnen Sie vor dem Drucken den gewnschten :hp1.Drucker:ehp1.,
ausw„hlen und Ausdruckparameter einstellen.
:artwork name='print1.bmp'.
:p.Klicken sie auf :hp1.Angepaát:ehp1. so k”nnen sie in den Seitenfeldern
die gewnschte Seitenanzahl einstellen. Der Stammbaum wird auf dieses
Maá vergr”áert oder verkleinert.
:artwork name='print2.bmp'.
:p.Ist :hp1.Angepaát:ehp1. nicht ausgew„hlt, so k”nnen Sie mit dem
Schieberegler :hp1.Zoom um Faktor:ehp1. einen festen Vergr”áerungsfaktor
ausw„hlen.
:artwork name='print3.bmp'.
:p.Klicken Sie auf :hp1.Drucken:ehp1. um den Druckvorgang zu starten,
:hp1.Eigenschaften:ehp1. um Druckeneinstellungen zu ver„ndern und
:hp1.Abbrechen:ehp1. um zum Stammbaum zurckzukehren.
:artwork name='print4.bmp'.


.* ------------------- winRexx ------------------------------.
:h1 res=&ID_NRexxDialog. id=winRexx.Rexxdialog
:i1.Rexxdialog
:p.In diesem Dialog k”nnen Sie ein Rexx-Skript ausw„hlen.
:artwork name='rexx1.bmp'.
:p.Es werden alle Rexx-Skripte (*.ftx) mit Autor und einer kurzen
Beschreibung angezeigt, die sich im aktuellen Verzeichnis befinden.
:artwork name='rexx2.bmp'.
:p.Sie k”nnen das angezeigt Skript :hp1.Starten:ehp1. oder den
Vorgang :hp1.Abbrechen:ehp1..
Nach der Auswahl des Buttons :hp1.Merken:ehp1. erscheint das
Skript im Rexxmen und kann dort direkt ausgew„hlt werden.
:p.FamilyTree hat momentan folgende Skripte:
:ol compact.
:li.Alphabetic List (ABC-List.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skrtip zeigt alle Personen in einer alphabetischen Liste.
:li.Anniversaries of Death (AnnDeath.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript zeigt die Todestage, sortiert nach Monat+Tag.
:li.List Birthdays (Birthday.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript zeigt Geburtstage, sortiert nach Monat+Tag.
:li.Check Consistency (Check.ftx) von :link reftype=hd refid=People.Peter Gervai:elink.
.br
Diese Skript berprft den aktullen Baum und untersucht ihn
nach Ungereimtheiten.
:li.List Descendants (Descends.ftx) von :link reftype=hd refid=People.Peter Gervai:elink.
.br
Dieses Skript zeigt die Nachfahren der aktuellen Person.
:li.Export Gedcom (ExGedcom.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript exportiert den aktuellen Baum zum GEDCOM-Format.
GEDCOM ist DER Standard fr Ahnenforschungsdaten und daher weit verbreitet.
:li.Full Dump (FullDump.ftx) von :link reftype=hd refid=People.Peter Gervai:elink.
.br
Dieses Skript gibt alle Informationen des Baumes durch Semikolons getrennt
aus, so daá Sie diese z.B. in eine Tabellenkalkulation bernehmen k”nnen.
:li.Import Gedcom (ImGedcom.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript importiert eine GEDCOM-Datei (siehe oben Export).
:li.Continue Import (ImContue.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript setzt einen Import fort, wenn beim letzten mal Personen (und Familien)
brig geblieben sind. Dies kann dann passieren, wenn importierte Daten keinen
zusammenh„ngenden Baum ergeben. Dann werden Sie gefragt, ob Sie die Personen
behalten m”chten, die nicht mit dem Ursprung verwandt sind. Dieses Skript l„át Sie
in diesem Fall einen separaten Baum erstellen.
:li.Longevity Report (Lngevity.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript zeigt statistische Information ber das Lebensalter der Personen im Baum.
:li.Report of a Person (Person.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript zeigt die Informationen der aktullen Person.
:li.List Weddings (Weddings.ftx) von :link reftype=hd refid=People.Nils Meier:elink.
.br
Dieses Skript zeigt die Hochzeitstage, sortier nach Monat + Tag.
:eol.

.* ------------------- winRexxOutput --------------------------.
:h1 res=&ID_NRexxOutput. id=winRexxOut.Rexxoutput
:i1.Rexxoutput
:p.Die Ausgabe eines Rexx-Skripts wird in diesem Fenster gezeigt.
Sie k”nnen die Operation :hp1.Abbrechen:ehp1., das Fenster durch
:hp1.OK:ehp1. schlieáen und die Ausgabe in eine Datei :hp1.Speichern:ehp1.

.* ------------------- Rexxing --------------------------.
:h1 id=rexxing.Rexx Programmieren
:i1.Rexx Programmieren
:p.FamilyTree kann in seinem Kontext Rexx-Skripte ausfhren,
um verschieden Aufgaben durchzufhren. Ein Skript kann Berichte
erstellen, Daten manipulieren und FamilyTree Informationen
importieren und exportieren.
:p.Sie mssen eine eigene Rexx-Datei (*.ftx) erstellen,
wenn Sie eigene Routinen schreiben m”chten. Der Kopf
des Skripts sollte so aussehen:

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

   Long Name is <
                 English&colon.    Long name in english &colon.English
                 Deutsch&colon.    Long name in german  &colon.Deutsch
                 Nederlands&colon. Long name in dutch   &colon.Nederlands
                 Francais&colon.   Long name in french  &colon.Francais
                >
*/
:exmp.:font facename=default size=0x0.

:p.Der Name des Authors (zwischen <>) und die Berschreibug (zwischen <>)
wird in dem Rexx-Auswahldialog angezeigt.
:p.Ein Rexx-Skript kann die Daten aus dem aktuellen Stammbaum
mit verschiedenen Funktion manipulieren und abfragen. Immer eine Mensch
und eine Familie ist ausgew„hlt. Sie k”nnen entweder die Auswahl eines Menschen
oder einer Familie durch das Bewegen von einem zum anderen „ndern, oder
Daten mit den Datenfunktionen abfragen/„ndern.
Bitte benutzen Sie die vorhandenen Skripte als Beispiel fr ein eigenes
Skript und schicken Sie dem Author eine Kopie Ihres Skriptes, falls es
auch fr andere Benutzer interessant ist.

:p.:hp2.The following functions let you query program settings: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ³ FUNCTION NAME     ³ ACTION                 ³ PARAMETERS                  ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³ getLanguage()     ³ returns language       ³ none                        ³
 ³                   ³    'deutsch'/'english' ³                             ³
 ³ selectFilename()  ³ returns filename       ³ 'Dialogtitle','*.*'         ³
 ³                   ³    '' for cancel       ³                             ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
:ecgraphic. :font facename=default size=0x0.


:p.:hp2.The following functions let you move through persons and families: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ³ FUNCTION NAME     ³ ACTION                 ³ PARAMETERS                  ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³ sortPersons()     ³ sort persons by        ³ 'N|n'ame                    ³
 ³                   ³                        ³ 'F|f'irst name              ³
 ³                   ³                        ³ 'B|b[D|M|Y]'irth date       ³
 ³                   ³                        ³ 'D|d[D|M|Y]'eath date       ³
 ³                   ³                        ³ 'L|l[D|M|Y]'ifetime         ³
 ³                   ³                        ³ 'P|p'ersons ID              ³
 ³                   ³                        ³ - Repeat via ','            ³
 ³                   ³                        ³ - 'A'scending/'d'escending  ³
 ³                   ³                        ³ - [Day,Month,Year]          ³
 ³ sortFamilies()    ³ sort families by       ³   'M|m[D|M|Y]'arriage date  ³
 ³                   ³                        ³ - see above                 ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³ selectPerson()    ³ selects a person       ³ 'F'irst                     ³
 ³                   ³ returns '1' if done    ³ 'L'ast                      ³
 ³                   ³         '0' otherwise  ³ 'P'revious                  ³
 ³                   ³                        ³ 'N'ext                      ³
 ³                   ³                        ³ 'R'emembered                ³
 ³                   ³                        ³ 'O'rigin                    ³
 ³                   ³                        ³*'f'ather of current family  ³
 ³                   ³                        ³*'m'other of current family  ³
 ³                   ³                        ³*'p'artner in current family ³
 ³                   ³                        ³*'1|2|3..' child in family   ³
 ³                   ³                        ³ - * results in error, when  ³
 ³                   ³                        ³ actual family was imported  ³
 ³ selectFamily()    ³ selects a family       ³ 'F'irst                     ³
 ³                   ³ returns '1' if done    ³ 'L'ast                      ³
 ³                   ³         '0' otherwise  ³ 'P'revious                  ³
 ³                   ³                        ³ 'N'ext                      ³
 ³                   ³                        ³*'p'arental fam of person    ³
 ³                   ³                        ³*'1|2|..' fam of act. person ³
 ³                   ³                        ³ - * results in error, when  ³
 ³                   ³                        ³ actual person was imported  ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³ doStack()         ³ stack operation        ³*'P(P|F)'ush on stack        ³
 ³                   ³                        ³ 'p(P|F)'op  from stack      ³
 ³                   ³                        ³ 'D(P|F)'rop from stack      ³
 ³                   ³                        ³*'S(P|F)'wap with stack      ³
 ³                   ³                        ³ - [P]erson|[F]amily         ³
 ³                   ³                        ³ - * results in error, when  ³
 ³                   ³                        ³ you try to push or swap an  ³
 ³                   ³                        ³ imported Person or Family   ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
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
 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ³ FUNCTION NAME     ³ ACTION                 ³ PARAMETERS                  ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³ getPID()          ³ returns ID             ³ none                        ³
 ³ getName()         ³ returns name           ³ none                        ³
 ³ getFirstName()    ³ returns first name     ³ none                        ³
 ³ getSex()          ³ returns sex 0|1|2      ³ none                        ³
 ³ getBirthDate()    ³ returns birth date     ³ none (full)                 ³
 ³                   ³                        ³ |'D'ay                      ³
 ³                   ³                        ³ |'M'onth                    ³
 ³                   ³                        ³ |'m'onth name               ³
 ³                   ³                        ³ |'Y'ear                     ³
 ³                   ³                        ³ |'d'ays (since 00.00.0000)  ³
 ³                   ³                        ³ |'c'omplete ? (1/0)         ³
 ³                   ³                        ³ | m'O'difier (see setDate)  ³
 ³ getBirthPlace()   ³ returns birth place    ³ none                        ³
 ³ getDeathDate()    ³ returns death date     ³ none (full date)            ³
 ³                   ³                        ³ |'D'ay                      ³
 ³                   ³                        ³ |'M'onth                    ³
 ³                   ³                        ³ |'m'onth name               ³
 ³                   ³                        ³ |'Y'ear                     ³
 ³                   ³                        ³ |'d'ays (since 00.00.0000)  ³
 ³                   ³                        ³ |'c'omplete ? (1/0)         ³
 ³                   ³                        ³ | m'O'difier (see setDate)  ³
 ³ getDeathPlace()   ³ returns death place    ³ none                        ³
 ³ getOccupation()   ³ returns occupation     ³ none                        ³
 ³ getAddress()      ³ returns address        ³ none                        ³
 ³ getMemo()         ³ ret. memo or '' if n/a ³ none (all)                  ³
 ³                   ³                        ³ |line '1','2','3',...       ³
 ³ getPicture()      ³ returns picture path   ³ none                        ³
 ³ getFile()         ³ ret. file attachment   ³ '1','2','3',...             ³
 ³ getUserField()    ³ ret. user field        ³ '1','2','3',...             ³
 ³ getUserFieldName()³ ret. user field name   ³ '1','2','3',...             ³
 ³ setPID()          ³ sets ID                ³ 'id'                        ³
 ³                   ³                        ³ - results in error, when    ³
 ³                   ³                        ³ actual is not an imported   ³
 ³                   ³                        ³ or id=0 or id is not num    ³
 ³ setName()         ³ sets name              ³ 'name'                      ³
 ³ setFirstName()    ³ sets first name        ³ 'first name'                ³
 ³ setSex()          ³ sets sex               ³ '0'None|'1'Male|'2'Female   ³
 ³ setBirthDate()    ³ sets birth date        ³ '[m,]dd.mm.yyyy'            ³
 ³                   ³                        ³ Modifier m=0 None           ³
 ³                   ³                        ³          m=1 About          ³
 ³                   ³                        ³          m=2 After          ³
 ³                   ³                        ³          m=3 Before         ³
 ³                   ³                        ³          m=4 Estimated      ³
 ³ setBirthPlace()   ³ sets birth place       ³ 'place'                     ³
 ³ setDeathDate()    ³ sets death date        ³ '[m,]dd.mm.yyyy'            ³
 ³                   ³                        ³ Modifier see above          ³
 ³ setDeathPlace()   ³ sets death place       ³ 'place'                     ³
 ³ setOccupation()   ³ sets occupation        ³ 'occupation'                ³
 ³ setAddress()      ³ sets address           ³ 'address'                   ³
 ³ setMemo()         ³ sets memo              ³ 'memo'                      ³
 ³ setPicture()      ³ sets picture path      ³ 'path'                      ³
 ³ addFile()         ³ adds file attachment   ³ 'file'                      ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
:ecgraphic. :font facename=default size=0x0.
:p.Example: Get name of actual person
:xmp. name=getName() :exmp.
:p.Example: Get birth month of actual person
:xmp. date=getBirthDate('M') :exmp.
:p.Example: Get 2nd line of the memo-field of the actual person
:xmp. memo=getMemo('2') :exmp.

:p.:hp2.The following functions refer to the data of the actual family: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ³ FUNCTION NAME     ³ ACTION                 ³ PARAMETERS                  ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³ getFID()          ³ returns ID             ³ none                        ³
 ³ getMarriageDate() ³ returns marriage date  ³ none (full date)            ³
 ³                   ³                        ³ |'D'ay                      ³
 ³                   ³                        ³ |'M'onth                    ³
 ³                   ³                        ³ |'m'onth name               ³
 ³                   ³                        ³ |'Y'ear                     ³
 ³                   ³                        ³ |'d'ays (since 00.00.0000)  ³
 ³                   ³                        ³ |'c'omplete ? (1/0)         ³
 ³                   ³                        ³ | m'O'difier (see setDate)  ³
 ³ getMarriagePlace()³ returns marriage place ³ none                        ³
 ³ getDivorceDate()  ³ returns divorce date   ³ none (full date)            ³
 ³                   ³                        ³ |'D'ay                      ³
 ³                   ³                        ³ |'M'onth                    ³
 ³                   ³                        ³ |'m'onth name               ³
 ³                   ³                        ³ |'Y'ear                     ³
 ³                   ³                        ³ |'d'ays (since 00.00.0000)  ³
 ³                   ³                        ³ |'c'omplete ? (1/0)         ³
 ³                   ³                        ³ | m'O'difier (see setDate)  ³
 ³ setFID()          ³ sets ID                ³ 'id'                        ³
 ³                   ³                        ³ - results in error, when    ³
 ³                   ³                        ³ actual is not an imported   ³
 ³                   ³                        ³ or id=0 or id is not num    ³
 ³ setMarriageDate() ³ sets marriage date     ³ '[m],dd.mm.yyyy'            ³
 ³                   ³                        ³ Modifier m=0 None           ³
 ³                   ³                        ³          m=1 About          ³
 ³                   ³                        ³          m=2 After          ³
 ³                   ³                        ³          m=3 Before         ³
 ³                   ³                        ³          m=4 Estimated      ³
 ³ setMarriagePlace()³ sets marriage place    ³ 'place'                     ³
 ³ setDivorceDate()  ³ sets divorce date      ³ '[m,]dd.mm.yyyy.'           ³
 ³                   ³                        ³ Modifier see above          ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
:ecgraphic. :font facename=default size=0x0.
:p.Example: Get divorce date of current family
:xmp. date=getDivorceDate() :exmp.
:p.Example: Get marriage year of current family
:xmp. date=getMarriageDate('Y') :exmp.

:p.:hp2.The following functions let you import new persons and families: :ehp2.
:font facename='System VIO' size=9x6. :cgraphic.
 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ³ FUNCTION NAME     ³ ACTION                 ³ PARAMETERS                  ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³ importPerson()    ³ create a new Person    ³ none                        ³
 ³ importFamily()    ³ create a new Family    ³ none                        ³
 ³ importAddPartner()³ add ID of Partner to   ³ 'id' of one Partner of the  ³
 ³                   ³ imported family        ³ last imported! Family       ³
 ³ importAddChild()  ³ add ID of Child to     ³ 'id' of one Child of the    ³
 ³                   ³ imported family        ³ last imported! Family       ³
 ³ importDone()      ³ trigger import routine ³ none                        ³
 ³                   ³ tree is resetted and   ³ objects with lost ID's and  ³
 ³                   ³ as much information as ³ Families that build loops   ³
 ³                   ³ possible imported      ³ are not imported            ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
:ecgraphic. :font facename=default size=0x0.
:p.Example: Import a Person
:xmp. ok=importPerson() :exmp.
:p.Example: Set ID of one Partners of the last imported family to 3
:xmp. ok=importAddPartner(3) :exmp.

.* ----------------- FileMenu -------------------------------------.
:h1 id=menuFile.Dateimen
:i1 id=menuFile.Dateimen
:p.Das Dateimen erm”glicht es, folgende Aktionen durchzufhren. Einen Stammbaum
:ol compact.
:li.:link reftype=hd refid=treeNew.neu anlegen:elink.,
:li.:link reftype=hd refid=treeLoad.einladen:elink.,
:li.:link reftype=hd refid=treeSave.abspeichern:elink.,
:li.:link reftype=hd refid=treeSaveAs.mit Namen abspeichern:elink.,
:li.:link reftype=hd refid=treePrint.drucken:elink.,
:li.:link reftype=hd refid=treeClose.schlieáen:elink. und das Programm beenden.
:eol.

:h2 res=&MI_FileNew. id=treeNew.Neuer Stammbaum
:i2 refid=menuFile.Neu
:p.Mit :hp1.Daten|Neu:ehp1. kann man mit einen neuen Stammbaum anfangen.

:h2 res=&MI_FileOpen. id=treeLoad.Stammbaum laden
:i2 refid=menuFile.Laden
:p.Mit :hp1.Daten|Laden:ehp1. kann man einen bestehenden Stammbaum einladen.

:h2 res=&MI_FileSave. id=treeSave.Stammbaum speichern
:i2 refid=menuFile.Speichern
:p.Mit :hp1.Daten|Speichern:ehp1. kann man einen bearbeiteten Stammbaum abspeichern,
der vorher eingeladen oder mit :link reftype=hd refid=treeSaveAs.Speichern unter:elink.
abgespeichert worden ist.

:h2 res=&MI_FileSaveAs. id=treeSaveAs.Stammbaum speichern unter
:i2 refid=menuFile.Speichern unter
:p.Mit :hp1.Daten|Speichern Unter:ehp1. kann man einen bearbeiteten Stammbaum abspeichern.

:h2 res=&MI_FileClose. id=treeClose.Stammbaum schlieáen
:i2 refid=menuFile.Schlieáen
:p.Mit :hp1.Daten|Schlieáen:ehp1. wird der Stammbaum geschlossen und das Programm
beendet.

:h2 res=&MI_FilePrint. id=treePrint.Stammbaum drucken
:i2 refid=menuPrint.Drucken
:p.Mit :hp1.Daten|Drucken:ehp1. wird der Stammbaum gedruckt.

.* ----------------- menuMankind -------------------------------------.
:h1 id=menuMankind.Ansichtmen
:i1 id=menuMankind.Ansichtme
:p.Das :hp1.Ansichtmen:ehp1. wird entweder ber das Hauptmen am oberen Rand
des Hauptfensters oder per rechtem Mausklick auf eine freie Stelle im Stammbaum
aktiviert.
:artwork name='menuMankind.bmp'.
Folgende Optionen stehen zur Verfgung:
:ol compact.
:li.:link reftype=hd refid=menuMankindAncestors.Vorfahren:elink.,
:li.:link reftype=hd refid=menuMankindDescendants.Nachfahren:elink.,
:li.:link reftype=hd refid=menuMankindSiblings.Geschwister:elink.
:lp.
:li.:link reftype=hd refid=menuMankindZoomIn.Vergr”áern:elink.
:li.:link reftype=hd refid=menuMankindZoomOut.Verkleinern:elink.
:lp.
:li.:link reftype=hd refid=menuMankindVert.Vertikal:elink.
:li.:link reftype=hd refid=menuMankindHori.Horizontal:elink.
:lp.
:li.:link reftype=hd refid=menuMankindList.Liste:elink.
:eol.

:h2 res=&MI_MankindAncestors. id=menuMankindAncestors.Vorfahren
:i2 refid=menuMankind.Vorfahren
:p.Durch die Wahl von :hp1.Ansicht|Vorfahren:ehp1. erscheint ein
Eingabefenster, in dem Sie angeben k”nnen, wieviele Generationen
von Vorfahren des "Ursprungs" sichtbar sein sollen. Wenn Sie also
keine Vorfahren sehen m”chten, geben Sie einfach eine 0 ein. Wenn
Sie das Eingabefeld leer lassen, sehen Sie alle Vorfahren.

:h2 res=&MI_MankindDescendants. id=menuMankindDescendants.Nachfahren
:i2 refid=menuMankind.Nachfahren
:p.Durch die Wahl von :hp1.Ansicht|Nachfahren:ehp1. erscheint ein
Eingabefenster, in dem Sie angeben k”nnen, wieviele Generationen
von Nachfahren des "Ursprungs" sichtbar sein sollen. Wenn Sie also
keine Nachfahren sehen m”chten, geben Sie einfach eine 0 ein. Wenn
Sie das Eingabefeld leer lassen, sehen Sie alle Nachfahren.

:h2 res=&MI_MankindSiblings. id=menuMankindSiblings.Geschwister
:i2 refid=menuMankind.Geschwister
:p.Durch die Wahl von :hp1.Ansicht|Geschwister:ehp1. werden alle Geschwister
und deren Nachfahren des "Ursprungs" gezeigt (wenn die Eltern sichtbar sind).
Durch eine zweite Auswahl, werden wieder nur die Nachfahren des "Ursprungs"
gezeigt.

:h2 res=&MI_MankindZoomIn. id=menuMankindZoomIn.Vergr”áern
:i2 refid=menuMankind.Vergr”áern
:p.Mit der Auswahl :hp1.Ansicht|Vergr”áern:ehp1., k”nnen
Sie die Zoom-Rate bis zum Maximum von 1 vergr”áern.

:h2 res=&MI_MankindZoomOut. id=menuMankindZoomOut.Verkleinern
:i2 refid=menuMankind.Verkleinern
:p.Mit der Auswahl :hp1.View|Verkleinern:ehp1., k”nnen
Sie die Zoom-Rate bis zum Minimum von 0.1 verkleinern.

:h2 res=&MI_MankindVert. id=menuMankindVert.Vertikal
:i2 refid=menuMankind.Vertikal
:p.Durch die Wahl von :hp1.Ansicht|Vertikal:ehp1. wird der Stammbaum senkrecht
angezeigt.

:h2 res=&MI_MankindHori. id=menuMankindHori.Horizontal
:i2 refid=menuMankind.Horizontal
:p.Durch die Wahl von :hp1.Ansicht|Horizontal:ehp1. wird der Stammbaum quer
angezeigt.

:h2 res=&MI_MankindList. id=menuMankindList.Liste
:i2 refid=menuMankind.Liste
:p.Durch die Wahl von :hp1.Ansicht|Liste:ehp1. wird ein
:link reftype=hd refid=winList.Listenfenster:elink. mit
den alphabetisch sortierten Namen aller Menschen im Stammbaum gezeigt.
:p.Ist das Listenfenster schon ge”ffnet, dann erscheint der Menpunkt abgehakt
und eine erneute Auswahl schlieát das Fenster.


.* ----------------- Man -----------------------------------------.
:h1 res=&MI_ManDel. id=menuMan.Mensch
:i1 id=menuMan.Mensch
:p.Das Kontextmen eines Menschen erlaubt folgende Aktionen
(™ffnen Sie es per rechtem Mausklick auf einen Menschen im Stammbaum):
:artwork name='menuMan.bmp'.
:ol compact.
:li.:link reftype=hd refid=menuManCenter.Zentrieren:elink.,
:li.:link reftype=hd refid=menuManEdit.Editieren:elink.,
:li.:link reftype=hd refid=menuManOrigin."Ursprung" bestimmen:elink.
:lp.
:li.:link reftype=hd refid=menuManDelPartner.L”schen des Partners:elink.
:li.:link reftype=hd refid=menuManDelChilds.L”schen der Kinder:elink.
:li.:link reftype=hd refid=menuManDelParents.L”schen der Eltern:elink.
:li.:link reftype=hd refid=menuManDelThis.L”schen des "Aktuellen":elink.
:lp.
:li.:link reftype=hd refid=menuManChild.Kind hinzufgen:elink.
:li.:link reftype=hd refid=menuManParents.Eltern hinzufgen:elink.
:li.:link reftype=hd refid=menuManPartner.Partner hinzufgen:elink.
:eol.

:h2 res=&MI_ManCenter. id=menuManCenter.Zentrieren
:i2 refid=menuMan.Zentrieren
:p.Durch die Wahl von :hp1.Zentrieren:ehp1. wird der Stammbaum so im Hauptfenster
positioniert, daá der "Aktuelle" zentriert erscheint
(Noch nicht implementiert).

:h2 res=&MI_ManEdit. id=menuManEdit.Editieren
:i2 refid=menuMan.Editieren
:p.Durch die Wahl von :hp1.Editieren:ehp1. wird ein
:link reftype=hd refid=winEdit.Editierfenster:elink.
ge”ffnet, zum ndern der Eigenschaften eines Menschen.
:p.Ist das Editierfenster schon ge”ffnet, dann erscheint der Menpunkt abgehakt
und eine erneute Auswahl schlieát es.

:h2 res=&MI_ManOrigin. id=menuManOrigin.Ursprung
:i2 refid=menuMan.Ursprung
:p.Durch die Wahl von :hp1.Urspung:ehp1. wird der "Aktuelle"
zum "Ursprung" des Stammbaumes.

:h2 res=&MI_ManDelPartner. id=menuManDelPartner.Partner l”schen
:i2 refid=menuMan.Partner l”schen
:p.Durch die Wahl von :hp1.L”schen|Partner:ehp1. wird der aus dem
Untermen ausgew„hlt Partner und die mit Ihm gezeugten Kinder gel”scht.

:h2 res=&MI_ManDelChilds. id=menuManDelChilds.Kinder l”schen
:i2 refid=menuMan.Kinder l”schen
:p.Durch die Wahl von :hp1.L”schen|Kinder mit:ehp1. werden die Kinder
des "Aktuellen" gel”scht. Wenn der "Aktuelle" mehrere Partner hat muá
der Partner ausgew„hlt werden, dessen gemeinsame Kinder gel”scht werden
sollen.
:warning text='Vorsicht:'.
Alle Kinder, deren Nachfahren und Vorfahren (ohne den "Aktuellen")
werden gel”scht !
:ewarning.

:h2 res=&MI_ManDelParents. id=menuManDelParents.Eltern l”schen
:i2 refid=menuMan.Eltern l”schen
:p.Durch die Wahl von :hp1.L”schen|Eltern:ehp1. werden die Eltern
des "Aktuellen" gel”scht.
:warning text='Vorsicht:'.
Die Eltern, deren Vorfahren und Nachfahren (auáer dem
"Aktuellen") werden gel”scht !
:ewarning.

:h2 res=&MI_ManDelThis. id=menuManDelThis.Person l”schen
:i2 refid=menuMan.Person l”schen
:p.Durch die Wahl von :hp1.L”schen|Diesen:ehp1. wird der "Aktuelle" gel”scht.
:p.Alle Personen, die nach dem L”schen des "Aktuellen" nicht mehr mit dem
"Ursprung" verwandt sind, werden auch gel”scht.

:h2 res=&MI_ManChild. id=menuManChild.Kind hinzufgen
:i2 refid=menuMan.Kind hinzufgen
:p.Durch die Wahl von :hp1.Neues Kind mit:ehp1. wird zum "Aktuellen"
ein Kind hinzugefgt. Wenn der "Aktuelle" mehrere Partner hat, muá der
Partner ausgew„hlt werden, zu dem das Kind geh”rt.
Standardm„áig wird bei Auswahl ein
:link reftype=hd refid=menuManChildNew.neues Kind erzeugt:elink.,
Sie k”nnen aber auch eine
:link reftype=hd refid=menuManChildRemembered.gemerkte Person als Kind:elink.
hinzufgen.
:p.Der Menpunk is nicht verfgbar, wenn der "Aktuelle" keinen Partner
oder die maximal erlaubte Zahl an Kinder hat.

:h2 res=&MI_ManChildNew. id=menuManChildNew.Kind hinzufgen (Neu)
:i2 refid=menuMan.Kind hinzufgen (Neu)
:p.Durch die Wahl von :hp1.Neues Kind mit|Partner|Neu:ehp1. wird ein neues Kind
erzeugt. Sie k”nnen auch eine
:link reftype=hd refid=menuManChildRemembered.gemerkte Person als Kind:elink.
hinzufgen.

:h2 res=&MI_ManChildRemembered. id=menuManChildRemembered.Kind hinzufgen (Gemerkter)
:i2 refid=menuMan.Kind hinzufgen (Gemerkter)
:p.Durch die Wahl von :hp1.Neues Kind mit|Partner|Gemerkter:ehp1. wird die
:link reftype=hd refid=menuManRemember.gemerkte:elink. Person als Kind hinzugefgt.
Sie k”nnen auch ein :link reftype=hd refid=menuManChildNew.neues Kind erzeugen:elink..

:h2 res=&MI_ManParents. id=menuManParents.Eltern hinzufgen
:i2 refid=menuMan.Eltern hinzufgen
:p.Durch die Wahl von :hp1.Neue Eltern:ehp1. werden zum "Aktuellen"
Eltern hinzugefgt.
:p.Der Menpunkt is nicht verfgbar, wenn der "Aktuelle" schon Eltern hat.

:h2 res=&MI_ManPartner. id=menuManPartner.Partner hinzufgen
:i2 refid=menuMan.Partner hinzufgen
:p.Durch die Wahl von :hp1.Neuer Partner:ehp1. wird dem "Aktuellen"
ein Partner hinzugefgt.
:p.Dieser Menpunkt ist ein Konditionalmen. Wenn Sie vorher einen Menschen
:link reftype=hd refid=menuManRemember.gemerkt:elink. haben, k”nnen Sie dem
"Aktuellen" einen neuen Partner oder die gemerkte Person hinzufgen.

:h2 res=&MI_ManPartnerNew. id=menuManPartnerNew.Partner hinzufgen (Neu)
:i2 refid=menuMan.Partner hinzufgen (Neu)
:p.Durch die Wahl von :hp1.Neuer Partner|Neu:ehp1. bekommt der "Aktuelle"
einen neuen Partner.
Sie k”nnen auch einen
:link reftype=hd refid=menuManPartnerRemembered.gemerkten Partner hinzufgen:elink..

:h2 res=&MI_ManPartnerRemembered. id=menuManPartnerRemembered.Partner hinzufgen (Gemerkter)
:i2 refid=menuMan.Partner hinzufgen (Gemerkter)
:p.Wenn Sie :hp1.Neuer Partner|Gemerkter:ehp1. ausw„hlen, wird dem "Aktuellen" die
vorher :link reftype=hd refid=menuManRemember.gemerkte:elink. Person hinzugefgt.
Sie k”nnen ihm auch
:link reftype=hd refid=menuManPartnerNew.einen neuen Partner geben:elink..
:p.Sie k”nnen nur eine gemerkte Person hinzufgen, wenn Sie nicht schon
mit dem "Aktuellen" verheiratet ist und der "Aktuelle" nicht die
gemerkte Person ist.

:h2 res=&MI_ManRemember. id=menuManRemember.Merken
:i2 refid=menuMan.Merken
:p.Durch die Auswahl von :hp1.Merken:ehp1., wird der "Aktuelle" gemerkt, so
daá Sie ihn einer anderen Person als Partner hinzufgen k”nnen, die schon
im Baum ist. Auáerdem k”nnen Sie diese gemerkter Person einer Familie als
Kind zuordnen.

.* ----------------- menuOptions -------------------------------------.
:h1 id=menuOptions.Optionsmen
:i1 id=menuOptions.Optionsmen
:p.Das :hp1.Optionsmen:ehp1. liegt im Hauptmen am oberen
Rand des Hauptfensters. Wenn Sie eines seiner Menpunkte ausw„hlen,
k”nnen Sie direkt zu eine Optionenseite des
:link reftype=hd refid=winOptions.Optionsdialog:elink.
springen.

Die folgenden Optionsseiten sind vorhanden:
:ol compact.
:li.:link reftype=hd refid=menuOptionsLayout.Layout:elink.,
:li.:link reftype=hd refid=menuOptionsDataAssistant.Datenassistent:elink.,
:li.:link reftype=hd refid=menuOptionsMiscellaneous.Verschiedenes:elink.
:li.:link reftype=hd refid=menuOptionsUserFields.Benutzerfelder:elink.
:eol.

:h2 res=&MI_OptionsLayout. id=menuOptionsLayout.Layout
:i2 refid=menuOptions.Layout
:p.Durch die Wahl von :hp1.Optionen|Layout:ehp1. springen
Sie direkt zur Layout-Seite des
:link reftype=hd refid=winOptions.Optionsdialog:elink..

:h2 res=&MI_OptionsDataAssistant. id=menuOptionsDataAssistant.Datenassistent
:i2 refid=menuOptions.Datenassistent
:p.Durch die Wahl von :hp1.Optionen|Datenassistent:ehp1. springen
Sie direkt zur Seite des :link reftype=hd refid=winOptions.Optionsdialog:elink.,
auf der Sie Datenassistentenparameter „ndern k”nnen.

:h2 res=&MI_OptionsMiscellaneous. id=menuOptionsMiscellaneous.Verschiedenes
:i2 refid=menuOptions.Verschiedenes
:p.Durch die Wahl von :hp1.Optionen|Verschiedenes:ehp1. springen
Sie direkt zur Seite des :link reftype=hd refid=winOptions.Optionsdialog:elink.,
auf der Sie verschiedene Einstellungen „ndern k”nnen.

:h2 res=&MI_OptionsUserFields. id=menuOptionsUserFields.Benutzerfelder
:i2 refid=menuOptions.Benutzerfelder
:p.Durch die Wahl von :hp1.Optionen|Benutzerfelder:ehp1. springen
Sie direkt zur Seite des :link reftype=hd refid=winOptions.Optionsdialog:elink.,
auf der Sie Benutzerfelder definieren k”nnen.

.* ----------------- menuRexx -------------------------------------.
:h1 res=&MI_RexxStart. id=menuRexxStart.Rexx starten
:i1 id=menuRexxStart.Rexx starten
:p.Sie k”nnen ein Rexx-Skript starten, indem Sie :hp1.Rexx|Starten:ehp1.
aus dem Hauptmen am oberen Rand des Hauptfensters ausw„hlen.
Es erscheint dann der
:link reftype=hd refid=winRexx.Rexx Dialog:elink.
zur n„heren Auswahl.
:p.Gemerkte Skripte k”nnen ohne Umweg direkt ausgew„hlt werden.

.* ----------------- People ---------------------------------------.
:h1 id=people.Team FamilyTree
:i1.Team

Diese Personen haben etwas mit FamilyTree zu tun:

:sl.
:li.:lines.
:hp1.Verantworlich, das Ganze angefangen zu haben:ehp1.
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
:hp1.Holl„ndische šbersetzung:ehp1.
Tjalco P. van der Mey (T.P.vd.Mey@net.hcc.nl)
Burg. Bloemersstraat 8
7271 DC Borculo
The Netherlands
Tel +31-545-273522
Fax +31-545-275108
:elines.
:li.:lines.
:hp1.Franz”sische šbersetzung:ehp1.
Francois Massonneau (island@worldnet.fr)
Operator of Island's BBS (use "FTREE" to get the latest version there):
Node 1 : +33-(0)251.397.478
Node 2 : +33-(0)251.395.251
Fidonet 2:326/2.0
V34+ (Up to 33600 bauds).
:elines.
:esl.


.* ----------------- Registration ---------------------------------.
:h1 res=&ID_NAboutDialog. id=register.Registrierung
:i1 id=register.Registrierung
:p.Family Tree ist :hp1.Shareware:ehp1.. Sie drfen dieses Programm x Tage lang
testen und benutzen (:color fc=red.x h„ngt von Ihnen ab:color fc=default.).
Wenn sie den Autor in seinen Bemhungen untersttzen wollen, Family Tree
weiterzuentwickeln, registrieren Sie bitte Family Tree.

:p.Die Registriergebhr betr„gt 20 DM (US $ 15).
Sie bekommen einen Registriercode, den Sie in den Informationsdialog eintragen
mssen (erreichbar ber den Menpunkt :hp1.Hilfe|šber:ehp1.).
Die Registrierung ist gltig fr alle Versionen 1.x und 2.x.
Sie erhalten auáerdem eine Nachricht, wenn eine neue Version von Family Tree
erscheint.
:p.Addieren Sie bitte 5 DM (US $ 5), wenn Sie die aktuelle Version auf Disk
zugeschickt bekommen wollen. Bitte teilen Sie mir mit, welches Format Sie
wnschen (3 1/2 oder 5 1/4).

:p.So k”nnen Sie bezahlen :
:ol compact.
:li.šberweisung an
.br
    Nils Meier
.br
    Kontonummer 0516 075 130
.br
    Allgemeine Deutsche Direktbank AG
.br
    Bankleitzahl 500 105 17
.br
Bitte geben Sie "FTree Registrierung" und Ihren Namen auf der šberweisung an.
:li.Bar per Post an :link reftype=hd refid=People.Nils Meier:elink.
:li.Eurocheque per Post, zahlbar an :link reftype=hd refid=People.Nils Meier:elink.
:li.Registrierung durch BMT Micro in Wilmington, NC 28408, USA
.br
    Bitte entnehmen Sie der Textdatei Order.BMT die n”tigen Informationen.
.br
    (Registriergebhr betr„gt US $ 20)
:eol.

:p.Bitte senden Sie per Post oder E-Mail die folgende Informationen :
:ol compact.
:li.Name
:li.Adresse
:li.E-Mail Adresse
:li.Zahlungsweg
:ul compact.
:li.šberweisung
:li.Scheck (mit der Post)
:li.Bar (mit der Post)
:eul.
:li.Wohin der Registriercode geschickt werden soll
:ul compact.
:li.per Post an obige Adresse
:li.per E-Mail an obige Adresse
:eul.
:eol.

:p.:font facename=Helv size=24x24.Danke fr's Registrieren !:font facename=default size=0x0.

:euserdoc.
