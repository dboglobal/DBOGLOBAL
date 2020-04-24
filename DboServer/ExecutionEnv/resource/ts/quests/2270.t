CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2270;
	title = 227002;

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
				conv = 227009;
				ctype = 1;
				idx = 5061118;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 227008;
				gtype = 1;
				area = 227001;
				goal = 227004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 227005;
				stype = 1;
				taid = 1;
				title = 227002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 227007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "5061118;";
			}
			CDboTSCheckClrQst
			{
				and = "2269;";
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
			cont = 227008;
			gtype = 1;
			oklnk = 2;
			area = 227001;
			goal = 227004;
			sort = 227005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 227002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 227014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 227001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5062108;";
			}
		}
	}
}

