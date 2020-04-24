CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1249;
	title = 124902;

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
				conv = 124909;
				ctype = 1;
				idx = 5041144;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 124908;
				gtype = 1;
				area = 124901;
				goal = 124904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 124905;
				stype = 2;
				taid = 1;
				title = 124902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 124907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "5041144;";
			}
			CDboTSCheckClrQst
			{
				and = "1248;";
				flink = 1;
				flinknextqid = "1271;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 124908;
			gtype = 1;
			oklnk = 2;
			area = 124901;
			goal = 124904;
			sort = 124905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 124902;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 124914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 124901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
		}
	}
}

