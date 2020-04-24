CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 496;
	title = 49602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99029;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 49609;
				ctype = 1;
				idx = 3221101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 49608;
				gtype = 2;
				area = 49601;
				goal = 49604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 49605;
				stype = 2;
				taid = 1;
				title = 49602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 49607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSCheckClrQst
			{
				and = "488;";
				flink = 1;
				flinknextqid = "497;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 49608;
			gtype = 2;
			oklnk = 2;
			area = 49601;
			goal = 49604;
			sort = 49605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 49602;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 49614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 49601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99029;
			}
			CDboTSClickNPC
			{
				npcidx = "3221101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 216;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 216;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

