/* mudruto1 DU4
 * sestaveni: gcc tel.c -o tel; nebo: make tel;
 * spusteni : ./tel
 */

#include <stdlib.h>
#include <stdio.h>

//LinkedList + Funkce
typedef struct {
  struct ll *next;
  void *data;
} ll;

ll *ll_new() {
  ll *list = (ll*) malloc(sizeof(ll));
  if(!list) exit(1);
  list->next = 0;
  list->data = 0;
  return list;
}

void ll_add(ll *list, void *ptr) {
  for(;list->next != 0;list = (ll *) list->next);
  list->next = (struct ll *) ll_new();
  list->data = (void *) ptr;
}

void ll_rm(ll **llptr) {
  ll *freeptr = *llptr;
  *llptr = (ll *) (*llptr)->next;
  free(freeptr);
}

void ll_walk(ll *list, void (*function)(ll *ptr)) {
  for(;list->next != 0;list = (ll *) list->next) function(list);
}

void ll_walk_data(ll *list, void (*function)(void *ptr)) {
  for(;list->next != 0;list = (ll *) list->next) if(list->data != 0) function(list->data);
}

//Kontakt + Funkce
typedef struct {
  char name[16];
  char sname[16];
  char telno[16];
} contact;

contact *contact_new() {
  contact *c = (contact*) malloc(sizeof(contact));
  if(!c) exit(1);
  scanf("%15s", c->name);
  if(c->name[0] == '.') return 0;
  scanf("%15s", c->sname);
  scanf("%15s", c->telno);
  return c;
}

void contact_print(contact *c) {
  printf("%-15s %-15s %-15s\n", c->name, c->sname, c->telno);
}

char contact_match(contact *c, char *str) { //vrati true, kdyz kontakt odpovida
  return !(strcmp(c->name, str) && strcmp(c->sname, str) && strcmp(c->telno, str));
}

char contact_compare(contact *a, contact *b) { //vrati true, kdyz jsou kontakty stejne
  return !(strcmp(a->name, b->name) || strcmp(a->sname, b->sname) || strcmp(a->telno, b->telno));
}


char find_string[64] = "";
void contact_find(ll *item) {
  if(contact_match(item->data, find_string)) {
    contact_print(item->data);
  }
}

void contact_remove(ll **llptr) {
  //bohuzel nejde pouzit contact_find + ll_walk protoze je treba mit referenci na prvni polozku pro pripad potreby jejiho odstraneni
  while((*llptr)->next != 0) {
    if(contact_match((*llptr)->data, find_string)) {
      printf("DELETED: "); contact_print((*llptr)->data);
      ll_rm(llptr);
      return;
    }
    llptr = (ll **) &((*llptr)->next);
  }
}



int main(void) {

/*
  //ukazka pouziti linked-listu
  ll *list = ll_new();
  ll_add(list, "foo");
  ll_add(list, "bar");
  ll_walk_data(list, puts);
  ll_rm(&list);
  puts("first item removed...");
  ll_walk_data(list, puts);
*/

/*
  //ukazka pouziti kontaktu
  contact *c = contact_new();
  if(!c) exit(2);
  contact_print(c);
*/

  puts(
    "\n\tTelefonni seznam 0.1 (mudruto1)\n"
    "Zadavejte kontakty ve formatu 'jmeno prijmeni cislo' kazdy na novou radku.\n"
    "Po zadani posledniho kontaktu zadejte radku zacinajici teckou.\n"
    "priklad:\n"
    "tomas mudrunka 123456789\n"
    "daniel novak 987654321\n"
    "alice obrovska 456789123\n"
    ".\n"
  );

  ll *seznam = (ll *) ll_new();
  contact *c;
  while(c = contact_new()) ll_add(seznam, c);
  puts(
    "------------------------------------------\n"
    "jmeno           prijmeni        cislo\n"
  );
  ll_walk_data(seznam, contact_print);

  puts("\n\nCo hledat?");
  scanf("%15s", find_string);
  ll_walk(seznam, contact_find);

  puts("\nCo smazat?");
  scanf("%15s", find_string);
  contact_remove(&seznam);

  puts("\nTakto vypada seznam bez smazane polozky:\n");
  ll_walk_data(seznam, contact_print);
}

