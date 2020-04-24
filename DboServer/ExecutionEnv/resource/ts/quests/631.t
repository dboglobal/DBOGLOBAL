CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 631;
	title = 63102;

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
				conv = 63109;
				ctype = 1;
				idx = 4261102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 63108;
				gtype = 1;
				area = 63101;
				goal = 63104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 63105;
				stype = 2;
				taid = 1;
				title = 63102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 63107;
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
				npcidx = "4261102;";
			}
			CDboTSCheckClrQst
			{
				and = "630;";
				flink = 1;
				flinknextqid = "632;";
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
			cont = 63108;
			gtype = 1;
			oklnk = 2;
			area = 63101;
			goal = 63104;
			sort = 63105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 63102;
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
			desc = 63114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 63101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183101;";
			}
		}
	}
}

