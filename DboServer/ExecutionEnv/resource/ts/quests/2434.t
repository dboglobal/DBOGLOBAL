CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2434;
	title = 243402;

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
				conv = 243409;
				ctype = 1;
				idx = 3291202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 243408;
				gtype = 1;
				area = 243401;
				goal = 243404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 243405;
				stype = 2;
				taid = 1;
				title = 243402;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 243414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 243401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072301;";
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
			cont = 243408;
			gtype = 1;
			oklnk = 2;
			area = 243401;
			goal = 243404;
			sort = 243405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 243402;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 243407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "3291202;";
			}
			CDboTSCheckClrQst
			{
				and = "2433;";
				flink = 1;
				flinknextqid = "2445;";
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

