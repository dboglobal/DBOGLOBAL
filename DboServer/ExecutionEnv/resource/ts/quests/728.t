CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 728;
	title = 72802;

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
				conv = 72809;
				ctype = 1;
				idx = 4511605;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 72808;
				gtype = 1;
				area = 72801;
				goal = 72804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 72805;
				stype = 1;
				taid = 1;
				title = 72802;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 72808;
			gtype = 1;
			oklnk = 2;
			area = 72801;
			goal = 72804;
			sort = 72805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 72802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 72814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 72801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351211;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 72807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 24;
			}
			CDboTSClickNPC
			{
				npcidx = "4511605;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "729;";
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

