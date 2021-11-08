/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controle_v3_capi.c
 *
 * Code generated for Simulink model 'controle_v3'.
 *
 * Model version                  : 2.1
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Mon Nov  8 11:19:10 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "controle_v3_capi_host.h"
#define sizeof(s)                      ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el)              ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s)               (s)
#else                                  /* HOST_CAPI_BUILD */
#include "builtin_typeid_types.h"
#include "controle_v3.h"
#include "controle_v3_capi.h"
#include "controle_v3_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               (NULL)
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif                                 /* HOST_CAPI_BUILD */

static const rtwCAPI_BlockParameters rtBlockParameters[] = {
  /* addrMapIndex, blockPath,
   * paramName, dataTypeIndex, dimIndex, fixPtIdx
   */
  { 0, TARGET_STRING("controle_v3/Integrator"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 1, TARGET_STRING("controle_v3/Integrator1"),
    TARGET_STRING("InitialCondition"), 0, 0, 0 },

  { 2, TARGET_STRING("controle_v3/Saturation"),
    TARGET_STRING("UpperLimit"), 0, 0, 0 },

  { 3, TARGET_STRING("controle_v3/Saturation"),
    TARGET_STRING("LowerLimit"), 0, 0, 0 },

  {
    0, (NULL), (NULL), 0, 0, 0
  }
};

/* Tunable variable parameters */
static const rtwCAPI_ModelParameters rtModelParameters[] = {
  /* addrMapIndex, varName, dataTypeIndex, dimIndex, fixPtIndex */
  { 4, TARGET_STRING("Observer"), 1, 0, 0 },

  { 5, TARGET_STRING("A"), 0, 0, 0 },

  { 6, TARGET_STRING("B"), 0, 0, 0 },

  { 7, TARGET_STRING("C"), 0, 0, 0 },

  { 8, TARGET_STRING("Ki"), 0, 0, 0 },

  { 9, TARGET_STRING("Ks"), 0, 0, 0 },

  { 10, TARGET_STRING("Kx"), 0, 0, 0 },

  { 0, (NULL), 0, 0, 0 }
};

#ifndef HOST_CAPI_BUILD

/* Declare Data Addresses statically */
static void* rtDataAddrMap[] = {
  &controle_v3_P.Integrator_IC,        /* 0: Block Parameter */
  &controle_v3_P.Integrator1_IC,       /* 1: Block Parameter */
  &controle_v3_P.Saturation_UpperSat,  /* 2: Block Parameter */
  &controle_v3_P.Saturation_LowerSat,  /* 3: Block Parameter */
  &controle_v3_P.Observer,             /* 4: Model Parameter */
  &controle_v3_P.A,                    /* 5: Model Parameter */
  &controle_v3_P.B,                    /* 6: Model Parameter */
  &controle_v3_P.C,                    /* 7: Model Parameter */
  &controle_v3_P.Ki,                   /* 8: Model Parameter */
  &controle_v3_P.Ks,                   /* 9: Model Parameter */
  &controle_v3_P.Kx,                   /* 10: Model Parameter */
};

/* Declare Data Run-Time Dimension Buffer Addresses statically */
static int32_T* rtVarDimsAddrMap[] = {
  (NULL)
};

#endif

/* Data Type Map - use dataTypeMapIndex to access this structure */
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap[] = {
  /* cName, mwName, numElements, elemMapIndex, dataSize, slDataId, *
   * isComplex, isPointer, enumStorageType */
  { "double", "real_T", 0, 0, sizeof(real_T), SS_DOUBLE, 0, 0, 0 },

  { "struct", "struct_ALRDS3SSLkjlF2CmbmN2xD", 7, 1, sizeof
    (struct_ALRDS3SSLkjlF2CmbmN2xD), SS_STRUCT, 0, 0, 0 }
};

#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif

/* Structure Element Map - use elemMapIndex to access this structure */
static TARGET_CONST rtwCAPI_ElementMap rtElementMap[] = {
  /* elementName, elementOffset, dataTypeIndex, dimIndex, fxpIndex */
  { (NULL), 0, 0, 0, 0 },

  { "wd", rt_offsetof(struct_ALRDS3SSLkjlF2CmbmN2xD, wd), 0, 1, 0 },

  { "PO", rt_offsetof(struct_ALRDS3SSLkjlF2CmbmN2xD, PO), 0, 1, 0 },

  { "zeta", rt_offsetof(struct_ALRDS3SSLkjlF2CmbmN2xD, zeta), 0, 1, 0 },

  { "wn", rt_offsetof(struct_ALRDS3SSLkjlF2CmbmN2xD, wn), 0, 1, 0 },

  { "poly", rt_offsetof(struct_ALRDS3SSLkjlF2CmbmN2xD, poly), 0, 2, 0 },

  { "poles", rt_offsetof(struct_ALRDS3SSLkjlF2CmbmN2xD, poles), 0, 1, 0 },

  { "L", rt_offsetof(struct_ALRDS3SSLkjlF2CmbmN2xD, L), 0, 1, 0 }
};

/* Dimension Map - use dimensionMapIndex to access elements of ths structure*/
static const rtwCAPI_DimensionMap rtDimensionMap[] = {
  /* dataOrientation, dimArrayIndex, numDims, vardimsIndex */
  { rtwCAPI_SCALAR, 0, 2, 0 },

  { rtwCAPI_MATRIX_COL_MAJOR, 0, 2, 0 },

  { rtwCAPI_MATRIX_COL_MAJOR, 2, 2, 0 }
};

/* Dimension Array- use dimArrayIndex to access elements of this array */
static const uint_T rtDimensionArray[] = {
  1,                                   /* 0 */
  1,                                   /* 1 */
  1,                                   /* 2 */
  2                                    /* 3 */
};

/* Fixed Point Map */
static const rtwCAPI_FixPtMap rtFixPtMap[] = {
  /* fracSlopePtr, biasPtr, scaleType, wordLength, exponent, isSigned */
  { (NULL), (NULL), rtwCAPI_FIX_RESERVED, 0, 0, 0 },
};

/* Sample Time Map - use sTimeIndex to access elements of ths structure */
static const rtwCAPI_SampleTimeMap rtSampleTimeMap[] = {
  /* samplePeriodPtr, sampleOffsetPtr, tid, samplingMode */
  {
    (NULL), (NULL), 0, 0
  }
};

static rtwCAPI_ModelMappingStaticInfo mmiStatic = {
  /* Signals:{signals, numSignals,
   *           rootInputs, numRootInputs,
   *           rootOutputs, numRootOutputs},
   * Params: {blockParameters, numBlockParameters,
   *          modelParameters, numModelParameters},
   * States: {states, numStates},
   * Maps:   {dataTypeMap, dimensionMap, fixPtMap,
   *          elementMap, sampleTimeMap, dimensionArray},
   * TargetType: targetType
   */
  { (NULL), 0,
    (NULL), 0,
    (NULL), 0 },

  { rtBlockParameters, 4,
    rtModelParameters, 7 },

  { (NULL), 0 },

  { rtDataTypeMap, rtDimensionMap, rtFixPtMap,
    rtElementMap, rtSampleTimeMap, rtDimensionArray },
  "float",

  { 1688877885U,
    2335969684U,
    4092645548U,
    131299869U },
  (NULL), 0,
  0
};

/* Function to get C API Model Mapping Static Info */
const rtwCAPI_ModelMappingStaticInfo*
  controle_v3_GetCAPIStaticMap(void)
{
  return &mmiStatic;
}

/* Cache pointers into DataMapInfo substructure of RTModel */
#ifndef HOST_CAPI_BUILD

void controle_v3_InitializeDataMapInfo(void)
{
  /* Set C-API version */
  rtwCAPI_SetVersion(controle_v3_M->DataMapInfo.mmi, 1);

  /* Cache static C-API data into the Real-time Model Data structure */
  rtwCAPI_SetStaticMap(controle_v3_M->DataMapInfo.mmi, &mmiStatic);

  /* Cache static C-API logging data into the Real-time Model Data structure */
  rtwCAPI_SetLoggingStaticMap(controle_v3_M->DataMapInfo.mmi, (NULL));

  /* Cache C-API Data Addresses into the Real-Time Model Data structure */
  rtwCAPI_SetDataAddressMap(controle_v3_M->DataMapInfo.mmi, rtDataAddrMap);

  /* Cache C-API Data Run-Time Dimension Buffer Addresses into the Real-Time Model Data structure */
  rtwCAPI_SetVarDimsAddressMap(controle_v3_M->DataMapInfo.mmi, rtVarDimsAddrMap);

  /* Cache the instance C-API logging pointer */
  rtwCAPI_SetInstanceLoggingInfo(controle_v3_M->DataMapInfo.mmi, (NULL));

  /* Set reference to submodels */
  rtwCAPI_SetChildMMIArray(controle_v3_M->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetChildMMIArrayLen(controle_v3_M->DataMapInfo.mmi, 0);
}

#else                                  /* HOST_CAPI_BUILD */
#ifdef __cplusplus

extern "C" {

#endif

  void controle_v3_host_InitializeDataMapInfo(controle_v3_host_DataMapInfo_T
    *dataMap, const char *path)
  {
    /* Set C-API version */
    rtwCAPI_SetVersion(dataMap->mmi, 1);

    /* Cache static C-API data into the Real-time Model Data structure */
    rtwCAPI_SetStaticMap(dataMap->mmi, &mmiStatic);

    /* host data address map is NULL */
    rtwCAPI_SetDataAddressMap(dataMap->mmi, NULL);

    /* host vardims address map is NULL */
    rtwCAPI_SetVarDimsAddressMap(dataMap->mmi, NULL);

    /* Set Instance specific path */
    rtwCAPI_SetPath(dataMap->mmi, path);
    rtwCAPI_SetFullPath(dataMap->mmi, NULL);

    /* Set reference to submodels */
    rtwCAPI_SetChildMMIArray(dataMap->mmi, (NULL));
    rtwCAPI_SetChildMMIArrayLen(dataMap->mmi, 0);
  }

#ifdef __cplusplus

}
#endif
#endif                                 /* HOST_CAPI_BUILD */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
