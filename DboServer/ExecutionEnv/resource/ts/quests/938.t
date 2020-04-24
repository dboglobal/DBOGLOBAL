CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 938;
	title = 93802;

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
				conv = 93809;
				ctype = 1;
				idx = 3144105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 93808;
				gtype = 1;
				area = 93801;
				goal = 93804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 93805;
				stype = 1;
				taid = 1;
				title = 93802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 93807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 33;
			}
			CDboTSClickNPC
			{
				npcidx = "3144105;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "941;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 93808;
			gtype = 1;
			oklnk = 2;
			area = 93801;
			goal = 93804;
			sort = 93805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 93802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 93814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 93801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3184102;";
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

