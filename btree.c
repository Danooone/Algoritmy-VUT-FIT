/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (!tree) { return false; }  // uzel je prázdný
  if (key == tree->key) {  // klíče se shodují
    *value = tree->value;  // zkopírujeme hodnotu
    return true;
  }
  if (key < tree->key) {  // klíč je menší
    return bst_search(tree->left, key, value);  // jdeme do levého podstromu
  }
  return bst_search(tree->right, key, value);  // klíč je větší, jdeme do pravého podstromu
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (!tree) { return; }  // ochrana před nulovým ukazatelem
  if (!*tree) {  // uzel je prázdný
    *tree = malloc(sizeof(bst_node_t));  // alokujeme paměť
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
  }
  if (key == (*tree)->key) {  // klíče se shodují
    (*tree)->value = value;   // nahradíme hodnotu
    return;
  }
  if (key < (*tree)->key) {  // klíč je menší,
    bst_insert(&(*tree)->left, key, value);  // jdeme do levého podstromu
    return;
  }
  bst_insert(&(*tree)->right, key, value);  // klíč je větší, jdeme do pravého podstromu
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree)->right) {
    bst_replace_by_rightmost(target, &(*tree)->right);  // rekurzivně voláme sami sebe, pokud existuje pravý podstrom
    return;
  }
  // Pokud pravý podstrom neexistuje
  target->key = (*tree)->key;  // zkopírujeme klíč
  target->value = (*tree)->value;  // zkopírujeme hodnotu

  bst_node_t* const to_delete = *tree;  // adresa uzlu k odstranění
  *tree = (*tree)->left;  // nahradíme ho levým podstromem
  free(to_delete);  // uvolníme paměť
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (!tree || !*tree) { return; }  // prázdný strom
  if (key == (*tree)->key) {  // klíče se shodují
    if ((*tree)->left && (*tree)->right) {  // mazaný uzel má oba podstromy
      bst_replace_by_rightmost(*tree, &(*tree)->left);
      return;
    }
    bst_node_t* const to_delete = *tree;  // adresa uzlu k odstranění
    if ((*tree)->left) {  // mazaný uzel má pouze levý podstrom
      *tree = (*tree)->left;
    } else if ((*tree)->right) {  // mazaný uzel má pouze pravý podstrom
      *tree = (*tree)->right;
    } else {
      *tree = NULL;  // mazaný uzel nemá žádné podstromy
    }
    free(to_delete);  // odstraníme uzel
    return;
  }
  if (key < (*tree)->key) {  // klíč je menší
    bst_delete(&(*tree)->left, key);  // jdeme do levého podstromu
    return;
  }
  bst_delete(&(*tree)->right, key);  // klíč je větší, jdeme do pravého podstromu
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if (!tree || !*tree) { return; }  // prázdný strom
  bst_dispose(&(*tree)->left);  // odstraníme levý podstrom
  bst_dispose(&(*tree)->right); // odstraníme pravý podstrom
  free(*tree);  // odstraníme aktuální podstrom
  *tree = NULL;  // vynulujeme ukazatel
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (!tree) { return; }  // prázdný uzel
  bst_add_node_to_items(tree, items);  // přidáme aktuální uzel
  bst_preorder(tree->left, items);  // přidáme levý uzel
  bst_preorder(tree->right, items);  // přidáme pravý uzel
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (!tree) { return; }  // prázdný uzel
  bst_inorder(tree->left, items);  // přidáme levý uzel
  bst_add_node_to_items(tree, items);  // přidáme aktuální uzel
  bst_inorder(tree->right, items);  // přidáme pravý uzel
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (!tree) { return; }  // prázdný uzel
  bst_postorder(tree->left, items);  // přidáme levý uzel
  bst_postorder(tree->right, items);  // přidáme pravý uzel
  bst_add_node_to_items(tree, items);  // přidáme aktuální uzel
}

