CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 490;
	title = 49002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 49009;
				ctype = 1;
				idx = 4372207;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 49008;
				gtype = 1;
				area = 49001;
				goal = 49004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 49005;
				stype = 2;
				taid = 1;
				title = 49002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 49014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 49001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3171101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 49007;
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
				minlvl = 13;
			}
			CDboTSClickNPC
			{
				npcidx = "4372207;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "504;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 49008;
			gtype = 1;
			oklnk = 2;
			area = 49001;
			goal = 49004;
			sort = 49005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 49002;
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

