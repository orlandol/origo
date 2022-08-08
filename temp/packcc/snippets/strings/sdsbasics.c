#include <stdio.h>

#include "sds.h"

/* Mini-ref:

  String Length:
    static inline size_t sdslen(const sds s);

  Create New SDS String And Copy ASCII-Z String:
    sds sdsnew(const char *init);

  Allocate Empty SDS String:
    sds sdsempty(void);

  [Allocate] And Copy SDS String:
    sds sdsdup(const sds s);

  Release SDS String:
    void sdsfree(sds s);

  [Allocate] SDS String And Append ASCII-Z Characters Up To len:
    sds sdscatlen(sds s, const void *t, size_t len);

  [Allocate] SDS String And Append ASCII-Z String:
    sds sdscat(sds s, const char *t);

  [Allocate] SDS String And Append ASCII-Z String:
    sds sdscatsds(sds s, const sds t);

  [Allocate] SDS String And Copy ASCII-Z Characters Up To len:
    sds sdscpylen(sds s, const char *t, size_t len);

  [Allocate] SDS String And Copy ASCII-Z Characters:
    sds sdscpy(sds s, const char *t);

  [Allocate] And Append(?) Formatted SDS String Using printf Specifiers:
    sds sdscatprintf(sds s, const char *fmt, ...);

  [Allocate] SDS String And Append Formatted
    sds sdscatfmt(sds s, char const *fmt, ...);

  Remove Specified Character Set From Left And Right Of SDS String:
    sds sdstrim(sds s, const char *cset);

  Remove Specified Characters Within Index Range of SDS String:
    void sdsrange(sds s, int start, int end);

  Reduce String To An Empty String:
    void sdsclear(sds s);

  Compare SDS Strings:
    int sdscmp(const sds s1, const sds s2);

    sds *sdssplitlen(const char *s, int len,
      const char *sep, int seplen, int *count);
    void sdsfreesplitres(sds *tokens, int count);
    void sdstolower(sds s);
    void sdstoupper(sds s);
    sds sdsfromlonglong(long long value);
    sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);
    sds sdsjoin(char **argv, int argc, char *sep);
    sds sdsjoinsds(sds *argv, int argc, const char *sep, size_t seplen);
 */

int main( int argc, char** argv ) {
  int identCount = 0;
  sds* qualifiedIdent = sdssplitlen("foo.bar.blah", 12, ".", 1, &identCount);

  int index = 0;
  while( index < identCount ) {
    printf( "[%s]\n", qualifiedIdent[index++] );
  }

  return 0;
}

#include "sds.c"
