CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2179;
	title = 217902;

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
				conv = 217909;
				ctype = 1;
				idx = 3031305;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 217908;
				gtype = 1;
				area = 217901;
				goal = 217904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 217905;
				stype = 1;
				taid = 1;
				title = 217902;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 217908;
			gtype = 1;
			oklnk = 2;
			area = 217901;
			goal = 217904;
			sort = 217905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 217902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 217914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 217901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3721201;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 217907;
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
				npcidx = "3031305;";
			}
			CDboTSCheckClrQst
			{
				and = "2178;";
				flink = 1;
				flinknextqid = "2180;";
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
	}
}

