CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2901;
	title = 290102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 290107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "5553103;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 290114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 290101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4481203;";
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
			cont = 290108;
			gtype = 0;
			oklnk = 2;
			area = 290101;
			goal = 290104;
			sort = 290105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 290102;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 290109;
				ctype = 1;
				idx = 5553103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 290108;
				gtype = 0;
				area = 290101;
				goal = 290104;
				m0fz = "-2653.159912";
				m0widx = 1;
				sort = 290105;
				stype = 2;
				m0fy = "-79.690002";
				m0pat = 0;
				grade = 132203;
				m0fx = "2121.689941";
				m0ttip = 290115;
				rwd = 100;
				taid = 1;
				title = 290102;
			}
		}
	}
}

