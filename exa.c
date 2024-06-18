/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
  bst_init(tree); // inicializace stromu
  while (*input) {
    char ch = tolower(*input++); // převedeme na malá písmena a přejdeme na další prvek
    if (!isalpha(ch)) { ch = '_'; }  // pokud se nejedná o písmeno, nahraďte ho písmenem '_'
    int count = 0;
    bst_search(*tree, ch, &count); // hledání znaku
    bst_insert(tree, ch, count + 1); // vložení nebo nahrazení uzlu
  }
}

/**
 * Rekurzivně přidá prostřední uzel mezi indexy vlevo (včetně) a vpravo (bez zahrnutí) do stromu stromu
*/

void bst_add_middle_recursive(bst_node_t **tree, bst_items_t *items, size_t left, size_t right) {
  if (right - left == 0) { return; }  // není co přidávat
  size_t middle = left + (right - left) / 2; // index prostředního prvku
  bst_insert(tree, items->nodes[middle]->key, items->nodes[middle]->value); // vložení prostředního prvku
  bst_add_middle_recursive(tree, items, left, middle); // přidání levé poloviny
  bst_add_middle_recursive(tree, items, middle + 1, right); // přidání pravé poloviny
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
  if (!tree || !*tree) { return; }  // prázdný strom
  bst_items_t items; // pole položek
  items.nodes = NULL;
  items.capacity = 0;
  items.size = 0;
  bst_inorder(*tree, &items); // procházení stromu v pořadí
  if (items.size > 2) { // pokračuj, pokud items.size > 2 (jinak je strom vyvážený)
    bst_node_t* new_tree; // vytvoří nový strom
    bst_init(&new_tree); // inicializace
    bst_add_middle_recursive(&new_tree, &items, 0, items.size); // vytvoří nový strom
    bst_dispose(tree); // vymazat starý strom
    *tree = new_tree; // přiřadit novou adresu
  }
  free(items.nodes); // vymažte pole položek
}