#ifndef _TIZENUDPLIBRARY_H_
#define _TIZENUDPLIBRARY_H_

/**
 * This header file is included to define _EXPORT_.
 */
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// This method is exported from tizenudplibrary.a
extern int tizentizenudplibrary(void);
extern int tulSendAsync(char * token, char * message, int(* callBack)(int ,int));
#ifdef __cplusplus
}
#endif
#endif // _TIZENUDPLIBRARY_H_

