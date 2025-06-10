#include "..\header\array.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define DATA_T void 
#define SIZE_T unsigned long long int 
#define SIZE_CAP ULLONG_MAX

// PRIVATE-----------------------------------------------
/*
Template for copy operation. Force or non-Force.
The size will be increased if necessary during the operation (not capacity).
*/
int copy_TEMPLATE_selective_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, int enable_overwrite, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData));
// overwriting enabled copy, be careful
int copy_FORCE_selective_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData));
// overwriting enabled copy, be careful
int copy_FORCE_array(struct array* pArray_dest, const struct array* pArray_src, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData));
// PUBLIC-----------------------------------------------
struct array* initialize_array(SIZE_T capacity){
    struct array* pNewArray = NULL;

    // checking. the size of the new array shall not exceed the the LIMIT of that data type minus one(-1). Becuase of increaments in a FOR loop.

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

                // initializes the elements with NULL ptr. ease any debugging method. 

                for (SIZE_T i = 0; i < get_capacity_array(pNewArray); i++){
                    pNewArray->ppData[i] = NULL; // dont touch this !!!
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
    if (offset < get_size_array(pArray)){
        isValid = 1;
    }
    return isValid;
}
int isIdentical_arrays_array(const struct array* pArray_1, const struct array* pArray_2, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref)){
    int isIdentical = 1;

    // only check if within the "size", not "capacity".

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
        pData = pArray->ppData[offset]; // dont change this !!!
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
int put_pData_array(struct array* pArray, SIZE_T offset, const DATA_T* pData){
    int isValid_offset = 0;
    if (isValid_offset_array(pArray, offset)){

        // good for initialize arrays. then elements are all NULL ptrs. 

        if (pArray->ppData[offset] == NULL){
            isValid_offset = 1;
            pArray->ppData[offset] = (DATA_T*)pData;
        }
    }
    return isValid_offset;
}
int insert_array(struct array* pArray, const DATA_T* pData){
    int isSuccess = 0;
    if (get_size_array(pArray) < get_capacity_array(pArray)){
        isSuccess = 1;
        // pArray->ppData[get_size_array(pArray)] = (DATA_T*)pData;
        // we have to eliminate possiblities of losing track of pointers.  
        pArray->size++;
        isSuccess = put_pData_array(pArray, get_size_array(pArray) - 1, pData);
        if (!isSuccess){
            pArray->size--;
        }
    }
    return isSuccess;
}
int replace_array(struct array* pArray, SIZE_T offset, DATA_T* pData, void(*free_pData)(DATA_T* pData)){
    int isValid_offset = 0;
    if (isValid_offset_array(pArray, offset)){
        isValid_offset = 1;

        // only do the replacement when the function pointer "free_pData" is provided.

        if (free_pData != NULL){
            free_pData(get_pData_array(pArray, offset));
            pArray->ppData[offset] = NULL;   
        }

        // return 0 (failure), if the destination element is not NULL ptr.  

        isValid_offset = put_pData_array(pArray, offset, pData);
    }
    return isValid_offset;
}
int swap_array(struct array* pArray, const SIZE_T offset_1, const SIZE_T offset_2){
    int isValid_offset = 0;
    if (isValid_offset_array(pArray, offset_1) && isValid_offset_array(pArray, offset_2)){
        isValid_offset = 1;
        DATA_T* pTempData = get_pData_array(pArray, offset_1);

        // REMINDER: dont capsulate with "replace_array" here, not necessary. 

        pArray->ppData[offset_1] = get_pData_array(pArray, offset_2); // dont touch this !!!
        pArray->ppData[offset_2] = pTempData;   // dont touch this !!!
    }
    return isValid_offset;
}
int copy_TEMPLATE_selective_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, int enable_overwrite, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData)){
    int isSuccess = 1;

    // checking. the copy area must be within the valid area.

    if (get_capacity_array(pArray_dest) < (SIZE_T)offset_start_dest + (SIZE_T)size || get_capacity_array(pArray_src) < (SIZE_T)offset_start_src + (SIZE_T)size){
        isSuccess = 0;
    }
    else {

        // !!! if updating "size" is needed for the destination array. since the destination array could be a empty array at first place. 

        if (get_size_array(pArray_dest) < (SIZE_T)offset_start_dest + (SIZE_T)size){
            pArray_dest->size = (SIZE_T)offset_start_dest + (SIZE_T)size;
        }

        // starts copy here

        for (SIZE_T i = (SIZE_T)offset_start_dest, u = (SIZE_T)offset_start_src; i < (SIZE_T)size + (SIZE_T)offset_start_dest; i++, u++){
            DATA_T* pData_copy = get_pData_array(pArray_src, u);

            // deep copy or not

            if (duplicate_deepCopy_pData == NULL){
                pData_copy = get_pData_array(pArray_src, u);
            }
            else {
                pData_copy = duplicate_deepCopy_pData(get_pData_array(pArray_src, u));
            }

            // !!! the copied element cannot be NULL what so ever.

            if (pData_copy != NULL){
                    if (enable_overwrite){
                        pArray_dest->ppData[i] = pData_copy;
                    }
                    else {
                        isSuccess = put_pData_array(pArray_dest, i, pData_copy);
                        if (!isSuccess){
                            break;
                        }
                    } 
                }
                else {
                    isSuccess = 0;
                    if (free_pData != NULL){
                        for (SIZE_T k = 0; k < i; k++){
                            // destroy all created new pDatas if any deep copy failed. 
                            free_pData(get_pData_array(pArray_dest, k)); 
                            pArray_dest->ppData[k] = NULL;  
                        }
                    }
                    break;
                }
        }
    }
    return isSuccess;
}
int copy_selective_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData)){
    return copy_TEMPLATE_selective_array(pArray_dest, pArray_src, offset_start_dest, offset_start_src, size, 0, duplicate_deepCopy_pData, free_pData);
}
int copy_array(struct array* pArray_dest, const struct array* pArray_src, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData)){
    return copy_selective_array(pArray_dest, pArray_src, 0, 0,get_size_array(pArray_dest), duplicate_deepCopy_pData, free_pData);
}
int copy_FORCE_selective_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData)){
    return copy_TEMPLATE_selective_array(pArray_dest, pArray_src, offset_start_dest, offset_start_src, size, 1, duplicate_deepCopy_pData, free_pData);
}
int copy_FORCE_array(struct array* pArray_dest, const struct array* pArray_src, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData)){
    int success = copy_FORCE_selective_array(pArray_dest, pArray_src, 0, 0, get_size_array(pArray_src), duplicate_deepCopy_pData, free_pData);
    return success;
}
struct array* duplicate_selective_array(const struct array* pArray, SIZE_T offset_start, SIZE_T offset_end, int doInherit_capacity, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData)){
    SIZE_T size_toCopy = offset_end - offset_start + 1;
    if (doInherit_capacity){
        size_toCopy = get_capacity_array(pArray);
    }
    struct array* pNewArray = initialize_array(size_toCopy);
    if (pNewArray != NULL){
        pNewArray->size = size_toCopy;
        int isSuccess = copy_selective_array(pNewArray, pArray, 0, offset_start, size_toCopy, duplicate_deepCopy_pData, free_pData);
        if (!isSuccess){

            // here, the argument has function pointer to be NULL. because the "copy_FORCE_selective_array" above wont create any new pDatas if any of those falied to be deep copied.
            
            free_array(pNewArray, NULL);
        }
    }
    return pNewArray;

}
struct array* duplicate_identical_array(const struct array* pArray, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void(*free_pData)(DATA_T* pData)){
    struct array* pNewArray = duplicate_selective_array(pArray, 0, get_offset_last_array(pArray), 1, duplicate_deepCopy_pData, free_pData);
    return pNewArray;
}
int merge_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2){
    int isSuccess = 0;
    if (pArray_dest != NULL){

        // need to make sure the size is enough for both of the source arrays.

        SIZE_T size_merged = get_size_array(pArray_1) + get_size_array(pArray_2);
        if (get_capacity_array(pArray_dest) <= size_merged){
            isSuccess = copy_selective_array(pArray_dest, pArray_1, 0, 0, get_size_array(pArray_1), NULL, NULL);
            if (isSuccess){
                isSuccess = copy_selective_array(pArray_dest, pArray_2, get_size_array(pArray_1), 0, get_size_array(pArray_2), NULL, NULL);
            }
        }
    }
    return isSuccess;
}
int merge_priority_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref)){
    int isSuccess = 0;
    if (pArray_dest != NULL){

        // need to make sure the size is enough for both of the source arrays.

        SIZE_T size_merged = get_size_array(pArray_1) + get_size_array(pArray_2);
        if (get_capacity_array(pArray_dest) <= size_merged){

            // check manuelly if the size need to be changed or not. because we are not using "copy" functions here to copy. This will never fail.

            if (get_size_array(pArray_dest) < size_merged){
                pArray_dest->size = size_merged;
            }

            // the special merging starts here. begin with the last offset. 

            SIZE_T offset_mergeProgress = get_offset_last_array(pArray_dest);
            while (isSuccess && !isEmpty_array(pArray_1) && !isEmpty_array(pArray_2)){
                DATA_T* pData_1 = get_pData_array(pArray_1, get_offset_last_array(pArray_1));
                DATA_T* pData_2 = get_pData_array(pArray_2, get_offset_last_array(pArray_2));
                if (pData_1 == NULL || pData_2 == NULL){
                    isSuccess = 0;
                    break;
                }
                // put the largest on the right most availabe offset of the array. Then decrease the counter by one. 

                if(isLarger_pData(pData_1, pData_2)){
                    isSuccess = put_pData_array(pArray_dest, offset_mergeProgress, pData_1);
                    pArray_1->size--;
                }
                else {
                    isSuccess = put_pData_array(pArray_dest, offset_mergeProgress, pData_2);
                    pArray_2->size--;
                }
                offset_mergeProgress--;
            }

            // copy the rest to the destination array. 

            if (isSuccess){
                isSuccess = copy_array(pArray_dest, pArray_1, NULL, NULL);
                if (isSuccess){
                    isSuccess = copy_array(pArray_dest, pArray_2, NULL, NULL);
            
                }
            }
        }
        
    }
    return isSuccess;
}
struct array_twin* split_array(struct array* pArray){
    SIZE_T different_offset = get_size_array(pArray);
    SIZE_T offset_1_start = 0;
    SIZE_T offset_1_end = different_offset / 2 - 1;
    SIZE_T offset_2_start = different_offset / 2;
    SIZE_T offset_2_end = get_offset_last_array(pArray);

    // nothing special. split a array to two at the middle. 

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
    return pTwin;
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
        struct array_twin* pTwin = split_array(pArray);
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
                    struct array* pArray_merged = initialize_array(get_capacity_array(pArray));
                    merge_priority_array(pArray_merged, pArray_1, pArray_2, isLarger_pData);
                    copy_FORCE_array(pArray, pArray_merged, NULL, NULL);
                    free_array(pArray_merged, NULL);
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
            fprintf(stderr, "[NULL]");
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