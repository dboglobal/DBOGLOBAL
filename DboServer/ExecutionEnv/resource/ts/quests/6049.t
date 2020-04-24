CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6049;
	title = 604902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 604909;
				ctype = 1;
				idx = 4141113;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 604908;
				gtype = 1;
				area = 604901;
				goal = 604904;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 604905;
				stype = 8;
				taid = 1;
				title = 604902;
			}
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
			stdiag = 604907;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6048;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4141113;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 604908;
			gtype = 1;
			oklnk = 2;
			area = 604901;
			goal = 604904;
			sort = 604905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 604902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 604914;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 604901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4143102;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
	}
}

