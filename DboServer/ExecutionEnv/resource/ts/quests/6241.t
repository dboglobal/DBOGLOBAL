CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6241;
	title = 624102;

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
				conv = 624109;
				ctype = 1;
				idx = 1654105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 624108;
				gtype = 2;
				area = 624101;
				goal = 624104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 624105;
				stype = 2;
				taid = 1;
				title = 624102;
			}
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
			cont = 624108;
			gtype = 2;
			oklnk = 2;
			area = 624101;
			goal = 624104;
			sort = 624105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 624102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 624114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 624101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653110;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 624107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "1654105;";
			}
		}
	}
}

