CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 35;
	title = 3502;

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
				conv = 3509;
				ctype = 1;
				idx = 5591201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 3508;
				gtype = 1;
				area = 3501;
				goal = 3504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 3505;
				stype = 1;
				taid = 1;
				title = 3502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 3507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 7;
			}
			CDboTSCheckClrQst
			{
				and = "37;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5591201;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 3508;
			gtype = 1;
			oklnk = 2;
			area = 3501;
			goal = 3504;
			sort = 3505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 3502;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 3514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 3501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2971101;";
			}
		}
	}
}

