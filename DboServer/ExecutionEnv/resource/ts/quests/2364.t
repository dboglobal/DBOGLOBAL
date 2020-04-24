CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2364;
	title = 236402;

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
				conv = 236409;
				ctype = 1;
				idx = 5731106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 236408;
				gtype = 1;
				area = 236401;
				goal = 236404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 236405;
				stype = 1;
				taid = 1;
				title = 236402;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 236414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 236401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111106;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 236407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "5731106;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2365;";
				not = 0;
			}
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
			cont = 236408;
			gtype = 1;
			oklnk = 2;
			area = 236401;
			goal = 236404;
			sort = 236405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 236402;
		}
	}
}

