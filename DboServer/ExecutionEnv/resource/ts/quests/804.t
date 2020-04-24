CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 804;
	title = 80402;

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
				conv = 80409;
				ctype = 1;
				idx = 3181204;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 80408;
				gtype = 1;
				area = 80401;
				goal = 80404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 80405;
				stype = 1;
				taid = 1;
				title = 80402;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 80414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 80401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2972102;";
			}
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
			cont = 80408;
			gtype = 1;
			oklnk = 2;
			area = 80401;
			goal = 80404;
			sort = 80405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 80402;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 80407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 27;
			}
			CDboTSClickNPC
			{
				npcidx = "3181204;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "805;";
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
	}
}

