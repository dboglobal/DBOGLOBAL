CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 301;
	title = 30102;

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
				conv = 30109;
				ctype = 1;
				idx = 4752101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 30108;
				gtype = 1;
				area = 30101;
				goal = 30104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 30105;
				stype = 1;
				taid = 1;
				title = 30102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 30107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "4752101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "302;";
				not = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 30108;
			gtype = 1;
			oklnk = 2;
			area = 30101;
			goal = 30104;
			sort = 30105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 30102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 30114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 30101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752104;";
			}
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

