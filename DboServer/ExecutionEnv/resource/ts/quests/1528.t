CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1528;
	title = 152802;

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
				conv = 152809;
				ctype = 1;
				idx = 2801110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 152808;
				gtype = 1;
				area = 152801;
				goal = 152804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 152805;
				stype = 1;
				taid = 1;
				title = 152802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 152814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 152801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142116;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 152808;
			gtype = 1;
			oklnk = 2;
			area = 152801;
			goal = 152804;
			sort = 152805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 152802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 152807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSCheckClrQst
			{
				and = "1527;";
				flink = 1;
				flinknextqid = "1529;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2801110;";
			}
		}
	}
}

