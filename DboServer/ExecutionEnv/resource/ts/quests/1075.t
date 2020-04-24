CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 1075;
	title = 107502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 50;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 107507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSClickNPC
			{
				npcidx = "3132101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 50;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 107509;
				ctype = 1;
				idx = 3132101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 16412100;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 107508;
				area = 107501;
				goal = 107504;
				m0fz = "1432.300049";
				m0widx = 1;
				scitem = -1;
				sort = 107505;
				stype = 1;
				m0fy = "-84.290001";
				m0pat = 0;
				grade = 132203;
				m0fx = "2124.350098";
				m0ttip = 107515;
				rwd = 100;
				taid = 1;
				title = 107502;
				gtype = 2;
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
				eitype = 0;
				idx0 = 16412100;
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
			desc = 107514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 107501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "50;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3132101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 107508;
			gtype = 2;
			oklnk = 2;
			area = 107501;
			goal = 107504;
			sort = 107505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 107502;
		}
	}
}

