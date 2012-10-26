#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Ausgabe der k-Substrings einer Sequenz seq der Laenge len
int substrings (char* seq, int len, int k)
{
	int start = 0;
	int i;
	char c;
	printf("Start bei: ");
	// Iteration ueber die ganze Sequenz
	for (i = 0; i < len; i++)
	{
		// lesen des aktuellen Symbols
		c = seq[i];
		// falls aktuelles Symbol eine genau spezifizierte Base ist
		if (c == 'a' || c == 'g' || c == 'c' || c == 't')
		{
			/* wenn die Laenge des Substrings vom Start bis zur
			   aktuellen Position bereits k ist => wir haben
			   einen Substring gefunden => ausgeben;
			   start wird hochgezaehlt da von dort der naechste
			   Substring der Laenge k starten koennte */
			
			if (i-start+1 == k)
			{
				printf("%d; ", start);
				start++;
			}
		}
		// falls aktuelles Symbol Stop-Symbol fuer Substrings
		else if (c == 'n')
		{
			/* ein neues Substring der Laenge k ohne Stop-Symbol
			   kann fruehestens an der naechsten Position beginnen;
			   ANMERKUNG: wenn diese Position nicht mehr existiert
			   => auch egal, Schleife terminiert dann ohnehin */
			start = i+1;
		}
		// falls unbekanntes Symbol auftaucht => Abbrch Rueckgabe von 0
		else
		{
			printf("\n");
			return 0;
		}
	}
	printf("\n");
	return 1;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("FEHLER: zu wenig Parameter uebergeben!\n");
		return 1;
	}
	// lies Sequenz aus dem 1. Parameter
	char* seq = argv[1];
	printf("Sequenz: %s\n", seq);
	int len = strlen(seq);
	// lies k aus dem 2. Parameter
	int k = atoi(argv[2]);
	if (k > len)
	{
		printf("FEHLER: k=%d ist laenger Sequenz (%d b)\n", k, len);
		return 1;
	}
	// falls substrings 0 zurueckliefert ist etwas schiefgegangen
	if (!substrings(seq, len, k))
	{
		printf ("FEHLER: Sequenz erhaelt nicht zugelassene Symbole!\n");
		return 1;
	}
	return 0;
}
