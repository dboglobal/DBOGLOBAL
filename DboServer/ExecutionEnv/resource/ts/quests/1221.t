CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1221;
	title = 122102;

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
				conv = 122109;
				ctype = 1;
				idx = 1351107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 122108;
				gtype = 1;
				area = 122101;
				goal = 122104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 122105;
				stype = 1;
				taid = 1;
				title = 122102;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 122108;
			gtype = 1;
			oklnk = 2;
			area = 122101;
			goal = 122104;
			sort = 122105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 122102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 122114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 122101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 122107;
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
				npcidx = "1351107;";
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

