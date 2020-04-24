CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 965;
	title = 96502;

	CNtlTSGroup
	{
		gid = 0;

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
			desc = 96514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 96501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072106;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 96509;
				ctype = 1;
				idx = 3613104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 96508;
				gtype = 1;
				area = 96501;
				goal = 96504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 96505;
				stype = 1;
				taid = 1;
				title = 96502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 96507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 34;
			}
			CDboTSClickNPC
			{
				npcidx = "3613104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "962;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 96508;
			gtype = 1;
			oklnk = 2;
			area = 96501;
			goal = 96504;
			sort = 96505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 96502;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
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

