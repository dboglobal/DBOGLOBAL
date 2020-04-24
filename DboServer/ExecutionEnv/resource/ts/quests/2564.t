CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2564;
	title = 256402;

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
				conv = 256409;
				ctype = 1;
				idx = 5101101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 256408;
				gtype = 1;
				area = 256401;
				goal = 256404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 256405;
				stype = 1;
				taid = 1;
				title = 256402;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 256414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 256401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7542101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 256407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5101101;";
			}
			CDboTSCheckClrQst
			{
				and = "2545;";
				flink = 0;
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
			cont = 256408;
			gtype = 1;
			oklnk = 2;
			area = 256401;
			goal = 256404;
			sort = 256405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 256402;
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

