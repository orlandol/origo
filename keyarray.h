#ifndef KEYARRAY_H
#define KEYARRAY_H

/*
 * =================================
 *  String Key Array implementation
 * =================================
 */
  #define DECLARE_STRING_KEYARRAY_TYPE(typeName, dataType, dataFieldName)
  typedef struct typeName {
    char* key;
    dataType dataFieldName;
  } typeName;
  typedef struct StringKeyArray {
    size_t reservedCount;
    size_t itemCount;
    StringKeyArrayItem* item;
  } StringKeyArray;
  #define DECLARE_STRING_KEYARRAY_LISTTYPE(typeName, dataType)

/*
 * ===================================
 *  Unsigned Key Array implementation
 * ===================================
 */

#endif
