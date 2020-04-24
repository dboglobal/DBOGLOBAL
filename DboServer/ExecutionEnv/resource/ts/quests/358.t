CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 358;
	title = 35802;

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
				conv = 35809;
				ctype = 1;
				idx = 4511405;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 35808;
				gtype = 1;
				area = 35801;
				goal = 35804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 35805;
				stype = 1;
				taid = 1;
				title = 35802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 35814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 35801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751204;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 35808;
			gtype = 1;
			oklnk = 2;
			area = 35801;
			goal = 35804;
			sort = 35805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 35802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 35807;
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
				raceflg = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4511405;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "360;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

