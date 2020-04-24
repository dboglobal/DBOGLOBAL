CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 2;
	rq = 0;
	tid = 529;
	title = 52902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 529;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 529;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 235;
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
				conv = 52909;
				ctype = 1;
				idx = 3172104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 52908;
				gtype = 3;
				area = 52901;
				goal = 52904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 52905;
				stype = 1;
				taid = 1;
				title = 52902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 52907;
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
				minlvl = 17;
			}
			CDboTSClickNPC
			{
				npcidx = "3172104;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;3;2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 52908;
			gtype = 3;
			oklnk = 2;
			area = 52901;
			goal = 52904;
			sort = 52905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 52902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 52914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 52901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1551106;";
			}
		}
	}
}

