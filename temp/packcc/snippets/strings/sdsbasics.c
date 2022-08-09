#include <stdio.h>

#include "sds.h"

/* SDS String Mini-Reference

  SDS string length:
    static inline size_t sdslen(const sds s);

  Create new SDS string and copy ASCII-Z string:
    sds sdsnew(const char *init);

  Allocate empty SDS string:
    sds sdsempty(void);

  [Allocate] and copy SDS string:
    sds sdsdup(const sds s);

  Release SDS string:
    void sdsfree(sds s);

  [Allocate] SDS string and append ASCII-Z characters up to len:
    sds sdscatlen(sds s, const void *t, size_t len);

  [Allocate] SDS string and append ASCII-Z string:
    sds sdscat(sds s, const char *t);

  [Allocate] SDS string and append ASCII-Z string:
    sds sdscatsds(sds s, const sds t);

  [Allocate] SDS string and copy ASCII-Z characters up to len:
    sds sdscpylen(sds s, const char *t, size_t len);

  [Allocate] SDS string and copy ASCII-Z characters:
    sds sdscpy(sds s, const char *t);

  [Allocate] SDS string and append a printf specifier formatted string:
    sds sdscatprintf(sds s, const char *fmt, ...);

  [Allocate] SDS string and append subset of printf-like formatted string:
    sds sdscatfmt(sds s, char const *fmt, ...);

  Remove specified character set from left and right of the SDS string:
    sds sdstrim(sds s, const char *cset);

  Reduce SDS string to the specified index range (negatives wrap around):
    void sdsrange(sds s, int start, int end);

  Reduce SDS string to an empty string:
    void sdsclear(sds s);

  Compare SDS strings:
    int sdscmp(const sds s1, const sds s2);

  Allocate an array of SDS strings according to specified separators:
    sds *sdssplitlen(const char *s, int len,
      const char *sep, int seplen, int *count);

  Release memory used by SDS string array:
    void sdsfreesplitres(sds *tokens, int count);

  Set SDS string to all lower case:
    void sdstolower(sds s);

  Set SDS string to all upper case:
    void sdstoupper(sds s);

  Directly allocate an SDS string converted from a numeric value:
    sds sdsfromlonglong(long long value);

  Replace specified characters in the SDS string to the replacement set:
    sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);
 */

int main( int argc, char** argv ) {
  int identCount = 0;
  sds parsedIdent = sdsnew("  foo .bar. blah ");
  sds* qualifiedIdent = sdssplitlen(parsedIdent, sdslen(parsedIdent), ".", 1, &identCount);

  int index = 0;
  while( index < identCount ) {
    qualifiedIdent[index] = sdstrim(qualifiedIdent[index], " ");
    printf( "[%s]\n", qualifiedIdent[index++] );
  }

  return 0;
}

#include "sds.c"
