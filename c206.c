/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->activeElement = list->firstElement = list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList *list) {
    // Start with the first element of the list
    while (list->firstElement != NULL) {
        // Save a pointer to the next element before deleting the current element
        DLLElementPtr item = list->firstElement->nextElement;

        // Free the memory occupied by the current element
        free(list->firstElement);

        // Update the pointer to the first element to point to the next element
        list->firstElement = item;
    }

    // After freeing all elements, reset the list pointers to make it empty
    list->activeElement = list->firstElement = list->lastElement = NULL;
}



/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList *list, int data) {
    // Allocate memory for a new element
    DLLElementPtr item = malloc(sizeof(struct DLLElement));
    if (item == NULL) {
        // Call DLL_Error() function in case of memory allocation failure
        DLL_Error();
        return;
    }

    // Initialize the new element
    item->previousElement = NULL;
    item->nextElement = list->firstElement;
    item->data = data;

    // Update pointers for the previous first element and the list's first element
    if (list->firstElement != NULL) {
        list->firstElement->previousElement = item;
    } else {
        // If the list was empty, update the last element pointer as well
        list->lastElement = item;
    }

    // Update the list's first element pointer to the new element
    list->firstElement = item;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList *list, int data) {
    // Allocate memory for a new element
    DLLElementPtr item = malloc(sizeof(struct DLLElement));
    if (item == NULL) {
        // Call DLL_Error() function in case of memory allocation failure
        DLL_Error();
        return;
    }

    // Initialize the new element
    item->previousElement = list->lastElement;
    item->nextElement = NULL;
    item->data = data;

    // Update pointers for the previous last element and the list's last element
    if (list->lastElement != NULL) {
        list->lastElement->nextElement = item;
    } else {
        // If the list was empty, update the first element pointer as well
        list->firstElement = item;
    }

    // Update the list's last element pointer to the new element
    list->lastElement = item;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList *list) {
    // Set the active element pointer to the first element of the list
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList *list) {
    // Set the active element pointer to the last element of the list
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList *list, int *dataPtr) {
    if (list->firstElement == NULL) {
        // Call DLL_Error() function if the list is empty
        DLL_Error();
        return;
    }

    // Retrieve the data of the first element and store it in 'dataPtr'
    *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList *list, int *dataPtr) {
    if (list->firstElement == NULL) {
        // Call DLL_Error() function if the list is empty
        DLL_Error();
        return;
    }

    // Retrieve the data of the last element and store it in 'dataPtr'
    *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList *list) {
    if (list->firstElement == NULL) return; // Nothing to delete if the list is empty

    if (list->activeElement == list->firstElement) {
        // If the first element was active, clear the active element
        list->activeElement = NULL;
    }

    // Update the next element's previous pointer
    DLLElementPtr next = list->firstElement->nextElement;
    if (next == NULL) {
        // If there's only one element in the list (first == last), set lastElement to NULL
        list->lastElement = NULL;
    } else {
        // Otherwise, update the next element's previous pointer
        next->previousElement = NULL;
    }

    // Free the memory occupied by the first element and update the firstElement pointer
    free(list->firstElement);
    list->firstElement = next;
}
/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList *list) {
    if (list->firstElement == NULL) return; // Nothing to delete if the list is empty

    if (list->activeElement == list->lastElement) {
        // If the last element was active, clear the active element
        list->activeElement = NULL;
    }

    // Update the previous element's next pointer
    DLLElementPtr previous = list->lastElement->previousElement;
    if (previous == NULL) {
        // If there's only one element in the list (first == last), set firstElement to NULL
        list->firstElement = NULL;
    } else {
        // Otherwise, update the previous element's next pointer
        previous->nextElement = NULL;
    }

    // Free the memory occupied by the last element and update the lastElement pointer
    free(list->lastElement);
    list->lastElement = previous;
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList *list) {
    if (list->activeElement == NULL || list->activeElement == list->lastElement) {
        // If the list is inactive or the active element is the last one, do nothing
        return;
    }

    // Get the next element to delete
    DLLElementPtr next = list->activeElement->nextElement;

    // Relink the pointers to bypass the element to delete
    list->activeElement->nextElement = next->nextElement; // Relink left-to-right
    if (next->nextElement != NULL) { // Relink right-to-left
        next->nextElement->previousElement = list->activeElement;
    } else {
        // The next item was the last one, so update the lastElement pointer
        list->lastElement = list->activeElement;
    }

    // Free the memory occupied by the deleted element
    free(next);
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList *list) {
    if (list->activeElement == NULL || list->activeElement == list->firstElement) {
        // If the list is inactive or the active element is the first one, do nothing
        return;
    }

    // Get the previous element to delete
    DLLElementPtr previous = list->activeElement->previousElement;

    // Relink the pointers to bypass the element to delete
    list->activeElement->previousElement = previous->previousElement;
    if (previous->previousElement != NULL) {
        previous->previousElement->nextElement = list->activeElement;
    } else {
        // The previous item was the first one, so update the firstElement pointer
        list->firstElement = list->activeElement;
    }

    // Free the memory occupied by the deleted element
    free(previous);
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter(DLList *list, int data) {
    if (list->activeElement == NULL) {
        // If the list is not active, do nothing
        return;
    }

    // Allocate memory for a new element
    DLLElementPtr new = malloc(sizeof(struct DLLElement));
    if (new == NULL) {
        // Call DLL_Error() function in case of memory allocation failure
        DLL_Error();
        return;
    }

    // Update pointers to insert the new element after the active element
    new->previousElement = list->activeElement;
    new->nextElement = list->activeElement->nextElement;

    if (new->nextElement != NULL) {
        // If there's a next element, update its previous pointer
        new->nextElement->previousElement = new;
    }

    // Set the data for the new element and update the active element's next pointer
    new->data = data;
    list->activeElement->nextElement = new;
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore(DLList *list, int data) {
    if (list->activeElement == NULL) {
        // If the list is not active, do nothing
        return;
    }

    // Allocate memory for a new element
    DLLElementPtr new = malloc(sizeof(struct DLLElement));
    if (new == NULL) {
        // Call DLL_Error() function in case of memory allocation failure
        DLL_Error();
        return;
    }

    // Update pointers to insert the new element before the active element
    new->nextElement = list->activeElement;
    new->previousElement = list->activeElement->previousElement;

    if (new->previousElement != NULL) {
        // If there's a previous element, update its next pointer
        new->previousElement->nextElement = new;
    }

    // Set the data for the new element and update the active element's previous pointer
    new->data = data;
    list->activeElement->previousElement = new;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList *list, int *dataPtr) {
    if (list->activeElement == NULL) {
        // If the list is not active, call the DLL_Error() function
        DLL_Error();
        return;
    }

    // Retrieve the data of the active element and store it in 'dataPtr'
    *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList *list, int data) {
    if (list->activeElement == NULL) {
        // If the list is not active, do nothing
        return;
    }

    // Update the data of the active element with the new 'data' value
    list->activeElement->data = data;
}
/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList *list) {
    if (list->activeElement == NULL) {
        // If the list is not active, do nothing
        return;
    }

    // Move the active element pointer to the next element
    list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList *list) {
    if (list->activeElement == NULL) {
        // If the list is not active, do nothing
        return;
    }

    // Move the active element pointer to the previous element
    list->activeElement = list->activeElement->previousElement;

    if (list->activeElement == NULL) {
        // If the active element becomes NULL (first element), the list becomes inactive
        list->activeElement = NULL;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;
}

/* Konec c206.c */