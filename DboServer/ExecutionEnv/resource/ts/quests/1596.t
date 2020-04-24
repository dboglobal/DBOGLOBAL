CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1596;
	title = 159602;

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
				conv = 159609;
				ctype = 1;
				idx = 3143116;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 159608;
				gtype = 1;
				area = 159601;
				goal = 159604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 159605;
				stype = 1;
				taid = 1;
				title = 159602;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 159608;
			gtype = 1;
			oklnk = 2;
			area = 159601;
			goal = 159604;
			sort = 159605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 159602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 159614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 159601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172401;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 159607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSCheckClrQst
			{
				and = "1588;";
				flink = 1;
				flinknextqid = "1607;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3143116;";
			}
		}
	}
}

