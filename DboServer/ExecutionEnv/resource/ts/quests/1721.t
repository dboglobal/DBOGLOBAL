CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1721;
	title = 172102;

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
				conv = 172109;
				ctype = 1;
				idx = 5411102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 172108;
				gtype = 1;
				area = 172101;
				goal = 172104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 172105;
				stype = 2;
				taid = 1;
				title = 172102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 172114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 172101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6311102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 172107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "5411102;";
			}
			CDboTSCheckClrQst
			{
				and = "1717;1720;";
				flink = 1;
				flinknextqid = "1722;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 172108;
			gtype = 1;
			oklnk = 2;
			area = 172101;
			goal = 172104;
			sort = 172105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 172102;
		}
	}
}

