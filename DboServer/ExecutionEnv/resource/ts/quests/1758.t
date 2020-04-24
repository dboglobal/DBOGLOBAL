CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1758;
	title = 175802;

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
				eitype = 1;
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
			stdiag = 175807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "1651109;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 175814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 175801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "1651109;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 175809;
				ctype = 1;
				idx = 1651109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 175808;
				m3widx = 10;
				m1fy = "0.000000";
				m2widx = 10;
				grade = 132203;
				m1pat = 2;
				m1widx = 10;
				m3fx = "355.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "26.000000";
				m4ttip = 175815;
				area = 175801;
				goal = 175804;
				m0fz = "-264.000000";
				m0widx = 10;
				m1ttip = 175816;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "77.000000";
				m3ttip = 175817;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-30.000000";
				m2fx = "194.000000";
				m2ttip = 175817;
				m4widx = 10;
				m0fx = "-155.000000";
				m0ttip = 175815;
				m3pat = 2;
				m4pat = 2;
				title = 175802;
				m1fx = "-141.000000";
				m2fz = "-32.000000";
				sort = 175805;
				m4fz = "-290.000000";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 4;
				eitype = 1;
				idx0 = 989;
				cnt0 = 4;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 990;
				taid = 3;
				esctype = 0;
				idx2 = 991;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 175808;
			gtype = 3;
			oklnk = 2;
			area = 175801;
			goal = 175804;
			sort = 175805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 175802;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 175807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 989;
				iidx2 = 991;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 990;
				iprob0 = "1.000000";
			}
		}
	}
}

