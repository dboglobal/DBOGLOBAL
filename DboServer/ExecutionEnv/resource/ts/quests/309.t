CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 309;
	title = 30902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 3092;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 30909;
				ctype = 1;
				idx = 4211101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 30908;
				gtype = 1;
				area = 30901;
				goal = 30904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 30905;
				stype = 2;
				taid = 1;
				title = 30902;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 3091;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 30914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 30901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 30907;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
			CDboTSCheckClrQst
			{
				and = "308;";
				flink = 1;
				flinknextqid = "372;";
				not = 0;
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
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 30908;
			gtype = 1;
			oklnk = 2;
			area = 30901;
			goal = 30904;
			sort = 30905;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 30902;
		}
	}
}

