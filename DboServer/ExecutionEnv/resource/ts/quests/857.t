CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 857;
	title = 85702;

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
			stdiag = 85707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "1311201;";
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
				conv = 85709;
				ctype = 1;
				idx = 1311201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 9611102;
				cnt0 = 6;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 9612101;
				taid = 3;
				esctype = 0;
				idx2 = 9613101;
			}
			CDboTSActRegQInfo
			{
				cont = 85708;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4305.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "4305.000000";
				m4ttip = 85716;
				area = 85701;
				goal = 85704;
				m0fz = "1962.000000";
				m0widx = 1;
				m1ttip = 85716;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "2343.000000";
				m3ttip = 85715;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1962.000000";
				m2fx = "4434.000000";
				m2ttip = 85717;
				m4widx = 1;
				m0fx = "4399.000000";
				m0ttip = 85715;
				m3pat = 2;
				m4pat = 2;
				title = 85702;
				m1fx = "4399.000000";
				m2fz = "2142.000000";
				sort = 85705;
				m4fz = "2343.000000";
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
			cont = 85708;
			gtype = 2;
			oklnk = 2;
			area = 85701;
			goal = 85704;
			sort = 85705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 85702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 85714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 85701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311201;";
			}
		}
	}
}

