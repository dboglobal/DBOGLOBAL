CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 674;
	title = 67402;

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
				conv = 67409;
				ctype = 1;
				idx = 3182102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 67408;
				gtype = 1;
				area = 67401;
				goal = 67404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 67405;
				stype = 2;
				taid = 1;
				title = 67402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 67407;
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
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "3182102;";
			}
			CDboTSCheckClrQst
			{
				and = "650;";
				flink = 1;
				flinknextqid = "675;";
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
			cont = 67408;
			gtype = 1;
			oklnk = 2;
			area = 67401;
			goal = 67404;
			sort = 67405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 67402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 67414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 67401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332203;";
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
	}
}

