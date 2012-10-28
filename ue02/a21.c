#include <stdlib.h>
#include <stdio.h>

int alignsRec (int, int, int*);
int alignsDyn (int, int, int*);

/* rekursive Implementierung der Funktion zur Berechnung der Anzahl
   Alignments; Rekursionsabbruch erfolgt bei m == 0 or n == 0;
   es wird ein Zeiger auf eine Zaehlervariable uebergeben */
int alignsRec (int m, int n, int* counter)
{
	if (m == 0 || n == 0)
	{
		return 1;
	}
	else
	{
		*counter += 3;
		return alignsRec (m-1, n, counter) + \
		alignsRec (m, n-1, counter) + alignsRec (m-1, n-1, counter);
	}
}

/* Implementierung der Berechnung der Anzahl Alignments mittels
   dynamischer Programmierung; auch hier wird ein Zeiger auf eine
   Zaehlervariable uebergeben */
// naive Implementierung, alle Indices werden einmal durchlaufen
int alignsDyn (int m, int n, int* counter)
{
	// Deklaration Schleifenvariablen und Wert des aktuellen Feldes
	int i,j;
	int val = 1;
	int var1 = 1;
	int var3 = 1;
	// Deklaration der Tabelle (m+1)x(n+1)  mit den Alignments; und
	// Speicherreservierung
	int* table = (int *) malloc((m+1)*(n+1)*sizeof(int));
	// fuellen der dynamischen Tabelle
	for (i = 0; i <= m; i++)
	{
		for (j = 0; j <= n; j++)
		{
			// "Rekursionsabbruch"; Wert ist 1 falls i == 0 or
			// j == 0
			if (i == 0 || j == 0)
			{
				val = 1;
			}
			else
			{
				/* Ansatz: [(i-1)*(n+1)+j] = 1, falls i-1 == 0
				   [(i-1)*(n+1)+j-1] = 1, falls i-1 or j-1 == 0
				   und [i*(n+1)+j-1] ist immer der zuvor
				   berechnete Wert (val)
				   => Minimierung der Zugriffe auf Array */
				if (i-1 == 0)
				{
					var1 = 1;
					var3 = 1;
				}		
				else if (j-1 == 0)
				{
					var1 = table[(i-1)*(n+1)+j];
					var3 = 1;
					*counter += 1;
				}
				else
				{
					var1 = table[(i-1)*(n+1)+j];
					var3 = table[(i-1)*(n+1)+j-1];
					*counter += 2;
				}	
				val = var1 + val + var3;
			}
			table[i*(n+1)+j] = val;
			*counter += 1;
		}
	}
	// hier nur Ausgabe zur Kontrolle
	/*
	for (i = 0; i <= m; i++)
	{
		for (j = 0; j <= n; j++)
		{
			printf("%d;", table[i*(n+1)+j]);
		}
		printf("\n");
	}
	*/
	// das Ergebnis befindet sich in dem letzten Feld des Arrays
	// (zuletzt berechnet)
	free (table);
	return val;
}

int main(int argc, char* argv[])
{
	// Ueberpruefung, ob korrekte Anzahl an Argumenten uebergeben wurde
	if (argc < 3)
	{
		printf("FEHLER: Es wurden zu wenig Parameter eingeben!\n");
		return 1;
	}
	
	// m und n von den uebergebenen Parametern (CString) lesen
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	// Zaehler startet fuer rekursive Version bei 1 (erster Aufruf)
	int counter = 1;
	// Ueberpruefung ob m und n im zugelassenen Bereich 0 <= m <= 15 liegen
	if (m > 15 || m < 0)
	{
		printf("m = %d ausserhalb des erlaubten Bereichs (0-15)!\n",m);
		return 1;
	}
	if (n > 15 || n < 0)
	{
		printf("n = %d ausserhalb des erlaubten Bereichs (0-15)!\n",n);
		return 1;
	}

	// Berechnung der Anzahl Alignments mit rekursiver Methode
	int result = alignsRec (m, n, &counter);
	printf("Rekursiv: \n");
	printf("%d %d %d (%d calls)\n",m, n, result, counter);
	// Berechnung der Anzahl Alignments mit dynamischer Programmierung
	counter = 0;
	result = alignsDyn (m, n, &counter);
	printf("Dynamisch: \n");
	printf("%d %d %d (%d acesses)\n",m, n, result, counter);
	return 0;
}
