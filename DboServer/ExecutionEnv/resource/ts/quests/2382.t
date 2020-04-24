CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2382;
	title = 238202;

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
				conv = 238209;
				ctype = 1;
				idx = 6073201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 238208;
				gtype = 1;
				area = 238201;
				goal = 238204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 238205;
				stype = 2;
				taid = 1;
				title = 238202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 238207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "6073201;";
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
			cont = 238208;
			gtype = 1;
			oklnk = 2;
			area = 238201;
			goal = 238204;
			sort = 238205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 238202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 238214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 238201;
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
	}
}

