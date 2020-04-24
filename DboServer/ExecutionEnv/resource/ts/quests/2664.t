CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2664;
	title = 266402;

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
				conv = 266409;
				ctype = 1;
				idx = 2801114;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 266408;
				gtype = 1;
				area = 266401;
				goal = 266404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 266405;
				stype = 2;
				taid = 1;
				title = 266402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 266407;
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
				npcidx = "2801114;";
			}
			CDboTSCheckClrQst
			{
				and = "2663;";
				flink = 1;
				flinknextqid = "2665;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 266408;
			gtype = 1;
			oklnk = 2;
			area = 266401;
			goal = 266404;
			sort = 266405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 266402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 266414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 266401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4652101;";
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
	}
}

