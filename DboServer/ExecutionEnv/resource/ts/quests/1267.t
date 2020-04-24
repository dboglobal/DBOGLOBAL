CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1267;
	title = 126702;

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
				conv = 126709;
				ctype = 1;
				idx = 4372101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 126708;
				gtype = 1;
				area = 126701;
				goal = 126704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 126705;
				stype = 1;
				taid = 1;
				title = 126702;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 126708;
			gtype = 1;
			oklnk = 2;
			area = 126701;
			goal = 126704;
			sort = 126705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 126702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 126714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 126701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4261104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 126707;
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
				npcidx = "4372101;";
			}
			CDboTSCheckClrQst
			{
				and = "1266;";
				flink = 1;
				flinknextqid = "1268;";
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
	}
}

