CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2317;
	title = 231702;

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
				conv = 231709;
				ctype = 1;
				idx = 7637104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 231708;
				gtype = 1;
				area = 231701;
				goal = 231704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 231705;
				stype = 1;
				taid = 1;
				title = 231702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 231707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "7637104;";
			}
			CDboTSCheckClrQst
			{
				and = "2316;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 231708;
			gtype = 1;
			oklnk = 2;
			area = 231701;
			goal = 231704;
			sort = 231705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 231702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 231714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 231701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6412113;";
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
	}
}

