CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6039;
	title = 603902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 603914;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 603901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752107;";
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
				conv = 603909;
				ctype = 1;
				idx = 4751108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 603908;
				gtype = 3;
				area = 603901;
				goal = 603904;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 603905;
				stype = 8;
				taid = 1;
				title = 603902;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 603908;
			gtype = 3;
			oklnk = 2;
			area = 603901;
			goal = 603904;
			sort = 603905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 603902;
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
			stdiag = 603907;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6038;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4751108;";
			}
		}
	}
}

