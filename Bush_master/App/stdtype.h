#ifndef  STDTYPE_H
#define  STDTYPE_H

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    
typedef signed   char  INT8S;                    
typedef unsigned short INT16U;                   
typedef signed   short INT16S;                   
typedef unsigned int   INT32U;                  
typedef signed   int   INT32S;                  
typedef float          FP32;                    
typedef double         FP64;                     


#ifndef TRUE
#define TRUE         		  1
#endif

#ifndef FALSE
#define FALSE       		  0
#endif

#ifndef ON
#define ON			          1
#endif

#ifndef OFF
#define OFF			          0
#endif

//位左移
#define BIT(x)                                   ((INT32U)(1<<x))
//置位
#define BIT_SET(val, mask)                       { (val) |=  (mask); }
//复位
#define BIT_CLR(val, mask)                       { (val) &= ~(mask); }
//判断位是否置位
#define BIT_IS_SET(val, mask)                    ((((val) & (mask)) == (mask)) ? (TRUE) : (FALSE ))
//判断位是否复位
#define BIT_IS_CLR(val, mask)                    (((val) & (mask))            ? (TRUE ) : (FALSE))
//获得最小值
#define MIN(a, b)                                (((a) < (b)) ? (a) : (b))
//获得最大值
#define MAX(a, b)                                (((a) > (b)) ? (a) : (b))

#endif
