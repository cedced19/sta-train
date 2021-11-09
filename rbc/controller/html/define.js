function CodeDefine() { 
this.def = new Array();
this.def["rt_OneStep"] = {file: "ert_main_c.html",line:37,type:"fcn"};
this.def["main"] = {file: "ert_main_c.html",line:74,type:"fcn"};
this.def["controle_v6_B"] = {file: "controle_v6_c.html",line:24,type:"var"};
this.def["controle_v6_X"] = {file: "controle_v6_c.html",line:27,type:"var"};
this.def["controle_v6_U"] = {file: "controle_v6_c.html",line:30,type:"var"};
this.def["controle_v6_Y"] = {file: "controle_v6_c.html",line:33,type:"var"};
this.def["controle_v6.c:controle_v6_M_"] = {file: "controle_v6_c.html",line:36,type:"var"};
this.def["controle_v6_M"] = {file: "controle_v6_c.html",line:37,type:"var"};
this.def["controle_v6.c:rt_ertODEUpdateContinuousStates"] = {file: "controle_v6_c.html",line:43,type:"fcn"};
this.def["controle_v6_step"] = {file: "controle_v6_c.html",line:110,type:"fcn"};
this.def["controle_v6_derivatives"] = {file: "controle_v6_c.html",line:217,type:"fcn"};
this.def["controle_v6_initialize"] = {file: "controle_v6_c.html",line:227,type:"fcn"};
this.def["controle_v6_terminate"] = {file: "controle_v6_c.html",line:270,type:"fcn"};
this.def["B_controle_v6_T"] = {file: "controle_v6_h.html",line:64,type:"type"};
this.def["X_controle_v6_T"] = {file: "controle_v6_h.html",line:69,type:"type"};
this.def["XDot_controle_v6_T"] = {file: "controle_v6_h.html",line:74,type:"type"};
this.def["XDis_controle_v6_T"] = {file: "controle_v6_h.html",line:79,type:"type"};
this.def["ODE4_IntgData"] = {file: "controle_v6_h.html",line:88,type:"type"};
this.def["ExtU_controle_v6_T"] = {file: "controle_v6_h.html",line:99,type:"type"};
this.def["ExtY_controle_v6_T"] = {file: "controle_v6_h.html",line:104,type:"type"};
this.def["P_controle_v6_T"] = {file: "controle_v6_types_h.html",line:27,type:"type"};
this.def["RT_MODEL_controle_v6_T"] = {file: "controle_v6_types_h.html",line:30,type:"type"};
this.def["controle_v6_P"] = {file: "controle_v6_data_c.html",line:24,type:"var"};
this.def["int8_T"] = {file: "rtwtypes_h.html",line:51,type:"type"};
this.def["uint8_T"] = {file: "rtwtypes_h.html",line:52,type:"type"};
this.def["int16_T"] = {file: "rtwtypes_h.html",line:53,type:"type"};
this.def["uint16_T"] = {file: "rtwtypes_h.html",line:54,type:"type"};
this.def["int32_T"] = {file: "rtwtypes_h.html",line:55,type:"type"};
this.def["uint32_T"] = {file: "rtwtypes_h.html",line:56,type:"type"};
this.def["int64_T"] = {file: "rtwtypes_h.html",line:57,type:"type"};
this.def["uint64_T"] = {file: "rtwtypes_h.html",line:58,type:"type"};
this.def["real32_T"] = {file: "rtwtypes_h.html",line:59,type:"type"};
this.def["real64_T"] = {file: "rtwtypes_h.html",line:60,type:"type"};
this.def["real_T"] = {file: "rtwtypes_h.html",line:66,type:"type"};
this.def["time_T"] = {file: "rtwtypes_h.html",line:67,type:"type"};
this.def["boolean_T"] = {file: "rtwtypes_h.html",line:68,type:"type"};
this.def["int_T"] = {file: "rtwtypes_h.html",line:69,type:"type"};
this.def["uint_T"] = {file: "rtwtypes_h.html",line:70,type:"type"};
this.def["ulong_T"] = {file: "rtwtypes_h.html",line:71,type:"type"};
this.def["char_T"] = {file: "rtwtypes_h.html",line:72,type:"type"};
this.def["uchar_T"] = {file: "rtwtypes_h.html",line:73,type:"type"};
this.def["byte_T"] = {file: "rtwtypes_h.html",line:74,type:"type"};
this.def["creal32_T"] = {file: "rtwtypes_h.html",line:84,type:"type"};
this.def["creal64_T"] = {file: "rtwtypes_h.html",line:89,type:"type"};
this.def["creal_T"] = {file: "rtwtypes_h.html",line:94,type:"type"};
this.def["cint8_T"] = {file: "rtwtypes_h.html",line:101,type:"type"};
this.def["cuint8_T"] = {file: "rtwtypes_h.html",line:108,type:"type"};
this.def["cint16_T"] = {file: "rtwtypes_h.html",line:115,type:"type"};
this.def["cuint16_T"] = {file: "rtwtypes_h.html",line:122,type:"type"};
this.def["cint32_T"] = {file: "rtwtypes_h.html",line:129,type:"type"};
this.def["cuint32_T"] = {file: "rtwtypes_h.html",line:136,type:"type"};
this.def["cint64_T"] = {file: "rtwtypes_h.html",line:143,type:"type"};
this.def["cuint64_T"] = {file: "rtwtypes_h.html",line:150,type:"type"};
this.def["pointer_T"] = {file: "rtwtypes_h.html",line:171,type:"type"};
}
CodeDefine.instance = new CodeDefine();
var testHarnessInfo = {OwnerFileName: "", HarnessOwner: "", HarnessName: "", IsTestHarness: "0"};
var relPathToBuildDir = "../ert_main.c";
var fileSep = "\\";
var isPC = true;
function Html2SrcLink() {
	this.html2SrcPath = new Array;
	this.html2Root = new Array;
	this.html2SrcPath["ert_main_c.html"] = "../ert_main.c";
	this.html2Root["ert_main_c.html"] = "ert_main_c.html";
	this.html2SrcPath["controle_v6_c.html"] = "../controle_v6.c";
	this.html2Root["controle_v6_c.html"] = "controle_v6_c.html";
	this.html2SrcPath["controle_v6_h.html"] = "../controle_v6.h";
	this.html2Root["controle_v6_h.html"] = "controle_v6_h.html";
	this.html2SrcPath["controle_v6_private_h.html"] = "../controle_v6_private.h";
	this.html2Root["controle_v6_private_h.html"] = "controle_v6_private_h.html";
	this.html2SrcPath["controle_v6_types_h.html"] = "../controle_v6_types.h";
	this.html2Root["controle_v6_types_h.html"] = "controle_v6_types_h.html";
	this.html2SrcPath["controle_v6_data_c.html"] = "../controle_v6_data.c";
	this.html2Root["controle_v6_data_c.html"] = "controle_v6_data_c.html";
	this.html2SrcPath["rtwtypes_h.html"] = "../rtwtypes.h";
	this.html2Root["rtwtypes_h.html"] = "rtwtypes_h.html";
	this.getLink2Src = function (htmlFileName) {
		 if (this.html2SrcPath[htmlFileName])
			 return this.html2SrcPath[htmlFileName];
		 else
			 return null;
	}
	this.getLinkFromRoot = function (htmlFileName) {
		 if (this.html2Root[htmlFileName])
			 return this.html2Root[htmlFileName];
		 else
			 return null;
	}
}
Html2SrcLink.instance = new Html2SrcLink();
var fileList = [
"ert_main_c.html","controle_v6_c.html","controle_v6_h.html","controle_v6_private_h.html","controle_v6_types_h.html","controle_v6_data_c.html","rtwtypes_h.html"];
