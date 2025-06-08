#ifndef ARRAY_TEMPLATE_H
#define ARRAY_TEMPLATE_H
#define DATA_T void 
#define SIZE_T long long int 
/*
Array that fits all data types.
*/
struct array{
    DATA_T** ppData;
    SIZE_T size_current;
    SIZE_T size_max;
};
struct array_twin{
    struct array* pArray_1;
    struct array* pArray_2;
};
/*
Malloc a pointer. 
->Return NULL upon failure [memory].
*/
struct array* initialize_array(SIZE_T size_max);
/*
Copy to existing array. Allow random access, use carefully.
No worry about overflow. Underflow need to be checked by user.
Deep_copies will be destroied properly.
->Return 0 upon failure [memory(deepCopy), overflow].
*/
int copy_force_array(struct array* pArray_dest, const struct array* pArray_src, const SIZE_T offset_start_dest, const SIZE_T offset_start_src, const SIZE_T size, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData));
/*
Copy to existing array. 
No worry about overflow & underflow.
Deep_copies will be destroied properly.
->Return 0 upon failure [memory(deepCopy), overflow].
*/
int copy_array(struct array* pArray_dest, const struct array* pArray_src, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData));
/*
Malloc and Duplicate "partial" of a existing array.
Deep_copies will be destroied properly if Malloc failed.
->Return NULL upon failure [memory].
*/
struct array* duplicate_selective_array(const struct array* pArray, SIZE_T offset_start, SIZE_T offset_end, int doInherit_size_max, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData));
/*
Malloc and Duplicate a existing array.
Deep_copies will be destroied properly if Malloc failed.
->Return NULL upon failure [memory].
*/
struct array* duplicate_identical_array(const struct array* pArray, DATA_T*(*duplicate_deepCopy_pData)(DATA_T* pData), void*(*free_pData)(DATA_T* pData));
/*
Merge two arrays.
Malloc if pArray_dest == NULL.
->Return NULL upon failure [memory].
*/
struct array* merge_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2);
/*
Merge two arrays, FIFO applies based on pData and offset.
Malloc if pArray_dest == NULL.
->Return NULL upon failure [memory].
*/
struct array* merge_FIFO_array(struct array* pArray_dest, struct array* pArray_1, struct array* pArray_2, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Malloc a pointer.
->Return NULL uppon failure [memory], nothing happens.
*/
struct array_twin* split_array(struct array* pArray, int isDestroyParent);
/*
->Return if the array is empty or not.
*/
int isEmpty_array(struct array* pArray);
/*
->Return if the offset is witin the range of the "size_current" or not.
*/
int isValid_offset_array(struct array* pArray, SIZE_T offset);
/*
->Return if two array are identical in "pData" and it's order or not, the "size_max" doesn't matter.
*/
int isIdentical_arrays_array(const struct array* pArray_1, const struct array* pArray_2, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Insert like stack.
->Return 0 upon failure.
*/
int insert_array(struct array* pArray, const DATA_T* pData);
/*
Modify the elements in the array. 
Does nothing if the offset is out of the range of "size_current".
->Return 0 upon failure.
*/
int modify_array(struct array* pArray, const DATA_T* pData, SIZE_T offset);
/*
Swap two elements in a array.
Does nothing if any of the offsets are out of the range of "size_current".
->Return 0 upon failure.
*/
int swap_array(struct array* pArray, SIZE_T offset_1, SIZE_T offset_2);
/*
Does nothing if the offset is out of the range of "size_current".
->Return the element stores on that offset.
*/
DATA_T* access_array(const struct array* pArray, SIZE_T offset);
/*
->Return "size_max" upon failure [can not find]. 
*/
SIZE_T query_bruteForce_array(const struct array* pArray, const DATA_T* pData_ref, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
->Return "size_max" upon failure [can not find]. 
*/
SIZE_T query_binarySearch_array(const struct array* pArray, const DATA_T* pData_ref, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Sort O(n^2).
*/
void sort_bubbleSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Sort O(n^2).
*/
void sort_selectionSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Sort O(n^2).
*/
void sort_insertionSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Sort O(n^2).
*/
void sort_quickSort_array(struct array* pArray, SIZE_T offset_start, SIZE_T offset_end,int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLesser_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Sort O(nlogn).
->Return 0 upon failure [memory].
*/
int sort_mergeSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
/*
Free the array. 
Destroy data if "free_pData" is not NULL. 
*/
void free_array(struct array* pArray, void(*free_pData)(DATA_T* pData));
/*
Free the twin. 
Destroy data and array if "free_pData" is not NULL. 
*/
void free_array_twin(struct array_twin* pTwin, void(*free_pData)(DATA_T* pData));
/*
Randomly swap the element. 
*/
void randomize_array(struct array* pArray);
/*
Display the array at "stderr". 
Exemple: "message: {1, 2, 3, ..., n}\n". 
The function ponter "toString_pData" cannot be NULL. 
*/
void debug_array(const struct array* pArray, const char* message, char*(*toString_pData)(DATA_T* pData));

#undef Array
#undef Array_Twin
#undef DATA_T
#undef SIZE_T
#undef DEFINE_TYPE_FOR_KEY_AND_DATA
#endif