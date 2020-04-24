CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2683;
	title = 268302;

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
				conv = 268309;
				ctype = 1;
				idx = 3144107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 268308;
				gtype = 1;
				area = 268301;
				goal = 268304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 268305;
				stype = 1;
				taid = 1;
				title = 268302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 268307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "3144107;";
			}
			CDboTSCheckClrQst
			{
				and = "2681;";
				flink = 1;
				flinknextqid = "2684;";
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
			cont = 268308;
			gtype = 1;
			oklnk = 2;
			area = 268301;
			goal = 268304;
			sort = 268305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 268302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 268314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 268301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3146108;";
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

