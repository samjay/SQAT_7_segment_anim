/*
 * switchmatrix.h
 *
 *  Created on: Jun 6, 2016
 *      Author: timppa
 */

#ifndef SWM_H_
#define SWM_H_

#include "chip.h"

#define SWM_RC_PARAM_ERROR -1
#define SWM_RC_OK           0

#ifdef __cplusplus
extern "C" {
#endif

int SWM_init(LPC_SYSCTL_T* pLPC_SYSCON,
		LPC_SWM_T* pLPC_SWM);

#ifdef __cplusplus
}
#endif

#endif /* SWM_H_ */
