# Entente Arduino - LaZor Computer


##  Einleitung


## Material und Werkzeug
#### Material

Allgemein
* 2x Arduino Uno 
* Diverse Kabel
  * 1 USB (Typ A auf B)
  * 10m Kabel (Litzen) bunt

LaZer Computer
* Holzbrett (120cmx60cmx0,4cm)
* Plexiglas (60cmx60cmx0,4cm)
* PC Bauteile (Netzteil, Lüfter, RAM, Festplatte, CPU, Grafikkarte)
* 2x Acrylglasspiegel (10cmx15cm)
* Acrylstange für Linsen (z.B. Gardinenstangen)
* Joystick
* Muttern
* 2x Gewindestangen
* Schrauben
* Arduino Uno
* 7x Led
* Widerstände
* 5x Photowiderstand (~630nm)
* 6x Lasermodul (5mW 630nm)
* Relais

Snake
* LED-Matrix (2x)
* Joystick ( 4 oder 8 Richtungen und 1 Select-Button )
* 2x neoPixel Matrix
* 1x Thumb-Joystick
* 1x Bread-Board
* 1x femail to femail wire to connect the two matrix
* 7x male to male wire for the matrix and thumb-joystick

 
#### Werkzeuge
* Akkubohrmaschine
* Lochsäge
* Säge (Metall und Holz)
* Lötkolben
* Hammer
* Schraubenzieher
* Flachzange
* Spitzzange
* Crimpzange
* Schmirgelpapier
* Kleber
* Lasercutter (optional)

---

## Vorbereitung
* PC zerlegen und Bauteile besorgen
* Weg und Lösung für Laser überlegen und skizzieren
* Programmierkenntnisse in C
  * Grundkenntnisse erforderlich
  * Bibliotheken, Guides und Beispiele (Sketchbook) in Arduino IDE


## Step-by-Step Guide
###Schritt 1
####Schritt 1.1 - Bibliotheken zum Schreiben der Programme für den Arduino

Programm SNAKE
* Adafruit_GFX
  *	Die Kern Grafik Bibliothek für alle Anzeigen. Stellt basis Grafik-Elemente (points, lines, circles)
* Adafruit_NeoMatrix
  * Steuert Controls NeoPixel Anzeigen. Benötigt Adafruit_NeoPixel und Adafruit_GFX libraries.	
* Adafruit_NeoPixel
* JoystickLib
  * Stellt API bereit zum lesen eines oder mehrerer Joysticks die an einem Arduino angeschlossen sind.
* NewPing

Programm Laz-Photodioden-LED Steuerung

####Schritt 1.2 - Bauen des Schaukastens und Bauelemente bearbeiten
#####Rückwand
Die Rückwand wird je nach Bedarf zurecht geschnitten. Wir empfehlen eine quadratische Form von mindestens 120x120 cm. Die stärke des Holzes sollte 4 mm nicht unterschreiten. Wir hatten schichtverleihmtes Pappel-Holz verwendet, wobei Fichte oder härters Holz zu empfehlen ist. Da es der Stabiliät der Konstruktion äußerst zuträglich ist, da an der Rückwand alle Computerbauteile befestigt werden. Aufgrund dieser Materialart haben wir uns dazu entschlossen es mit einem Teppischmesser statt einer Säge zu zerschneiden. Hierbei empfiehlt es sich das Holz ein zu spannen und ein stabiles Stück Holz entlang der Trennmarkieren zu fixieren. Das Stück Holz sichert den Schnittverlauf des Teppischmessers und dient zusätzlich als Orientierungshilfe. Da Teppischmesser bzw. der Benutzer schneidet das Holz dann in mehreren Zügen.

#####Löcher bohren
Die Rückwand auf eine feste Unterlage, am besten ein entbehrliches Holzbrett, legen. Den Weg des Lasers, also die Positionen von Lasermodulen, Fotowiderständen und LEDs, sowie die Griffe und zugehörigen PC Bauteile auf das Holzbrett skizzieren. Die PC Bauteile werden zusätzlich auf Schrauben gestellt, damit sie erhöht sind und der Laser darunter durch leuchten kann. Dann die passenden Holzbohrer ( 5er und 7er) verwenden. Wir hatten eine Nummer kleiner verwenden, da nachträgliches Vergrößern der Bohrung ohne Probleme möglich ist. Eine Akku-Bohrmaschine mit den Bohrern bestücken und die Löcher bohren.
Die Löcher für die Griffe anschließend auf die Plexiglasscheibe abpausen. Zum Bohren der Löcher sollte man eine Ständerbohrmaschiene verwenden, wobei dies auch per Hand geht. Es ist nur darauf zu achten, dass dabei kein zusätzlicher Druck auf die Plexiglasscheibe ausgeübt wird. Auch sollte man mit hoher Drehzahl arbeitet und in kurzen Intervallen. Damit die Scheibe nicht anfängt an der Bohrstelle  zu schmelzen bzw. zu weich wird. Da das Bohrloch sonst zu groß bzw. unförmig werden könnte. 
Das Loch für den Kabelbaum und den Lüfter sollte mit einer Lochsäge angefertigt werden.

#####Seitenwände
Für die 4 Seiten gilt das gleiche wie für die Rückwand, wobei man darauf achten sollte, dass zwei Seiten, die gegenüberliegen, etwas kürzer sein sollten. Die Stärke des Holzes sollte abgezogen werden, damit kein Brett übersteht und der Kasten auch wirklich quadratisch ist. Anschließend sollten alle Teile zusammengesetzt und verleimt werden bevor man weiter macht. 

#####Verleimen
Hierbei die Kontaktfächen dünn mit Holzleim bestreichen und kurz (~1min) antrocknen lassen und zusammenfügen. Die Kontaktflächen können zuvor etwas angeraut werden, damit die Oberfläche vergrößert und die Klebewirkung verstärkt wird. Unter leichtem Druck je Seite 5 min. halten. Am besten alles über Nacht stehen lassen. 
Je nach Dicke des Holzes können auch Schrauben oder Nägel verwendet werden. Bei Schrauben sollte man auf jedenfall vorbohren. Hierbei sollte der Bohrer die Hälfte des Durchmessers der Schraube haben.

#####Griffe erstellen
Die Gewindestange in gleich lange Teile sägen. Die Länge richtet sich nach der Höhe der Seiten plus der Höhe der Griffe und Muttern. Die Enden mit sollte man mit Metallschleifpapier abschleifen, da beim Sägen scharfe Kanten entstehen.
Die Spiegel sollten in kleine Rechteck von 2x2cm geschnitten werden. 
Zuvor auf der Rückseite anzeichnen und mit einem Teppichmesser durchtrennen. Es werden pro Halterung bzw. Gewindestange 3 Spiegel benötigt. Bei 5 Griffen sind es 15 Spiegel.
Es werden pro Stange 8x M5 Muttern benötigt. Diese werden gekontert. Das heißt, dass je zwei Muttern gegenläufig fest gedreht werden. Dies verhindert, dass sich diese beim Drehen der Griffe lösen können. 
Die Halterung für die Spiegel sind 5 gleichseitige Dreiecke je 5 mm die miteinander verleimt werden. Diese haben in der Mitte eine Bohrung für die Gewindestange. Mit Hilfe eines Lasercutters können diese Dreiecke passgenau ausgeschnitten werden.

Als Ergebnis sollte man nun folgendes haben.
Die Gewindestange von Unten nach Oben betrachtet.
* Zwei Muttern
* Rückwand
* Zwei Muttern
* Halterung plus Spiegeln
* Zwei Muttern
* Mutter
* Plexiglasscheibe
* Eine Mutter
* Griff und eingelassene Mutter

#####Linsen sägen
Damit der Lserstrahl auf dem Boden sichtbar wird, muss man ihn auffächern. Dies geht am besten mit einer durchsichtigen Acrylstange, wie z.B. eine Gardinenstange. Diese wird in kleine, ca. 1cm lange, Teile gesägt. Beim Sägen muss darauf geachtet werden, dass man nicht zu viel Druck ausübt und nicht zu schnell sägt, da der Kunststoff sonst wieder zusammenklebt.

#####Holz lackiert
Am besten weiß lackieren und keine glänzende Farbe verwenden.	Auch sollte versucht werden eine raue Oberfläche zu erzeugen. 
Dies macht den Strahl des Lasers besser durch die Streuung. Wir habe Kreidespray verwendet, was aber verhindert das man weiter Farbe für zum Beispiel Markierungen aufbreigen kann.
Die Griffe kann man nach belieben farblich gestalten. 

#####Joystick Box erstellen
Mithilfe der Webseite makercase.com ein Box für den Joystick erstellen, sägen und kleben. Der Joystick sollte zwei analoge Anschlüsse für die X und Y Axen, 5V und Ground haben. 

#####Snake Display
Für das LED-Pixel Display für das Spiel "Snake" sollte noch eine weiß transparente Plexiglasplatte zugeschnitten werden, damit man nicht direkt in die hellen LEDs schaut.

###Schritt 2 - Verlötung der Schnittstellen
####LEDs, Laser und Photowiderstände
Für den Lazer Computer benötigt man 6 LEDs, 6 Laser und 6 Photowiderstände, wovon jeweils einer als Ersatzteil dient. 
Für die LEDs ist jeweils noch mal 1 Widerstand notwendig.Die einzelnen Teile müssen alle jeweils mit Litzekabeln verlängert werden. Je Teil benötigt man zwei Litzekabel (Plus und Minus). 30 cm Kabellänge sind ausreichend.
(=> (6+6+6)*2 = 32 Kabel werden benötigt)
Jedes Kabel muss an beiden Seiten abisoliert werden. Ein Ende wird an Plus und eines an Minus gelötet. Dabei ist es ratsam das Litzekabelende vorher zu verzwirbeln und anschließend, um den Plus Anschlussdraht des Teils zu wickeln.
Damit diese Kabel auch am Arduino bzw. der Kupferplatine anschließbar sind, werden einzelne Pin-Header an die beiden übrigen Enden der Kabel gelötet. Diese erhält man bspw. wenn man die Enden von Widerständen abknipst.
Um zu vermeiden, dass die offenen Stellen an der und um die Lötstellen herum das Teil oder andere Teile ungewollt beeinflussen, werden diese anschließend mit Schrumpfschläuchen überzogen und mit einem Heißluftfön geschrumpft. Damit sind die Kabel und Drähte isoliert.
Farben der Kabel:
* Plus = rot
* Minus = blau

####Joystick
Die Schnittstellen-Kabel des Joysticks wurden ebenfalls mit Kabel verlängert. Zu den Schnittstellen gehören die Inputs Plus-, Minus- und Select-Button-Kabel und die Outputs X-Koordiante, Y-Koordinate.
Die Länge der Kabel reichte in unserem Fall jedoch nicht aus und aus diesem Grund haben wir diese mit weiteren Litzekabeln verlötet. Ebenso werden Pin-Header an die übrigen Enden der Kabel gelötet und alle offenen Stellen isoliert.
Hierbei wurden einfach 3 Zwillingskabel, aus dem verwendeten Rechner ausgebaut und ineinander verdreht. (Von diesen einzelnen Kabeln werden nur 5 von 6 benötigt).
Die einzelnen Kabel werden mit den Schnittstellen des Joystick verlötet. Da der Joystick in die vom Lasercutter ausgeschnittene Box eingebaut wird, wird in das Kabelbündel anschließend ein Knoten gemacht, der sich dann zwischen Joystick und Boxausgang befindet. Damit wird sichergestellt, dass der Joystick nicht durch festes Ziehen am Kabel beschädigt werden kann.
Die Kabel müssen lang genug sein, damit der User später den Joystick problemlos bedienen kann, ohne an der Scheibe “kleben” zu müssen.
Farben der Kabel:
* Plus = grün
* Minus = schwarz aus dem Zwillingskabel mit gelb
* Select-Button = gelb
* Y-Koordinate = schwarz

####LED-Platinen
Die Schnittstellen-Kabel der LED-Platinen wurden ebenfalls mit Kabeln verlängert. Zu den Schnittstellen gehören die Inputs Plus-, Minus- und Signalkabel und ein Output Signalkabel.
Die Installations-Komponente “Bildschirm” besteht aus zwei LED-Platinen. 
=> 2 Plus-, 2 Minus-, 2 Input-Signal- und 2 Output-Schnittstellen sind vorhanden.
Die LED-Platinen werden zur programmatischen Vereinfachung in Reihe geschaltet.
Hierzu wird nur das eine Input-Signalkabel an den Arduino bzw. an die Kupferplatine angeschlossen. Das Output-Signalkabel dieser LED-Platine wird mit dem Input-Signalkabel der anderen LED-Platine verlötet. Hierbei ist darauf zu achten, dass der Schrumpfschlauch zur späteren Isolation bereits vor dem Verlöten drauf gezogen werden muss, da es sonst nicht mehr möglich ist.
Das Output-Signal-Kabel des letzten Arduinos wird nicht verwendet und muss auch nicht verlängert werden. 
=> Verlängert werden 2 Plus, 2 Minus und 1 Input-Signalkabel.
Ansonsten wird hierbei genauso vorgegangen wie mit den Lasern und den Photowiderständen.
Farben der Kabel
* Plus ist braun
* Minus ist schwarze
* Signalkabel (Input und Output) sind lila

###Schritt 3 - Alle Bauteile in den Kasten montieren
Da nun alle Elemente bearbeitet und erstellt wurden, kann alles in den Kasten gebaut werden.
Dazu gehören:
* Computerelemente
* Griffe
* Lasermodule und Linsen
* LEDs
* Fotowiderstände
* Displays

Die PC Bauteile werden auf Schrauben geschraubt, um dem Laser freien Platz auf dem Boden zu lassen. Die Lasermodule und die  dazu gehörigen Linsen werden auf kleine Holzklötze für einen besseren Halt geklebt, die wiederum in den Kasten geklebt werden. Die LEDs und Fotowiderstände kommen in die vorgesehenen Löcher. Anschließend werden die LED Matrix Displays eingebaut und die kleine Plexiglasplatte daraufgeschraubt.
Damit die Lasermodule die Fotowiderstände richtig treffen, werden diese am besten mit Alu-Folie etwas abgeschirmt, sodass an diesen Stellen das Licht etwas gebündelt wird.

Optional können noch kleine Bauteile eines Mainboards (z.B. Kondensatoren) hinein geklebt werden. 
Wenn alle Elemente im Kasten sind, kann die große Plexiglasplatte und die Griffe montiert werden. 

###Schritt 4 - Schaltplan
Bild des Schaltplans


Die Kabel des LaZer Computers können entweder auf ein Breadboard gesteckt werden oder auf eine Platine gelötet werden.
Für den Arduino der Steuerung haben wir eine Platine geätzt und für den Arduino für Snake haben wir eine Lochrasterplatine verwendet.

###Schritt 5 - Code
Code für Arduino herunterladen und auf die zwei Arduinos laden
* laZer_pc_driver für die Steuerung der Laser, Wiederstände und Komponenten
* Snake für das Spiel Snake


Alle Einstellungen können im Anfangsbereich des Lazer-Driver und Snake verändert werden und Angepasst werden.
* Belegung der Pins
* Konstanten und Wertebereiche
* Spieleinstellung für Snake




