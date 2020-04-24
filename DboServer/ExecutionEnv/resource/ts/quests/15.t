CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 15;
	title = 1502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 1508;
				gtype = 1;
				area = 1501;
				goal = 1504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 1505;
				stype = 2;
				taid = 1;
				title = 1502;
			}
			CDboTSActNPCConv
			{
				conv = 1509;
				ctype = 1;
				idx = 5591202;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1508;
			gtype = 1;
			oklnk = 2;
			area = 1501;
			goal = 1504;
			sort = 1505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 1502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 1514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 1501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031101;";
			}
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "5591202;";
			}
			CDboTSCheckClrQst
			{
				and = "14;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

