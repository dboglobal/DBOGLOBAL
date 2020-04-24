CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1241;
	title = 124102;

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
				conv = 124109;
				ctype = 1;
				idx = 5533402;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 124108;
				gtype = 1;
				area = 124101;
				goal = 124104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 124105;
				stype = 1;
				taid = 1;
				title = 124102;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 124107;
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
				npcidx = "5533402;";
			}
			CDboTSCheckClrQst
			{
				and = "1240;";
				flink = 1;
				flinknextqid = "1242;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 124108;
			gtype = 1;
			oklnk = 2;
			area = 124101;
			goal = 124104;
			sort = 124105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 124102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 124114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 124101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1431102;";
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

