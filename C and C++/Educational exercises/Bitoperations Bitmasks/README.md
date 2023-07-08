# Bitoperationen

## Lernziel
Das Arbeiten mit Bitoperationen und Bitmasken soll anhand des BCD-Codes implementiert werden.

## Beschreibung

Erstellen Sie ein Programm, das als Parameter eine positive Zahl übernimmt und im Anschluss falls möglich die Zahl vom Dualcode in den BCD-Code und wieder zurück konvertiert. Das Programm soll dabei logisch in Funktionen gegliedert sein und alle Berechnungen soweit möglich mit Bitoperationen und Bitmasken errechnen.

Der BCD-Code (Binary Coded Decimal) stellt eine Zahl so dar, dass er für jede Ziffer (0-9) ein Nibble (4 Bit, ein halbes Byte) verwendet. 

## Funktionsweise


Das Programm erwartet als (einzigen) Parameter eine positive Ganzzahl (unsigned int) im Wertebereich 1 - UINT_MAX (definiert in limits.h), der erste Wert ist inklusive, der zweite exklusive. Zur Konvertierung des Parameters soll die Funktion strtoul (aus stdlib.h) verwendet werden, wobei der zweite Parameter auf NULL und der dritte auf 10 gesetzt werden muss. Dadurch ist bis auf den Wertebereich keine weitere Eingabeüberprüfung notwendig.

Weiters müssen 2 Funktionen mit den folgenden Funktionsprototypen implentiert werden:
```
unsigned long convertBinaryToBCD(unsigned long number);
```
welches eine Zahl als Parameter übergeben bekommt und den entsprechenden BCD-Code zurückgibt und
```
unsigned long convertBCDToBinary(unsigned long number);
```
welches einen BCD-Code als Parameter übergeben bekommt und die entsprechende Zahl zurückgibt. Falls die Konvertierung nicht möglich ist, muss dies speziell behandelt werden (siehe Rückgabewerte). Außerdem dürfen in diesen beiden Funktionen keinerlei Ausgaben auf stdout erfolgen, sonst schlagen die Testcases fehl!

### Output

Das Programm gibt im Anschluss folgenden Text aus:
```
 Input:  [Input] -  [Bits of Input]\n
   BCD:    [BCD] -    [Bits of BCD]\n
Binary: [Binary] - [Bits of Binary]\n
```
Hierbei werden die Begriffe in eckigen Klammern durch die entsprechenden Werte ersetzt:

Für die Werte selbst ([Input], [BCD], [Binary]) ist folgender Format String zu verwenden: "%10lu". Die Bitweise Ausgabe soll jedes Nibble mit einem Leerzeichen trennen und jedes Byte zusätzlich mit einem vertikalen Strich |, der ebenfalls von Leerzeichen umgeben ist. [BCD] soll die Konvertierung des Inputs zum BCD-Code sein und [Binary] soll die Rückkonvertierung von [BCD] zu Binary sein.

### Beispiele
```
$./ass-b3 101
 Input:        101 - 0000 0000 | 0000 0000 | 0000 0000 | 0110 0101
   BCD:        257 - 0000 0000 | 0000 0000 | 0000 0001 | 0000 0001
Binary:        101 - 0000 0000 | 0000 0000 | 0000 0000 | 0110 0101
$./ass-b3 9999999999
Number out of range: allowed range is 1 to 4294967294
$./ass-b3 100000000
 Input:  100000000 - 0000 0101 | 1111 0101 | 1110 0001 | 0000 0000
   BCD: Binary Code too large.
Binary: Binary Code too large.
$./ass-b3 1 2
Usage: ./ass-b3 number
```

Wird der Funktion **convertBinaryToBCD** als Parameter 101 übergeben, soll der Rückgabe-Wert 257 sein.
Wird der Funktion **convertBCDToBinary** als Parameter 257 übergeben, soll der Rückgabe-Wert 101 sein.

### Rückgabewerte

Es müssen zumindest folgende Programmrückgabewerte und Fehlerbehandlungen implementiert werden:

| Wert | Bedeutung   | Fehlermeldung |
| :--: | ----------- | ----------- |
| 0    | Erfolgsfall |              |
| -1   | Wenn die Parameteranzahl nicht genau 1 ist.  | Usage: [Name] number\n |
| -2   | Wenn die Zahl im Parameter nicht aus dem Wertebereich 1 bis UINT_MAX - 1 ist. | number out of range: allowed range is 1 to [Max]\n |


Es kann sein, dass die Zahl im gültigen Wertebereich liegt, aber nicht konvertiert werden kann, wobei die folgenden zwei Fehler unabhängig voneinander auftreten können und die jeweilige Funktion soll dann den Rückgabe-Wert UINT_MAX zurückgeben:

#### Dualcode -> BCD-Code (convertBinaryToBCD)

Fehlermeldung: "Binary Code too large.\n"

Ein Integer hat 4 Byte = 32 Bit = 8 Nibble, kann also maximal die Zahl 99999999 in BCD darstellen, wurde eine größere Zahl eingegeben ist diese Fehlermeldung statt der Werte auszugeben (siehe Beispiele). 

#### BCD-Code -> Dualcode (convertBCDToBinary)

Da ein Nibble 2^4, also 16 mögliche Werte annehmen kann, es aber nur 10 Ziffern (0-9) gibt, sind die Werte größer 9 im BCD-Code nicht möglich. Beachten Sie, dass dieser Fehler im Laufe des gesamten Programms, **nicht** auftreten kann da nur Dezimalzahlen eingelesen werden. Falls jedoch die Funktion **convertBCDToBinary** eine Zahl bekommt die nicht konvertiert werden kann soll UINT_MAX zurückgegeben werden (wird getestet!).

## Spezifikation

* der Zugriff und die Berechnung von Nibbles im BCD Code muss mit bitweisen Operationen erfolgen
* es dürfen keine Arrays für die Berechnungen oder das Speichern der Nibbles verwendet werden 
* keine zusätzlichen Ausgaben
* alle Ausgaben erfolgen auf stdout
  * keinerlei Ausgaben auf stderr 

## Abgabe

* Abgabe auf Progpipe mittels git
* Dateiname: ass-b3.c
* Abgabe mit git über die Kommandozeilenumgebung (**nicht** über das GitLab-Frontend)
* Keine Tags notwendig - es wird der aktuellste Commit als Abgabe betrachtet.

## Verantwortlicher Tutor
* Amel Smajic
