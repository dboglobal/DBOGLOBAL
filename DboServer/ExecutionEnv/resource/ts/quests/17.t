CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 17;
	title = 1702;

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
				conv = 1709;
				ctype = 1;
				idx = 2801101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 1708;
				gtype = 1;
				area = 1701;
				goal = 1704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 1705;
				stype = 2;
				taid = 1;
				title = 1702;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1707;
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
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "2801101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "18;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1708;
			gtype = 1;
			oklnk = 2;
			area = 1701;
			goal = 1704;
			sort = 1705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 1702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 1714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 1701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2413101;";
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
	}
}

