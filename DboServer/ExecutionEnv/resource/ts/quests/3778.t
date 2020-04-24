CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3778;
	title = 377802;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 377809;
				ctype = 1;
				idx = 5631101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 377808;
				area = 377801;
				goal = 377804;
				m0fz = "-5490.470215";
				m0widx = 15;
				scitem = -1;
				sort = 377805;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = -1;
				m0fx = "-4451.299805";
				m0ttip = 377815;
				rwd = 100;
				taid = 1;
				title = 377802;
				gtype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377807;
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
				npcidx = "5631101;";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 3778;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 377808;
			gtype = 1;
			oklnk = 2;
			area = 377801;
			goal = 377804;
			sort = 377805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 377802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 377814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 377801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
		}
	}
}

