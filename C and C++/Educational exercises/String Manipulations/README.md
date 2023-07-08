# Stringmanipulation

## Lernziel

Das Arbeiten mit Pointern soll am Beispiel von C-Strings (Character-Pointern) geübt werden. Eine Funktion soll nach einer detaillierten Vorgabe implementiert werden.

## Beschreibung

Schreiben Sie eine Funktion, welche die einzelnen Teile eines Namens (gespeichert in einem C-String) rückwärts geschrieben ausgibt.

### Funktionsspezifikation

#### Funktionsname

* reversePrint
* Für das Testsystem soll die Funktion wie folgt aussehen:
  * `int reversePrint(char *name)`

#### Parameter

1. einen C-String: der Name bestehend aus einem oder mehreren Vornamen und einem oder mehreren Nachnamen, jeweils durch genau 1 Leerzeichen getrennt. Es kommen nur Buchstaben und Leerzeichen vor.

#### Rückgabewert

* ein Fehlerwert (siehe unten)

## Funktionsweise

Die Funktion gibt die einzelnen Teile des Namens jeweils rückwärts buchstabiert aus. Hierbei ist darauf zu achten, dass jeweils das erste ausgegebene Zeichen eines Namensteils großgeschrieben wird, während der nun an letzter Stelle stehende eigentlich erste Buchstabe zu einem Kleinbuchstaben wird. Die übergebenen Namensteile folgen sicher diesem Format (erster Buchstabe groß, Rest klein). Beachten Sie, dass **kein** abschließender Zeilenumbruch o.ä. ausgegeben wird.

Falls ein Fehlerfall auftritt wird **kein** Name ausgegeben.

### Beispiel

Lautet der Parameter beispielsweise "John Joe Jelly" so wird der Text "Nhoj Eoj Yllej" ausgegeben.

### Rückgabewerte

| Wert | Bedeutung   |
| :--: | ----------- |
| 0    | Erfolgsfall |
| -1   | Fehlerfall: leerer Parameter   |
| -2   | Fehlerfall: nicht mindestes zwei Namensteile im Parameter |

## Spezifikation

* Rufen Sie die Funktion `int reversePrint(char *name)` über die main-Funktion auf um diese zu testen
* keine zusätzlichen Ausgaben
* alle Ausgaben erfolgen auf stdout
  * keinerlei Ausgaben auf stderr
* keine Verwendung der string.h Library

## Abgabe

* Abgabe auf Progpipe mittels git
* Dateiname: ass-b1.c
* Abgabe mit git über die Kommandozeilenumgebung (**nicht** über das GitLab-Frontend)
* Keine Tags notwendig - es wird der aktuellste Commit als Abgabe betrachtet.

## Verantwortlicher Tutor
* Martin Bauer
* Kevin Innerebner
