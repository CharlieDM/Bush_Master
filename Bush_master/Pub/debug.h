
#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Return void after assert error, with error information   */
#define CHECK_IF_RET_VOID(Cond, reason)      if((Cond)) {return;}
/* Return value after assert error, with error information  */
#define CHECK_IF_RET_VAL(Cond, ret,reason)   if((Cond)) {return ret;}
/* Return SW_ERR after assert error, with error information */
#define CHECK_IF_RET_ST(Cond, reason)        if((Cond)) {return SW_ERR;}


#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_H_ */

/* End of file */


