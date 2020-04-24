CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1370;
	title = 137002;

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
				conv = 137009;
				ctype = 1;
				idx = 4111101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 137008;
				gtype = 1;
				area = 137001;
				goal = 137004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 137005;
				stype = 1;
				taid = 1;
				title = 137002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 137014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 137001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 137007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 44;
			}
			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
			CDboTSCheckClrQst
			{
				and = "1368;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 137008;
			gtype = 1;
			oklnk = 2;
			area = 137001;
			goal = 137004;
			sort = 137005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 137002;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

