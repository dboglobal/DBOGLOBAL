CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6033;
	title = 603302;

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
				conv = 603309;
				ctype = 1;
				idx = 4511608;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 603308;
				gtype = 1;
				area = 603301;
				goal = 603304;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 603305;
				stype = 8;
				taid = 1;
				title = 603302;
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
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 603308;
			gtype = 1;
			oklnk = 2;
			area = 603301;
			goal = 603304;
			sort = 603305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 603302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 603307;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4511608;";
			}
			CDboTSCheckClrQst
			{
				and = "6032;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 603314;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 603301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4512102;";
			}
		}
	}
}

