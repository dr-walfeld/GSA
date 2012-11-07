#include <stdlib.h>
#include <stdio.h>

char getS();
void printWords(int);

// simuliert Ausgabe eines Zeichens
int pos = 0;
//char s[] = "gattagcctaccgtctagcaatcacgttg";
char s[] = "gattagcctac";
char getS()
{
	char a = s[pos];
	if (a != '\0')
		pos++;
	return a;
}

// gibt alle Worte der Laenge k auf der Sequenz s aus
void printWords(int k)
{
	int i = 0;
	// lies das erste Zeichen
	char a = getS();
	// Speicher fuer Wort der Laenge k reservieren
	char* w;
	w = (char *) malloc(k);
	
	// solange die Sequenz noch nicht am ENde ist
	while(a != '\0')
	{
		// ueberpruefen, ob w bereits voll
		if (i < k)
		{
			w[i] = a;
		}
		else
		{
			// wenn w bereits voll: ruecke alle Symbole in w 
			// nach vorne und schreibe neues Symbol ans Ende
			int j;
			for (j = 0; j < k-1; j++)
			{
				w[j] = w[j+1];
			}
			w[k-1] = a;
		}
		// sobald k gefuellt ist, wird jedes Mal das
		// neue Wort w ausgegeben
		if (i >= k-1)
		{
			printf("%s\n", w);
		}
		i++;
		a = getS();
	}
	// Speicher fuer w wieder freigeben
	free(w);
}

int main()
{
	// gesamte Sequenz zur Kontrolle ausgeben
	printf("%s\n", s);
        int k = 5;
        printWords(k);
	return 0;
}
