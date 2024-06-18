/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int index = get_hash(key);
  ht_item_t *item = (*table)[index];

  // Projdi spojový seznam v daném indexu tabulky
  // až do nalezení hledaného prvku nebo konce seznamu.
  while (item && strcmp(item->key, key) != 0) {
    item = item->next;
  }

  // Vrátí nalezený prvek nebo NULL, pokud prvek není v tabulce.
  return item;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  int index = get_hash(key);  
  ht_item_t *existing_item = ht_search(table, key);

  // Pokud prvek s daným klíčem již existuje, aktualizuj hodnotu.
  if (existing_item) {
    existing_item->value = value;
  } else {
    // Vytvoř nový prvek a vlož ho na začátek seznamu na daném indexu.
    ht_item_t *new_item = malloc(sizeof(ht_item_t));
    new_item->key = key;  
    new_item->value = value;
    new_item->next = (*table)[index];
    (*table)[index] = new_item;
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  // Vyhledej prvek v tabulce podle klíče.
  ht_item_t *item = ht_search(table, key);
  
  // Pokud byl prvek nalezen a klíč odpovídá, vrátí ukazatel na hodnotu.
  if (item && strcmp(item->key, key) == 0) {
    return &item->value;
  }

  // V opačném případě vrátí NULL, protože prvek není v tabulce.
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int index = get_hash(key);
  ht_item_t *item = (*table)[index];
  ht_item_t *prev_item = NULL;

  // Hledání prvku v seznamu synonym podle klíče.
  while (item && strcmp(item->key, key) != 0) {
    prev_item = item;
    item = item->next;
  }

  // Pokud prvek není v tabulce, není co mazat.
  if (!item) {
    return;
  }

  // Smazání prvku z tabulky.
  if (prev_item) {
    prev_item->next = item->next;
  } else {
    (*table)[index] = item->next;
  }

  // Uvolnění paměti alokované pro mazaný prvek.
  free(item);
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  // Projdi všechny indexy v tabulce.
  for (int i = 0; i < HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];
    
    // Projdi všechny prvky v seznamu synonym daného indexu.
    while (item) {
      ht_item_t *next_item = item->next;
      // Uvolni paměť alokovanou pro aktuální prvek.
      free(item);
      // Přesuň se na další prvek v seznamu.
      item = next_item;
    }

    // Nastav hodnotu na NULL, čímž vytvoříš prázdný seznam na daném indexu.
    (*table)[i] = NULL;
  }
}
