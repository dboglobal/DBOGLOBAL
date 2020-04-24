CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 654;
	title = 65402;

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
				cont = 65408;
				gtype = 1;
				area = 65401;
				goal = 65404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 65405;
				stype = 2;
				taid = 1;
				title = 65402;
			}
			CDboTSActNPCConv
			{
				conv = 65409;
				ctype = 1;
				idx = 3031101;
				taid = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 65414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 65401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 65407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
			CDboTSCheckClrQst
			{
				and = "653;";
				flink = 1;
				flinknextqid = "655;";
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
			cont = 65408;
			gtype = 1;
			oklnk = 2;
			area = 65401;
			goal = 65404;
			sort = 65405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 65402;
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

