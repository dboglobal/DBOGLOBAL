CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 937;
	title = 93702;

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
				conv = 93709;
				ctype = 1;
				idx = 2951102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 93708;
				gtype = 1;
				area = 93701;
				goal = 93704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 93705;
				stype = 2;
				taid = 1;
				title = 93702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 93714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 93701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172103;";
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
			cont = 93708;
			gtype = 1;
			oklnk = 2;
			area = 93701;
			goal = 93704;
			sort = 93705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 93702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 93707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 32;
			}
			CDboTSClickNPC
			{
				npcidx = "2951102;";
			}
			CDboTSCheckClrQst
			{
				and = "914;921;";
				flink = 1;
				not = 0;
			}
		}
	}
}

