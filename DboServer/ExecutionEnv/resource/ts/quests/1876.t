CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1876;
	title = 187602;

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
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 187609;
				ctype = 1;
				idx = 4072118;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 187608;
				m1fx = "811.469971";
				m1fy = "0.000000";
				sort = 187605;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1922.989990";
				grade = 132203;
				m0fx = "989.359985";
				m0ttip = 187615;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 187602;
				gtype = 2;
				area = 187601;
				goal = 187604;
				m0fz = "1940.619995";
				m0widx = 1;
				m1ttip = 187615;
				scitem = -1;
				stype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1084;
				cnt0 = 9;
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
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 187614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 187601;
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
				npcidx = "4072118;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 187608;
			gtype = 2;
			oklnk = 2;
			area = 187601;
			goal = 187604;
			sort = 187605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 187602;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 187607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSClickNPC
			{
				npcidx = "4072118;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1084;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 187607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

