CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1897;
	title = 189702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 189707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "6312103;";
			}
			CDboTSCheckClrQst
			{
				and = "1896;";
				flink = 1;
				flinknextqid = "1898;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 189714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 189701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1501103;";
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
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 189708;
			gtype = 1;
			oklnk = 2;
			area = 189701;
			goal = 189704;
			sort = 189705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 189702;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 189709;
				ctype = 1;
				idx = 6312103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 189708;
				gtype = 1;
				area = 189701;
				goal = 189704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 189705;
				stype = 2;
				taid = 1;
				title = 189702;
			}
		}
	}
}

