CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6205;
	title = 620502;

	CNtlTSGroup
	{
		gid = 0;

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
			cont = 620508;
			gtype = 1;
			oklnk = 2;
			area = 620501;
			goal = 620504;
			sort = 620505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 620502;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 620509;
				ctype = 1;
				idx = 3031101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 620508;
				gtype = 1;
				area = 620501;
				goal = 620504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 620505;
				stype = 2;
				taid = 1;
				title = 620502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 620514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 620501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 620507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
			CDboTSCheckClrQst
			{
				and = "6204;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
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

