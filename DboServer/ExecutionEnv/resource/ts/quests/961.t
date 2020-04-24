CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 961;
	title = 96102;

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
				conv = 96109;
				ctype = 1;
				idx = 1652101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 96108;
				gtype = 3;
				area = 96101;
				goal = 96104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 96105;
				stype = 2;
				taid = 1;
				title = 96102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 96107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 33;
			}
			CDboTSClickNPC
			{
				npcidx = "1652101;";
			}
			CDboTSCheckClrQst
			{
				and = "949;";
				flink = 1;
				flinknextqid = "1879;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
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
			cont = 96108;
			gtype = 3;
			oklnk = 2;
			area = 96101;
			goal = 96104;
			sort = 96105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 96102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 96114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 96101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3273102;";
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

