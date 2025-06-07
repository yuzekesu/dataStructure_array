#include "..\header\array.h"
#include "..\header\test.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DATA_T struct number
#define SIZE_T int

struct array* initialize_array(SIZE_T size_max){
    struct array* pNewArray = malloc(sizeof(struct array));
    pNewArray->ppData = malloc(sizeof(DATA_T*) * size_max);
    pNewArray->size_current = 0;
    pNewArray->size_max = size_max;
    return pNewArray;
}
int copy_force_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData)){
    int success = 1;
    if (pArray_dest->size_max < (SIZE_T)offset_start_dest + (SIZE_T)size || pArray_src->size_max < (SIZE_T)offset_start_src + (SIZE_T)size){
        success = 0;
    }
    else {
        if (pArray_dest->size_current < (SIZE_T)offset_start_dest + (SIZE_T)size){
            pArray_dest->size_current = pArray_src->size_current;
        }
        if (duplicate_deepCopy_pData == NULL){
            for (SIZE_T i = (SIZE_T)offset_start_dest, u = (SIZE_T)offset_start_src; i < (SIZE_T)size + (SIZE_T)offset_start_dest; i++, u++){
                pArray_dest->ppData[i] = pArray_src->ppData[u];
            }
        }
        else {
            for (SIZE_T i = (SIZE_T)offset_start_dest, u = (SIZE_T)offset_start_src; i < (SIZE_T)size + (SIZE_T)offset_start_dest; i++, u++){
                pArray_dest->ppData[i] = duplicate_deepCopy_pData(pArray_src->ppData[u]);
                if (pArray_dest->ppData[i] == NULL){
                    for (; i >= 0; i--){
                        free_pData(pArray_dest->ppData[i]); 
                    }
                    success = 0;
                    break;
                }
            }
        }
    }
    return success;
}
int copy_array(struct array* pArray_dest, const struct array* pArray_src, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData)){
    int success = copy_force_array(pArray_dest, pArray_src, 0, 0, pArray_src->size_current, duplicate_deepCopy_pData, free_pData);
    return success;
}
struct array* duplicate_selective_array(const struct array* pArray, SIZE_T offset_start, SIZE_T offset_end, int doInherit_size_max, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData)){
    struct array* pNewArray;
    SIZE_T size_toCopy = offset_end - offset_start + 1;
    if (size_toCopy < 1){
        pNewArray = NULL;
    }
    else {
        SIZE_T size_max;
        if (doInherit_size_max){
            size_max = pArray->size_max;
        }
        else {
            size_max = size_toCopy;
        }
        pNewArray = initialize_array(size_max);
        if (pNewArray != NULL){
            pNewArray->size_current = size_toCopy;
            if (duplicate_deepCopy_pData == NULL){
                copy_force_array(pNewArray, pArray, 0, offset_start, size_toCopy, NULL, NULL);
            }
            else {
                int isSuccess = copy_force_array(pNewArray, pArray, 0, offset_start, size_toCopy, duplicate_deepCopy_pData, free_pData);
                if (!isSuccess){
                    free_array(pNewArray, NULL);
                    pNewArray = NULL;
                }
            }
        }
    }
    return pNewArray;

}
struct array* duplicate_identical_array(const struct array* pArray, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData)){
    struct array* pNewArray = duplicate_selective_array(pArray, 0, pArray->size_current - 1, 1, duplicate_deepCopy_pData, free_pData);
    return pNewArray;
}
struct array* merge_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2){
    if (pArray_dest == NULL){
        SIZE_T newSize_max = pArray_1->size_max + pArray_2->size_max;
        SIZE_T newSize_current = pArray_1->size_current + pArray_2->size_current;
        pArray_dest = initialize_array(newSize_max);
        if (pArray_dest != NULL){
            pArray_dest->size_current = newSize_current;
            pArray_dest->size_max = newSize_max;
        }
    }
    if (pArray_dest != NULL){
        copy_force_array(pArray_dest, pArray_1, 0, 0, pArray_1->size_current, NULL, NULL);
        copy_force_array(pArray_dest, pArray_2, pArray_1->size_current, 0, pArray_2->size_current, NULL, NULL);
        free_array(pArray_1, NULL);
        free_array(pArray_2, NULL);
    }
    return pArray_dest;
}
struct array* merge_FIFO_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    if (pArray_dest == NULL){
        SIZE_T newSize_max = pArray_1->size_max + pArray_2->size_max;
        SIZE_T newSize_current = pArray_1->size_current + pArray_2->size_current;
        pArray_dest = initialize_array(newSize_max);
        if (pArray_dest != NULL){
            pArray_dest->size_current = newSize_current;
            pArray_dest->size_max = newSize_max;
        }
    }
    if (pArray_dest != NULL){
        SIZE_T offset_mergeProgress = pArray_dest->size_current - 1;
        while (!isEmpty_array(pArray_1) && !isEmpty_array(pArray_2)){
            DATA_T* pData_1 = pArray_1->ppData[pArray_1->size_current - 1];
            DATA_T* pData_2 = pArray_2->ppData[pArray_2->size_current - 1];
            if(isLarger_pData(pData_1, pData_2)){
                pArray_dest->ppData[offset_mergeProgress] = pData_1;
                pArray_1->size_current--;
            }
            else {
                pArray_dest->ppData[offset_mergeProgress] = pData_2;
                pArray_2->size_current--;
            }
            offset_mergeProgress--;
        }
        if (!isEmpty_array(pArray_1)){ // dont use memcpy, might cause weird problems.
            copy_array(pArray_dest, pArray_1, NULL, NULL);
        }
        else if (!isEmpty_array(pArray_2)){
            copy_array(pArray_dest, pArray_2, NULL, NULL);
        }
    }
    return pArray_dest;
}
struct array_twin* split_array(struct array* pArray, int isDestroyParent){
    SIZE_T different_offset = pArray->size_current - 0;
    SIZE_T offset_1_start = 0;
    SIZE_T offset_1_end = 0 + different_offset / 2 - 1;
    SIZE_T offset_2_start = 0 + different_offset / 2;
    SIZE_T offset_2_end = pArray->size_current - 1;
    struct array* pArray_1 = duplicate_selective_array(pArray, offset_1_start, offset_1_end, 0, NULL, NULL);
    struct array* pArray_2 = duplicate_selective_array(pArray, offset_2_start, offset_2_end, 0, NULL, NULL);
    struct array_twin* pTwin = NULL;
    if (pArray_1 == NULL || pArray == NULL){
        free(pArray_1);
        free(pArray_2);
    }
    else {
        pTwin = malloc(sizeof(struct array_twin));
        pTwin->pArray_1 = pArray_1;
        pTwin->pArray_2 = pArray_2;
    }
    if (isDestroyParent){
        free_array(pArray, NULL);
    }
    return pTwin;
}
int isEmpty_array(struct array* pArray){
    int isEmpty = 0;
    if (pArray->size_current == 0){
        isEmpty = 1;
    }
    return isEmpty;
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
int isIdentical_arrays_array(const struct array* pArray_1, const struct array* pArray_2, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref)){
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
        pArray->ppData[pArray->size_current] = (DATA_T*)pData;
        pArray->size_current++;
    }
    return isNotFull;
}
int modify_array(struct array* pArray, const DATA_T* pData, SIZE_T offset){
    int isValid_offset = 0;
    if (isValid_offset_array(pArray, offset)){
        isValid_offset = 1;
        pArray->ppData[offset] = (DATA_T*)pData;
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
    if (isValid_offset_array((struct array*)pArray, offset)){
        pData = pArray->ppData[offset];
    }
    return pData;
}
SIZE_T query_bruteForce_array(const struct array* pArray, const DATA_T* pData_ref, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref)){
    SIZE_T offset = pArray->size_max;
    for (SIZE_T i = 0; i < pArray->size_current; i++){
        if (isIdentical_pData(pArray->ppData[i], (DATA_T*)pData_ref)){
            offset = i;
            break;
        }
    }
    return offset;
}
SIZE_T query_binarySearch_array(const struct array* pArray, const DATA_T* pData_ref, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    SIZE_T offset = pArray->size_max;
    SIZE_T offset_start = 0;
    SIZE_T offset_end = pArray->size_current - 1;
    while (offset_start != offset_end){
        SIZE_T offset_middle = (offset_start + offset_end) / 2;
        if (isLarger_pData(pArray->ppData[offset_middle], (DATA_T*)pData_ref)){
            offset_start = offset_middle + 1;
        }
        else {
            offset_end = offset_middle - 1;
        }
    }
    if (isIdentical_pData(pArray->ppData[offset_start], (DATA_T*)pData_ref)){
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
    for (SIZE_T i = 0; i < pArray->size_current; i++){
        int isSorted = 1;
        SIZE_T offset_minKey = i;
        for (SIZE_T u = 1 + i; u < pArray->size_current; u++){
            if (isLarger_pData(pArray->ppData[offset_minKey], pArray->ppData[u])){
                offset_minKey = u;
                isSorted = 0;
            }
        }
        if (!isSorted){
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
int sort_mergeSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    int isSuccess = 1;
    if (pArray->size_current < 2){
        ;
    }
    else{
        struct array_twin* pTwin = split_array(pArray, 0);
        struct array* pArray_1 = NULL;
        struct array* pArray_2 = NULL;
        if (pTwin == NULL){
            isSuccess = 0;
        }
        else {
            pArray_1 = pTwin->pArray_1;
            pArray_2 = pTwin->pArray_2;
            free_array_twin(pTwin, NULL);
            isSuccess = sort_mergeSort_array(pArray_1, isLarger_pData);
            if (isSuccess){
                isSuccess = sort_mergeSort_array(pArray_2, isLarger_pData);
                   if (isSuccess){
                    merge_FIFO_array(pArray, pArray_1, pArray_2, isLarger_pData);
                   } 
                }
                else {
                    isSuccess = 0;
            }
        free_array(pArray_1, NULL);
        free_array(pArray_2, NULL);
        }
    }
    return isSuccess;
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
void free_array_twin(struct array_twin* pTwin, void(*free_pData)(DATA_T* pData)){
    if (free_pData == NULL){
        ;
    }
    else {
        free_array(pTwin->pArray_1, free_pData);
        free_array(pTwin->pArray_2, free_pData);
    }
    free(pTwin);
}
void randomize_array(struct array* pArray){
    for (SIZE_T i = 0; i < pArray->size_current; i++){
        if (rand() % 2 - 1){
            swap_array(pArray, i, rand() % pArray->size_current);
        }
    }
}
void debug_array(const struct array* pArray, const char* message, char*(*toString_pData)(DATA_T* pData)){
    fprintf(stderr, "%s: ", message);
    putc('{', stderr);
    for (SIZE_T i = 0; i < pArray->size_current - 1; i++){
        char* pChar_pData = toString_pData(pArray->ppData[i]);
        fprintf(stderr, "%s, ", pChar_pData);
        free(pChar_pData);
    }
    if (pArray->size_current > 0){
        char* pChar_pData = toString_pData(pArray->ppData[pArray->size_current - 1]);
        fprintf(stderr, "%s", pChar_pData);
        free(pChar_pData);
    }
    putc('}', stderr);
    putc('\n', stderr);
}

#undef DATA_T
#undef SIZE_T