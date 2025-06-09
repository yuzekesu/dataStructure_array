/* #include "..\header\array.h"
#include <stdlib.h>
#include <stdio.h>
#define DATA_T void 
#define SIZE_T unsigned long long int  */

#include "..\header\array.h"
#include "..\header\test.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define DATA_T struct number 
#define SIZE_T unsigned long long int 
#define SIZE_CAP ULLONG_MAX

// PRIVATE-----------------------------------------------
/*
Copy to existing array. Allow random access, use carefully.
No worry about overflow. Underflow need to be checked by user.
Deep_copies will be destroied properly.
->Return 0 upon failure [memory(deepCopy), overflow].
*/
int copy_FORCE_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData));
// PUBLIC-----------------------------------------------
struct array* initialize_array(SIZE_T capacity){
    struct array* pNewArray = NULL;
    if (capacity < SIZE_CAP){
        pNewArray = malloc(sizeof(struct array));
        if (pNewArray != NULL){
            pNewArray->ppData = malloc(sizeof(DATA_T*) * capacity);
            if (pNewArray->ppData == NULL){
                free(pNewArray);
            }
            else {
                pNewArray->size = 0;
                pNewArray->capacity = capacity;
                for (SIZE_T i = 0; i < get_capacity_array(pNewArray); i++){
                    pNewArray->ppData[i] = NULL;
                }
            }
        }
    }
    return pNewArray;
}
int isEmpty_array(struct array* pArray){
    int isEmpty = 0;
    if (get_size_array(pArray) == 0){
        isEmpty = 1;
    }
    return isEmpty;
}
int isValid_offset_array(struct array* pArray, SIZE_T offset){
    int isValid = 0;
    if (offset >= get_size_array(pArray)){
        ;
    }
    else {
        isValid = 1;
    }
    return isValid;
}
int isIdentical_arrays_array(const struct array* pArray_1, const struct array* pArray_2, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref)){
    int isIdentical = 1;
    if (get_size_array(pArray_1) == get_size_array(pArray_2)){
        for (SIZE_T i = 0; i < pArray_1->size; i++){
            if (isIdentical_pData(get_pData_array(pArray_1, i), get_pData_array(pArray_2, i))){
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
DATA_T* get_pData_array(const struct array* pArray, const SIZE_T offset){
    DATA_T* pData = NULL;
    if (isValid_offset_array((struct array*)pArray, offset)){
        pData = pArray->ppData[offset];
    }
    return pData;
}
SIZE_T get_size_array(const struct array* pArray){
    return pArray->size;
}
SIZE_T get_capacity_array(const struct array* pArray){
    return pArray->capacity;
}
SIZE_T get_offset_last_array(const struct array* pArray){
    return get_size_array(pArray) - 1;
}
int copy_FORCE_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData)){
    int success = 1;
    if (get_capacity_array(pArray_dest) < (SIZE_T)offset_start_dest + (SIZE_T)size || get_capacity_array(pArray_src) < (SIZE_T)offset_start_src + (SIZE_T)size){
        success = 0;
    }
    else {
        if (get_size_array(pArray_dest) < (SIZE_T)offset_start_dest + (SIZE_T)size){
            pArray_dest->size = (SIZE_T)offset_start_dest + (SIZE_T)size;
        }
        if (duplicate_deepCopy_pData == NULL){
            for (SIZE_T i = (SIZE_T)offset_start_dest, u = (SIZE_T)offset_start_src; i < (SIZE_T)size + (SIZE_T)offset_start_dest; i++, u++){
                pArray_dest->ppData[i] = get_pData_array(pArray_src, u);
            }
        }
        else {
            for (SIZE_T i = (SIZE_T)offset_start_dest, u = (SIZE_T)offset_start_src; i < (SIZE_T)size + (SIZE_T)offset_start_dest; i++, u++){
                pArray_dest->ppData[i] = duplicate_deepCopy_pData(get_pData_array(pArray_src, u));
                if (pArray_dest->ppData[i] == NULL){
                    for (; i >= 0; i--){
                        free_pData(get_pData_array(pArray_dest, i)); 
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
    int success = copy_FORCE_array(pArray_dest, pArray_src, 0, 0, get_size_array(pArray_src), duplicate_deepCopy_pData, free_pData);
    return success;
}
struct array* duplicate_selective_array(const struct array* pArray, SIZE_T offset_start, SIZE_T offset_end, int doInherit_capacity, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData)){
    SIZE_T size_toCopy = offset_end - offset_start + 1;
    if (doInherit_capacity){
        size_toCopy = get_capacity_array(pArray);
    }
    struct array* pNewArray = initialize_array(size_toCopy);
    if (pNewArray != NULL){
        pNewArray->size = size_toCopy;
        copy_FORCE_array(pNewArray, pArray, 0, offset_start, size_toCopy, duplicate_deepCopy_pData, free_pData);
    }
    return pNewArray;

}
struct array* duplicate_identical_array(const struct array* pArray, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData)){
    struct array* pNewArray = duplicate_selective_array(pArray, 0, get_offset_last_array(pArray), 1, duplicate_deepCopy_pData, free_pData);
    return pNewArray;
}
struct array* merge_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2){
    if (pArray_dest != NULL){
        copy_FORCE_array(pArray_dest, pArray_1, 0, 0, get_size_array(pArray_1), NULL, NULL);
        copy_FORCE_array(pArray_dest, pArray_2, get_size_array(pArray_1), 0, get_size_array(pArray_2), NULL, NULL);
    }
    return pArray_dest;
}
struct array* merge_FIFO_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    if (pArray_dest == NULL){
        SIZE_T newcapacity = get_capacity_array(pArray_1) + get_capacity_array(pArray_2);
        SIZE_T newsize = get_size_array(pArray_1) + get_size_array(pArray_2);
        pArray_dest = initialize_array(newcapacity);
        if (pArray_dest != NULL){
            pArray_dest->size = newsize;
            pArray_dest->capacity = newcapacity;
        }
    }
    if (pArray_dest != NULL){
        SIZE_T offset_mergeProgress = get_offset_last_array(pArray_dest);
        while (!isEmpty_array(pArray_1) && !isEmpty_array(pArray_2)){
            DATA_T* pData_1 = get_pData_array(pArray_1, get_offset_last_array(pArray_1));
            DATA_T* pData_2 = get_pData_array(pArray_2, get_offset_last_array(pArray_2));
            if(isLarger_pData(pData_1, pData_2)){
                pArray_dest->ppData[offset_mergeProgress] = pData_1;
                pArray_1->size--;
            }
            else {
                pArray_dest->ppData[offset_mergeProgress] = pData_2;
                pArray_2->size--;
            }
            offset_mergeProgress--;
        }
        copy_array(pArray_dest, pArray_1, NULL, NULL);
        copy_array(pArray_dest, pArray_2, NULL, NULL);
    }
    return pArray_dest;
}
struct array_twin* split_array(struct array* pArray, int isDestroyParent){
    SIZE_T different_offset = get_size_array(pArray);
    SIZE_T offset_1_start = 0;
    SIZE_T offset_1_end = different_offset / 2 - 1;
    SIZE_T offset_2_start = different_offset / 2;
    SIZE_T offset_2_end = get_offset_last_array(pArray);
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
int insert_array(struct array* pArray, const DATA_T* pData){
    int isNotFull = 0;
    if (get_size_array(pArray) == get_capacity_array(pArray)){
        ;
    }
    else {
        isNotFull = 1;
        pArray->ppData[get_size_array(pArray)] = (DATA_T*)pData;
        pArray->size++;
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
        DATA_T* pTempData = get_pData_array(pArray, offset_1);
        pArray->ppData[offset_1] = get_pData_array(pArray, offset_2);
        pArray->ppData[offset_2] = pTempData;
    }
    return isValid_offset;
}
SIZE_T query_bruteForce_array(const struct array* pArray, const DATA_T* pData_ref, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref)){
    SIZE_T offset = get_capacity_array(pArray);
    for (SIZE_T i = 0; i < get_size_array(pArray); i++){
        if (isIdentical_pData(get_pData_array(pArray, i), (DATA_T*)pData_ref)){
            offset = i;
            break;
        }
    }
    return offset;
}
SIZE_T query_binarySearch_array(const struct array* pArray, const DATA_T* pData_ref, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    SIZE_T offset = get_capacity_array(pArray);
    SIZE_T offset_start = 0;
    SIZE_T offset_end = get_offset_last_array(pArray);
    while (offset_start != offset_end){
        SIZE_T offset_middle = (offset_start + offset_end) / 2;
        if (isLarger_pData(get_pData_array(pArray, offset_middle), (DATA_T*)pData_ref)){
            offset_start = offset_middle + 1;
        }
        else {
            offset_end = offset_middle - 1;
        }
    }
    if (isIdentical_pData(get_pData_array(pArray, offset_start), (DATA_T*)pData_ref)){
        offset = offset_start;
    }
    return offset;
}
void sort_bubbleSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    for (SIZE_T i = 1; i < get_size_array(pArray); i++){
        int isSorted = 1;
        for (SIZE_T u = 1; u < get_size_array(pArray) - i; u++){
            if (isLarger_pData(get_pData_array(pArray, u - 1), get_pData_array(pArray, u))){
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
    for (SIZE_T i = 0; i < get_size_array(pArray); i++){
        int isSorted = 1;
        SIZE_T offset_minKey = i;
        for (SIZE_T u = 1 + i; u < get_size_array(pArray); u++){
            if (isLarger_pData(get_pData_array(pArray, offset_minKey), get_pData_array(pArray, u))){
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
    for (SIZE_T i = 1; i < get_size_array(pArray); i++){
        DATA_T* pTempData = get_pData_array(pArray, i);
        SIZE_T u = i - 1;
        for (; u >= 0 && u < get_size_array(pArray) && isLarger_pData(get_pData_array(pArray, u), pTempData); u--){
            pArray->ppData[u + 1] = get_pData_array(pArray, u);
        }
        pArray->ppData[u + 1] = pTempData;
    }
}
int sort_quickSort_array(struct array* pArray, SIZE_T offset_start, SIZE_T offset_end,int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLesser_pData)(DATA_T* pData, DATA_T* pData_ref)){
    int isSuccess = 1;
    SIZE_T size = offset_end - offset_start + 1;
    if (size > 1){
        SIZE_T offset_newPivot = partition_hoarePartition_selective_array(pArray, offset_start, offset_end, offset_end, isLarger_pData, isLesser_pData);
        if (!isValid_offset_array(pArray, offset_newPivot)){
            isSuccess = 0;
        }
        else {
            isSuccess = sort_quickSort_array(pArray, offset_start, offset_newPivot - 1, isLarger_pData, isLesser_pData);
            if (isSuccess){
                isSuccess = sort_quickSort_array(pArray, offset_newPivot + 1, offset_end, isLarger_pData, isLesser_pData);
            }
        }
    }
    return isSuccess;
}
int sort_mergeSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    int isSuccess = 1;
    if (get_size_array(pArray) > 1){
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
                   else {
                    isSuccess = 0;
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
        for (SIZE_T i = 0; i < get_size_array(pArray); i++){
            free_pData(get_pData_array(pArray, i));
        }
    }
    if (pArray != NULL){
        free(pArray->ppData);
        free(pArray);
    }
}
void free_array_twin(struct array_twin* pTwin, void(*free_pData)(DATA_T* pData)){
    if (free_pData != NULL){
        if (pTwin != NULL){
            free_array(pTwin->pArray_1, free_pData);
            free_array(pTwin->pArray_2, free_pData);
        }
    }
    free(pTwin);
}
void randomize_array(struct array* pArray){
    for (SIZE_T i = 0; i < get_size_array(pArray); i++){
        if (rand() % 2 - 1){
            swap_array(pArray, i, rand() % get_size_array(pArray));
        }
    }
}
SIZE_T partition_hoarePartition_selective_array(struct array* pArray, SIZE_T offset_start, SIZE_T offset_end, SIZE_T offset_pivot, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLesser_pData)(DATA_T* pData, DATA_T* pData_ref)){
    if (!isValid_offset_array(pArray, offset_start) || !isValid_offset_array(pArray, offset_end) || !isValid_offset_array(pArray, offset_pivot)){
        offset_pivot = get_capacity_array(pArray);
    }
    else {
        swap_array(pArray, offset_pivot, offset_end);
        offset_pivot = offset_end;
        SIZE_T size = offset_end - offset_start + 1;
        if (size > 1){
            SIZE_T offset_left = offset_start;
            SIZE_T offset_right = offset_end;
            DATA_T* pData_pivot = get_pData_array(pArray, offset_pivot);
            enum mode{
                LEFT,
                RIGHT,
                SWAP
            };
            enum mode mode = LEFT;
            while (offset_left < offset_right){
                switch (mode)
                {
                case LEFT:
                    mode = RIGHT;
                    for (SIZE_T i = 0; i < size; i++){
                        offset_left = offset_start + i;
                        if (!isLesser_pData(get_pData_array(pArray, offset_left), pData_pivot)){
                            break;
                        }
                        else if (offset_left >= offset_right){
                            break;
                        }
                    }
                    break;
                case RIGHT:
                    mode = SWAP;
                    for (SIZE_T i = 0; i < size - 1; i++){
                        offset_right = offset_end - i - 1;
                        if (!isLarger_pData(get_pData_array(pArray, offset_right), pData_pivot)){
                            break;
                        }
                        else if (offset_left >= offset_right){
                            break;
                        }
                    }
                    break;
                case SWAP:
                    mode = LEFT;
                    swap_array(pArray, offset_left, offset_right);
                    break;
                default:
                    break;
                }
            }
            swap_array(pArray, offset_pivot, offset_right);
            offset_pivot = offset_right;
        }
    }
    return offset_pivot;
}
SIZE_T partition_hoarePartition_array(struct array* pArray, SIZE_T offset_pivot, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLesser_pData)(DATA_T* pData, DATA_T* pData_ref)){
    return partition_hoarePartition_selective_array(pArray, 0, get_offset_last_array(pArray), offset_pivot, isLarger_pData, isLesser_pData);
}
void debug_array(const struct array* pArray, const char* message, char*(*toString_pData)(DATA_T* pData)){
    fprintf(stderr, "%s: ", message);
    putc('{', stderr);
    for (SIZE_T i = 0; i < get_offset_last_array(pArray); i++){
        if (get_pData_array(pArray, i) == NULL){
            fprintf(stderr, "[NULL], ");
        }
        else {
            char* pChar_pData = toString_pData(get_pData_array(pArray, i));
            fprintf(stderr, "%s, ", pChar_pData);
            free(pChar_pData);
        }
    }
    if (get_size_array(pArray) > 0){
        if (get_pData_array(pArray, get_offset_last_array(pArray)) == NULL){
            fprintf(stderr, "[NULL], ");
        }
        else {
            char* pChar_pData = toString_pData(get_pData_array(pArray, get_offset_last_array(pArray)));
            fprintf(stderr, "%s", pChar_pData);
            free(pChar_pData);
        }
    }
    putc('}', stderr);
    putc('\n', stderr);
}

#undef DATA_T
#undef SIZE_T