#ifndef _VOLTE_CONFIG_H_
#define _VOLTE_CONFIG_H_

#include "common.h"
static char g_proc_name[][32]
{
	"sesn_2_teleSesn",
	"csvWCDR_3_teleCsvW",
	"WSCodePri_6_teleWSCode",
	"WSCodeSec_6_teleWSCode",
	"docTcpip_7_teleDoc"
};

static char g_mem_name[][32]
{
	"memFlow_80000000",
	"memSesn_40000000",
	"memDoc_20000000",
	"memPdp_40000000",
	"memPdpSesn_40000000",
	"memTcpSendXDR_40000000",
	"memTcpSendSesn_40000000",
	"memWSImPri_20000000",
	"memWSImSec_20000000",
	"memWSExPri_20000000",
	"memWSExSec_20000000",
	"memCmdQuery_40000000",
	"memCmdResult_40000000",
	"memCmdQueryXdr_40000000",
	"memCmdResultXdr_40000000"
};

static char g_proc_child_name[][2]
{
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",

};
void test_config();

#endif 

