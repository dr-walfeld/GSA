#include <stdlib.h>
#include <stdio.h>

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)

int alignsRec (int, int, int*);
int alignsDyn (int, int, int*);
inline void calcVal(int*, int, int, int, int*);

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

inline void calcVal(int* table, int i, int j, int col, int* counter)
{
	int t,val;
	// "Rekursionsabbruch"; Wert ist 1 falls i == 0 or
	// j == 0
	if (i == 0 || j == 0)
	{
		val = 1;
	}
	else
	{
		// Knackpunkt: hier muss die Symmetrie
		// ausgenutzt werden, da bei i = j der
		// Eintrag t sonst nicht existiert
		t = i*(col)+j-1;
		if (i == j)
		{
			t = (j-1)*(col)+i;
		}
		val = table[(i-1)*(col)+j] \
		     +table[t] \
		     +table[(i-1)*(col)+j-1];
		*counter += 3;
	}
	table[i*(col)+j] = val;
	*counter += 1;
}

/* Implementierung der Berechnung der Anzahl Alignments mittels
   dynamischer Programmierung; auch hier wird ein Zeiger auf eine
   Zaehlervariable uebergeben */
// optimierte Implementierung; es werden Symmetrien genutzt
int alignsDyn (int m, int n, int* counter)
{
	// Deklaration Schleifenvariablen und Wert des aktuellen Feldes
	int i,j;
	// Deklaration der Tabelle (m+1)x(n+1)  mit den Aligns; und
	// Speicherreservierung
	int* table = (int *) malloc((m+1)*(n+1)*sizeof(int));
	// an dieser Stelle Verabschiedung von m und n,
	// es sind nur max und min wichtig; moeglich, da m und n bei der
	// Anzahl Aligns kommutieren [aligns(m,n) = aligns(n,m)]
	int min = MIN(m,n);
	int max = MAX(m,n);
	/* fuellen der dynamischen Tabelle
	   zentrale Tricks fuer Minimierung der Zugriffe:
	   1. nutze Symmetrien wenn moeglich (fuer die min x min Submatrix)
	   2. Berechnung der halben Matrix ist ausreichend
	   ANMERKUNG: die Anzahl der Zeilen ist min+1, die Anzahl der Spalten
	   ist max+1;
	   asymptotische Laufzeit bleibt aber O(n*m) */
	for (i = 0; i <= min; i++)
	{
		for (j = i; j <= min; j++)
		{
			calcVal(table, i, j, max+1, counter);
		}
	}
	*counter += 1;
	// nutze Eigenschaft, dass Anzahl Spalten max+1 und Anzahl Zeilen min+1
	//
	for (i = 0; i <= min; i++)
	{
		for (j = min+1; j <= max; j++)
		{
			calcVal(table, i, j, max+1, counter);
		}
	}
	// nur Kontrolle: Ausgabe der Tablle (auch hier: max+1 Spalten)
	printf("--- Dynamische Tabelle ---\n");
	for (i = 0; i <= min; i++)
	{
		for (j = 0; j <= max; j++)
		{
			printf("%d;", table[i*(max+1)+j]);
		}
		printf("\n");
	}
	printf("--------------------------\n");
	
	// das Ergebnis befindet sich in dem letzten Feld des Arrays
	int result = table[(m+1)*(n+1)-1];
	free (table);
	return result;
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
