/* Coral library for platform compatibility
 Copyright (C) 1996-2018 AdaptiveFlow
 ( adaptiveflow@gmail.com )

 All files contained in Coral library are dual-licensed:

 GNU General Public License version 3 as published by
the Free Software Foundation. For the terms of this 
license, see <http://www.gnu.org/licenses/>.

 Alternatively, you can use these files under your own
license if you have written permission of the copyright
holder or corresponding equivalent conditions.
Refer to <http://www.orionids.org/projects/license.txt> */

/* language neutral preprocessor constants */

#ifndef __CORAL_CONST_H
#	define __CORAL_CONST_H


/* Generic constants */

#define SUCCESS_EXIST       1     /* an object is already present */
#define SUCCESS_NORMAL      0     /* normal result */
#define FAIL_IO            -1     /* io operation error */
#define FAIL_ENTRY         -2     /* file or directory entry node was not found */
#define FAIL_MEMORY        -3     /* memory allocation error */
#define FAIL_OVERFLOW      -4     /* the given buffer is full and no space to store data */
#define FAIL_UNDERFLOW     -5
#define FAIL_PARAM         -6     /* invalid parameter is present */
#define FAIL_UNAVAILABLE   -7     /* the resource is unavailable */
#define FAIL_DUPLICATED    -8     /* the resource is duplicated ( the resource should be unique ) */
#define FAIL_UNKNOWN       -9     /* the reason was unknown */
#define FAIL_PARSE         -10    /* parse error */
#define FAIL_PENDING       -11    /* requested operation is pending now */
#define FAIL_SIGNATURE     -12    /* signature mismatch */
#define FAIL_NOSPACE       -13    /* external memory no space to store data */
#define FAIL_EXECUTE       -14    /* execution failed */
#define FAIL_MISMATCH      -15    /* information mismatched */
#define FAIL_CONNECTION    -16    /* connection problem */
#define FAIL_RESERVED    -2048    /* reserved error codes */

/* general purpose flags */

#define MASK_COMBINATED_SIZE   0x00ff  /* to get combinated size : up to 255 can be combinated */
#define MASK_BYTE_ORDER        0x3000  /* to get byte order instructions */
#define BO_LITTLE_ENDIAN       0x1000
#define BO_BIG_ENDIAN          0x2000
#define BO_PDP_ENDIAN          0x3000
#define BO_NETWORK             BO_BIG_ENDIAN
#define FLAG_READ              0x0001
#define FLAG_WRITE             0x0002
#define FLAG_ERROR             0x0004
#define FLAG_ASYNC             0x0008
#define FLAG_BUNDLE            0x0010
#define FLAG_TEMPORARY         0x4000
#define FLAG_DEFAULT           0x0000
#define FLAG_CLEAR             0x0001
#define FLAG_BUFFER_EXPANSION  0x0002    /* allow buffer expansion */
#define FLAG_PRIVATE           0x0004    /* private usage */
#define FLAG_SPECIAL           0x8000    /* a special value */

#define FLAG_ROW_VARIABLE      0x4000
#define FLAG_COL_VARIABLE      0x8000

#define FLAG_POINTER           0x1000  /* pointer in a class */
#define FLAG_CASE_INSENSITIVE  0x2000  /* case insensitive string comparison */

#ifndef FLAG_PATH_NAME
#	define FLAG_PATH_NAME FLAG_CASE_INSENSITIVE
#endif

#define FLAG_DBCS              0x4000 /* double-byte character system */


/* DL subsystem details */
#define SS_JAR  0
#define SS_JAVA 1

/* I/O properties */
#define IO_DEFAULT         0      /* generic synchronous I/O */
#define IO_PIPE             0x0100 /* a pipe */
#define IO_SOCKET           0x0200 /* network socket */
#define IO_SOCKET_SERVER    0x0300 /* network server */
#define IO_SERIAL           0x0400 /* serial communication */
#define IO_EXTENSION        0x2000 /* start of I/O extension */
#define IO_REGISTRY         0x3e00
#define IO_BUNDLE           0x3f00
#define IO_MASK_PROPERTY    ( 0x3f00 | IO_FLAG_SCAN ) /* property mask */
#define IO_MASK_FLAG        0xC000 /* flag mask */
#define IO_FLAG_OVERRIDE    0x4000 /* overriden I/O */
#define IO_FLAG_SCAN        0x8000
#define IO_FLAG_ACCEPTED    0x8000
#define IO_FLAG_EXTRA_PARAM 0x0080 /* extra parameter is present for deviceObject */


/* fileOpen, opening modes */
/* FLAG_READ, reading mode
   FLAG_WRITE, writing mode
   FLAG_ASYNC,
   FLAG_BUNDLE,
   PROCESS_FILE_PATH
   PROCESS_FLAG_RESOLVE_PATH */
#define FILE_FLAG_OVERWRITE        0x0004  /* overwrite the existing file ( FLAG_ERROR is not used so this value can be 0x0004 ) */
#define FILE_FLAG_TRUNCATE         0x0020  /* truncate file for overwriting mode */
#define FILE_FLAG_EXISTENCE        0x0040  /* file existence test should be done for writing mode : FILE_FLAG_OVERWRITE is meaningless because overwriting is default for this option */
#define FILE_FLAG_CREATE_DIRECTORY 0x0080  /* create directory if needed */
#define FILE_FLAG_APPEND           0x0100 /* append to the end of output file : output offset must not be supplied */



#define ASYNC_STATE_MAX      5     /* max of async states */
#define DL_EVENT             0xff00 /* start of dl events */
#define DL_ERROR             0xff64 /* beginning of error codes */
#define IO_SERVER_DISABLED   0xff65 /* server don't work */

#define IO_OPEN              0xff27
#define IO_CLOSE             0xff28 /* no param : iob.context is released */
#define IO_SEEK              0xff29
#define IO_START_SCAN        0xff2d
#define IO_STOP_SCAN         0xff2e

#define PROCESS_FLAG_ABNORMAL    0x0001 /* abnormal termination */
#define PROCESS_FLAG_USER        0x0002 /* terminated by user */
#define PROCESS_FLAG_BACKGROUND  0x0004 /* background task termination */
#define PROCESS_FLAG_CONTINUE    0x0008 /* continue termination */

/* PROCESS_MESSAGE */
#define PROCESS_USER_MESSAGE          0
#define PROCESS_BOOT_COMPLETED        1
#define PROCESS_SECRET_CODE           2
#define PROCESS_APPLICATION_INSTALLED 3
#define PROCESS_APPLICATION_REPLACED  4
#define PROCESS_NETWORK_STATE         5

#define ASYNC_CHANNEL_0     0   /* channel 0 event */
#define ASYNC_CHANNEL_1     1   /* channel 1, extra channel event */


#define TALK_SMS   0x8001
#define TALK_VOICE 0x8002

#define DL_STRING_LENGTH_SIZE    2

#endif /* __CORAL_CONST_H */

