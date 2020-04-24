CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1039;
	title = 103902;

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
				conv = 103909;
				ctype = 1;
				idx = 6412105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 103908;
				gtype = 1;
				area = 103901;
				goal = 103904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 103905;
				stype = 2;
				taid = 1;
				title = 103902;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 103908;
			gtype = 1;
			oklnk = 2;
			area = 103901;
			goal = 103904;
			sort = 103905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 103902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 103914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 103901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1752101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 103907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 38;
			}
			CDboTSClickNPC
			{
				npcidx = "6412105;";
			}
			CDboTSCheckClrQst
			{
				and = "1044;";
				flink = 1;
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
	}
}

