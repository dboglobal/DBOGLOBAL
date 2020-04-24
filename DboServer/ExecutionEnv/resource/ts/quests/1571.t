CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1571;
	title = 157102;

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
				conv = 157109;
				ctype = 1;
				idx = 1654105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 157108;
				gtype = 1;
				area = 157101;
				goal = 157104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 157105;
				stype = 1;
				taid = 1;
				title = 157102;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 157108;
			gtype = 1;
			oklnk = 2;
			area = 157101;
			goal = 157104;
			sort = 157105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 157102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 157114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 157101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6203101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 157107;
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
				flink = 1;
				flinknextqid = "1583;1607;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1654105;";
			}
		}
	}
}

