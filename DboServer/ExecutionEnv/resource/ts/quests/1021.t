CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1021;
	title = 102102;

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
				conv = 102109;
				ctype = 1;
				idx = 4071104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 102108;
				gtype = 1;
				area = 102101;
				goal = 102104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 102105;
				stype = 2;
				taid = 1;
				title = 102102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 102107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 37;
			}
			CDboTSClickNPC
			{
				npcidx = "4071104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1022;";
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
			cont = 102108;
			gtype = 1;
			oklnk = 2;
			area = 102101;
			goal = 102104;
			sort = 102105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 102102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 102114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 102101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2801103;";
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

