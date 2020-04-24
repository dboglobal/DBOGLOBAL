CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1008;
	title = 100802;

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
				conv = 100809;
				ctype = 1;
				idx = 4072114;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 100808;
				gtype = 1;
				area = 100801;
				goal = 100804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 100805;
				stype = 2;
				taid = 1;
				title = 100802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 100814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 100801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3147201;";
			}
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
			cont = 100808;
			gtype = 1;
			oklnk = 2;
			area = 100801;
			goal = 100804;
			sort = 100805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 100802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 100807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 36;
			}
			CDboTSClickNPC
			{
				npcidx = "4072114;";
			}
			CDboTSCheckClrQst
			{
				and = "1007;";
				flink = 1;
				flinknextqid = "1009;";
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

