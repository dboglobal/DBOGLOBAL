CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1433;
	title = 143302;

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
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 143308;
			gtype = 2;
			oklnk = 2;
			area = 143301;
			goal = 143304;
			sort = 143305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 143302;
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
				itype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 143314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 143301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071108;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 143307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "4071108;";
			}
			CDboTSCheckClrQst
			{
				and = "1431;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 143309;
				ctype = 1;
				idx = 4071108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 9;
				eitype = 0;
				idx0 = 6611203;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 9411202;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 143308;
				m3widx = 2;
				m1fy = "0.000000";
				m2widx = 2;
				grade = 132203;
				m1pat = 2;
				m1widx = 2;
				m3fx = "-329.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "4205.000000";
				m4ttip = 143317;
				area = 143301;
				goal = 143304;
				m0fz = "663.000000";
				m0widx = 2;
				m1ttip = 143316;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "831.000000";
				m3ttip = 143316;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "663.000000";
				m2fx = "-329.000000";
				m2ttip = 143315;
				m4widx = 1;
				m0fx = "-502.000000";
				m0ttip = 143315;
				m3pat = 2;
				m4pat = 0;
				title = 143302;
				m1fx = "-502.000000";
				m2fz = "831.000000";
				sort = 143305;
				m4fz = "4384.000000";
			}
		}
	}
}

