#include "..\header\array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DATA_T void
#define SIZE_T int

struct array* initialize_array(SIZE_T size_max){
    struct array* pNewArray = malloc(sizeof(struct array));
    pNewArray->ppData = malloc(sizeof(DATA_T*) * size_max);
    pNewArray->size_current = 0;
    pNewArray->size_max = size_max;
    return pNewArray;
}
struct array* copy_shallowCopy_array(const struct array* pArray){
    struct array* pNewArray = initialize_array(pArray->size_max);
    memcpy(pNewArray->ppData, pArray->ppData, sizeof(DATA_T*) * pArray->size_current);
    pNewArray->size_current = pArray->size_current;
    pNewArray->size_max = pNewArray->size_max;
    return pNewArray;
}
struct array* copy_deepCopy_array(const struct array* pArray, DATA_T*(*copy_deepCopy_pData)(DATA_T* pData)){
    struct array* pNewArray = initialize_array(pArray->size_max);
    for (SIZE_T i = 0; i < pArray->size_current; i++){
        pNewArray->ppData[i] = copy_deepCopy_pData(pArray->ppData[i]);
    }
    pNewArray->size_current = pArray->size_current;
    pNewArray->size_max = pNewArray->size_max;
    return pNewArray;
}
struct array* merge_array(struct array* pArray_1, struct array* pArray_2){
    SIZE_T newSize_max = pArray_1->size_max + pArray_2->size_max;
    SIZE_T newSize_current = pArray_1->size_current + pArray_2->size_current;
    struct array* pNewArray = initialize_array(newSize_max);
    memcpy(pNewArray->ppData, pArray_1->ppData, sizeof(DATA_T*) * pArray_1->size_current);
    memcpy(pNewArray->ppData + pArray_1->size_current, pArray_2->ppData, sizeof(DATA_T*) * pArray_2->size_current);
    pNewArray->size_current = newSize_current;
    pNewArray->size_max = newSize_max;
    free_array(pArray_1, NULL);
    free_array(pArray_2, NULL);
    return pNewArray;
}
int isValid_offset_array(struct array* pArray, SIZE_T offset){
    int isValid = 0;
    if (offset >= pArray->size_current){
        ;
    }
    else {
        isValid = 1;
    }
    return isValid;
}
int isIdentical_pData_array(const struct array* pArray_1, const struct array* pArray_2, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref)){
    int isIdentical = 1;
    if (pArray_1->size_current == pArray_2->size_current){
        for (SIZE_T i = 0; i < pArray_1->size_current; i++){
            if (isIdentical_pData(pArray_1->ppData[i], pArray_2->ppData[i])){
                ;
            }
            else {
                isIdentical = 0;
                break;
            }
        }
    }
    else {
        isIdentical = 0;
    }
    return isIdentical;
}
int insert_array(struct array* pArray, const DATA_T* pData){
    int isNotFull = 0;
    if (pArray->size_current == pArray->size_max){
        ;
    }
    else {
        isNotFull = 1;
        pArray->ppData[pArray->size_current] = pData;
        pArray->size_current++;
    }
    return isNotFull;
}
int modify_array(struct array* pArray, const DATA_T* pData, SIZE_T offset){
    int isValid_offset = 0;
    if (isValid_offset_array(pArray, offset)){
        isValid_offset = 1;
        pArray->ppData[offset] = pData;
    }
    return isValid_offset;
}
int swap_array(struct array* pArray, SIZE_T offset_1, SIZE_T offset_2){
    int isValid_offset = 0;
    if (isValid_offset_array(pArray, offset_1) && isValid_offset_array(pArray, offset_2)){
        isValid_offset = 1;
        DATA_T* pTempData = pArray->ppData[offset_1];
        pArray->ppData[offset_1] = pArray->ppData[offset_2];
        pArray->ppData[offset_2] = pTempData;
    }
    return isValid_offset;
}
DATA_T* access_array(const struct array* pArray, SIZE_T offset){
    DATA_T* pData = NULL;
    if (isValid_offset_array(pArray, offset)){
        pData = pArray->ppData[offset];
    }
    return pData;
}
SIZE_T query_bruteForce_array(const struct array* pArray, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref), const DATA_T* pData_ref){
    SIZE_T offset = pArray->size_max;
    for (SIZE_T i = 0; i < pArray->size_current; i++){
        if (isIdentical_pData(pArray->ppData[i], pData_ref)){
            offset = i;
            break;
        }
    }
    return offset;
}
SIZE_T query_binarySearch_array(const struct array* pArray, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLarger_pData)(DATA_T* pData), const DATA_T* pData_ref){
    SIZE_T offset = pArray->size_max;
    SIZE_T offset_start = 0;
    SIZE_T offset_end = pArray->size_current - 1;
    while (offset_start != offset_end){
        SIZE_T offset_middle = (offset_start + offset_end) / 2;
        if (isLarger_pData(pArray->ppData[offset_middle], pData_ref)){
            offset_start = offset_middle + 1;
        }
        else {
            offset_end = offset_middle - 1;
        }
    }
    if (isIdentical_pData(pArray->ppData[offset_start], pData_ref)){
        offset = offset_start;
    }
    return offset;
}
void sort_bubbleSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    for (SIZE_T i = 1; i < pArray->size_current; i++){
        int isSorted = 1;
        for (SIZE_T u = 1; u < pArray->size_current - i; u++){
            if (isLarger_pData(pArray->ppData[u - 1], pArray->ppData[u])){
                swap_array(pArray, u - 1, u);
                isSorted = 0;
            }
        }
        if (isSorted){
            break;
        }
    }
}
void sort_selectionSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    for (SIZE_T i = 1; i < pArray->size_current; i++){
        int isSorted = 1;
        SIZE_T offset_minKey = i;
        for (SIZE_T u = 1 + i; u < pArray->size_current; u++){
            if (isLarger_pData(pArray->ppData[offset_minKey], pArray->ppData[u])){
                offset_minKey = u;
                isSorted = 0;
            }
        }
        if (isSorted){
            break;
        }
        else {
            swap_array(pArray, i, offset_minKey);
        }
    }
}
void sort_insertionSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    for (SIZE_T i = 1; i < pArray->size_current; i++){
        DATA_T* pTempData = pArray->ppData[i];
        SIZE_T u = i - 1;
        for (; u >= 0 && isLarger_pData(pArray->ppData[u], pTempData); u--){
            swap_array(pArray, u, u + 1);
        }
        pArray->ppData[u + 1] = pTempData;
    }
}
void sort_quickSort_array(struct array* pArray, SIZE_T offset_start, SIZE_T offset_end,int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLesser_pData)(DATA_T* pData, DATA_T* pData_ref)){
    SIZE_T offset_pivot = offset_end;
    SIZE_T offset_left = offset_start;
    SIZE_T offset_right = offset_end - 1;
    for (; offset_left <= offset_end - 1; offset_left++){
        if (isLarger_pData(pArray->ppData[offset_left], pArray->ppData[offset_pivot])){
            break;
        }
    }
    for (; offset_right >= offset_start; offset_right--){
        if (isLesser_pData(pArray->ppData[offset_right], pArray->ppData[offset_pivot])){
            break;
        }
    }
    if (offset_left > offset_right){
        ;
    }
    else {
        swap_array(pArray, offset_pivot, offset_left);
        sort_quickSort_array(pArray, offset_start, offset_left - 1, isLarger_pData, isLesser_pData);
        sort_quickSort_array(pArray, offset_left + 1, offset_end, isLarger_pData, isLesser_pData);
    }
}
void sort_mergeSort_array(struct array* pArray, SIZE_T offset_start, SIZE_T offset_end, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    SIZE_T different_offset = offset_end - offset_start;
    if (different_offset > 0){
        SIZE_T offset_1_start = offset_start;
        SIZE_T offset_1_end = offset_start + different_offset / 2;
        SIZE_T offset_2_start = offset_start + different_offset / 2 + 1;
        SIZE_T offset_2_end = offset_end;
        sort_mergeSort_array(pArray, offset_1_start, offset_1_end, isLarger_pData);
        sort_mergeSort_array(pArray, offset_2_start, offset_2_end, isLarger_pData);
        while (offset_1_start > offset_1_end || offset_2_start > offset_2_end){
            if (isLarger_pData(pArray->ppData[offset_1_start], pArray->ppData[offset_2_start])){
                swap_array(pArray, offset_1_start, offset_2_start);
                offset_1_start++;
                offset_2_start++;
            }
            else {
                offset_1_start++;
            }
        }
    }
}
void free_array(struct array* pArray, void(*free_pData)(DATA_T* pData)){
    if (free_pData != NULL){
        for (SIZE_T i = 0; i < pArray->size_current; i++){
            free_pData(pArray->ppData[i]);
        }
    }
    free(pArray->ppData);
    free(pArray);
}
void debug_array(const struct array* pArray, char*(*convertToString_pData)(DATA_T* pData), const char* message){
    fprintf(stderr, "%s: ", message);
    putc('{', stderr);
    for (SIZE_T i = 0; i < pArray->size_current - 1; i++){
        char* pChar_pData = convertToString(pArray->ppData[i]);
        fprintf(stderr, "%s, ", pChar_pData);
    }
    if (pArray->size_current > 0){
        char* pChar_pData = convertToString(pArray->ppData[pArray->size_current - 1]);
        fprintf(stderr, "%s", pChar_pData);
    }
    putc('}', stderr);
}

#undef DATA_T
#undef SIZE_T