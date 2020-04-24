CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 613;
	title = 61302;

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
				cont = 61308;
				gtype = 1;
				area = 61301;
				goal = 61304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 61305;
				stype = 2;
				taid = 1;
				title = 61302;
			}
			CDboTSActNPCConv
			{
				conv = 61309;
				ctype = 1;
				idx = 3332203;
				taid = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 61314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 61301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3032103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 61307;
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
				npcidx = "3332203;";
			}
			CDboTSCheckClrQst
			{
				and = "612;";
				flink = 1;
				flinknextqid = "614;";
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
			cont = 61308;
			gtype = 1;
			oklnk = 2;
			area = 61301;
			goal = 61304;
			sort = 61305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 61302;
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

