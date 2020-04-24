CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2201;
	title = 220102;

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
				conv = 220109;
				ctype = 1;
				idx = 4511611;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 220108;
				gtype = 1;
				area = 220101;
				goal = 220104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 220105;
				stype = 1;
				taid = 1;
				title = 220102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 220107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				and = "2200;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4511611;";
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
			cont = 220108;
			gtype = 1;
			oklnk = 2;
			area = 220101;
			goal = 220104;
			sort = 220105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 220102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 220114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 220101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8411121;";
			}
		}
	}
}

