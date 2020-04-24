CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2289;
	title = 228902;

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
				conv = 228909;
				ctype = 1;
				idx = 7637103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 228908;
				gtype = 1;
				area = 228901;
				goal = 228904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 228905;
				stype = 2;
				taid = 1;
				title = 228902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 228907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				and = "2288;";
				flink = 1;
				flinknextqid = "2290;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7637103;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 228908;
			gtype = 1;
			oklnk = 2;
			area = 228901;
			goal = 228904;
			sort = 228905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 228902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 228914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 228901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1491102;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

