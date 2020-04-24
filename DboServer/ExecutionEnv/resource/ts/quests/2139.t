CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2139;
	title = 213902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 213909;
				ctype = 1;
				idx = 4153102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 213908;
				gtype = 1;
				area = 213901;
				goal = 213904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 213905;
				stype = 1;
				taid = 1;
				title = 213902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 213914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 213901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2801207;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 213907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "4153102;";
			}
			CDboTSCheckClrQst
			{
				and = "2137;";
				flink = 1;
				flinknextqid = "2140;";
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
			cont = 213908;
			gtype = 1;
			oklnk = 2;
			area = 213901;
			goal = 213904;
			sort = 213905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 213902;
		}
	}
}

