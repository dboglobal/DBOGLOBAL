CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1168;
	title = 116802;

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
				conv = 116809;
				ctype = 1;
				idx = 3031208;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 116808;
				gtype = 2;
				area = 116801;
				goal = 116804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 116805;
				stype = 2;
				taid = 1;
				title = 116802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 116807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSClickNPC
			{
				npcidx = "3031208;";
			}
			CDboTSCheckClrQst
			{
				and = "1167;";
				flink = 1;
				flinknextqid = "1169;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 116808;
			gtype = 2;
			oklnk = 2;
			area = 116801;
			goal = 116804;
			sort = 116805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 116802;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 116814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 116801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242111;";
			}
		}
	}
}

