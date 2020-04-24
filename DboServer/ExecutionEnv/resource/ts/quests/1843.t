CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1843;
	title = 184302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 184309;
				ctype = 1;
				idx = 3612102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 5812201;
				cnt0 = 13;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5812301;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 184308;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "693.429993";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "727.250000";
				m4ttip = 184316;
				area = 184301;
				goal = 184304;
				m0fz = "-1329.000000";
				m0widx = 1;
				m1ttip = 184316;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-1122.089966";
				m3ttip = 184316;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1329.000000";
				m2fx = "848.159973";
				m2ttip = 184315;
				m4widx = 1;
				m0fx = "661.000000";
				m0ttip = 184315;
				m3pat = 2;
				m4pat = 2;
				title = 184302;
				m1fx = "661.000000";
				m2fz = "-1348.060059";
				sort = 184305;
				m4fz = "-808.580017";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 184308;
			gtype = 2;
			oklnk = 2;
			area = 184301;
			goal = 184304;
			sort = 184305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 184302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 184314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 184301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3612102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 184307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 37;
			}
			CDboTSClickNPC
			{
				npcidx = "3612102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
}

