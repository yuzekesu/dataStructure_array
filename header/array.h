#ifndef ARRAY_TEMPLATE_H
#define ARRAY_TEMPLATE_H
#define DATA_T void
#define SIZE_T int

struct array{
    DATA_T** ppData;
    SIZE_T size_current;
    SIZE_T size_max;
};

struct array* initialize_array(SIZE_T size_max);
struct array* copy_shallowCopy_array(const struct array* pArray);
struct array* copy_deepCopy_array(const struct array* pArray, DATA_T*(*copy_deepCopy_pData)(DATA_T* pData));
struct array* merge_array(struct array* pArray_1, struct array* pArray_2);
int isValid_offset_array(struct array* pArray, SIZE_T offset);
int isIdentical_pData_array(const struct array* pArray_1, const struct array* pArray_2, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref));
int insert_array(struct array* pArray, const DATA_T* pData);
int modify_array(struct array* pArray, const DATA_T* pData, SIZE_T offset);
int swap_array(struct array* pArray, SIZE_T offset_1, SIZE_T offset_2);
DATA_T* access_array(const struct array* pArray, SIZE_T offset);
SIZE_T query_bruteForce_array(const struct array* pArray, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref), const DATA_T* pData_ref);
SIZE_T query_binarySearch_array(const struct array* pArray, int(*isIdentical_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLarger_pData)(DATA_T* pData), const DATA_T* pData_ref);
void sort_bubbleSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
void sort_selectionSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
void sort_insertionSort_array(struct array* pArray, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref), int(*isLesser_pData)(DATA_T* pData, DATA_T* pData_ref));
void sort_quickSort_array(struct array* pArray, SIZE_T offset_start, SIZE_T offset_end,int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
void sort_mergeSort_array(struct array* pArray, SIZE_T offset_start, SIZE_T offset_end, int(*isLarger_pData)(DATA_T* pData, DATA_T* pData_ref));
void free_array(struct array* pArray, void(*free_pData)(DATA_T* pData));
void debug_array(const struct array* pArray, char*(*convertToString_pData)(DATA_T* pData), const char* message);

#undef DATA_T
#undef SIZE_T
#undef DEFINE_TYPE_FOR_KEY_AND_DATA
#endif