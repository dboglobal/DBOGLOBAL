CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2994;
	title = 299402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 299407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "4755101;";
			}
			CDboTSCheckClrQst
			{
				and = "2991;";
				flink = 0;
				flinknextqid = "2995;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 299414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 299401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511314;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 299408;
			gtype = 1;
			oklnk = 2;
			area = 299401;
			goal = 299404;
			sort = 299405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 299402;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 299409;
				ctype = 1;
				idx = 4755101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 299408;
				area = 299401;
				goal = 299404;
				m0fz = "-812.650024";
				m0widx = 6;
				scitem = -1;
				sort = 299405;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "711.950012";
				m0ttip = 299415;
				rwd = 100;
				taid = 1;
				title = 299402;
				gtype = 1;
			}
		}
	}
}

