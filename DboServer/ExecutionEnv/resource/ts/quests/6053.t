CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6053;
	title = 605302;

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
				conv = 605309;
				ctype = 1;
				idx = 4151108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 605308;
				gtype = 1;
				area = 605301;
				goal = 605304;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 605305;
				stype = 8;
				taid = 1;
				title = 605302;
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
			stdiag = 605307;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4151108;";
			}
			CDboTSCheckClrQst
			{
				and = "6052;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 605314;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 605301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4143102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 605308;
			gtype = 1;
			oklnk = 2;
			area = 605301;
			goal = 605304;
			sort = 605305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 605302;
		}
	}
}

