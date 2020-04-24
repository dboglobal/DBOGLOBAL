CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2357;
	title = 235702;

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
				conv = 235709;
				ctype = 1;
				idx = 5063109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 235708;
				gtype = 1;
				area = 235701;
				goal = 235704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 235705;
				stype = 2;
				taid = 1;
				title = 235702;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 235708;
			gtype = 1;
			oklnk = 2;
			area = 235701;
			goal = 235704;
			sort = 235705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 235702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 235714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 235701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351121;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 235707;
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
				npcidx = "5063109;";
			}
			CDboTSCheckClrQst
			{
				and = "2356;";
				flink = 1;
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
	}
}

