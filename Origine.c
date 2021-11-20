/*
Sviluppo di applicazione in ANSI C per gestire i punti fedelt� di un
supermercato.

Su un archivio (allegato all'attivit� 'spesa.txt') � memorizzato lo
storico degli acquisti dei clienti di un supermercato
(transazioni). L'archivio � un file di testo in cui ogni riga riporta,
separati da spazio, le seguenti informazioni: codice cliente, data,
importo della spesa, saldo punti. Ogni 10 euro di spesa, il cliente
guadagna 1 punto fedelt�. Il cliente, a fronte di una spesa minima di
50 euro utilizzar� 50 punti fedelt� per ricevere uno sconto di 5 euro
sul totale della spesa. I saldo punti verr� aggiornato in base
all'importo effettivamente pagato. E' possibile utilizzare al pi� 50
punti per transazione.

Alcuni esempi:

1) Saldo punti 30, Importo spesa 100 euro --> Importo pagato 100 euro, saldo punti 40

2) Saldo punti 200, Importo spesa 100 euro --> Importo pagato 95 euro, saldo punti 200-50+9=159


Implementare le seguenti funzioni:

1. CaricaStorico: carica lo storico in una lista di transazioni.

2. ServizioCassa: aggiunge una transazione alla lista delle
transazioni.

3. SpesaMensile: calcola l'importo mensile per spesa di un cliente

La descrizione dettagliata di ogni funzione � riportata come commento
immediatamente prima della definizione della funione.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_FILE "elenco.txt"

typedef struct Transazione
{
	char  codice[8];
	char  data[11]; // nel formato gg/mm/aaaa
	float totale_euro;
	int   saldo_punti;
} TTransazione;

typedef struct NodoLista
{
	TTransazione      transazione;
	struct NodoLista* succ;
} TNodoLista;

typedef TNodoLista* PNodoLista;

/* ListaAggiungi inserisce un elemento in testa alla lista passata
   come primo parametro. Restituisce 1 se l'operazione va a buon fine,
   0 altrimenti */
int ListaAggiungi(PNodoLista* plista, TTransazione transazione){
	PNodoLista aux;
	aux = (PNodoLista)malloc(sizeof(TNodoLista));
	if (aux == 0)	return 0;
	aux->transazione = transazione;
	aux->succ = *plista;
	*plista = aux;
	return 1;
}

/*
   1. CaricaStorico: carica lo storico in una lista di
   transazioni. Restituisce 1 se l'operazione va a buon fine, 0
   altrimenti.
*/
int CaricaStorico(PNodoLista* plista, char* nome_file)
{
	// ... da completare a cura dello studente ...
	FILE* f;
	TTransazione tr;
	
	f = fopen(nome_file, "r");
	if (f == NULL) return 0;
	while (fscanf(f, "%s%s%f%d", tr.codice, tr.data, &tr.totale_euro, &tr.saldo_punti) == 4) {
		ListaAggiungi(plista, tr);
	}
	return 1;

}

/*
   2. ServizioCassa: aggiunge una transazione alla lista delle
   transazioni. Dato il codice_cliente, la data, e l'importo della
   spesa, viene aggiunta una nuova transazione in testa alla lista. Il
   campo saldo_punti della transazione aggiunta dovr� essere
   opportunamente aggiornato in base a quanto descritto nel testo del
   compito. Il saldo attuale del cliente � quello dell'ultima
   transizione effettuata dal cliente (quindi quella pi� vicina alla
   testa della lista).
*/
void ServizioCassa(PNodoLista* lista,char* codice_cliente, char* data, float importo){
	TTransazione tr;
	strcpy(tr.codice,codice_cliente);
	strcpy(tr.data,data);
	tr.totale_euro = importo;
	int punti = (int)importo / 10;
	//otterr�, per 54 euro di spesa, 5 punti! cio� 54/10 ->5
	printf("%d",punti);
	tr.saldo_punti = punti;
	ListaAggiungi(lista, tr);
}

/*
3. SpesaMensile: calcola l'importo mensile per spesa di un
cliente. Dato il codice cliente, la funzione restituisce un array che
per ogni mese dell'anno riporta il totale degli importi per la spesa
effettuata da quel cliente.
*/
void SpesaMensile(PNodoLista lista,char* codice_cliente, float* spesa_per_mese){
	int anno, giorno, mese;
	for (int i = 0; i < 12; i++) {
		spesa_per_mese[i] = 0.0; //inizializzo
	}
	while (lista != NULL) {
		if (strcmp(lista->transazione.codice, codice_cliente) == 0) {//trovo il cliente scorrendo la lista
			//prendo i dati della data per avere il mese
			sscanf(lista->transazione.data, "%d/%d/%d", &giorno, &mese, &anno);
			//vedo se in quel mese � gi� presente un elemento.
			//mese -1 perch� il for � inizializzato da 0, quindi va da 0 a 11. Mentre mese da 1 a 12.
			if (spesa_per_mese[mese - 1] == 0.0) { //valore nullo, lo inserisco
				spesa_per_mese[mese - 1] = lista->transazione.totale_euro;
			}else { //� gi� presente un valore, allora si deve sommare
				spesa_per_mese[mese - 1] = spesa_per_mese[mese - 1] + lista->transazione.totale_euro;
			}
		}	lista = lista->succ;
	}
}

void stampa(PNodoLista lista) {
	while (lista != NULL) {
		printf("\nCliente: %s  \til %s  \tImporto %.2f   Saldo %d", 
			lista->transazione.codice, lista->transazione.data, lista->transazione.totale_euro, lista->transazione.saldo_punti);
		lista = lista->succ;
	}
}

PNodoLista select(PNodoLista lista,char* codice) {
	PNodoLista aux = NULL; //se no mi dice mem non inizializzata
	while (lista != NULL) {
		if (strcmp(lista->transazione.codice, codice) == 0) {
			ListaAggiungi(&aux, lista->transazione);
		}
		lista = lista->succ;
	}
	return aux;
}

void ordina(PNodoLista *lista,PNodoLista Visita_lista) {
	
	while (Visita_lista != NULL) {
		if (*lista == NULL) {
			(*lista)->transazione = Visita_lista->transazione; //violazione di accesso alla memoria!! errore.
			(*lista)->succ = NULL;
			continue; //ricomincia il while
		}
		if (Visita_lista->transazione.totale_euro>(*lista)->transazione.totale_euro) {
			ListaAggiungi(&(*lista),Visita_lista->transazione);
		}
		else {
			*lista = (*lista)->succ;
			ordina(lista, Visita_lista);
		}

		Visita_lista = Visita_lista->succ;
	}
}


int Menu(){
	int scelta;
	printf("\n\n*** M E N U ***\n"
		"1 - CaricaStorico\n"
		"2 - ServizioCassa\n"
		"3 - SpesaMensile\n"
		"4 - Stampa\n"
		"5 - SelezionaCliente"
		"6 - Ordina $ - NOT WORKING"
		"0 - Uscita\n\n"
		"Scelta: ");
	scanf("%d", &scelta);
	return scelta;
}

void DisallocaLista(PNodoLista lista)
{
	PNodoLista p;

	while (lista != NULL)
	{
		p = lista;
		lista = lista->succ;
		free(p);
	}
}

int main(void){
	//PNodoLista lista;
	PNodoLista lista = NULL;
	int        scelta;
	float      spesa_mensile[12]; //lo inizializzo nel quesito3
	char       codice[8];
	char       data[11];
	float      importo;
	//TTransazione tr;
	PNodoLista sel = NULL; //quesito 5
	PNodoLista ordinata = NULL;

	do {
		scelta = Menu();

		switch (scelta)
		{
		case 1:
			// Invocare la funzione CaricaStorico e visualizzare un messaggio in caso di errore
			if (CaricaStorico(&lista, NOME_FILE) == 1) {
				printf("\nElementi caricati.");
			}
			else {
				printf("\nErrore caricamento");
			}
			break;

		case 2:
			// Acquisire il codice cliente, la data e l'importo
			printf("Inserire Codice: ");
			scanf("%s", codice);
			printf("Inserire data: ");
			scanf("%s", data);
			printf("Inserire l'importo: ");
			scanf("%f", &importo);
			// Invocare la funzione ServizioCassa
			ServizioCassa(&lista,codice,data,importo);
			break;

		case 3:
			// Acquisire il codice cliente
			printf("Acquisire il codice cliente ");
			scanf("%s", codice);
			// Invocare la funzione SpesaMensile
			SpesaMensile(lista, codice, spesa_mensile);
			for (int i = 0; i < 12; i++) {
				if (spesa_mensile[i] == 0.00)
					printf(" - |");
				else printf(" %.2f | ", spesa_mensile[i]);
			}
			break;
		case 4:
			stampa(lista);
			break;
		case 5:
			printf("Inserire codice cliente da attenzionare : ");
			scanf("%s", codice);
			sel = select(lista, codice);
			stampa(sel);
			break;
		case 6:
			ordina(&ordinata,lista);
			stampa(ordinata);
		}

	} while (scelta != 0);

	//DisallocaLista(...); // passare il parametro opportuno

	return 0;
}

