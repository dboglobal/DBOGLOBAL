CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 147;
	title = 14702;

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
				conv = 14709;
				ctype = 1;
				idx = 3142203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 14708;
				gtype = 1;
				area = 14701;
				goal = 14704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 14705;
				stype = 1;
				taid = 1;
				title = 14702;
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
			cont = 14708;
			gtype = 1;
			oklnk = 2;
			area = 14701;
			goal = 14704;
			sort = 14705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 14702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 14714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 14701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7071101;";
			}
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
			stdiag = 14707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 18;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "148;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3142203;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
		}
	}
}

