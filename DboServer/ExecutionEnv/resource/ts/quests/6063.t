CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6063;
	title = 606302;

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
				conv = 606309;
				ctype = 1;
				idx = 4261111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 606308;
				gtype = 1;
				area = 606301;
				goal = 606304;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 606305;
				stype = 8;
				taid = 1;
				title = 606302;
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
			stdiag = 606307;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4261111;";
			}
			CDboTSCheckClrQst
			{
				and = "6062;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 606314;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 606301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372107;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 606308;
			gtype = 1;
			oklnk = 2;
			area = 606301;
			goal = 606304;
			sort = 606305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 606302;
		}
	}
}

