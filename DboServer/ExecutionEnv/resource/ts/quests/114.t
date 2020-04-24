CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 16;
	rq = 0;
	tid = 114;
	title = 11402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 11408;
			gtype = 2;
			oklnk = 2;
			area = 11401;
			goal = 11404;
			sort = 11405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 11402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 11414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 11401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 11407;
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
				minlvl = 10;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "115;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7071101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 11409;
				ctype = 1;
				idx = 7071101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 11408;
				gtype = 2;
				area = 11401;
				goal = 11404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 11405;
				stype = 64;
				taid = 1;
				title = 11402;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRB
			{
				cond = 1;
				idx = -1;
			}
		}
	}
}

