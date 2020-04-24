CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6244;
	title = 624402;

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
				conv = 624409;
				ctype = 1;
				idx = 1653110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 624408;
				gtype = 2;
				area = 624401;
				goal = 624404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 624405;
				stype = 2;
				taid = 1;
				title = 624402;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 624414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 624401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5411102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 624407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "1653110;";
			}
			CDboTSCheckClrQst
			{
				and = "6241;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 624408;
			gtype = 2;
			oklnk = 2;
			area = 624401;
			goal = 624404;
			sort = 624405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 624402;
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

