CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1686;
	title = 168602;

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
				conv = 168609;
				ctype = 1;
				idx = 7135201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 168608;
				gtype = 1;
				area = 168601;
				goal = 168604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 168605;
				stype = 2;
				taid = 1;
				title = 168602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 168607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "7135201;";
			}
			CDboTSCheckClrQst
			{
				and = "1684;";
				flink = 1;
				flinknextqid = "1704;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 168608;
			gtype = 1;
			oklnk = 2;
			area = 168601;
			goal = 168604;
			sort = 168605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 168602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 168614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 168601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5313202;";
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

