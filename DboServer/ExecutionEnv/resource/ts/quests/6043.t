CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6043;
	title = 604302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 604308;
			gtype = 3;
			oklnk = 2;
			area = 604301;
			goal = 604304;
			sort = 604305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 604302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 604314;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 604301;
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
				conv = 604309;
				ctype = 1;
				idx = 4751407;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 604308;
				gtype = 3;
				area = 604301;
				goal = 604304;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 604305;
				stype = 8;
				taid = 1;
				title = 604302;
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
			stdiag = 604307;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4751407;";
			}
			CDboTSCheckClrQst
			{
				and = "6042;";
				flink = 0;
				not = 0;
			}
		}
	}
}

