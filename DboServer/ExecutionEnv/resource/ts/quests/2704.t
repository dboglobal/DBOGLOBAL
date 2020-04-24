CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2704;
	title = 270402;

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
				conv = 270409;
				ctype = 1;
				idx = 3292102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 270408;
				gtype = 1;
				area = 270401;
				goal = 270404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 270405;
				stype = 2;
				taid = 1;
				title = 270402;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 270414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 270401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4692101;";
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
			cont = 270408;
			gtype = 1;
			oklnk = 2;
			area = 270401;
			goal = 270404;
			sort = 270405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 270402;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 270407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				and = "2703;";
				flink = 1;
				flinknextqid = "2705;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3292102;";
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

