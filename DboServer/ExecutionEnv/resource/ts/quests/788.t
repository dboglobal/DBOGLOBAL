CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 788;
	title = 78802;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 78808;
			gtype = 2;
			oklnk = 2;
			area = 78801;
			goal = 78804;
			sort = 78805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 78802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 78814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 78801;
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
				npcidx = "6312109;";
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
			stdiag = 78807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 26;
			}
			CDboTSClickNPC
			{
				npcidx = "6312109;";
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
				conv = 78809;
				ctype = 1;
				idx = 6312109;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 1;
				idx0 = 568;
				cnt0 = 4;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 569;
				taid = 3;
				esctype = 0;
				idx2 = 570;
			}
			CDboTSActRegQInfo
			{
				cont = 78808;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "6089.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "6089.000000";
				m4ttip = 78816;
				area = 78801;
				goal = 78804;
				m0fz = "1320.000000";
				m0widx = 1;
				m1ttip = 78816;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "1250.000000";
				m3ttip = 78815;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1320.000000";
				m2fx = "5550.000000";
				m2ttip = 78817;
				m4widx = 1;
				m0fx = "5550.000000";
				m0ttip = 78815;
				m3pat = 2;
				m4pat = 2;
				title = 78802;
				m1fx = "5550.000000";
				m2fz = "1320.000000";
				sort = 78805;
				m4fz = "1250.000000";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 78807;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 568;
				iidx2 = 570;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 569;
				iprob0 = "1.000000";
			}
		}
	}
}

