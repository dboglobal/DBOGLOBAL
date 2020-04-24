CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 563;
	title = 56302;

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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 56307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "2611101;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 56314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 56301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611101;";
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
				conv = 56309;
				ctype = 1;
				idx = 2611101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 5612104;
				cnt0 = 7;
				cnt2 = 7;
				ectype = -1;
				etype = 0;
				idx1 = 5611106;
				taid = 3;
				esctype = 0;
				idx2 = 1712201;
			}
			CDboTSActRegQInfo
			{
				cont = 56308;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "6450.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "6450.000000";
				m4ttip = 56317;
				area = 56301;
				goal = 56304;
				m0fz = "-1334.930054";
				m0widx = 1;
				m1ttip = 56316;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-1284.000000";
				m3ttip = 56316;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1334.930054";
				m2fx = "6784.160156";
				m2ttip = 56317;
				m4widx = 1;
				m0fx = "6784.160156";
				m0ttip = 56315;
				m3pat = 2;
				m4pat = 2;
				title = 56302;
				m1fx = "6784.160156";
				m2fz = "-1334.930054";
				sort = 56305;
				m4fz = "-1284.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 56308;
			gtype = 2;
			oklnk = 2;
			area = 56301;
			goal = 56304;
			sort = 56305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 56302;
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
	}
}

