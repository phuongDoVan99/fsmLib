#include "fsm.h"

enum FSM_DEMO_STATE {
    FSM_DEMO_STATE_0 = 0,
    FSM_DEMO_STATE_1,
    FSM_DEMO_STATE_2,
    FSM_DEMO_STATE_3,
    FSM_DEMO_STATE_MAX
};

enum FSM_DEMO_EVENT {
    FSM_DEMO_EVENT_A = 0,
    FSM_DEMO_EVENT_B,
    FSM_DEMO_EVENT_C,
    FSM_DEMO_EVENT_MAX
};

struct fsm_demo_target {
    char data[32];
    FSM fsm_state;
} FSMDemoTarget;

int DoNothing_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);
    FSMStepState(fsm, o, FSMCurrentState(fsm, object));
    return 0;
}

int Do_State0_EventA_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);
    FSMStepState(fsm, o, FSM_DEMO_STATE_1);
    return 0;
}

int Do_State1_EventB_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);    
    FSMStepState(fsm, o, FSM_DEMO_STATE_0);
    return 0;
}

int Do_State1_EventC_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);
    FSMStepState(fsm, o, FSM_DEMO_STATE_2);
    return 0;
}

int Do_State2_EventA_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);
    FSMStepState(fsm, o, FSM_DEMO_STATE_0);
    return 0;
}

int Do_State2_EventB_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);
    FSMStepState(fsm, o, FSM_DEMO_STATE_2);
    return 0;
}

int Do_State2_EventC_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);
    FSMStepState(fsm, o, FSM_DEMO_STATE_3);
    return 0;
}

int Do_State3_EventB_Action(FSM* fsm, void* object, void* context, void* data)
{
    FSMDemoTarget* o = (FSMDemoTarget*)object;
    printf("[%s:%d] -> Enter\n", __FUNCTION__, __LINE__);
    FSMStepState(fsm, o, FSM_DEMO_STATE_1);
    return 0;
}

bool g_EndDemoProgram = false;

#ifdef A1D
FSMAction FSMDemoTargetActions[FSM_DEMO_STATE_MAX * FSM_DEMO_EVENT_MAX] = {
    Do_State0_EventA_Action, DoNothing_Action, DoNothing_Action, /* State 0 */
    DoNothing_Action, Do_State1_EventB_Action, Do_State1_EventC_Action, /* State 1 */
    Do_State2_EventA_Action, Do_State2_EventB_Action, Do_State2_EventC_Action, /* State 2 */
    DoNothing_Action, Do_State3_EventB_Action, DoNothing_Action /* State 3 */
};
#else
FSMAction FSMDemoTargetActions[FSM_DEMO_STATE_MAX][FSM_DEMO_EVENT_MAX] = {
    { Do_State0_EventA_Action, DoNothing_Action, DoNothing_Action }, /* State 0 */
    { DoNothing_Action, Do_State1_EventB_Action, Do_State1_EventC_Action }, /* State 1 */
    { Do_State2_EventA_Action, Do_State2_EventB_Action, Do_State2_EventC_Action }, /* State 2 */
    { DoNothing_Action, Do_State3_EventB_Action, DoNothing_Action } /* State 3 */
};
#endif /*A1D */

int fsm_demo_run(FSM* s, FSMDemoTarget* o);
{
    FSM_DEMO_EVENT event;
    printf("Current state is %d\n", FSMCurrentState(s, o));
    while (g_EndDemoProgram == false) {
        printf("Please enter event id: ");
        fscanf(stdin, "%d", &((int)event));
        if (event < 0 || event >= FSM_DEMO_EVENT_MAX) {
            printf("event must be on range: %d ~ %d\n", 0, FSM_DEMO_EVENT_MAX - 1);
            continue;
        }
        FSMEventAction(s, o, event, o, NULL);
        printf("Current state is %d\n", FSMCurrentState(s, o));
    }

    return 0;
}

int fsm_demo_main(int argc, char* argv[])
{
    FSMDemoTarget d;
    FSMDemoTarget* o = &d;
    FSM* s = &o->fsm_state;

    FSMInit(s, o, FSMDemoTargetActions, FSM_DEMO_STATE_0, FSM_DEMO_STATE_MAX, FSM_DEMO_EVENT_MAX);
    /* Re-register event actions if nessecery */
    FSMRegisterAction(s, o, FSM_DEMO_STATE_0, FSM_DEMO_EVENT_A, Do_State0_EventA_Action);
    FSMRegisterAction(s, o, FSM_DEMO_STATE_1, FSM_DEMO_EVENT_B, Do_State1_EventB_Action);
    FSMRegisterAction(s, o, FSM_DEMO_STATE_1, FSM_DEMO_EVENT_C, Do_State1_EventC_Action);
    FSMRegisterAction(s, o, FSM_DEMO_STATE_2, FSM_DEMO_EVENT_A, Do_State2_EventA_Action);
    FSMRegisterAction(s, o, FSM_DEMO_STATE_2, FSM_DEMO_EVENT_B, Do_State2_EventB_Action);
    FSMRegisterAction(s, o, FSM_DEMO_STATE_2, FSM_DEMO_EVENT_C, Do_State2_EventC_Action);
    FSMRegisterAction(s, o, FSM_DEMO_STATE_3, FSM_DEMO_EVENT_B, Do_State3_EventB_Action);
    
    FSMStart(s, o, FSM_DEMO_STATE_0);

    fsm_demo_run(s, o);

    FSMStop(s, o, FSM_DEMO_STATE_0);
    FSMClean(s, o);

    return 0;
}
