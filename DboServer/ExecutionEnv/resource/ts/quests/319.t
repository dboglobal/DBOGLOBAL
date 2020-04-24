CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 15;
	rq = 0;
	tid = 319;
	title = 31902;

	CNtlTSGroup
	{
		gid = 0;

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
			cont = 31908;
			gtype = 2;
			oklnk = 2;
			area = 31901;
			goal = 31904;
			sort = 31905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 31902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 31914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 31901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752202;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			stdiag = 31907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "4752202;";
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
				conv = 31909;
				ctype = 1;
				idx = 4752202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 31908;
				gtype = 2;
				area = 31901;
				goal = 31904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 31905;
				stype = 64;
				taid = 1;
				title = 31902;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSFreeBattle
			{
			}
		}
	}
}

