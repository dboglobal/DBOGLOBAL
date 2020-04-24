CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1083;
	title = 108302;

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
			stdiag = 108307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "1655201;";
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
				conv = 108309;
				ctype = 1;
				idx = 1655201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 8;
				eitype = 0;
				idx0 = 2612103;
				cnt0 = 7;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 1651104;
				taid = 3;
				esctype = 0;
				idx2 = 1653100;
			}
			CDboTSActRegQInfo
			{
				cont = 108308;
				m3widx = 4;
				m1fy = "0.000000";
				m2widx = 4;
				grade = 132203;
				m1pat = 2;
				m1widx = 4;
				m3fx = "-268.329987";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "86.919998";
				m4ttip = 108316;
				area = 108301;
				goal = 108304;
				m0fz = "-53.020000";
				m0widx = 4;
				m1ttip = 108315;
				m2fy = "0.000000";
				m2pat = 1;
				m3fz = "193.339996";
				m3ttip = 108316;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-171.589996";
				m2fx = "17.700001";
				m2ttip = 108317;
				m4widx = 4;
				m0fx = "129.520004";
				m0ttip = 108315;
				m3pat = 2;
				m4pat = 2;
				title = 108302;
				m1fx = "315.100006";
				m2fz = "-146.199997";
				sort = 108305;
				m4fz = "162.960007";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 108314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 108301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1655201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 108308;
			gtype = 3;
			oklnk = 2;
			area = 108301;
			goal = 108304;
			sort = 108305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 108302;
		}
	}
}

