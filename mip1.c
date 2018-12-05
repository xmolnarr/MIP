#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AUTO
{
	char kategoria[52];
	char znacka[52];
	char predajca[102];
	int cena;
	int datum_v;
	char stav[202];
	struct AUTO *dalsi;
}AUTO;

void nacitaj(FILE **f1,AUTO **prvy)
{
	FILE *f;
	f = *f1;
	int n;
	char x[3],c;
	AUTO *aktualny;
	
	n = 0;
	aktualny = (AUTO*)malloc(sizeof(AUTO));
	*prvy = aktualny;
	if ((f = fopen("auta.txt", "r")) == NULL)
	{
		printf("Zaznamy neboli nacitane\n");
		return;
	}
	else
	{
		while (!(feof(f)))
		{
			if (n > 0) {
				aktualny->dalsi = (AUTO*)malloc(sizeof(AUTO));
				aktualny = aktualny->dalsi;
			}
			fgets(x, 3, f);
			fgets(aktualny->kategoria, 52, f);
			fgets(aktualny->znacka, 52, f);
			fgets(aktualny->predajca, 52, f);
			fscanf(f, "%d", &aktualny->cena);
			fscanf(f, "%d", &aktualny->datum_v);
			c = getc(f);
			fgets(aktualny->stav, 52, f);
			aktualny->dalsi = NULL;
			n++;
		}
		aktualny->dalsi = NULL;
		//*prvy = p;
		printf("Nacitalo sa %d zaznamov.\n", n);
	}
	return;
}

int main()
{
	FILE *f;
	AUTO *prvy;
	nacitaj(&f, &prvy);
	vypis(&prvy);

	return 0;
}