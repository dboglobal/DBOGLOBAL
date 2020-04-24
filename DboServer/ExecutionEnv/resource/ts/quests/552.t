CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 552;
	title = 55202;

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
				conv = 55209;
				ctype = 1;
				idx = 2611201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 55208;
				gtype = 1;
				area = 55201;
				goal = 55204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 55205;
				stype = 2;
				taid = 1;
				title = 55202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 55214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 55201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141107;";
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
			stdiag = 55207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "2611201;";
			}
			CDboTSCheckClrQst
			{
				and = "547;";
				flink = 1;
				flinknextqid = "573;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 55208;
			gtype = 1;
			oklnk = 2;
			area = 55201;
			goal = 55204;
			sort = 55205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 55202;
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

