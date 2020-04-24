CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 2038;
	title = 203802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1507;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
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
				eitype = 3;
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
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 3;
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
				conv = 203809;
				ctype = 1;
				idx = 5063103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 203808;
				m1fx = "2111.000000";
				m1fy = "0.000000";
				sort = 203805;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "5076.000000";
				grade = 132203;
				m0fx = "1805.000000";
				m0ttip = 203815;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 203802;
				gtype = 1;
				area = 203801;
				goal = 203804;
				m0fz = "5017.000000";
				m0widx = 1;
				m1ttip = 203815;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 203814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 203801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 10;
				iidx = 1507;
			}
			CDboTSClickNPC
			{
				npcidx = "5063103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 203807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "5063103;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 203808;
			gtype = 1;
			oklnk = 2;
			area = 203801;
			goal = 203804;
			sort = 203805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 203802;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1507;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 203807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

