CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2114;
	title = 211402;

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
				conv = 211409;
				ctype = 1;
				idx = 6313108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 211408;
				gtype = 1;
				area = 211401;
				goal = 211404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 211405;
				stype = 1;
				taid = 1;
				title = 211402;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 211414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 211401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1652203;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 211408;
			gtype = 1;
			oklnk = 2;
			area = 211401;
			goal = 211404;
			sort = 211405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 211402;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 211407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "6313108;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2115;";
				not = 0;
			}
		}
	}
}

