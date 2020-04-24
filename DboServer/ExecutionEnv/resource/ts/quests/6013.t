CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6013;
	title = 601302;

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
				conv = 601309;
				ctype = 1;
				idx = 1751107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 601308;
				gtype = 2;
				area = 601301;
				goal = 601304;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 601305;
				stype = 8;
				taid = 1;
				title = 601302;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 601308;
			gtype = 2;
			oklnk = 2;
			area = 601301;
			goal = 601304;
			sort = 601305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 601302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 601314;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 601301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1752104;";
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
			stdiag = 601307;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "1751107;";
			}
			CDboTSCheckClrQst
			{
				and = "6012;";
				flink = 0;
				not = 0;
			}
		}
	}
}

