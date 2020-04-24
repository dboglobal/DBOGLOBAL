CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1456;
	title = 145602;

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
				conv = 145609;
				ctype = 1;
				idx = 6312104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 145608;
				gtype = 1;
				area = 145601;
				goal = 145604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 145605;
				stype = 2;
				taid = 1;
				title = 145602;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 145614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 145601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1654104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 145607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSClickNPC
			{
				npcidx = "6312104;";
			}
			CDboTSCheckClrQst
			{
				and = "1455;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			cont = 145608;
			gtype = 1;
			oklnk = 2;
			area = 145601;
			goal = 145604;
			sort = 145605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 145602;
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

