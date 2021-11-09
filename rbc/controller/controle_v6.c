/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controle_v6.c
 *
 * Code generated for Simulink model 'controle_v6'.
 *
 * Model version                  : 2.8
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Nov  9 15:25:47 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "controle_v6.h"
#include "controle_v6_private.h"

/* Block signals (default storage) */
B_controle_v6_T controle_v6_B;

/* Continuous states */
X_controle_v6_T controle_v6_X;

/* External inputs (root inport signals with default storage) */
ExtU_controle_v6_T controle_v6_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_controle_v6_T controle_v6_Y;

/* Real-time model */
static RT_MODEL_controle_v6_T controle_v6_M_;
RT_MODEL_controle_v6_T *const controle_v6_M = &controle_v6_M_;

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  controle_v6_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  controle_v6_step();
  controle_v6_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  controle_v6_step();
  controle_v6_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  controle_v6_step();
  controle_v6_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void controle_v6_step(void)
{
  real_T Dfrenage;
  real_T rtb_Sum1;
  real_T x;
  if (rtmIsMajorTimeStep(controle_v6_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&controle_v6_M->solverInfo,
                          ((controle_v6_M->Timing.clockTick0+1)*
      controle_v6_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(controle_v6_M)) {
    controle_v6_M->Timing.t[0] = rtsiGetT(&controle_v6_M->solverInfo);
  }

  /* Sum: '<Root>/Sum1' incorporates:
   *  Gain: '<Root>/Gain1'
   *  Inport: '<Root>/Vitesse_Reelle'
   *  Integrator: '<Root>/Integrator'
   */
  rtb_Sum1 = controle_v6_X.Integrator_CSTATE - controle_v6_P.Kx /
    controle_v6_P.C * controle_v6_U.Vitesse_Reelle;

  /* Saturate: '<Root>/Saturation' */
  if (rtb_Sum1 > controle_v6_P.Saturation_UpperSat) {
    /* Saturate: '<Root>/Saturation' */
    controle_v6_Y.Vitesse_Envoyee = controle_v6_P.Saturation_UpperSat;
  } else if (rtb_Sum1 < controle_v6_P.Saturation_LowerSat) {
    /* Saturate: '<Root>/Saturation' */
    controle_v6_Y.Vitesse_Envoyee = controle_v6_P.Saturation_LowerSat;
  } else {
    /* Saturate: '<Root>/Saturation' */
    controle_v6_Y.Vitesse_Envoyee = rtb_Sum1;
  }

  /* End of Saturate: '<Root>/Saturation' */

  /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
   *  Inport: '<Root>/Position '
   *  Inport: '<Root>/Position_2'
   *  Inport: '<Root>/Vitesse_Consigne'
   *  Inport: '<Root>/Vitesse_Reelle'
   */
  x = controle_v6_U.Position - controle_v6_U.Position_2;
  Dfrenage = controle_v6_U.Vitesse_Reelle * controle_v6_U.Vitesse_Reelle * 0.03
    + 0.28 * controle_v6_U.Vitesse_Reelle;
  if (x <= 20.0) {
    x = 0.0;
  } else if ((20.0 < x) && (x <= Dfrenage + 20.0)) {
    x = (x * x - 400.0) * controle_v6_U.Vitesse_Consigne / ((Dfrenage + 20.0) *
      (Dfrenage + 20.0));
  } else {
    x = controle_v6_U.Vitesse_Consigne;
  }

  /* End of MATLAB Function: '<Root>/MATLAB Function' */

  /* MATLAB Function: '<S2>/MATLAB Function' incorporates:
   *  Inport: '<Root>/Light'
   */
  if (!(controle_v6_U.Light == 0.0)) {
    if (controle_v6_U.Light == 1.0) {
      x *= 0.2;
    } else {
      x = 0.0;
    }
  }

  /* End of MATLAB Function: '<S2>/MATLAB Function' */

  /* Sum: '<Root>/Sum2' incorporates:
   *  Gain: '<Root>/Gain'
   *  Gain: '<Root>/Gain6'
   *  Inport: '<Root>/Vitesse_Reelle'
   *  Sum: '<Root>/Sum'
   *  Sum: '<Root>/Sum4'
   */
  controle_v6_B.Sum2 = controle_v6_P.Ks * 10.0 * (controle_v6_Y.Vitesse_Envoyee
    - rtb_Sum1) + (x - controle_v6_U.Vitesse_Reelle) * controle_v6_P.Ki;
  if (rtmIsMajorTimeStep(controle_v6_M)) {
    rt_ertODEUpdateContinuousStates(&controle_v6_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++controle_v6_M->Timing.clockTick0;
    controle_v6_M->Timing.t[0] = rtsiGetSolverStopTime
      (&controle_v6_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.034s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.034, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      controle_v6_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void controle_v6_derivatives(void)
{
  XDot_controle_v6_T *_rtXdot;
  _rtXdot = ((XDot_controle_v6_T *) controle_v6_M->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  _rtXdot->Integrator_CSTATE = controle_v6_B.Sum2;
}

/* Model initialize function */
void controle_v6_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&controle_v6_M->solverInfo,
                          &controle_v6_M->Timing.simTimeStep);
    rtsiSetTPtr(&controle_v6_M->solverInfo, &rtmGetTPtr(controle_v6_M));
    rtsiSetStepSizePtr(&controle_v6_M->solverInfo,
                       &controle_v6_M->Timing.stepSize0);
    rtsiSetdXPtr(&controle_v6_M->solverInfo, &controle_v6_M->derivs);
    rtsiSetContStatesPtr(&controle_v6_M->solverInfo, (real_T **)
                         &controle_v6_M->contStates);
    rtsiSetNumContStatesPtr(&controle_v6_M->solverInfo,
      &controle_v6_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&controle_v6_M->solverInfo,
      &controle_v6_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&controle_v6_M->solverInfo,
      &controle_v6_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&controle_v6_M->solverInfo,
      &controle_v6_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&controle_v6_M->solverInfo, (&rtmGetErrorStatus
      (controle_v6_M)));
    rtsiSetRTModelPtr(&controle_v6_M->solverInfo, controle_v6_M);
  }

  rtsiSetSimTimeStep(&controle_v6_M->solverInfo, MAJOR_TIME_STEP);
  controle_v6_M->intgData.y = controle_v6_M->odeY;
  controle_v6_M->intgData.f[0] = controle_v6_M->odeF[0];
  controle_v6_M->intgData.f[1] = controle_v6_M->odeF[1];
  controle_v6_M->intgData.f[2] = controle_v6_M->odeF[2];
  controle_v6_M->intgData.f[3] = controle_v6_M->odeF[3];
  controle_v6_M->contStates = ((X_controle_v6_T *) &controle_v6_X);
  rtsiSetSolverData(&controle_v6_M->solverInfo, (void *)&controle_v6_M->intgData);
  rtsiSetSolverName(&controle_v6_M->solverInfo,"ode4");
  rtmSetTPtr(controle_v6_M, &controle_v6_M->Timing.tArray[0]);
  controle_v6_M->Timing.stepSize0 = 0.034;

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  controle_v6_X.Integrator_CSTATE = controle_v6_P.Integrator_IC;
}

/* Model terminate function */
void controle_v6_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
