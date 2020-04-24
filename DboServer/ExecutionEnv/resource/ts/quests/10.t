CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 10;
	title = 1002;

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
				cont = 1008;
				gtype = 1;
				area = 1001;
				goal = 1004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 1005;
				stype = 64;
				taid = 1;
				title = 1002;
			}
			CDboTSActNPCConv
			{
				conv = 1009;
				ctype = 1;
				idx = 5041137;
				taid = 2;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 1014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 1001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8411101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "9;";
				flink = 1;
				flinknextqid = "11;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5041137;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1008;
			gtype = 1;
			oklnk = 2;
			area = 1001;
			goal = 1004;
			sort = 1005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 1002;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

