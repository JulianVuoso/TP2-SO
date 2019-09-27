#include "listADT.h"
#include <stdlib.h>

typedef struct listCDT{
	struct node *first;
	unsigned int size;
} listCDT;

struct node {
	elementType elem;
	struct node *next;
	// void (*print) (elementType);
};

static struct node *addRec (struct node *l, elementType elem, int *size);
static struct node *delRec (struct node *l, elementType elem, int *size);

listADT newList(){
	return calloc(1, sizeof(struct listCDT));
}


// devuelve 1 si agrego un elemento
int add (listADT l, elementType elem){
	int aux=l->size;
	l->first=addRec(l->first, elem, &l->size);
	return (l->size!=aux);
}

static struct node *addRec (struct node *l, elementType elem, int *size){
	int aux=compare(elem, l->elem);
	if (l==NULL || aux<0){
		struct node *aux=malloc(sizeof(struct node));
		aux->elem=elem;
		aux->next=l;
		(*size)++;
		return aux;
	}
	if (aux>0)
		l->next=addRec(l->next, elem, size);
	return l;
}

void delete (listADT l, elementType elem){
	l->first=delRec(l->first, elem, &l->size);
}

static struct node *delRec (struct node *l, elementType e, int *size){
	int c=compare(e, l->elem);
	// aux<0 es como poner un < entre argumentos
	if (l==NULL || c<0)
		return l;
	if (c==0){
		struct node *aux=l->next;
		free(l); // Aca borro lo de arriba
		(*size)--;
		return aux;
	}
	l->next=delRec(l->next, e, size);
	return l;
}

int size(const listADT l){
	return l->size;
}

void printElems (const listADT l, void (*p)(elementType e)){
	struct node *aux=l->first;
	while (aux!=NULL){
		p(aux->elem); // l->print(aux->elem);
		aux=aux->next;
	}
}

elementType get (const listADT l, unsigned int idx)
{
	if (idx>=l->size)
		exit (1);
	return getRec(l->first, idx); // Nunca llego a NULL porque sé que el indice esta
}

static elemenType getRec (struct node *f, unsigned int idx)
{
	if (idx==0)
		return f->elem;
	return getRec (f->next, idx-1);
}
