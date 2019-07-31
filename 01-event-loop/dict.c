/* Hash Tables Implementation. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>
#include <assert.h>
#include "dict.h"
//#include "zmalloc.h"

/*
 * 通过 dictEnableResize() 和 dictDisableResize() 两个函数.
 * 程序可以手动地允许或阻止哈希表进行 rehash
 */

