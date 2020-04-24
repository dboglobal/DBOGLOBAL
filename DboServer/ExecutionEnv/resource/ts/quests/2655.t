CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2655;
	title = 265502;

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
			stdiag = 265507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickObject
			{
				objidx = "1453;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 265508;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "545.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "200.000000";
				m4ttip = 265516;
				area = 265501;
				goal = 265504;
				m0fz = "-2556.000000";
				m0widx = 1;
				m1ttip = 265515;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-2777.000000";
				m3ttip = 265516;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2799.000000";
				m2fx = "105.000000";
				m2ttip = 265516;
				m4widx = 1;
				m0fx = "200.000000";
				m0ttip = 265515;
				m3pat = 2;
				m4pat = 2;
				title = 265502;
				m1fx = "545.000000";
				m2fz = "-2755.000000";
				sort = 265505;
				m4fz = "-2556.000000";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 8;
				eitype = 0;
				idx0 = 4711111;
				cnt0 = 7;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5741101;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 265514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 265501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5313102;";
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
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 265508;
			gtype = 2;
			oklnk = 2;
			area = 265501;
			goal = 265504;
			sort = 265505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 265502;
		}
	}
}

