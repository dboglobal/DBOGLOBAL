CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 986;
	title = 98602;

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
				conv = 98609;
				ctype = 1;
				idx = 8411111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 98608;
				gtype = 1;
				area = 98601;
				goal = 98604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 98605;
				stype = 2;
				taid = 1;
				title = 98602;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 98608;
			gtype = 1;
			oklnk = 2;
			area = 98601;
			goal = 98604;
			sort = 98605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 98602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 98614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 98601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142206;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 98607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 36;
			}
			CDboTSClickNPC
			{
				npcidx = "8411111;";
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

