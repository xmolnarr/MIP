#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AUTO																//struktura na uchovavanie zaznamov
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
	int n = 0;
	char x, c;
	AUTO *aktualny;

	aktualny = (AUTO*)malloc(sizeof(AUTO));										//predpokladam ze subor nie je prazdny				
	*prvy = aktualny;
	if ((f = fopen("auta.txt", "r")) == NULL)									//otvaranie suboru
	{
		printf("Zaznamy neboli nacitane\n");
		return;
	}
	else
	{
		while (!(feof(f)))														//prechadzam cely subor
		{
			x = getc(f);
			if (x != '$') break;												//pozeram sa ci tam je zaznam
			c = getc(f);

			if (n > 0) {														//vytvaranie dalsieho zaznamu ak je potrebne
				aktualny->dalsi = (AUTO*)malloc(sizeof(AUTO));
				aktualny = aktualny->dalsi;
			}

			fgets(aktualny->kategoria, 52, f);									//nacitanie jednotlivych poloziek do zoznamu
			fgets(aktualny->znacka, 52, f);										//pouzivam fgets na nacitanie stringu aj s medzerou - ak sa tam nachadza
			fgets(aktualny->predajca, 102, f);

			fscanf(f, "%d", &aktualny->cena);
			fscanf(f, "%d", &aktualny->datum_v);

			c = getc(f);
			fgets(aktualny->stav, 202, f);
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
	int n = 1;
	p = *prvy;

	while (p != NULL)															//prejdem cely zoznam
	{
		printf("%d.\nkategoria: %sznacka: %spredajca: %s", n, p->kategoria, p->znacka, p->predajca);
		printf("cena: %d\nrok_vyroby: %d\nstav_vozidla: %s", p->cena, p->datum_v, p->stav);
		p = p->dalsi;
		n++;
	}
}

void pridaj(AUTO **prvy)
{
	AUTO *p, *pred = NULL, *pridaj;
	int pozicia, n = 1, pom = 0;
	char c;
	p = *prvy;
	pridaj = (AUTO*)malloc(sizeof(AUTO));										//nacitam pridavanu polozku

	scanf("%d", &pozicia);
	c = getchar();																//osetrenie proti nacitaniu konca riadku

	fgets(pridaj->kategoria, 52, stdin);
	fgets(pridaj->znacka, 52, stdin);
	fgets(pridaj->predajca, 102, stdin);

	scanf("%d", &pridaj->cena);
	scanf("%d", &pridaj->datum_v);

	c = getchar();
	fgets(pridaj->stav, 202, stdin);

	if ((pozicia == 1) || (*prvy == NULL))										//na 1. poziciu ide ak je pozicia 1, resp. v zozname sa nic nenachadza
	{
		*prvy = pridaj;
		pridaj->dalsi = p;
		pom = 1;
	}
	else
		while (p != NULL)														//inac sa prida na konkretnu poziciu
		{
			if (pozicia == n)													//potrebujem si pamatat aj prechadzajuci zaznam, aby som pridavany zaznam nan mohol napojit
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

	if (pom == 0)																//ak som ho nikam nepridal tak ho dam na koniec
	{
		pred->dalsi = pridaj;
		pridaj->dalsi = NULL;
	}

}

int porovnaj(char *slovo1, char *slovo2)										//porovnavam, ci sa v stringu nachadza substring
{
	int i = 0, j = 0, k = 0;

	while (slovo1[i] != '\n')													//kazdy string v zozname obsahuje \n a idem len poò, kvoli tomu, ze substring sa nemusi nachadzat na konci stringu
	{
		if ((slovo1[i] == slovo2[j]) || (slovo1[i] == (slovo2[j] + 32)) || (slovo1[i] == (slovo2[j] - 32)))		//podmienka prejde, ak su pismena rovnake bez ohladu na to, ci je to male alebo velke pismeno
			while (slovo2[j] != '\n')
			{
				k = 1;
				if (((slovo1[i + j] % 65) % 32) != ((slovo2[j] % 65) % 32))				//ak sa nejaka cast substringu nerovna s povodnym - bez ohladu na to ci je male alebo velke, tak tak sa druhy cyklus breakne
				{
					k = 2;
					j = 0;
					break;
				}
				j++;
			}
		if (k == 1)break;														//ak som ho tam nasiel - presiel vnoreny cyklus az do konca a nenasiel chybu, tak skonci
		i++;
	}

	return k;
}

void zmaz(AUTO **prvy)
{
	AUTO *p, *pred = NULL;														//tiez si pamatam predchadzajuci zaznam, lebo pri zmazani dany zaznam akurat vypojim z pola
	int k = 0;
	char c, slovo[52];
	p = *prvy;

	c = getchar();
	fgets(slovo, 52, stdin);

	while (p != NULL)
	{
		if (porovnaj(p->znacka, slovo) == 1)									//ak sa tam subtring nachadza tak ho vypojim
		{
			k++;
			if (pred == NULL)													//ak mazem prvy musim posunut hlavny pointer
			{
				*prvy = p->dalsi;
				free(p);														//a freenem ho
				p = *prvy;
			}
			else
			{																	//ak nie tak ho len vypojim
				pred->dalsi = p->dalsi;
				free(p);														// a freenem
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

int zhoda(char *slovo1, char *slovo2)											//podobne porovnavanie ako v predoslom pripade
{
	int i = 0, k = 0;

	while (slovo1[i] != '\0')													//tu idem az po \0, lebo vtedy sa nestane ze zaciatok stringu je rovnaky a ptm pokracije
	{
		k = 1;
		if (((slovo1[i] % 65) % 32) != ((slovo2[i] % 65) % 32))					//a nespustam vnoreny cyklus, lebo porovnavam vzdy i-ty prvok s i-tym
		{
			k = 2;
			break;																//a ak niekde nastane chyba skoncim
		}
		i++;
	}

	return k;
}

void hladaj(AUTO **prvy)
{
	AUTO *p;
	char znacka[52], c;
	int i = 1, suma;

	p = *prvy;
	c = getchar();
	fgets(znacka, 52, stdin);
	scanf("%d", &suma);

	while (p != NULL)
	{
		if ((zhoda(p->znacka, znacka) == 1) && (suma >= p->cena))				//porovnavam ci su stringy zhodne a ci je cena nejakeho auta mensia ako zadana suma
		{
			printf("%d.\nkategoria: %sznacka: %spredajca: %s", i, p->kategoria, p->znacka, p->predajca);
			printf("cena: %d\nrok_vyroby: %d\nstav_vozidla: %s", p->cena, p->datum_v, p->stav);
			i++;
		}
		p = p->dalsi;
	}
	if (i == 1) printf("V ponuke nie su pozadovane auta\n");
}

void aktualizuj(AUTO **prvy)
{
	AUTO *p;
	char znacka[52], c;
	int i = 0, rok;
	p = *prvy;

	c = getchar();
	fgets(znacka, 52, stdin);
	scanf("%d", &rok);

	while (p != NULL)
	{
		if ((zhoda(p->znacka, znacka) == 1) && (rok == p->datum_v))				//pozeram sa ci sa tam nejake take auto nenachadza
		{
			p->cena = p->cena - 100;
			i++;
			if (p->cena < 0) p->cena = 0;										//osetrenie zapornej sumy
		}
		p = p->dalsi;
	}
	printf("Aktualizovalo sa %d zaznamov\n", i);
}

void uvolni(AUTO **prvy)														//vlastna funkcia na uvolnenie celeho zoznamu
{
	AUTO *p, *pom = NULL;
	p = *prvy;

	while (p != NULL)															//uvolnujem kazdy jeden zaznam, nestaci iba prvy smernik
	{
		pom = p;
		p = p->dalsi;
		free(pom);
		pom = NULL;
	}
	*prvy = NULL;
}

void koniec()
{
	exit(0);
}

int main()
{
	FILE *f;
	AUTO *prvy = NULL;
	char c;

	while (scanf("%c", &c) == 1)												//nacitavanie vstupu
	{
		if (c == 'n')
		{
			if (prvy != NULL) uvolni(&prvy);
			nacitaj(&f, &prvy);
		}
		if (c == 'v') vypis(&prvy);
		if (c == 'p') pridaj(&prvy);
		if (c == 'z') zmaz(&prvy);
		if (c == 'h') hladaj(&prvy);
		if (c == 'a') aktualizuj(&prvy);
		if (c == 'k')
		{
			uvolni(&prvy);
			koniec();
		}
	}

	return 0;
}