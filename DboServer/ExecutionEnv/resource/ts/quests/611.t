CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 611;
	title = 61102;

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
				cont = 61108;
				gtype = 1;
				area = 61101;
				goal = 61104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 61105;
				stype = 2;
				taid = 1;
				title = 61102;
			}
			CDboTSActNPCConv
			{
				conv = 61109;
				ctype = 1;
				idx = 3031203;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 61108;
			gtype = 1;
			oklnk = 2;
			area = 61101;
			goal = 61104;
			sort = 61105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 61102;
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
			desc = 61114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 61101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3173103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 61107;
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
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "3031203;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "612;";
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
	}
}

