CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 204;
	title = 20402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 3;

			CDboTSClickNPC
			{
				npcidx = "4651101;";
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 3;
			idx = 2051;
			lilnk = 255;
			oklnk = 100;
			prelnk = "4;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 20409;
				ctype = 1;
				idx = 4211101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 20408;
				gtype = 1;
				area = 20401;
				goal = 20404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 20405;
				stype = 2;
				taid = 1;
				title = 20402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 20407;
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
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "205;";
				not = 0;
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
			cont = 20408;
			gtype = 1;
			oklnk = 2;
			area = 20401;
			goal = 20404;
			sort = 20405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 20402;
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
			canclnk = 4;
			cid = 100;
			rwdzeny = 0;
			desc = 20414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 20401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;3;";
			usetbl = 1;
		}
	}
}

