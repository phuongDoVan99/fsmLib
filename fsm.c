#include "fsm.h"

/* Finite State Machine */

int FSMInit(FSM* fsm, void* object,
#ifdef A1D
    FSMAction* actions,
#else
    FSMAction** actions,
#endif /* A1D */
    unsigned short beginState, unsigned short maxStateNo, unsigned short maxEventNo)
{
    if (fsm == NULL || object == NULL || actions == NULL) {
        return -1;
    }
    if (beginState >= maxStateNo) {
        return -1;
    }

    memset(fsm, 0, sizeof(FSM));
    fsm->Object = object;
    fsm->Action = actions;

    fsm->InFlow = false;
    fsm->State = fsm->PreviousState = fsm->BeginState = beginState;
    fsm->MaxStateNo = maxStateNo;
    fsm->MaxEventNo = maxEventNo;

    return 0;
}

int FSMClean(FSM* fsm, void* object)
{
    if (fsm == NULL || object == NULL) {
        return -1;
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }
    memset(fsm, 0, sizeof(FSM));
    return 0;
}

int FSMStart(FSM* fsm, void* object, unsigned short beginState)
{
    if (fsm == NULL || object == NULL) {
        return -1;
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }
    FSMStepState(fsm, object, beginState);
    fsm->InFlow = true;
    return 0;
}

int FSMStop(FSM* fsm, void* object, unsigned short endState);
{
    if (fsm == NULL || object == NULL) {
        return -1;
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }
    FSMStepState(fsm, object, endState);
    fsm->InFlow = false;
    return 0;
}

int FSMRegisterAction(FSM* fsm, void* object, unsigned short state, unsigned short event, FSMAction action)
{
    if (fsm == NULL || object == NULL) {
        return -1;
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }
    if (state >= fsm->MaxStateNo || event >= fsm->MaxEventNo) {
        return -1;
    }
    
#ifdef A1D
    fsm->Action[state * fsm->MaxEventNo + event] = action;
#else
    fsm->Action[state][event] = action;
#endif
    return 0;
}

int FSMEventAction(FSM* fsm, void* object, unsigned short event, void* context, void* data)
{
    if (fsm == NULL || object == NULL) {
        return -1;
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }
    if (fsm->InFlow == false) {
        return -1;
    }
    if (event >= fsm->MaxEventNo) {
        return -1;
    }

    FSMAction eventAction = NULL;
    unsigned short state = fsm->State;
    fsm->CurrentEvent = event;
#ifdef A1D
    eventAction = fsm->Action[state * fsm->MaxEventNo + event];
#else
    eventAction = fsm->Action[state][event];
#endif
    if (eventAction != NULL) {
        (*eventAction)(fsm, object, context, data);
    }
    return 0;
}

int FSMStepState(FSM* fsm, void* object, unsigned short state)
{
    if (fsm == NULL || object == NULL) {
        return -1;
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }
    if (fsm->InFlow == false) {
        return -1;
    }
    if (state >= fsm->MaxStateNo) {
        return -1;
    }

    if (state == fsm->State) {
        /* Don't update PreviousState */
        return 0;
    }

    fsm->PreviousState = fsm->State;
    fsm->State = state;
    return 0;
}

unsigned short FSMCurrentState(FSM* fsm, void* object)
{
    if (fsm == NULL || object == NULL) {
        return (unsigned short)(-1);
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }

    return object->State;
}

unsigned short FSMPrevioustate(FSM* fsm, void* object)
{
    if (fsm == NULL || object == NULL) {
        return (unsigned short)(-1);
    }
    if (fsm->Action == NULL) {
        return -1;
    }
    if (fsm->Object != object) {
        printf("!");
    }

    return object->PreviousState;
}

