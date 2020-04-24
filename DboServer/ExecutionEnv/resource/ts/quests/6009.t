CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6009;
	title = 600902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 600908;
			gtype = 1;
			oklnk = 2;
			area = 600901;
			goal = 600904;
			sort = 600905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 600902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 600914;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 600901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1752104;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 600909;
				ctype = 1;
				idx = 2804128;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 600908;
				gtype = 1;
				area = 600901;
				goal = 600904;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 600905;
				stype = 8;
				taid = 1;
				title = 600902;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 600907;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "2804128;";
			}
			CDboTSCheckClrQst
			{
				and = "6008;";
				flink = 0;
				not = 0;
			}
		}
	}
}

