# Datei: scom.py (Python3)
# Autor: 
# letzte Aenderung: 08.09.2016
#
# Versenden, Empfang und Ausgabe der Zeichenkette "Hallo Welt!"
#
# Auf dem Mikrorechner muss das Programm "echo" laufen.
# Als COM-Schnittstelle die der seriellen Verbindung einstellen,
# wahlweise fuer die Kabel- oder die Bluetooth-Verbindung.

import serial # Modul serial importieren
ser = serial.Serial("COM6")

test = [b"\xDD\xDD\xDD\xDD\x00\x05",
 b"\x00\x00\xDD\xDD\x00\x05",
 b"\x0D\xF3\x00\x00\x00\x09",
 b"\x00\x00\x00\x00\x00\x01",
 b"\x2D\xFF\xDD\xDD\x00\x05"]

for t in test:
  ser.write(t)
  y = ser.read(1)
#print("Empfangene Daten: " + bytes.fromhex(y))
ser.close()
