#pragma once

#include "NtlPacketAM.h"
#include "NtlPacketAU.h"
#include "NtlPacketCM.h"
#include "NtlPacketCQ.h"
#include "NtlPacketCU.h"
#include "NtlPacketGM.h"
#include "NtlPacketGN.h"
#include "NtlPacketGQ.h"
#include "NtlPacketGT.h"
#include "NtlPacketGU.h"
#include "NtlPacketMA.h"
#include "NtlPacketMC.h"
#include "NtlPacketMG.h"
#include "NtlPacketMT.h"
#include "NtlPacketMQ.h"
#include "NtlPacketNG.h"
#include "NtlPacketQC.h"
#include "NtlPacketQG.h"
#include "NtlPacketQT.h"
#include "NtlPacketQM.h"
#include "NtlPacketTG.h"
#include "NtlPacketTL.h"
#include "NtlPacketTM.h"
#include "NtlPacketTQ.h"
#include "NtlPacketTU.h"
#include "NtlPacketUA.h"
#include "NtlPacketUC.h"
#include "NtlPacketUG.h"
#include "NtlPacketUT.h"
#include "NtlPacketXR.h"

/*
	NOTICE :

	1) Naming Op-code

		To distinguish type of a packet from other types, we use some alphabet letters in op-codes.
			U - User(Game Client)
			M - Master Server
			A - Auth Server
			C - Character Server
			G - Game Server
			N - NPC Server
			Q - Query Server
			T - Community Server
			L - Log Server
			W - WEB SERVER
			X - UNDEFINED SERVER

		For example, if we design a packet that is sent from a game server to a query server, its op-code must start with "GQ".

	2) Ranges that Op-code values belong to.

		SYSTEM series packets' Op code should be in [0, 99].

		UA series packets' Op code should be in [100, 999].
		AU series packets' Op code should be in [1000, 1999].

		UC series packets' Op code should be in [2000, 2999].
		CU series packets' Op code should be in [3000, 3999].

		UG series packets' Op code should be in [4000, 4999].
		GU series packets' Op code should be in [5000, 5999].

		UT series packets' Op code should be in [6000, 6999].
		TU series packets' Op code should be in [7000, 7999].

		MA series packets' Op code should be in [8000, 8999].
		AM series packets' Op code should be in [9000, 9999].

		MC series packets' Op code should be in [10000, 10999].
		CM series packets' Op code should be in [11000, 11999].

		MG series packets' Op code should be in [12000, 12999].
		GM series packets' Op code should be in [13000, 13999].

		MN series packets' Op code should be in [14000, 14999].
		NM series packets' Op code should be in [15000, 15999].

		MW series packets' Op code should be in [16000, 16999].
		WM series packets' Op code should be in [17000, 17999].

		MT series packets' Op code should be in [18000, 18999].
		TM series packets' Op code should be in [19000, 19999].

		ML series packets' Op code should be in [20000, 20999].
		LM series packets' Op code should be in [21000, 21999].

		MP series packets' Op code should be in [22000, 22999].
		PM series packets' Op code should be in [23000, 23999].

		MS series packets' Op code should be in [24000, 24999].
		SM series packets' Op code should be in [25000, 25999].


		AC series packets' Op code should be in [26000, 26999].
		CA series packets' Op code should be in [27000, 27999].

		AL series packets' Op code should be in [28000, 28999].
		LA series packets' Op code should be in [29000, 29999].


		CG series packets' Op code should be in [30000, 30999].
		GC series packets' Op code should be in [31000, 31999].

		CQ series packets' Op code should be in [32000, 32999].
		QC series packets' Op code should be in [33000, 33999].

		CL series packets' Op code should be in [34000, 34999].
		LC series packets' Op code should be in [35000, 35999].


		GN series packets' Op code should be in [36000, 36999].
		NG series packets' Op code should be in [37000, 37999].

		GQ series packets' Op code should be in [38000, 38999].
		QG series packets' Op code should be in [39000, 39999].

		GT series packets' Op code should be in [40000, 40999].
		TG series packets' Op code should be in [41000, 41999].

		GL series packets' Op code should be in [42000, 42999].
		LG series packets' Op code should be in [43000, 43999].

		GP series packets' Op code should be in [44000, 44999].
		PG series packets' Op code should be in [45000, 45999].


		NL series packets' Op code should be in [46000, 46999].
		LN series packets' Op code should be in [47000, 47999].


		QL series packets' Op code should be in [48000, 48999].
		LQ series packets' Op code should be in [49000, 49999].


		TL series packets' Op code should be in [50000, 50999].
		LT series packets' Op code should be in [51000, 51999].


		ME series packets' Op code should be in [52000, 52999].
		EM series packets' Op code should be in [53000, 53999].


		EO series packets' Op code should be in [54000, 54999].
		OE series packets' Op code should be in [55000, 55999].


		QT series packets' Op code should be in [56000, 56999].
		TQ series packets' Op code should be in [57000, 57999].

		MR series packets' Op code should be in [58000, 58499].
		RM series packets' Op code should be in [58500, 58999].

		XR series packets' Op code should be in [59000, 59499].
		RX series packets' Op code should be in [59500, 59999].


		PT series packets' Op code should be in [60000, 60499].
		TP series packets' Op code should be in [60500, 60999].

		PS series packets' Op code should be in [61000, 61499].
		SP series packets' Op code should be in [61500, 61999].

		PG series packets' Op code should be in [62000, 62499].
		GP series packets' Op code should be in [62500, 62999].

		PC series packets' Op code should be in [63000, 63499].
		CP series packets' Op code should be in [63500, 63999].


		BABM series packets' Op code should be in [64000, 64099].
		BMBA series packets' Op code should be in [64100, 64199].

		BCBM series packets' Op code should be in [64200, 64299].
		BMBC series packets' Op code should be in [64300, 64399].

		PA series packets' Op code should be in [65000, 65099].
		AP series packets' Op code should be in [65100, 65199].
*/
