CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2579;
	title = 257902;

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
				conv = 257909;
				ctype = 1;
				idx = 7073110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 257908;
				gtype = 1;
				area = 257901;
				goal = 257904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 257905;
				stype = 2;
				taid = 1;
				title = 257902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 257914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 257901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1352102;";
			}
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
			cont = 257908;
			gtype = 1;
			oklnk = 2;
			area = 257901;
			goal = 257904;
			sort = 257905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 257902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 257907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "7073110;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2586;";
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
	}
}

