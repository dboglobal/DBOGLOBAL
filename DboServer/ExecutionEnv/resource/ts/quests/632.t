CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 632;
	title = 63202;

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
				conv = 63209;
				ctype = 1;
				idx = 3183101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 63208;
				gtype = 1;
				area = 63201;
				goal = 63204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 63205;
				stype = 2;
				taid = 1;
				title = 63202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 63207;
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
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "3183101;";
			}
			CDboTSCheckClrQst
			{
				and = "631;";
				flink = 1;
				flinknextqid = "633;";
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
			cont = 63208;
			gtype = 1;
			oklnk = 2;
			area = 63201;
			goal = 63204;
			sort = 63205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 63202;
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
			desc = 63214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 63201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4152101;";
			}
		}
	}
}

