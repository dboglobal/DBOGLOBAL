CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2902;
	title = 290202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 290208;
			gtype = 1;
			oklnk = 2;
			area = 290201;
			goal = 290204;
			sort = 290205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 290202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 290214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 290201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511312;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 290207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "2903;2904;2906;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4511312;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 290209;
				ctype = 1;
				idx = 4511312;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 290201;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 290215;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 290208;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 290205;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2206.270020";
				rwd = 100;
				taid = 1;
				gtype = 1;
				m3fy = "0.000000";
				area = 290201;
				goal = 290204;
				m0fz = "-2374.260010";
				m0widx = 1;
				m1ttip = 290215;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-2642.929932";
				m3ttip = 290215;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2726.969971";
				m2fx = "2245.750000";
				m2ttip = 290215;
				m0fx = "2560.360107";
				m0ttip = 290215;
				m3pat = 2;
				title = 290202;
				m1fx = "2534.270020";
				m2fz = "-2333.409912";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
	}
}

