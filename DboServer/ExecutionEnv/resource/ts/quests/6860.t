CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 1;
	tid = 6860;
	title = 686002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 686008;
				gtype = 1;
				area = 686001;
				goal = 686004;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 686005;
				stype = 128;
				taid = 1;
				title = 686002;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 11253100;
				cnt0 = 30;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
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
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContReward
		{
			desc = 686014;
			nextlnk = 4;
			rwdtbl = 686001;
			rwdtype = 0;
			sidx0 = 109;
			stype1 = 0;
			sval0 = 1;
			ltime = -1;
			prelnk = "3;";
			sidx1 = 111;
			stype0 = 0;
			sval1 = 1;
			usetbl = 0;
			rwdexp = 0;
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;

			CDboTSClickNPC
			{
				npcidx = "2413115;2413116;2413117;2413118;2413119;2413120;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
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
			cont = 686008;
			gtype = 1;
			oklnk = 2;
			area = 686001;
			goal = 686004;
			sort = 686005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 686002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 686007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickSideIcon
			{
				sitype = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 50;
				minlvl = 41;
			}
		}
	}
}

