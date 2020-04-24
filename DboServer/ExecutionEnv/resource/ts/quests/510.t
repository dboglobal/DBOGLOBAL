CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 510;
	title = 51002;

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
				conv = 51009;
				ctype = 1;
				idx = 2971105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 51008;
				gtype = 1;
				area = 51001;
				goal = 51004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 51005;
				stype = 1;
				taid = 1;
				title = 51002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 51014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 51001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063101;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 51007;
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
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "2971105;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "511;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 51008;
			gtype = 1;
			oklnk = 2;
			area = 51001;
			goal = 51004;
			sort = 51005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 51002;
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

