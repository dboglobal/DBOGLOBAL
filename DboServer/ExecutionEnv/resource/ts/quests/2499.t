CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2499;
	title = 249902;

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
				conv = 249909;
				ctype = 1;
				idx = 1351121;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 249908;
				gtype = 1;
				area = 249901;
				goal = 249904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 249905;
				stype = 2;
				taid = 1;
				title = 249902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 249907;
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
				npcidx = "1351121;";
			}
			CDboTSCheckClrQst
			{
				and = "2475;2488;";
				flink = 1;
				not = 0;
			}
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
			cont = 249908;
			gtype = 1;
			oklnk = 2;
			area = 249901;
			goal = 249904;
			sort = 249905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 249902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 249914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 249901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111202;";
			}
		}
	}
}

