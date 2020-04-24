CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = 20;
	rq = 0;
	tid = 2044;
	title = 204402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 204407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "2981109;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
				id = 660;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;3;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 204414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 204401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2981109;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 3;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 204409;
				ctype = 1;
				idx = 2981109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 204408;
				gtype = 1;
				area = 204401;
				goal = 204404;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 204405;
				stype = 1;
				taid = 1;
				title = 204402;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 204408;
			gtype = 1;
			oklnk = 2;
			area = 204401;
			goal = 204404;
			sort = 204405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 204402;
		}
	}
}

