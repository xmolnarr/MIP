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

void nacitaj(FILE **f1, AUTO **prvy)
{
	FILE *f;
	f = *f1;
	int n;
	char x, c;
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
			x = getc(f);
			if (x != '$') break;
			c = getc(f);
			if (n > 0) {
				aktualny->dalsi = (AUTO*)malloc(sizeof(AUTO));
				aktualny = aktualny->dalsi;
			}
			
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
		printf("Nacitalo sa %d zaznamov.\n", n);
	}
	return;
}

void vypis(AUTO **prvy)
{
	AUTO *p;
	p = *prvy;
	int n;
	n = 1;
	while (p != NULL)
	{
		printf("%d.\nkategoria: %sznacka: %spredajca: %s", n, p->kategoria, p->znacka, p->predajca);
		printf("cena: %d\ndatum_vyroby: %d\nstav_vozidla: %s", p->cena, p->datum_v, p->stav);
		p = p->dalsi;
		n++;
	}
}

void pridaj(AUTO **prvy)
{
	AUTO *p,*pred = NULL,*pridaj;
	int pozicia, n = 1, pom = 0;
	char c;
	p = *prvy;
	pridaj = (AUTO*)malloc(sizeof(AUTO));
	scanf("%d", &pozicia);
	c = getchar();
	fgets(pridaj->kategoria, 52, stdin);
	fgets(pridaj->znacka, 52, stdin);
	fgets(pridaj->predajca, 52, stdin);
	scanf("%d", &pridaj->cena);
	scanf("%d", &pridaj->datum_v);
	c = getchar();
	fgets(pridaj->stav, 52, stdin);

	if (pozicia == 1)
	{
		*prvy = pridaj;
		pridaj->dalsi = p;
		pom = 1;
	}
	else
		while (p != NULL)
		{
			if (pozicia == n)
			{
				pred->dalsi = pridaj;
				pridaj->dalsi = p;
				pom = 1;
				break;
			}
			else
			{
				pred = p;
				p = p->dalsi;
				n++;
			}
		}

	if (pom == 0)
	{
		pred->dalsi = pridaj;
		pridaj->dalsi = NULL;
	}


}

int porovnaj(char *slovo1, char *slovo2)
{
	int i, j, k = 0;

	i = 0;
	j = 0;

	while (slovo1[i] != '\n')
	{
		if ((slovo1[i] == slovo2[j]) || (slovo1[i] == (slovo2[j]+32)) || (slovo1[i] == (slovo2[j] - 32)))
			while (slovo2[j] != '\n')
			{
				k = 1;
				//j++;
				if (((slovo1[i+j]%65)%32) != ((slovo2[j]%65)%32))
				{
					k = 2;
					j = 0;
					break;
				}
				j++;
			}
		if (k == 1)break;
		i++;
	}


	return k;
}

void zmaz(AUTO **prvy)
{
	AUTO *p, *pred = NULL;
	int k = 0;
	char slovo[52];
	p = *prvy;
	fgets(slovo, 52, stdin);
	printf("%s", slovo);
	while (p != NULL)
	{
		if (porovnaj(p->znacka, slovo) == 1)
		{
			k++;
			if (pred == NULL) 
			{ 
				p = p->dalsi;
				*prvy = p;
			}
			else
			{
				pred->dalsi = p->dalsi;
				free(p);
				p = pred->dalsi;
			}
		}
		else
		{
			pred = p;
			p = p->dalsi;
		}
	}
	printf("Vymazalo sa %d zaznamov\n", k);
}

int zhoda(char *slovo1, char *slovo2)
{
	int i = 0, k = 0;

	while (slovo1[i] != '\0')
	{
		k = 1;
		if (((slovo1[i] % 65) % 32) != ((slovo2[i] % 65) % 32))
		{
			k = 2;
			break;
		}
		i++;
	}
	return k;
}

void hladaj(AUTO **prvy)
{
	AUTO *p;
	char znacka[52];
	int i = 0, suma;
	p = *prvy;
	fgets(znacka, 52, stdin);
	scanf("%d", &suma);
	while (p != NULL)
	{
		if ((zhoda(p->znacka, znacka) == 1) && (suma <= p->cena))
		{
			printf("%d.\nkategoria: %sznacka: %spredajca: %s", i, p->kategoria, p->znacka, p->predajca);
			printf("cena: %d\ndatum_vyroby: %d\nstav_vozidla: %s", p->cena, p->datum_v, p->stav);
			i++;
		}
		p = p->dalsi;
	}
	if (i == 0)printf("V ponuke nie su pozadovane auta\n");
}

int main()
{
	FILE *f;
	AUTO *prvy = NULL;
	nacitaj(&f, &prvy);
	vypis(&prvy);
	/*pridaj(&prvy);
	vypis(&prvy);*/
	zmaz(&prvy);
	vypis(&prvy);

	hladaj(&prvy);

	return 0;
}