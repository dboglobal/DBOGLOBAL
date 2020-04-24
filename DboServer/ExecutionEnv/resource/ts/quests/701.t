CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 701;
	title = 70102;

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
				conv = 70109;
				ctype = 1;
				idx = 3183108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 70108;
				gtype = 1;
				area = 70101;
				goal = 70104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 70105;
				stype = 2;
				taid = 1;
				title = 70102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 70114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 70101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061202;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 70107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "3183108;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "702;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 70108;
			gtype = 1;
			oklnk = 2;
			area = 70101;
			goal = 70104;
			sort = 70105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 70102;
		}
	}
}

