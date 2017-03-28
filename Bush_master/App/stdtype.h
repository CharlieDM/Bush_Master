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

//λ����
#define BIT(x)                                   ((INT32U)(1<<x))
//��λ
#define BIT_SET(val, mask)                       { (val) |=  (mask); }
//��λ
#define BIT_CLR(val, mask)                       { (val) &= ~(mask); }
//�ж�λ�Ƿ���λ
#define BIT_IS_SET(val, mask)                    ((((val) & (mask)) == (mask)) ? (TRUE) : (FALSE ))
//�ж�λ�Ƿ�λ
#define BIT_IS_CLR(val, mask)                    (((val) & (mask))            ? (TRUE ) : (FALSE))
//�����Сֵ
#define MIN(a, b)                                (((a) < (b)) ? (a) : (b))
//������ֵ
#define MAX(a, b)                                (((a) > (b)) ? (a) : (b))

#endif
