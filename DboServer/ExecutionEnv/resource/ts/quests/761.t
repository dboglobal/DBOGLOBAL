CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 761;
	title = 76102;

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
				conv = 76109;
				ctype = 1;
				idx = 1311202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 76108;
				gtype = 1;
				area = 76101;
				goal = 76104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 76105;
				stype = 2;
				taid = 1;
				title = 76102;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 76108;
			gtype = 1;
			oklnk = 2;
			area = 76101;
			goal = 76104;
			sort = 76105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 76102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 76114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 76101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4142102;";
			}
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
			stdiag = 76107;
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
				minlvl = 25;
			}
			CDboTSCheckClrQst
			{
				and = "760;";
				flink = 1;
				flinknextqid = "762;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3173204;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

