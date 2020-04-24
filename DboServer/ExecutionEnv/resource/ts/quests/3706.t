CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 3706;
	title = 370602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 370609;
				ctype = 1;
				idx = 3481101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 16416100;
				cnt0 = 80;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 370608;
				m1fx = "-2738.469971";
				m2fz = "-4245.930176";
				m1fy = "-84.889999";
				sort = 370605;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 370601;
				goal = 370604;
				m0fz = "-4367.839844";
				m0widx = 15;
				m1ttip = 370615;
				m2fy = "-110.500000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "-112.010002";
				m0pat = 2;
				m1fz = "-4137.509766";
				m2fx = "-2926.060059";
				m2ttip = 370615;
				m0fx = "-2677.909912";
				m0ttip = 370615;
				title = 370602;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 370607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "1501107;";
			}
			CDboTSCheckClrQst
			{
				and = "3705;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 370614;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 370601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5533409;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 370608;
			gtype = 3;
			oklnk = 2;
			area = 370601;
			goal = 370604;
			sort = 370605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 370602;
		}
	}
}

