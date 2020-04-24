CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1684;
	title = 168402;

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
				conv = 168409;
				ctype = 1;
				idx = 6312110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 168408;
				gtype = 1;
				area = 168401;
				goal = 168404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 168405;
				stype = 2;
				taid = 1;
				title = 168402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 168407;
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
				npcidx = "6312110;";
			}
			CDboTSCheckClrQst
			{
				and = "1683;";
				flink = 1;
				flinknextqid = "1686;";
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
			cont = 168408;
			gtype = 1;
			oklnk = 2;
			area = 168401;
			goal = 168404;
			sort = 168405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 168402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 168414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 168401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7135201;";
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

