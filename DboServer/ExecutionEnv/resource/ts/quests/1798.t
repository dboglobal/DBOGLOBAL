CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1798;
	title = 179802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 179807;
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
				npcidx = "3142217;";
			}
			CDboTSCheckClrQst
			{
				and = "1797;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 179809;
				ctype = 1;
				idx = 3142217;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 179808;
				gtype = 3;
				area = 179801;
				goal = 179804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 179805;
				stype = 1;
				taid = 1;
				title = 179802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 179814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 179801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242301;";
			}
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
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 179808;
			gtype = 3;
			oklnk = 2;
			area = 179801;
			goal = 179804;
			sort = 179805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 179802;
		}
	}
}

