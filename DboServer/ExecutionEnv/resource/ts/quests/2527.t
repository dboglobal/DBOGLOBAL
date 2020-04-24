CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2527;
	title = 252702;

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
				conv = 252709;
				ctype = 1;
				idx = 3181108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 252708;
				gtype = 1;
				area = 252701;
				goal = 252704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 252705;
				stype = 2;
				taid = 1;
				title = 252702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 252707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "3181108;";
			}
			CDboTSCheckClrQst
			{
				and = "2519;";
				flink = 0;
				not = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 252708;
			gtype = 1;
			oklnk = 2;
			area = 252701;
			goal = 252704;
			sort = 252705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 252702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 252714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 252701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1483101;";
			}
		}
	}
}

