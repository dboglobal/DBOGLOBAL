CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1096;
	title = 109602;

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
				conv = 109609;
				ctype = 1;
				idx = 4072101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 109608;
				gtype = 1;
				area = 109601;
				goal = 109604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 109605;
				stype = 2;
				taid = 1;
				title = 109602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 109607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSClickNPC
			{
				npcidx = "4072101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1102;";
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
			cont = 109608;
			gtype = 1;
			oklnk = 2;
			area = 109601;
			goal = 109604;
			sort = 109605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 109602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 109614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 109601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072118;";
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

