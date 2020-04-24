CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1233;
	title = 123302;

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
				conv = 123309;
				ctype = 1;
				idx = 6412101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 123308;
				gtype = 1;
				area = 123301;
				goal = 123304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 123305;
				stype = 1;
				taid = 1;
				title = 123302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 123314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 123301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 123308;
			gtype = 1;
			oklnk = 2;
			area = 123301;
			goal = 123304;
			sort = 123305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 123302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 123307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "6412101;";
			}
			CDboTSCheckClrQst
			{
				and = "1229;";
				flink = 1;
				not = 0;
			}
		}
	}
}

