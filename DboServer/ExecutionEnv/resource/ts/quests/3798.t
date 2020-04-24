CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 24;
	rq = 0;
	tid = 3798;
	title = 379802;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 379807;
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
				npcidx = "1657105;";
			}
			CDboTSCheckClrQst
			{
				and = "3714;";
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
				conv = 379809;
				ctype = 1;
				idx = 1657105;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 15;
				eitype = 0;
				idx0 = 18412100;
				cnt0 = 15;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 11631100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 379808;
				m3widx = 15;
				m1fy = "0.000000";
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-2106.439941";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-2331.479980";
				m4ttip = 379816;
				area = 379801;
				goal = 379804;
				m0fz = "-5381.000000";
				m0widx = 15;
				m1ttip = 379815;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-5488.410156";
				m3ttip = 379816;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-5295.700195";
				m2fx = "-2395.840088";
				m2ttip = 379816;
				m4widx = 15;
				m0fx = "-1897.699951";
				m0ttip = 379815;
				m3pat = 2;
				m4pat = 2;
				title = 379802;
				m1fx = "-1952.989990";
				m2fz = "-5487.109863";
				sort = 379805;
				m4fz = "-5220.799805";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 379808;
			gtype = 2;
			oklnk = 2;
			area = 379801;
			goal = 379804;
			sort = 379805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 379802;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 379814;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 379801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1657105;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
	}
}

