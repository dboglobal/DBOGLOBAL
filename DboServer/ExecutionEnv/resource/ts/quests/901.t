CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 901;
	title = 90102;

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
				conv = 90109;
				ctype = 1;
				idx = 5061101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 90108;
				gtype = 1;
				area = 90101;
				goal = 90104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 90105;
				stype = 2;
				taid = 1;
				title = 90102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 90114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 90101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2951102;";
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
			stdiag = 90107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
			CDboTSClickNPC
			{
				npcidx = "5061101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "902;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 90108;
			gtype = 1;
			oklnk = 2;
			area = 90101;
			goal = 90104;
			sort = 90105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 90102;
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

