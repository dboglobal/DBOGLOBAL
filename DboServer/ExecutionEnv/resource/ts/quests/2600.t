CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2600;
	title = 260002;

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
				conv = 260009;
				ctype = 1;
				idx = 1471106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 260008;
				gtype = 1;
				area = 260001;
				goal = 260004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 260005;
				stype = 2;
				taid = 1;
				title = 260002;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 260008;
			gtype = 1;
			oklnk = 2;
			area = 260001;
			goal = 260004;
			sort = 260005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 260002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 260014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 260001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5101102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 260007;
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
				npcidx = "1471106;";
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

