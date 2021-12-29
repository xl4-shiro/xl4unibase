## Prerequisites
C99 standard level library functions and C compiler functions must be supported to
build and use this library.

## overriding standard functions
Some platforms don't allow to use malloc.
In such case, malloc function must be overridden in user applicatoin.
```
#define malloc user_malloc
```
can replace 'malloc' to 'user_malloc', but 'user_malloc' definition must exist.

To accommodate such overriding of standard functions, 'unibase.h' has the next
statements at the very beginning of the file.
```
#ifdef UB_OVERRIDE_H
#include UB_OVERRIDE_H
#endif
```
adding
```
-DUB_OVERRIDE_H=\"user_override.h\"
```
in the preprocessor command line,
'user_override.h' is included, and the user can add any definition of
overriding in the file.

## initialization
The initialization is done by 'unibase_init', and it must be closed by
calling 'unibase_close'.

In the initialization parameters, there is a set of callback functions,
that support all of needed platform specific functions.
To be simple we minimized the callback functions.
For the detail of the callback functions, look at unibase.h

The other part of the initialization is defining logging levels.
The logging level can be initialized by a single string, and applications
can change it at run time.
For the detail of the logging control, look at the next section.

## ub_logging
When 'unibase_init' is called, 'ub_log_initstr' is passed.
E.G. in the gptp2d main function,
```
	ubb_default_initpara(&init_para);
	init_para.ub_log_initstr=UBL_OVERRIDE_ISTR("4,ubase:45,cbase:45,gptp:46", "UBL_GPTP");
	unibase_init(&init_para);
```
Here, "4,ubase:45,cbase:45,gptp:46" is a string to be passed, and an environment
variable: "UBL_GPTP" overrides the string if it is defined.

The logging is controlled per category, and each category has a console log and a memory log.
2 digits number defines the logging level for 'console log' and 'memory log'.
If it is 1 digit, the same number is applied on the both.

The logging levels are defined as follows:
NONE=0, FATAL=1, ERROR=2, WARN=3, INFO=4, INFOV=5, DEBUG=6, DEBUGV=7

In the example, "4,ubase:45,cbase:45,gptp:46"
+ the first comma separated section is '4', this is a defult logging level(4 is INFO level).
  The single digit number is applied on both 'console log' and 'memory log'.
+ the second section is 'ubase:45'.  This defines the logging for 'category 0'.
  The category name is 'ubase', 'console log' level is 4(INFO) and 'memory log' level is 5(INFOV)
+ the third section is 'cbase:45'.  This defines the logging for 'category 1'.
  The category name is 'cbase', 'console log' level is 4(INFO) and 'memory log' level is 5(INFOV)
+ the fourth section is 'gptp:46'.  This defines the logging for 'category 2'.
  The category name is 'gptp', 'console log' level is 4(INFO) and 'memory log' level is 6(DEBUG)
+ For 'category 3' and higher category number, the default is applied.
  The category name is 'def03', 'def04',,,,
'console log' level is 4(INFO) and 'memory log' level is 4(INFO)

Adding timestamp on the log messages is controlled by a flag:UB_LOGTSTYPE in 'ub_log_print',
UB_TLOG macro can be used conveniently.

When you want to supersede the flag and put timestamps on all log messages,
it can be done adding a suffix character on the 2-digit number.

Adding suffix 'r' as 'ubase:45r', the realtime clock based timestamps are added
on all log messages of 'category 0' regardless UB_LOGTSTYPE flag.

In ths same way, suffix 'm' uses the monotonic based timestamps,
and suffix 'g' uses gPTP based timestamp.

'unibase_posix.c' doesn't support gPTP based timestamp in 'ubb_default_initpara'.
To use it, you need to set gptp time fuction by calling 'set_gptp_gettime64'.

To override a hardcoded string to initialize the logging functions,
an environment variable can be used.

In the gptp2d example above, 'UBL_GPTP' can be used, and you can run it as follows:
+ $ UBL_GPTP="4,ubase:56m,cbase:56m,gptp:57m" gptp2d<br/>
    "4,ubase:56m,cbase:56m,gptp:57m" replaces the entire initialization string.
+ $ UBL_GPTP="47r" gptp2d<br/>
    "47r" replaces the entire initialization string.
+ $ UBL_GPTP="gptp:57m" gptp2d<br/>
    only the gptp section is replaced with "gptp:57m", change it as follows:<br/>
    "4,ubase:45,cbase:45,gptp:46" -> "4,ubase:45,cbase:45,gptp:57m"

The rule here is as follows:
+ if ',' exists or ':' doesn't exist, then the entire string is replaces.
+ otherwise, it replaces only the named part.

To retrieve the memory log messages, 'ubb_memory_out_alldata' or 'ubb_memory_file_out'
can be called.

For more detail, look at ub_logging.h

## ub_esarray
We often meet a situation in which data array is needed but the element
size can't be predefined.
In many cases, we define MAX_NUMBER_OF_... kind of macro, and allocate
possible maximum size.

That way wastes memory and requires tiresome checks not to exceed the maximum size.

'ub_esarray' can ease such situation.
Set 'esunit' as maximum element size in common usage.  The element size
expands by the unit of 'esunit' up to 'maxeles', and shrinks when it has 2*'esunit'
of unused elements.  Because of the hysteresis of 'esunit' between expand and shrink,
re-allocation doesn't happen so often.

For more detail, look at ub_esarray.h

## ub_strutils
Some string related functions are supported here.
Especially handling MAC address and Stream ID are focused,
because of our working area.

For more detail, look at ub_strutils.h

## ub_llist
simple doubly linke list.

For more detail, look at ub_llist.h

## ub_confuitls
This layer will be deprecated in future.
We have developed a separated project:xl4unipac, which has more functions.

When there are so many configuration items, using command line arguments is
not practically useful.

'ub_genconf.py' generates c codes from a default definition file.
Using the generated codes as a call back function for 'read_config_file',
runtime configuration by a configuration file is easily achieved.
