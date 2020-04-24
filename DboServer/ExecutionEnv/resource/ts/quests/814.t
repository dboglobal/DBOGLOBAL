CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 814;
	title = 81402;

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
				conv = 81409;
				ctype = 1;
				idx = 3031207;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 81408;
				gtype = 1;
				area = 81401;
				goal = 81404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 81405;
				stype = 2;
				taid = 1;
				title = 81402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 81407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 28;
			}
			CDboTSClickNPC
			{
				npcidx = "3031207;";
			}
			CDboTSCheckClrQst
			{
				and = "813;";
				flink = 1;
				flinknextqid = "815;";
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
			cont = 81408;
			gtype = 1;
			oklnk = 2;
			area = 81401;
			goal = 81404;
			sort = 81405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 81402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 81414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 81401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653101;";
			}
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

