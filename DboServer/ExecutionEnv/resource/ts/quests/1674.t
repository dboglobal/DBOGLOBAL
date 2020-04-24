CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1674;
	title = 167402;

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
				conv = 167409;
				ctype = 1;
				idx = 5533403;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 167408;
				gtype = 1;
				area = 167401;
				goal = 167404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 167405;
				stype = 1;
				taid = 1;
				title = 167402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 167407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "5533403;";
			}
			CDboTSCheckClrQst
			{
				and = "1673;";
				flink = 1;
				flinknextqid = "1675;";
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
			cont = 167408;
			gtype = 1;
			oklnk = 2;
			area = 167401;
			goal = 167404;
			sort = 167405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 167402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 167414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 167401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3143108;";
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

