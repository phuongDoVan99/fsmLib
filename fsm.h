#ifndef __FSM_H__
#define __FSM_H__

/* Finite State Machine */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>

typedef int (*FSMAction)(FSM* fsm, void* object, void* context, void* data);

/* 
 * State should be 0:1:MaxStateNo
 * Event should be 0:1:MaxEventNo
 */

typedef struct fsm {
    unsigned short State;
    unsigned short PreviousState;
    unsigned short BeginState;
    unsigned short MaxStateNo;
    void* Object;
    bool InFlow;
    unsigned short CurrentEvent;
    unsigned short MaxEventNo;
#ifdef A1D
    FSMAction* Action;
#else
    FSMAction** Action;
#endif
} FSM;

int FSMInit(FSM* fsm, void* object,
#ifdef A1D
    FSMAction* actions,
#else
    FSMAction** actions,
#endif /* A1D */
    unsigned short beginState, unsigned short maxStateNo, unsigned short maxEventNo);
int FSMClean(FSM* fsm, void* object);
int FSMStart(FSM* fsm, void* object, unsigned short beginState);
int FSMStop(FSM* fsm, void* object, unsigned short endState);
int FSMRegisterAction(FSM* fsm, void* object, unsigned short state, unsigned short event, FSMAction action);
int FSMEventAction(FSM* fsm, void* object, unsigned short event, void* context, void* data);
int FSMStepState(FSM* fsm, void* object, unsigned short state);
unsigned short FSMCurrentState(FSM* fsm, void* object);
unsigned short FSMPrevioustate(FSM* fsm, void* object);

#endif /* __FSM_H__ */
