CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1271;
	title = 127102;

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
				conv = 127109;
				ctype = 1;
				idx = 4111101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 127108;
				gtype = 1;
				area = 127101;
				goal = 127104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 127105;
				stype = 2;
				taid = 1;
				title = 127102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 127114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 127101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 127107;
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
				npcidx = "4111101;";
			}
			CDboTSCheckClrQst
			{
				and = "1249;";
				flink = 1;
				flinknextqid = "1272;";
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
			cont = 127108;
			gtype = 1;
			oklnk = 2;
			area = 127101;
			goal = 127104;
			sort = 127105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 127102;
		}
	}
}

