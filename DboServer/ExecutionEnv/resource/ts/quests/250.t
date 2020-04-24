CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 250;
	title = 25002;

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
				cont = 25008;
				gtype = 1;
				area = 25001;
				goal = 25004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 25005;
				stype = 1;
				taid = 1;
				title = 25002;
			}
			CDboTSActNPCConv
			{
				conv = 25009;
				ctype = 1;
				idx = 4751202;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 25008;
			gtype = 1;
			oklnk = 2;
			area = 25001;
			goal = 25004;
			sort = 25005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 25002;
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
			desc = 25014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 25007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "4751202;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "271;";
				not = 0;
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

