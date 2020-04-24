CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1277;
	title = 127702;

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
				conv = 127709;
				ctype = 1;
				idx = 2611102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 127708;
				gtype = 1;
				area = 127701;
				goal = 127704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 127705;
				stype = 2;
				taid = 1;
				title = 127702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 127707;
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
				npcidx = "2611102;";
			}
			CDboTSCheckClrQst
			{
				and = "1276;";
				flink = 1;
				flinknextqid = "1278;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 127708;
			gtype = 1;
			oklnk = 2;
			area = 127701;
			goal = 127704;
			sort = 127705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 127702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 127714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 127701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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

