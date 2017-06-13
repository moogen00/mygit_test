/*
******************************************************************
* COPYRIGHT ¨Ï 2017 Obigo Inc.		    *
* All rights reserved.     
*                                                     
* This software is covered by the license agreement between
* the end user and Obigo Inc.,and may be 
* used and copied only in accordance with the terms of the 
* said agreement.
*
* Obigo Inc. assumes no responsibility or 
* liability for any errors or inaccuracies in this software, 
* or any consequential, incidental or indirect damage arising
* out of the use of the software.
*-----------------------------------------------------------------*
* MODULE : *
*
* PROGRAMMER : *
* DATE : *
* VERSION : *
*
*----------------------------------------------------------------*
*
* MODULE SUMMARY : *
*
*
*
*
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS *
*
******************************************************************/

#ifndef EF_EFTHREAD_H
#define EF_EFTHREAD_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <errno.h>

#include "Types.h"
#include "RetCode.h"
#include "Thread.h"

namespace ef {

    /* Task Handle */

    #define NAME_SIZE 32

typedef void* EF_Thread_D;
    

    class EFThread{
        public:
            EFThread() {}
            virtual ~EFThread() {}
            
            virtual int runThread();
            virtual int runThread (void* params);
            virtual RetStatus startThread();
            
            RetStatus stopThread();

            EF_Thread_D EfThreadSpawn(const char *strName, int iPrio);
        
            EF_Thread_D EfThreadSpawnParam(const char *strName, int iPrio,  void * pvParam);
        
            RetStatus EfThreadDelete(EF_Thread_D taskId);
        
            RetStatus EfThreadExit(void);
        
            RetStatus EfThreadJoin(EF_Thread_D taskId);
        
            RetStatus EfThreadDelay(int milliseconds);
        
            RetStatus EfThreadLock(void);
        
            RetStatus EfThreadUnLock(void);
        
            static void *static_runThread (void *);

public:	
        char    strName_[NAME_SIZE];
        void*  pvParam_;

private :
        pthread_t       pthread_;
        pthread_attr_t  attr_;

    	};    


}  // namespace ef

#endif  // EF_EFTHREAD_H
