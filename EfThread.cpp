/*
******************************************************************
* COPYRIGHT ⓒ 2017 Obigo Inc.		    *
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



#include "EfThread.h"
#include "LogMessage.h"

// C1
// C1A
// B_M_1 
// B_M_2 
// B_M_3 
// B_M_4 
// B_M_8 
namespace ef {


void* EFThread::static_runThread(void * param)
{
    EFThread *pThread = (EFThread*)param;

    if (NULL != pThread)
    {
        if (pThread->strName_)
        {
            //prctl(PR_SET_NAME, pThread->strName_, 0, 0.,0);
        }

        if (NULL != pThread->pvParam_)
        {
	      pThread->runThread(pThread->pvParam_);
        }
        else
        {
	      pThread->runThread();
        }

    }

    pthread_exit(0);
	
    return 0;

}


/**
 * Thread Spawn function.
 *
 * @param
 *    char *strName - task name
 *
 * @param
 *    ThreadPriority iPrio - task priority
 *
 * @param
 *    int iStackSize - task stack size
 *
 * @param
 *    FUNCPTR_PARAM func - task procedure function
 *
 * @retval
 *    int pthreadRecord - pointer to the task record, if successful
 *    NULL if not successful.
 */
EF_Thread_D EFThread::EfThreadSpawn(const char *strName,
                       int iPrio
                       )
{
    return (EfThreadSpawnParam( strName, iPrio, NULL ));
}


/**
 * Thread Spawn Parameter function.
 *
 * @param
 *    char *strName - task name
 *
 * @param
 *    ThreadPriority iPrio - task priority
 *
 * @param
 *    int iStackSize - task stack size
 *
 * @param
 *    FUNCPTR_PARAM func - task procedure function
 *
 * @param
 *    void * pvParam - task input parameter
 *
 * @param
 *    void * pvStackBaseParam - stack pointer, allows stack base to be passed
 *                             in rather than OS_Malloc'd.
 *
 * @retval
 *    int pthreadRecord - pointer to the task record, if successful
 *    NULL if not successful.
 */
EF_Thread_D EFThread::EfThreadSpawnParam(const char *strName,
                            int iPrio, 
                            void * pvParam
                            )
{
    int     iRetStatus      = 0;
    static  int once = 0;
    static  int realtime_enabled = 0;

    if (once == 0) {
        once = 1;

        if (realtime_enabled == 1)
        {
            LOG_ERROR("enabling realtime scheduling\n");
        }
    }

    /* Check the validity of the given priority */
    if ( (iPrio < THREAD_MIN_PRIORITY) || (iPrio > THREAD_MAX_PRIORITY) )
    {
        LOG_ERROR("%s: Invalid thread priority (%d)!\n", __FUNCTION__, iPrio);
        goto error_handler;
    }

    /* Save the paramater */
    pvParam_ = pvParam;

    if (strName && strName[0])
    {
        strncpy(strName_, strName, NAME_SIZE - 1);
    }
    else
    {
        strncpy(strName_, "Unnamed Thread", NAME_SIZE - 1);
    }
    strName_[NAME_SIZE - 1] = 0;

    /* Create the thread attributes */
    iRetStatus = pthread_attr_init(&attr_);
    if (iRetStatus != 0)
    {
        LOG_ERROR("%s: pthread_attr_init failed with error %s\n", __FUNCTION__, strerror(iRetStatus));
        goto error_handler;
    }


    /* Create the thread */
    iRetStatus = pthread_create(&pthread_, &attr_, EFThread::static_runThread, (void *)this);
    if (iRetStatus != 0)
    {
        LOG_ERROR("%s: pthread_create failed with error %s\n", __FUNCTION__, strerror(iRetStatus));
        goto error_handler;
    }

    /* Return the thread record as an integer */
    return ( this );

error_handler:

    return (NULL);
	
}


/**
 * Thread Delete function.
 *
 * @param
 *    EF_Thread_D taskId - task identifier
 *
 * @retval
 *    This function will print a status message if it was not successful.
 */
RetStatus EFThread::EfThreadDelete(EF_Thread_D taskId)
{
    int iRetStatus;

    /* Cast the given task identifier to a pointer to a task record */
    EFThread *pThread = (EFThread*)taskId;

    if (pThread != 0)
    {
        /* Cancel the given thread */
        iRetStatus = pthread_cancel(pThread->pthread_);
        if ((iRetStatus != 0) && (iRetStatus != ESRCH))
        {
            LOG_ERROR("%s: pthread_cancel failed with error %s\n", __FUNCTION__, strerror(iRetStatus));
        }

        /* wait for the task to exit */		
        EfThreadJoin(taskId);

        /* delete the pthread attributes */		
        iRetStatus = pthread_attr_destroy(&pThread->attr_);
        if (iRetStatus != 0)
        {
            LOG_ERROR("%s: pthread_attr_destroy failed with error %s\n", __FUNCTION__, strerror(iRetStatus));
        }

    }
}


/**
 * Thread Exit function.
 *
 * @param
 *    None.
 *
 * @retval
 *    None.
 */
RetStatus EFThread::EfThreadExit(void)
{
    pthread_exit(0);
}


/**
 * Thread  Join function.
 *
 * @param
 *    EF_TASK_ID taskId - task identifier
 *
 * @retval
 *    Returns RET_SUCCESS.
 */
RetStatus EFThread::EfThreadJoin(EF_Thread_D taskId)
{
    RetStatus status = RET_SUCCESS;

    /* Cast the given task identifier to a pointer to a task record */
    EFThread *pThread = (EFThread*)taskId;

    if (pThread != 0)
    {
        /* Join the given thread */
        if (pthread_join(pThread->pthread_, NULL) != 0)
        {
            status = RET_FAILUE;
        }
    }
    else
    {
        status = RET_FAILUE;
    }

    return (status);
}


/**
 * Thread Delay function.
 *
 * @param
 *    milliseconds - time to delay the current task, in milliseconds
 *
 * @retval
 *    This function is always successful.
 */
RetStatus EFThread::EfThreadDelay(int milliseconds)
{
    usleep(milliseconds*1000);
}
/**
 * Thread Lock function.
 *
 * @param
 *    None.
 *
 * @retval
 *    Returns RET_SUCCESS on success, true on failure.
 */
RetStatus EFThread::EfThreadLock(void)
{
#if 0
    if (pthread_mutex_lock(&mutex_) != 0)
    {
        return (RET_FAILUE);
    }
#endif	

    return (RET_SUCCESS);
}

/**
 * Thread Unlock function.
 *
 * @param
 *    None.
 *
 * @retval
 *    Returns RET_SUCCESS on success, true on failure.
 */
RetStatus EFThread::EfThreadUnLock(void)
{

#if 0
    if (pthread_mutex_unlock(&mutex_) != 0)
    {
        return (RET_FAILUE);
    }
#endif	

    return (RET_SUCCESS);
}

int EFThread::runThread () { return 0; } /* 어짜피 오버라이딩 될 부분 */
int EFThread::runThread (void* params) { return 0; } /* 어짜피 오버라이딩 될 부분 */

RetStatus EFThread::startThread(){


	
    EfThreadSpawn("EfThread", THREAD_NORMAL_PRIORITY);

}


RetStatus EFThread::stopThread(){
	

	
}

}

