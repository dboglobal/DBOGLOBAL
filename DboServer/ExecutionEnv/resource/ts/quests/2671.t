CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2671;
	title = 267102;

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
				conv = 267109;
				ctype = 1;
				idx = 5312107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 267108;
				gtype = 1;
				area = 267101;
				goal = 267104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 267105;
				stype = 1;
				taid = 1;
				title = 267102;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 267108;
			gtype = 1;
			oklnk = 2;
			area = 267101;
			goal = 267104;
			sort = 267105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 267102;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 267114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 267101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4652101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 267107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5312107;";
			}
			CDboTSCheckClrQst
			{
				and = "2670;";
				flink = 1;
				flinknextqid = "2672;";
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
	}
}

