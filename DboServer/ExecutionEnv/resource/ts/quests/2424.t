CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2424;
	title = 242402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;4;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "6463101;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 242420;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickObject
			{
				objidx = "115;";
				widx = 14;
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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 242407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "6463101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2425;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 242414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 242401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 242409;
				ctype = 1;
				idx = 6463101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 242408;
				gtype = 2;
				area = 242401;
				goal = 242404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 242405;
				stype = 2;
				taid = 1;
				title = 242402;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 242408;
			gtype = 2;
			oklnk = 2;
			area = 242401;
			goal = 242404;
			sort = 242405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 242402;
		}
	}
}

