CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1896;
	title = 189602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 189607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "8031201;";
			}
			CDboTSCheckClrQst
			{
				and = "1052;";
				flink = 1;
				flinknextqid = "1897;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 189608;
			gtype = 1;
			oklnk = 2;
			area = 189601;
			goal = 189604;
			sort = 189605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 189602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 189614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 189601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6312103;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 189609;
				ctype = 1;
				idx = 8031201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 189608;
				gtype = 1;
				area = 189601;
				goal = 189604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 189605;
				stype = 2;
				taid = 1;
				title = 189602;
			}
		}
	}
}

