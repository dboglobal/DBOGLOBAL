CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1471;
	title = 147102;

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
				conv = 147109;
				ctype = 1;
				idx = 4111101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 147108;
				gtype = 1;
				area = 147101;
				goal = 147104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 147105;
				stype = 2;
				taid = 1;
				title = 147102;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 147114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 147101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6431101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 147108;
			gtype = 1;
			oklnk = 2;
			area = 147101;
			goal = 147104;
			sort = 147105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 147102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 147107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSCheckClrQst
			{
				and = "1277;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

