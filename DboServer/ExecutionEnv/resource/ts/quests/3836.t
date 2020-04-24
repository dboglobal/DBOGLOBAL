CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 24;
	rq = 0;
	tid = 3836;
	title = 383602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 383607;
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
				npcidx = "5062201;";
			}
			CDboTSCheckClrQst
			{
				and = "3775;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 383609;
				ctype = 1;
				idx = 5062201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 383608;
				m1fx = "-4940.069824";
				m2fz = "-5356.740234";
				m1fy = "0.000000";
				sort = 383605;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 383601;
				goal = 383604;
				m0fz = "-5543.990234";
				m0widx = 15;
				m1ttip = 383615;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-5571.009766";
				m2fx = "-4863.580078";
				m2ttip = 383615;
				m0fx = "-5126.450195";
				m0ttip = 383615;
				title = 383602;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 16621101;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 383608;
			gtype = 2;
			oklnk = 2;
			area = 383601;
			goal = 383604;
			sort = 383605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 383602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 383614;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 383601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5062201;";
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
	}
}

