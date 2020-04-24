CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1053;
	title = 105302;

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
				conv = 105309;
				ctype = 1;
				idx = 8031201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 105308;
				gtype = 1;
				area = 105301;
				goal = 105304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 105305;
				stype = 2;
				taid = 1;
				title = 105302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 105307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSClickNPC
			{
				npcidx = "8031201;";
			}
			CDboTSCheckClrQst
			{
				and = "1052;";
				flink = 1;
				flinknextqid = "1076;1054;";
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
			cont = 105308;
			gtype = 1;
			oklnk = 2;
			area = 105301;
			goal = 105304;
			sort = 105305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 105302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 105314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 105301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071114;";
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

