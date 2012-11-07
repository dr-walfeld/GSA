#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void reverseComp(char*, int);
char complement(char);

// Eingabe ist String mit Sequenz und dessen Laenge;
// es wird in-place das reverse Complement gebildet
void reverseComp(char* s, int len)
{
	int j;
	char temp;
	// Berechnung der mittleren Position des Strings;
	// bei gerade Laenge existiert keine Mitte => waehle 
	int halb = len/2;
	if (len%2 == 0)
		halb--;

	// laufe bis zu mitte des Strings und tausche mit entgegengesetzem
	// Symbol (jeweils als Complement)
	for(j = 0; j <= halb; j++)
	{
		temp = s[j];
		s[j] = complement(s[len-1-j]);
		s[len-1-j] = complement(temp);
	}
}


// bildet Complement einer eingegeben Base;
// gibt '*' zurueck, wenn Eingabe-Symbol unbekannt
char complement(char a)
{
	char b;
	switch(a)
	{
		case 'a': 
			b = 't';
		break;
		case 'c':
			b = 'g';
		break;
		case 'g':
			b = 'c';
		break;
		case 't':
			b = 'a';
		break;
		default:
			b = '*';
	}
	return b;
}

int main()
{
	// Demonstration der Funktion mit einer Sequenz gerader und
	// einer Sequenz ungerader Laenge
	char s1[]  = "gatccgtacgttac";
	int len1 = strlen(s1);
	printf("%s\n", s1);
	reverseComp(s1, len1);
	printf("%s\n", s1);
	printf("------------------------------\n");
	char s2[]  = "gatccgtacgttacg";
	int len2 = strlen(s2);
	printf("%s\n", s2);
	reverseComp(s2, len2);
	printf("%s\n", s2);
	return 0;
}
