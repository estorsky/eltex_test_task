#include "units.h"

/* 
void Change_Value (struct unit *Element, const int NewValue)
{
    Element->Value = NewValue;
}

struct unit *Go_To_Element (struct unit *Head, const int index)
{
    unsigned int counter = 0;
    while (counter != index) {
        Head = Head->Next;
        counter++;
    }
    return Head;
}

struct unit *New_Element (struct unit **Head, const int ValueToNew, const unsigned int ListLength)
{
    struct unit *New = (struct unit*) malloc(sizeof(struct unit));
    Change_Value(New, ValueToNew);
    New->Next = NULL;
    if (*Head != NULL) {
        Go_To_Element(*Head, ListLength - 1)->Next = New;
    }
    return New;
}

char Delete_Element (struct unit *ElementToDelete, struct unit **Head)
{
    //0 for normal, -1 for error
    if (ElementToDelete == NULL) {
        return -1;
    }
    if (ElementToDelete == *Head) {
        *Head = ElementToDelete->Next;
        free(ElementToDelete);
        return 0;
    }
    struct unit *Temp = *Head;
    while (Temp->Next != ElementToDelete) {
        Temp = Temp->Next;
    }
    Temp->Next = ElementToDelete->Next;
    free(ElementToDelete);
    return 0;
}

char Swap_Elements(struct unit *FirstList, struct unit *SecondElement)
{
    //0 for normal, -1 for error
    if (FirstList == NULL || SecondElement == NULL) {
        return -1;
    }
    FirstList->Value += SecondElement->Value;
    SecondElement->Value = FirstList->Value - SecondElement->Value;
    FirstList->Value -= SecondElement->Value;
    return 0;
}

struct unit *Search(struct unit *Head, int SearchValue)
{
    while (SearchValue != Head->Value) {
        if (Head->Next == NULL)
            return NULL;
        Head = Head->Next;
    }

    return Head;
}

void Free_All(struct unit **Head, unsigned int ListLength){
    while (ListLength > 0){
        Delete_Element(Go_To_Element(*Head, --ListLength), Head);
    }
} 
*/

