/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controle_v7_1.h
 *
 * Code generated for Simulink model 'controle_v7_1'.
 *
 * Model version                  : 2.10
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Nov  9 15:49:49 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_controle_v7_1_h_
#define RTW_HEADER_controle_v7_1_h_
#include <string.h>
#ifndef controle_v7_1_COMMON_INCLUDES_
#define controle_v7_1_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* controle_v7_1_COMMON_INCLUDES_ */

#include "controle_v7_1_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Sum2;                         /* '<Root>/Sum2' */
} B_controle_v7_1_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
} X_controle_v7_1_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
} XDot_controle_v7_1_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<Root>/Integrator' */
} XDis_controle_v7_1_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Vitesse_Consigne;             /* '<Root>/Vitesse_Consigne' */
  real_T Light;                        /* '<Root>/Light' */
  real_T Distance;                     /* '<Root>/Distance ' */
  real_T Vitesse_Reelle;               /* '<Root>/Vitesse_Reelle' */
} ExtU_controle_v7_1_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Vitesse_Envoyee;              /* '<Root>/Vitesse_Envoyee' */
} ExtY_controle_v7_1_T;

/* Parameters (default storage) */
struct P_controle_v7_1_T_ {
  real_T C;                            /* Variable: C
                                        * Referenced by: '<Root>/Gain1'
                                        */
  real_T Ki;                           /* Variable: Ki
                                        * Referenced by: '<Root>/Gain'
                                        */
  real_T Ks;                           /* Variable: Ks
                                        * Referenced by: '<Root>/Gain6'
                                        */
  real_T Kx;                           /* Variable: Kx
                                        * Referenced by: '<Root>/Gain1'
                                        */
  real_T Integrator_IC;                /* Expression: 0
                                        * Referenced by: '<Root>/Integrator'
                                        */
  real_T Saturation_UpperSat;          /* Expression: 50
                                        * Referenced by: '<Root>/Saturation'
                                        */
  real_T Saturation_LowerSat;          /* Expression: 0
                                        * Referenced by: '<Root>/Saturation'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_controle_v7_1_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_controle_v7_1_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[1];
  real_T odeF[4][1];
  ODE4_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_controle_v7_1_T controle_v7_1_P;

/* Block signals (default storage) */
extern B_controle_v7_1_T controle_v7_1_B;

/* Continuous states (default storage) */
extern X_controle_v7_1_T controle_v7_1_X;

/* External inputs (root inport signals with default storage) */
extern ExtU_controle_v7_1_T controle_v7_1_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_controle_v7_1_T controle_v7_1_Y;

/* Model entry point functions */
extern void controle_v7_1_initialize(void);
extern void controle_v7_1_step(void);
extern void controle_v7_1_terminate(void);

/* Real-time Model object */
extern RT_MODEL_controle_v7_1_T *const controle_v7_1_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'controle_v7_1'
 * '<S1>'   : 'controle_v7_1/MATLAB Function'
 * '<S2>'   : 'controle_v7_1/Subsystem'
 * '<S3>'   : 'controle_v7_1/Subsystem/MATLAB Function'
 */
#endif                                 /* RTW_HEADER_controle_v7_1_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
